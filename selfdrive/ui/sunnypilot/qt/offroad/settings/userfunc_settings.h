#pragma once

#include "common/model.h"
#include "selfdrive/ui/sunnypilot/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/sunnypilot/qt/widgets/controls.h"
#include "selfdrive/ui/sunnypilot/qt/widgets/scrollview.h"

class TurnSteepNess : public OptionControlSP {
  Q_OBJECT

public:
  TurnSteepNess();
  void refresh();
private:
  Params params;
};

class TurnLatAccel : public OptionControlSP {
  Q_OBJECT

public:
  TurnLatAccel();
  void refresh();
private:
  Params params;
};

class TurnMaxFactor : public OptionControlSP {
  Q_OBJECT

public:
  TurnMaxFactor();
  void refresh();
private:
  Params params;
};

class StopDistance : public OptionControlSP {
  Q_OBJECT

public:
  StopDistance();
  void refresh();
private:
  Params params;
};

class CruiseOnDist : public OptionControlSP {
  Q_OBJECT

public:
  CruiseOnDist();
  void refresh();
private:
  Params params;
};

class ComfortBrake : public OptionControlSP {
  Q_OBJECT

public:
  ComfortBrake();
  void refresh();
private:
  Params params;
};

class StartAccel : public OptionControlSP {
  Q_OBJECT

public:
  StartAccel();
  void refresh();
private:
  Params params;
};

class StopAccel : public OptionControlSP {
  Q_OBJECT

public:
  StopAccel();
  void refresh();
private:
  Params params;
};

class MaxStopAccel : public OptionControlSP {
  Q_OBJECT

public:
  MaxStopAccel();
  void refresh();
private:
  Params params;
};

class vEgoStopping : public OptionControlSP {
  Q_OBJECT

public:
  vEgoStopping();
  void refresh();
private:
  Params params;
};

class ExperimentalModeSpeed : public OptionControlSP {
  Q_OBJECT

public:
  ExperimentalModeSpeed();
  void refresh();
private:
  Params params;
};

class ExperimentalModeAndSpeed : public OptionControlSP {
  Q_OBJECT

public:
  ExperimentalModeAndSpeed();
  void refresh();
private:
  Params params;
};

class ExperimentalModeAngle : public OptionControlSP {
  Q_OBJECT

public:
  ExperimentalModeAngle();
  void refresh();
private:
  Params params;
};


class JEgoCost : public OptionControlSP {
  Q_OBJECT

public:
  JEgoCost();
  void refresh();
private:
  Params params;
};

class AChangeCostStart : public OptionControlSP {
  Q_OBJECT

public:
  AChangeCostStart();
  void refresh();
private:
  Params params;
};

class AChangeCost : public OptionControlSP {
  Q_OBJECT

public:
  AChangeCost();
  void refresh();
private:
  Params params;
};

class UserFuncPanel : public QFrame {
  Q_OBJECT

public:
  explicit UserFuncPanel(QWidget *parent = nullptr);
  void showEvent(QShowEvent *event) override;
  void hideEvent(QHideEvent* event) override;

public slots:
  void updateToggles();

private:
  QStackedLayout* main_layout = nullptr;
  QWidget* userScreen = nullptr;
  Params params;
  std::map<std::string, ParamControlSP*> toggles;
  ParamWatcher *param_watcher;
  ScrollView *scrollView = nullptr;

  StopDistance *stop_distance;
  ComfortBrake *comfort_break;
  StartAccel *start_accel;
  StopAccel *stop_accel;
  MaxStopAccel *max_stop_accel;
  vEgoStopping *vego_stopping;

  ExperimentalModeAngle *experimental_mode_angle;
  ExperimentalModeAndSpeed *experimental_mode_and_speed;
  ExperimentalModeSpeed *experimental_mode_speed;

  TurnSteepNess *turn_steep_ness;
  TurnLatAccel *turn_lat_acc;
  TurnMaxFactor *turn_max_factor;

  JEgoCost *j_ego_cost;
  AChangeCostStart *a_change_cost_start;
  AChangeCost *a_change_cost;

  LabelControlSP *label_turn_steep_ness;
  LabelControlSP *label_turn_lat_accel;
  LabelControlSP *label_turn_max_factor;

  LabelControlSP *label_experimental_angle_and_speed;
  LabelControlSP *label_experimental_speed;
  ParamControlSP *toggle_dm;
};
