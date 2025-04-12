/**
The MIT License

Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Last updated: July 29, 2024
***/

#include "selfdrive/ui/sunnypilot/qt/offroad/settings/sunnypilot/experimental_settings.h"

#include <map>
#include <string>
#include <tuple>
#include <vector>

ExperimentalModeSettings::ExperimentalModeSettings(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* main_layout = new QVBoxLayout(this);
  main_layout->setContentsMargins(50, 20, 50, 20);
  main_layout->setSpacing(20);

  // Back button
  PanelBackButton* back = new PanelBackButton(tr("Back"));
  connect(back, &QPushButton::clicked, [=]() { emit backPress(); });
  main_layout->addWidget(back, 0, Qt::AlignLeft);

  ListWidgetSP *list = new ListWidgetSP(this, false);
  // param, title, desc, icon
  std::vector<std::tuple<QString, QString, QString, QString>> toggle_defs{
    {
      "ConditionExperimentalMode",
      tr("Enable Condition Experimental Mode"),
      tr("Enable this toggle to setting the experimental mode, Disable experimental mode when the speed is greater than the specified value or the Angle is greater than the specified value"),
      "../assets/offroad/icon_blank.png",
    },
  };

  std::vector<std::tuple<QString, QString, QString, QString>> toggle_trafficlight_defs{
    {
      "EnableTrafficLight",
      tr("Enable trafficLight"),
      tr("Enable the trafficLight function"),
      "../assets/offroad/icon_blank.png",
    },

    {
      "DisTrafficLeadDistance",
      tr("Disable trafficLight follow distance setting"),
      tr("Disable trafficLight when follow distance less than the value"),
      "../assets/offroad/icon_blank.png",
    },
  };

  //ConditionExperimentalMode选择项
  for (auto &[param, title, desc, icon] : toggle_defs) {
    auto toggle = new ParamControlSP(param, title, desc, icon, this);
    list->addItem(toggle);
    toggles[param.toStdString()] = toggle;

    if (param == "ConditionExperimentalMode") {
    }
  }

  list->addItem(new LabelControlSP(tr("Experimental SteerAngle And Speed Setting")));

  // Controls: Auto Lane Change Timer
  experimental_mode_angle = new ExperimentalModeAngle();
  experimental_mode_angle->showDescription();
  connect(experimental_mode_angle, &OptionControlSP::updateLabels, experimental_mode_angle, &ExperimentalModeAngle::refresh);
  list->addItem(experimental_mode_angle);

  experimental_mode_and_speed = new ExperimentalModeAndSpeed();
  experimental_mode_and_speed->showDescription();
  connect(experimental_mode_and_speed, &OptionControlSP::updateLabels, experimental_mode_and_speed, &ExperimentalModeAndSpeed::refresh);
  list->addItem(experimental_mode_and_speed);

  list->addItem(new LabelControlSP(tr("Experimental Speed Setting")));

  // Pause Lateral Below Speed w/ Blinker
  experimental_mode_speed = new ExperimentalModeSpeed();
  experimental_mode_speed->showDescription();
  connect(experimental_mode_speed, &OptionControlSP::updateLabels, experimental_mode_speed, &ExperimentalModeSpeed::refresh);
  list->addItem(experimental_mode_speed);

  connect(toggles["ConditionExperimentalMode"], &ToggleControlSP::toggleFlipped, [=](bool state) {

  });

  list->addItem(new LabelControlSP(tr("Stop Distance Setting(600x0.01m)")));

  stop_distance_carrot = new StopDistanceCarrot();
  stop_distance_carrot->showDescription();
  connect(stop_distance_carrot, &OptionControlSP::updateLabels, stop_distance_carrot, &StopDistanceCarrot::refresh);
  list->addItem(stop_distance_carrot);

  list->addItem(new LabelControlSP(tr("Break Comfort Setting(240x0.01m/s^2)")));

  comfort_break = new ComfortBrake();
  comfort_break->showDescription();
  connect(comfort_break, &OptionControlSP::updateLabels, comfort_break, &ComfortBrake::refresh);
  list->addItem(comfort_break);

  //DisTrafficLeadDistance选择项
  for (auto &[param, title, desc, icon] : toggle_trafficlight_defs) {
    auto toggle = new ParamControlSP(param, title, desc, icon, this);
    list->addItem(toggle);
    toggles[param.toStdString()] = toggle;
  }

  connect(toggles["EnableTrafficLight"], &ToggleControlSP::toggleFlipped, [=](bool state) {
  });
  connect(toggles["DisTrafficLeadDistance"], &ToggleControlSP::toggleFlipped, [=](bool state) {
  });

  //红绿灯跟车距离设置
  dis_traffic_lead_distance_thr = new DisTrafficLeadDistanceThr();
  dis_traffic_lead_distance_thr->showDescription();
  connect(dis_traffic_lead_distance_thr, &OptionControlSP::updateLabels, dis_traffic_lead_distance_thr, &DisTrafficLeadDistanceThr::refresh);
  list->addItem(dis_traffic_lead_distance_thr);

  //标签
  list->addItem(new LabelControlSP(tr("TrafficLight follow distance hysteresis")));

  //红绿灯跟车距离迟滞设置
  dis_traffic_lead_distance_hys = new DisTrafficLeadDistanceHys();
  dis_traffic_lead_distance_hys->showDescription();
  connect(dis_traffic_lead_distance_hys, &OptionControlSP::updateLabels, dis_traffic_lead_distance_hys, &DisTrafficLeadDistanceHys::refresh);
  list->addItem(dis_traffic_lead_distance_hys);

  main_layout->addWidget(new ScrollViewSP(list, this));
}

