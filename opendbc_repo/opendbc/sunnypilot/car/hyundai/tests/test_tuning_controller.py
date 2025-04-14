import unittest
import numpy as np
from unittest.mock import Mock, patch
from opendbc.sunnypilot.car.hyundai.longitudinal.tuning_controller import LongitudinalTuningController, LongitudinalTuningState
from opendbc.sunnypilot.car.hyundai.values import HyundaiFlagsSP
from opendbc.car import DT_CTRL, structs
from opendbc.car.interfaces import CarStateBase
from opendbc.car.hyundai.values import HyundaiFlags

LongCtrlState = structs.CarControl.Actuators.LongControlState


class TestLongitudinalTuningController(unittest.TestCase):
  def setUp(self):
    self.mock_CP = Mock(carFingerprint="KIA_NIRO_EV", flags=0)
    self.mock_CP_SP = Mock(flags=0)

    # Mock car_config
    with patch('opendbc.sunnypilot.car.hyundai.longitudinal.helpers.get_car_config') as mock_get_config:
      mock_get_config.return_value = Mock(jerk_limits=[0.53, 5.0, 2.2])
      self.controller = LongitudinalTuningController(self.mock_CP, self.mock_CP_SP)
    print(f"\n[SETUP] Controller initialized with jerk limits: {self.controller.car_config.jerk_limits}")

  def test_init(self):
    """Test controller initialization"""
    self.assertIsInstance(self.controller.state, LongitudinalTuningState)
    self.assertEqual(self.controller.desired_accel, 0.0)
    self.assertEqual(self.controller.actual_accel, 0.0)
    self.assertEqual(self.controller.jerk_upper, 0.0)
    self.assertEqual(self.controller.jerk_lower, 0.0)

  def test_make_jerk_flag_off(self):
    """Test when LONG_TUNING_BRAKING flag is off"""
    mock_CC, mock_CS = Mock(spec=structs.CarControl), Mock(spec=CarStateBase)

    # Test with PID state
    self.controller.make_jerk(mock_CC, mock_CS, LongCtrlState.pid)
    print(f"[PID state] jerk_upper={self.controller.jerk_upper:.2f}, jerk_lower={self.controller.jerk_lower:.2f}")
    self.assertEqual(self.controller.jerk_upper, 3.0)
    self.assertEqual(self.controller.jerk_lower, 5.0)

    # Test with non-PID state
    self.controller.make_jerk(mock_CC, mock_CS, LongCtrlState.stopping)
    print(f"[Non-PID state] jerk_upper={self.controller.jerk_upper:.2f}, jerk_lower={self.controller.jerk_lower:.2f}")
    self.assertEqual(self.controller.jerk_upper, 1.0)
    self.assertEqual(self.controller.jerk_lower, 5.0)

  def test_make_jerk_flag_on(self):
    """Test when LONG_TUNING_BRAKING flag is on"""
    self.controller.CP_SP.flags = HyundaiFlagsSP.LONG_TUNING_BRAKING
    # Also set CANFD flag on mock_CP for test coverage
    self.controller.CP.flags = HyundaiFlags.CANFD
    print(f"\n[test_make_jerk_flag_on] LONG_TUNING_BRAKING flag set: {self.controller.CP_SP.flags}")

    # Setup test mocks
    mock_CC = Mock()
    mock_CC.actuators = Mock(accel=1.0)
    mock_CS = Mock()
    mock_CS.out = Mock(aEgo=0.8, vEgo=3.0)


    print(f"First call with planned_accel={mock_CC.actuators.accel}, current_accel={mock_CS.out.aEgo}")

    # Calculate expected values
    blended_accel = 1.0
    dt = DT_CTRL * 2
    tau = 0.25
    k = dt / (tau + dt)
    expected_filtered = blended_accel * k
    expected_jerk = expected_filtered / dt

    self.controller.make_jerk(mock_CC, mock_CS, LongCtrlState.pid)

    print(f"Blended: {blended_accel}, Filtered: {self.controller.accel_filter.x}, k={k}")
    print(f"Expected jerk: {expected_jerk}, Actual jerk: {self.controller.state.jerk}")
    print(f"Jerk limits (upper/lower): {self.controller.jerk_upper:.4f}/{self.controller.jerk_lower:.4f}")

    self.assertAlmostEqual(self.controller.state.jerk, expected_jerk, places=5)
    self.assertGreaterEqual(self.controller.jerk_upper, 0.6)  # Min at low velocity

  def test_filter_behavior(self):
    """Test FirstOrderFilter behavior with step input"""
    self.controller.CP_SP.flags = HyundaiFlagsSP.LONG_TUNING_BRAKING
    # Add CANFD flag for coverage
    self.controller.CP.flags = HyundaiFlags.CANFD
    mock_CC = Mock()
    mock_CS = Mock()

    # Setup a step input
    mock_CC.actuators = Mock(accel=2.0)  # Step to 2.0 m/s²
    mock_CS.out = Mock(aEgo=0.0, vEgo=10.0)

    # Calculate filter parameters
    dt = DT_CTRL * 2
    tau = 0.25
    k = dt / (tau + dt)

    # Calculate expected response to step input
    expected_values = []
    x = 0.0
    blended_accel = 2.0  # blend with aEgo=0
    for _ in range(10):
        x = x * (1-k) + blended_accel * k
        expected_values.append(x)

    # Run the controller and compare
    print("\n[test_filter_behavior] Testing filter response to step input:")
    print(f"  Filter params: k={k}")

    for i in range(10):
        self.controller.make_jerk(mock_CC, mock_CS, LongCtrlState.pid)
        print(f"  Iter {i}: expected={expected_values[i]:.5f}, actual={self.controller.accel_filter.x:.5f}, jerk={self.controller.state.jerk:.5f}")
        self.assertAlmostEqual(self.controller.accel_filter.x, expected_values[i], places=5)

  def test_jerk_calculation(self):
    """Test jerk calculation with various inputs"""
    self.controller.CP_SP.flags = HyundaiFlagsSP.LONG_TUNING_BRAKING
    # Add CANFD flag for coverage
    self.controller.CP.flags = HyundaiFlags.CANFD
    print("\n[test_jerk_calculation] Testing with various acceleration values")

    mock_CC, mock_CS = Mock(), Mock()
    mock_CC.actuators = Mock()
    mock_CS.out = Mock(aEgo=0.0, vEgo=10.0)

    test_deltas = [-2.0, -1.0, -0.5, -0.1, -0.01, 0.0, 0.01, 0.1, 0.5, 1.0, 2.0]
    dt = DT_CTRL * 2
    tau = 0.25
    k = dt / (tau + dt)

    for planned_accel in test_deltas:
      self.controller.accel_filter.x = 0.0
      mock_CC.actuators.accel = planned_accel
      mock_CS.out.aEgo = planned_accel * 0.5

      # Expected uses planned_accel only
      expected_first_filtered = planned_accel * k
      expected_first_jerk = expected_first_filtered / dt

      self.controller.make_jerk(mock_CC, mock_CS, LongCtrlState.pid)

      print(f"\nTesting planned_accel={planned_accel}")
      print(f"  Blended: {planned_accel}, Filtered: {self.controller.accel_filter.x:.5f}")
      print(f"  Expected jerk: {expected_first_jerk:.5f}, Actual jerk: {self.controller.state.jerk:.5f}")
      print(f"  Jerk limits (upper/lower): {self.controller.jerk_upper:.4f}/{self.controller.jerk_lower:.4f}")

      self.assertAlmostEqual(self.controller.state.jerk, expected_first_jerk, places=5)

      # Check minimum jerk limits for small deltas
      if abs(planned_accel) < 1.0:
        # Use controller logic: if planned_accel < -0.1 use car_config value, otherwise 0.5
        if planned_accel < -0.1:
          expected_min = self.controller.car_config.jerk_limits[0]
        else:
          expected_min = 0.5
        if self.controller.state.jerk > 0:
          self.assertGreaterEqual(self.controller.jerk_upper, expected_min)
        else:
          self.assertGreaterEqual(self.controller.jerk_lower, expected_min)

  def test_a_value_jerk_scaling(self):
    """Test a_value jerk scaling"""
    self.controller.CP_SP.flags = HyundaiFlagsSP.LONG_TUNING
    self.controller.jerk_upper = 0.1 / (DT_CTRL * 2)
    mock_CC = Mock(enabled=True)
    mock_CC.actuators = Mock(accel=1.0)
    self.controller.calculate_a_value(mock_CC)
    self.assertAlmostEqual(self.controller.actual_accel, 0.1)

    self.controller.jerk_upper = 0.2 / (DT_CTRL * 2)
    mock_CC.actuators.accel = 0.7
    self.controller.calculate_a_value(mock_CC)
    # Second update integrates based on previous state (0.1 + 0.2 = 0.3)
    self.assertAlmostEqual(self.controller.actual_accel, 0.3)

  def test_make_jerk_realistic_profile(self):
    """Test make_jerk with realistic velocity and acceleration profile"""
    self.controller.CP_SP.flags = HyundaiFlagsSP.LONG_TUNING_BRAKING

    # Generate test data
    np.random.seed(42)
    num_points = 20
    segments = [
      np.random.uniform(0.3, 0.8, num_points//4),     # Mild acceleration
      np.random.uniform(0.8, 1.6, num_points//4),     # Moderate acceleration
      np.random.uniform(-0.2, 0.2, num_points//4),    # Cruise
      np.random.uniform(-1.2, -0.5, num_points//8),   # Moderate deceleration
      np.random.uniform(-2.2, -1.2, num_points//8)    # Hard deceleration
    ]
    accelerations = np.concatenate(segments)[:num_points]

    # Calculate velocities
    velocities = np.zeros(len(accelerations))
    velocities[0] = 1.0  # Starting velocity
    for i in range(1, len(accelerations)):
        velocities[i] = velocities[i-1] + accelerations[i-1] * 0.2
    velocities = np.clip(velocities, 0.0, 30.0)
    accelerations_list = [float(a) for a in accelerations]
    velocities_list = [float(v) for v in velocities]

    # Setup mocks and test
    mock_CC, mock_CS = Mock(), Mock()
    mock_CC.actuators, mock_CS.out = Mock(), Mock()

    print("\n[test_make_jerk_realistic_profile] Testing velocity profile:")
    for i, (v, a) in enumerate(zip(velocities_list, accelerations_list, strict=True)):
      mock_CS.out.vEgo = v
      mock_CS.out.aEgo = a
      mock_CC.actuators.accel = a

      self.controller.make_jerk(mock_CC, mock_CS, LongCtrlState.pid)

      print(f"  Step {i:2d}: v={v:5.2f} m/s, a={a:5.2f} m/s², jerk={self.controller.state.jerk:5.2f}, \
            jerk_upper={self.controller.jerk_upper:5.2f}, jerk_lower={self.controller.jerk_lower:5.2f}")

      # Verify minimum jerk limits based on velocity
      min_jerk = self.controller.car_config.jerk_limits[0]
      if v > 3.611:  # Above walking speed
        self.assertGreaterEqual(self.controller.jerk_upper, min_jerk)

if __name__ == "__main__":
  unittest.main()
