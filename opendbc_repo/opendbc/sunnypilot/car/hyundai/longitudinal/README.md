
# **How custom longitudinal tuning works for Hyundai/Kia/Genesis vehicles in sunnypilot.**

To start this readme, I would like to first present the safety guidelines followed to create the tune:

Our main safety guideline considered is [ISO 15622:2018](https://www.iso.org/obp/ui/en/#iso:std:iso:15622:ed-3:v1:en)
This provides the groundwork of safety limits this tune must adhere too, and therefore, must be followed.
For example, in our jerk calculations throughout this tune, you will see how maximum jerk is clipped using the equation provided.

In the tuning you will see a set of equations, the first being jerk, **but what exactly is jerk?**
Jerk is the rate of change of acceleration (how quickly acceleration changes). It's calculated by finding the 
difference between the new filtered acceleration and the previous one. This difference is divided by the
time step (2 × 0.01 seconds = 0.02 seconds). The result tells you how quickly the acceleration is changing in m/s³:

    planned_accel = CC.actuators.accel
    current_accel = CS.out.aEgo
    blended_accel = 0.8 * planned_accel + 0.2 * current_accel

    prev_filtered_accel = self.accel_filter.x

    self.accel_filter.update(blended_accel)
    filtered_accel = self.accel_filter.x

    self.state.jerk = (filtered_accel - prev_filtered_accel) / (DT_MDL * 3)

planned_accel is what the controller wants the car to do (target acceleration), where current_accel is what the
car is actually doing right now (measured acceleration). The code combines these with an 80%/20% ratio to create
a more realistic target. This prevents sudden jerkiness that can sometimes be produced. Before calculating anything
new, the code saves the previous filtered acceleration. This value will be needed to calculate the rate of change (jerk)
A first-order filter is like a smoothing function that reduces sudden changes. `update()` feeds the new
blended acceleration into this filter. The filter gradually moves toward the target value rather than jumping to it.
This then goes through our minimum and maximum clipping which forces a value between our set min and max,
which I discuss later in this readme.

Moving on, the accel_last_jerk, stores current accel after each iteration and uses that in the calculation as previous accel for
our jerk calculations. Now we see the calculation of jerk max and jerk min. 

**Let's dive into how jerk lower limit max is calculated:**

     velocity = CS.out.vEgo
    if velocity < 5.0:
      decel_jerk_max = self.car_config.jerk_limits[1]
    elif velocity > 20.0:
      decel_jerk_max = 2.5
    else:
      if self.CP.flags & HyundaiFlags.CANFD:
        decel_jerk_max = 5.83 - (velocity/6)
      else:
        decel_jerk_max = 3.64284 - (0.05714 * velocity)

This equation above is set by ISO 15622, and dictates that jerk lower limit can only be five when below 5 m/s. In our equation,

    self.car_config.jerk_limits[1] 

Jerk_limits[1] represents a jerk value of 5.0 m/s^3, which is the maximum analyzed lower jerk rate seen on stock SCC CAN.
Between 5 m/s and 20 m/s jerk is capped using the calculation:

    decel_jerk_max = 5.83 - (velocity/6)
    decel_jerk_max = 3.64284 - 0.05714 * velocity

This equation first determines if the user is in a CANFD vehicle, to which higher jerk lower limits are requested, 
then it calculates the linear jerk from 6m/s to 19m/s, scaling down from 3.3 to 2.5 m/s^3.
This means that if current velocity is say, 15 m/s the final jerk max value would be capped at 2.78 m/s^3.
Anything above 20 m/s is capped to a lower jerk max of 2.5 m/s^3. This allows for a smoother jerk range, while complying to ISO standards to a tee.
The current jerk Lower Limit you will see in openpilot before this tune, is 5.0 m/s^3; Which as you can see from using the above calculation,
the 5.0 m/s^3 technically does not comply with ISO standards at any speed above 5.0 m/s^3.
Having our jerk max be clipped to these values not only allows for better consistency with ISO standards, 
but also enables us to have a much smoother braking experience.

**Getting into our next topic, I would like to explain how our minimum jerk was chosen.**

Minimum jerk was chosen based off of the following guideline proposed by Handbook of Intellegent Vehicles (2012):
`Ride comfort may be sacrificed only under emergency conditions when vehicle and occupant safety consideration may preclude comfort.`

**What the value of 0.53 m/s^3 of the jerk lower limit was chosen based off of**

[Carlowitz et al. (2024).](https://www.researchgate.net/publication/382274551_User_evaluation_of_comfortable_deceleration_profiles_for_highly_automated_driving_Findings_from_a_test_track_study)
This research study identified the average lower jerk used in comfortable driving settings, which is 0.53 m/s^3.
This is then inputted to jerk_limits[0] as 0.53 m/s^3 represents the value used in upper jerk absolute minimum.

     min_lower_jerk = self.car_config.jerk_limits[0]

As shown above, lower jerk minimum of 0.53 is used for our lower_jerk minimum bounds.

**Why our minimum upper jerk is conditional**

Our minimum upper band jerk is conditional as well and is denoted below:

    min_upper_jerk = self.car_config.jerk_limits[0] if (velocity > 3.611) else 0.725

This means that for speeds under 3.611 m/s (8.077 mph/ 13 kph) we have a minimum jerk of 0.60. This allows for smooth
takeoffs while not causing lag. For all other speeds, we use our normal min jerk_limit, which is 0.53.

**Why our minimum lower jerk is conditional** 

Our minimum lower band is conditional, and is based on the following condition:
    
    min_lower_jerk = self.car_config.jerk_limits[0] if (velocity < 12.0) else 0.625

This means that for speeds under 12 m/s, we have our minimum jerk of 0.53 (jerk_limits[0]), but for all other speeds,
minimum jerk is set to 0.625. 

**Next, we have our acceleration limiting**

For acceleration limiting, we use TCS signal brakeLightsDEPRECATED to measure when to enact the standstill delay 
which stock SCC uses to allow smoother transitions in acceleration.

**Next, we have our accel value calculations for hyundaican.py**

For our accel value calculations we have the following:

   `self.accel_value = np.clip(self.accel_raw, self.state.accel_last - jerk_number, self.state.accel_last + jerk_number)`

This essentialy means that we have our accel_raw, which is acceleration (m/s^2), followed by our clipping variables. 
jerk_number in this equation represents exactly `0.1`, which is subtracted or added by self.state.accel_last, which is 
previous calculated accel_value. Furthermore, we have `self.state.accel_last`, which is caculated as the stored accel from
the above calculations.