void ExperimentalModeSettings::showEvent(QShowEvent *event) {
  updateToggles();
}

void ExperimentalModeSettings::updateToggles() {
  if (!isVisible()) {
    return;
  }
}

ExperimentalModeAngle::ExperimentalModeAngle() : OptionControlSP(
  "ExperimentalModeAngle",
  "",
  tr("Disable Experimental Mode when the angle large than the value."),
  "../assets/offroad/icon_blank.png",
  {0, 90},
  5) {

  refresh();
}

void ExperimentalModeAngle::refresh() {
  QString option = QString:: fromStdString(params.get("ExperimentalModeAngle"));
  if (option == "0") {
    setLabel(tr("Disable"));
  } else {
    setLabel(option + " " + tr("Degree"));
  }
}

ExperimentalModeSpeed::ExperimentalModeSpeed() : OptionControlSP(
  "ExperimentalModeSpeed",
  "",
  tr("Disable Experimental Mode when the speed large than the value."),
  "../assets/offroad/icon_blank.png",
  {0, 255},
  5) {

  refresh();
}

void ExperimentalModeSpeed::refresh() {
  QString option = QString:: fromStdString(params.get("ExperimentalModeSpeed"));
  bool is_metric = params.getBool("IsMetric");

  if (option == "0") {
    setLabel(tr("Disable"));
  } else {
    setLabel(option + " " + (is_metric ? tr("km/h") : tr("mph")));
  }
}

ExperimentalModeAndSpeed::ExperimentalModeAndSpeed() : OptionControlSP(
  "ExperimentalModeAndSpeed",
  "",
  tr("Disable Experimental Mode when the speed large than the value."),
  "../assets/offroad/icon_blank.png",
  {0, 255},
  1) {

  refresh();
}

void ExperimentalModeAndSpeed::refresh() {
  QString option = QString:: fromStdString(params.get("ExperimentalModeAndSpeed"));
  bool is_metric = params.getBool("IsMetric");

  if (option == "0") {
    setLabel(tr("Disable"));
  } else {
    setLabel(option + " " + (is_metric ? tr("km/h") : tr("mph")));
  }
}

StopDistanceCarrot::StopDistanceCarrot() : OptionControlSP(
  "StopDistanceCarrot",
  "",
  tr("Stop Distance value."),
  "../assets/offroad/icon_blank.png",
  {100, 2000},
  10) {

  refresh();
}

void StopDistanceCarrot::refresh() {
  QString option = QString:: fromStdString(params.get("StopDistanceCarrot"));
  setLabel(option);
}

ComfortBrake::ComfortBrake() : OptionControlSP(
  "ComfortBrake",
  "",
  tr("Comfort Break Setting."),
  "../assets/offroad/icon_blank.png",
  {50, 500},
  5) {

  refresh();
}

void ComfortBrake::refresh() {
  QString option = QString:: fromStdString(params.get("ComfortBrake"));
  setLabel(option);
}

//红绿灯跟车距离设置
DisTrafficLeadDistanceThr::DisTrafficLeadDistanceThr() : OptionControlSP(
  "DisTrafficLeadDistanceThr",
  "",
  tr("Traffic Lead Distance Setting."),
  "../assets/offroad/icon_blank.png",
  {0, 200},
  1) {

  refresh();
}

void DisTrafficLeadDistanceThr::refresh() {
  QString option = QString:: fromStdString(params.get("DisTrafficLeadDistanceThr"));
  setLabel(option + " m");
}

//红绿灯跟车距离迟滞设置
DisTrafficLeadDistanceHys::DisTrafficLeadDistanceHys() : OptionControlSP(
  "DisTrafficLeadDistanceHys",
  "",
  tr("Traffic Lead Distance Hysteresis Setting."),
  "../assets/offroad/icon_blank.png",
  {0, 200},
  1) {

  refresh();
}

void DisTrafficLeadDistanceHys::refresh() {
  QString option = QString:: fromStdString(params.get("DisTrafficLeadDistanceHys"));
  setLabel(option + " m");
}
