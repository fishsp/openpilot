#include <cassert>
#include <cmath>
#include <string>
#include <tuple>
#include <vector>
#include <thread> //차선캘리

#include <QDebug>
#include <QProcess>

#include "common/watchdog.h"
#include "common/util.h"
#include "selfdrive/ui/qt/network/networking.h"
#include "selfdrive/ui/qt/offroad/settings.h"
#include "selfdrive/ui/qt/qt_window.h"
#include "selfdrive/ui/qt/widgets/prime.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"
#include "selfdrive/ui/qt/offroad/developer_panel.h"
#include "selfdrive/ui/qt/offroad/firehose.h"

TogglesPanel::TogglesPanel(SettingsWindow *parent) : ListWidget(parent) {
  // param, title, desc, icon
  std::vector<std::tuple<QString, QString, QString, QString>> toggle_defs{
    {
      "OpenpilotEnabledToggle",
      tr("Enable openpilot"),
      tr("Use the openpilot system for adaptive cruise control and lane keep driver assistance. Your attention is required at all times to use this feature. Changing this setting takes effect when the car is powered off."),
      "../assets/img_chffr_wheel.png",
    },
    {
      "ExperimentalMode",
      tr("Experimental Mode"),
      "",
      "../assets/img_experimental_white.svg",
    },
    {
      "DisengageOnAccelerator",
      tr("Disengage on Accelerator Pedal"),
      tr("When enabled, pressing the accelerator pedal will disengage openpilot."),
      "../assets/offroad/icon_disengage_on_accelerator.svg",
    },
    {
      "IsLdwEnabled",
      tr("Enable Lane Departure Warnings"),
      tr("Receive alerts to steer back into the lane when your vehicle drifts over a detected lane line without a turn signal activated while driving over 31 mph (50 km/h)."),
      "../assets/offroad/icon_warning.png",
    },
    {
      "AlwaysOnDM",
      tr("Always-On Driver Monitoring"),
      tr("Enable driver monitoring even when openpilot is not engaged."),
      "../assets/offroad/icon_monitoring.png",
    },
    {
      "RecordFront",
      tr("Record and Upload Driver Camera"),
      tr("Upload data from the driver facing camera and help improve the driver monitoring algorithm."),
      "../assets/offroad/icon_monitoring.png",
    },
    {
      "RecordAudio",
      tr("Record and Upload Microphone Audio"),
      tr("Record and store microphone audio while driving. The audio will be included in the dashcam video in comma connect."),
      "../assets/offroad/microphone.png",
    },
    {
      "IsMetric",
      tr("Use Metric System"),
      tr("Display speed in km/h instead of mph."),
      "../assets/offroad/icon_metric.png",
    },
  };


  std::vector<QString> longi_button_texts{tr("Aggressive"), tr("Standard"), tr("Relaxed") , tr("MoreRelaxed") };
  long_personality_setting = new ButtonParamControl("LongitudinalPersonality", tr("Driving Personality"),
                                          tr("Standard is recommended. In aggressive mode, openpilot will follow lead cars closer and be more aggressive with the gas and brake. "
                                             "In relaxed mode openpilot will stay further away from lead cars. On supported cars, you can cycle through these personalities with "
                                             "your steering wheel distance button."),
                                          "../assets/offroad/icon_speed_limit.png",
                                          longi_button_texts);

  // set up uiState update for personality setting
  QObject::connect(uiState(), &UIState::uiUpdate, this, &TogglesPanel::updateState);

  for (auto &[param, title, desc, icon] : toggle_defs) {
    auto toggle = new ParamControl(param, title, desc, icon, this);

    bool locked = params.getBool((param + "Lock").toStdString());
    toggle->setEnabled(!locked);

    addItem(toggle);
    toggles[param.toStdString()] = toggle;

    // insert longitudinal personality after NDOG toggle
    if (param == "DisengageOnAccelerator") {
      addItem(long_personality_setting);
    }
  }

  // Toggles with confirmation dialogs
  toggles["ExperimentalMode"]->setActiveIcon("../assets/img_experimental.svg");
  toggles["ExperimentalMode"]->setConfirmation(true, true);
}

void TogglesPanel::updateState(const UIState &s) {
  const SubMaster &sm = *(s.sm);

  if (sm.updated("selfdriveState")) {
    auto personality = sm["selfdriveState"].getSelfdriveState().getPersonality();
    if (personality != s.scene.personality && s.scene.started && isVisible()) {
      long_personality_setting->setCheckedButton(static_cast<int>(personality));
    }
    uiState()->scene.personality = personality;
  }
}

void TogglesPanel::expandToggleDescription(const QString &param) {
  toggles[param.toStdString()]->showDescription();
}

void TogglesPanel::showEvent(QShowEvent *event) {
  updateToggles();
}

void TogglesPanel::updateToggles() {
  auto experimental_mode_toggle = toggles["ExperimentalMode"];
  const QString e2e_description = QString("%1<br>"
                                          "<h4>%2</h4><br>"
                                          "%3<br>"
                                          "<h4>%4</h4><br>"
                                          "%5<br>")
                                  .arg(tr("openpilot defaults to driving in <b>chill mode</b>. Experimental mode enables <b>alpha-level features</b> that aren't ready for chill mode. Experimental features are listed below:"))
                                  .arg(tr("End-to-End Longitudinal Control"))
                                  .arg(tr("Let the driving model control the gas and brakes. openpilot will drive as it thinks a human would, including stopping for red lights and stop signs. "
                                          "Since the driving model decides the speed to drive, the set speed will only act as an upper bound. This is an alpha quality feature; "
                                          "mistakes should be expected."))
                                  .arg(tr("New Driving Visualization"))
                                  .arg(tr("The driving visualization will transition to the road-facing wide-angle camera at low speeds to better show some turns. The Experimental mode logo will also be shown in the top right corner."));

  const bool is_release = params.getBool("IsReleaseBranch");
  auto cp_bytes = params.get("CarParamsPersistent");
  if (!cp_bytes.empty()) {
    AlignedBuffer aligned_buf;
    capnp::FlatArrayMessageReader cmsg(aligned_buf.align(cp_bytes.data(), cp_bytes.size()));
    cereal::CarParams::Reader CP = cmsg.getRoot<cereal::CarParams>();

    if (hasLongitudinalControl(CP)) {
      // normal description and toggle
      experimental_mode_toggle->setEnabled(true);
      experimental_mode_toggle->setDescription(e2e_description);
      long_personality_setting->setEnabled(true);
    } else {
      // no long for now
      experimental_mode_toggle->setEnabled(false);
      long_personality_setting->setEnabled(false);
      params.remove("ExperimentalMode");

      const QString unavailable = tr("Experimental mode is currently unavailable on this car since the car's stock ACC is used for longitudinal control.");

      QString long_desc = unavailable + " " + \
                          tr("openpilot longitudinal control may come in a future update.");
      if (CP.getAlphaLongitudinalAvailable()) {
        if (is_release) {
          long_desc = unavailable + " " + tr("An alpha version of openpilot longitudinal control can be tested, along with Experimental mode, on non-release branches.");
        } else {
          long_desc = tr("Enable the openpilot longitudinal control (alpha) toggle to allow Experimental mode.");
        }
      }
      experimental_mode_toggle->setDescription("<b>" + long_desc + "</b><br><br>" + e2e_description);
    }

    experimental_mode_toggle->refresh();
  } else {
    experimental_mode_toggle->setDescription(e2e_description);
  }
}

DevicePanel::DevicePanel(SettingsWindow *parent) : ListWidget(parent) {
  setSpacing(50);
  addItem(new LabelControl(tr("Dongle ID"), getDongleId().value_or(tr("N/A"))));
  addItem(new LabelControl(tr("Serial"), params.get("HardwareSerial").c_str()));

  // power buttons
  QHBoxLayout* power_layout = new QHBoxLayout();
  power_layout->setSpacing(30);

  QPushButton* reboot_btn = new QPushButton(tr("重启"));
  reboot_btn->setObjectName("reboot_btn");
  power_layout->addWidget(reboot_btn);
  QObject::connect(reboot_btn, &QPushButton::clicked, this, &DevicePanel::reboot);
  //차선캘리
  QPushButton *reset_CalibBtn = new QPushButton(tr("重新校准"));
  reset_CalibBtn->setObjectName("reset_CalibBtn");
  power_layout->addWidget(reset_CalibBtn);
  QObject::connect(reset_CalibBtn, &QPushButton::clicked, this, &DevicePanel::calibration);

  QPushButton* poweroff_btn = new QPushButton(tr("关机"));
  poweroff_btn->setObjectName("poweroff_btn");
  power_layout->addWidget(poweroff_btn);
  QObject::connect(poweroff_btn, &QPushButton::clicked, this, &DevicePanel::poweroff);

  if (false && !Hardware::PC()) {
      connect(uiState(), &UIState::offroadTransition, poweroff_btn, &QPushButton::setVisible);
  }

  addItem(power_layout);

  QHBoxLayout* init_layout = new QHBoxLayout();
  init_layout->setSpacing(30);

  QPushButton* init_btn = new QPushButton(tr("Git 拉取 & 重启"));
  init_btn->setObjectName("init_btn");
  init_layout->addWidget(init_btn);
  //QObject::connect(init_btn, &QPushButton::clicked, this, &DevicePanel::reboot);
  QObject::connect(init_btn, &QPushButton::clicked, [&]() {
    if (ConfirmationDialog::confirm(tr("执行 Git 拉取 & 重启？"), tr("是"), this)) {
      QString cmd =
        "bash -c 'cd /data/openpilot && "
        "git fetch && "
        "if git status -uno | grep -q \"Your branch is behind\"; then "
        "git pull && reboot; "
        "else "
        "echo \"Already up to date.\"; "
        "fi'";

      if (!QProcess::startDetached(cmd)) {
        ConfirmationDialog::alert(tr("启动更新过程失败。"), this);
      }
      else {
        ConfirmationDialog::alert(tr("更新过程已启动。如果有更新，设备将重启。"), this);
      }
    }
    });

  QPushButton* default_btn = new QPushButton(tr("恢复默认"));
  default_btn->setObjectName("default_btn");
  init_layout->addWidget(default_btn);
  //QObject::connect(default_btn, &QPushButton::clicked, this, &DevicePanel::poweroff);
  QObject::connect(default_btn, &QPushButton::clicked, [&]() {
    if (ConfirmationDialog::confirm(tr("恢复为默认设置？"), tr("是"), this)) {
      //emit parent->closeSettings();
      QTimer::singleShot(1000, []() {
        printf("恢复为默认设置\n");
        Params().putInt("SoftRestartTriggered", 2);
        printf("恢复为默认设置完成\n");
        });
    }
    });

  QPushButton* remove_mapbox_key_btn = new QPushButton(tr("移除 Mapbox Key"));
  remove_mapbox_key_btn->setObjectName("remove_mapbox_key_btn");
  init_layout->addWidget(remove_mapbox_key_btn);
  QObject::connect(remove_mapbox_key_btn, &QPushButton::clicked, [&]() {
    if (ConfirmationDialog::confirm(tr("移除 Mapbox Key？"), tr("是"), this)) {
      QTimer::singleShot(1000, []() {
        Params().put("MapboxPublicKey", "");
        Params().put("MapboxSecretKey", "");
        });
    }
    });


  setStyleSheet(R"(
    #reboot_btn { height: 120px; border-radius: 15px; background-color: #2CE22C; }
    #reboot_btn:pressed { background-color: #24FF24; }
    #reset_CalibBtn { height: 120px; border-radius: 15px; background-color: #FFBB00; }
    #reset_CalibBtn:pressed { background-color: #FF2424; }
    #poweroff_btn { height: 120px; border-radius: 15px; background-color: #E22C2C; }
    #poweroff_btn:pressed { background-color: #FF2424; }
    #init_btn { height: 120px; border-radius: 15px; background-color: #2C2CE2; }
    #init_btn:pressed { background-color: #2424FF; }
    #default_btn { height: 120px; border-radius: 15px; background-color: #BDBDBD; }
    #default_btn:pressed { background-color: #A9A9A9; }
    #remove_mapbox_key_btn { height: 120px; border-radius: 15px; background-color: #BDBDBD; }
    #remove_mapbox_key_btn:pressed { background-color: #A9A9A9; }
  )");
  addItem(init_layout);

  pair_device = new ButtonControl(tr("Pair Device"), tr("PAIR"),
                                  tr("Pair your device with comma connect (connect.comma.ai) and claim your comma prime offer."));
  connect(pair_device, &ButtonControl::clicked, [=]() {
    PairingPopup popup(this);
    popup.exec();
  });
  addItem(pair_device);

  // offroad-only buttons

  auto dcamBtn = new ButtonControl(tr("Driver Camera"), tr("PREVIEW"),
                                   tr("Preview the driver facing camera to ensure that driver monitoring has good visibility. (vehicle must be off)"));
  connect(dcamBtn, &ButtonControl::clicked, [=]() { emit showDriverView(); });
  addItem(dcamBtn);

  auto retrainingBtn = new ButtonControl(tr("Review Training Guide"), tr("REVIEW"), tr("Review the rules, features, and limitations of openpilot"));
  connect(retrainingBtn, &ButtonControl::clicked, [=]() {
    if (ConfirmationDialog::confirm(tr("Are you sure you want to review the training guide?"), tr("Review"), this)) {
      emit reviewTrainingGuide();
    }
  });
  addItem(retrainingBtn);

  auto statusCalibBtn = new ButtonControl(tr("Calibration Status"), tr("SHOW"), "");
  connect(statusCalibBtn, &ButtonControl::showDescriptionEvent, this, &DevicePanel::updateCalibDescription);
  addItem(statusCalibBtn);

  std::string calib_bytes = params.get("CalibrationParams");
  if (!calib_bytes.empty()) {
    try {
      AlignedBuffer aligned_buf;
      capnp::FlatArrayMessageReader cmsg(aligned_buf.align(calib_bytes.data(), calib_bytes.size()));
      auto calib = cmsg.getRoot<cereal::Event>().getLiveCalibration();
      if (calib.getCalStatus() != cereal::LiveCalibrationData::Status::UNCALIBRATED) {
        double pitch = calib.getRpyCalib()[1] * (180 / M_PI);
        double yaw = calib.getRpyCalib()[2] * (180 / M_PI);
        QString position = QString("%2 %1° %4 %3°")
                           .arg(QString::number(std::abs(pitch), 'g', 1), pitch > 0 ? "↓" : "↑",
                                QString::number(std::abs(yaw), 'g', 1), yaw > 0 ? "←" : "→");
        params.put("DevicePosition", position.toStdString());
      }
    } catch (kj::Exception) {
      qInfo() << "invalid CalibrationParams";
    }
  }

  if (Hardware::TICI()) {
    auto regulatoryBtn = new ButtonControl(tr("Regulatory"), tr("VIEW"), "");
    connect(regulatoryBtn, &ButtonControl::clicked, [=]() {
      const std::string txt = util::read_file("../assets/offroad/fcc.html");
      ConfirmationDialog::rich(QString::fromStdString(txt), this);
    });
    addItem(regulatoryBtn);
  }

  auto translateBtn = new ButtonControl(tr("Change Language"), tr("CHANGE"), "");
  connect(translateBtn, &ButtonControl::clicked, [=]() {
    QMap<QString, QString> langs = getSupportedLanguages();
    QString selection = MultiOptionDialog::getSelection(tr("Select a language"), langs.keys(), langs.key(uiState()->language), this);
    if (!selection.isEmpty()) {
      // put language setting, exit Qt UI, and trigger fast restart
      params.put("LanguageSetting", langs[selection].toStdString());
      qApp->exit(18);
      watchdog_kick(0);
    }
  });
  addItem(translateBtn);

  QObject::connect(uiState()->prime_state, &PrimeState::changed, [this] (PrimeState::Type type) {
    pair_device->setVisible(type == PrimeState::PRIME_TYPE_UNPAIRED);
  });
  QObject::connect(uiState(), &UIState::offroadTransition, [=](bool offroad) {
    for (auto btn : findChildren<ButtonControl *>()) {
      if (btn != pair_device) {
        btn->setEnabled(offroad);
      }
    }
    translateBtn->setEnabled(true);
    statusCalibBtn->setEnabled(true);
  });

}

void DevicePanel::updateCalibDescription() {
  QString desc =
      tr("openpilot requires the device to be mounted within 4° left or right and "
         "within 5° up or 9° down. openpilot is continuously calibrating, resetting is rarely required.");
  std::string calib_bytes = params.get("CalibrationParams");
  if (!calib_bytes.empty()) {
    try {
      AlignedBuffer aligned_buf;
      capnp::FlatArrayMessageReader cmsg(aligned_buf.align(calib_bytes.data(), calib_bytes.size()));
      auto calib = cmsg.getRoot<cereal::Event>().getLiveCalibration();
      if (calib.getCalStatus() != cereal::LiveCalibrationData::Status::UNCALIBRATED) {
        double pitch = calib.getRpyCalib()[1] * (180 / M_PI);
        double yaw = calib.getRpyCalib()[2] * (180 / M_PI);
        desc += tr(" Your device is pointed %1° %2 and %3° %4.")
                    .arg(QString::number(std::abs(pitch), 'g', 1), pitch > 0 ? tr("down") : tr("up"),
                         QString::number(std::abs(yaw), 'g', 1), yaw > 0 ? tr("left") : tr("right"));
      }
    } catch (kj::Exception) {
      qInfo() << "invalid CalibrationParams";
    }
  }
  qobject_cast<ButtonControl *>(sender())->setDescription(desc);
}

void DevicePanel::reboot() {
  if (!uiState()->engaged()) {
    if (ConfirmationDialog::confirm(tr("Are you sure you want to reboot?"), tr("Reboot"), this)) {
      // Check engaged again in case it changed while the dialog was open
      if (!uiState()->engaged()) {
        params.putBool("DoReboot", true);
      }
    }
  } else {
    ConfirmationDialog::alert(tr("Disengage to Reboot"), this);
  }
}

//차선캘리
void execAndReboot(const std::string& cmd) {
    system(cmd.c_str());
    Params().putBool("DoReboot", true);
}

void DevicePanel::calibration() {
  if (!uiState()->engaged()) {
    if (ConfirmationDialog::confirm(tr("Are you sure you want to reset calibration?"), tr("ReCalibration"), this)) {
      if (!uiState()->engaged()) {
        std::thread worker(execAndReboot, "cd /data/params/d_tmp;  rm -f CalibrationParams");
        worker.detach();
      }
    }
  } else {
    ConfirmationDialog::alert(tr("Reboot & Disengage to Calibration"), this);
  }
}

void DevicePanel::poweroff() {
  if (!uiState()->engaged()) {
    if (ConfirmationDialog::confirm(tr("Are you sure you want to power off?"), tr("Power Off"), this)) {
      // Check engaged again in case it changed while the dialog was open
      if (!uiState()->engaged()) {
        params.putBool("DoShutdown", true);
      }
    }
  } else {
    ConfirmationDialog::alert(tr("Disengage to Power Off"), this);
  }
}

void SettingsWindow::showEvent(QShowEvent *event) {
  setCurrentPanel(0);
}

void SettingsWindow::setCurrentPanel(int index, const QString &param) {
  if (!param.isEmpty()) {
    // Check if param ends with "Panel" to determine if it's a panel name
    if (param.endsWith("Panel")) {
      QString panelName = param;
      panelName.chop(5); // Remove "Panel" suffix

      // Find the panel by name
      for (int i = 0; i < nav_btns->buttons().size(); i++) {
        if (nav_btns->buttons()[i]->text() == tr(panelName.toStdString().c_str())) {
          index = i;
          break;
        }
      }
    } else {
      emit expandToggleDescription(param);
    }
  }

  panel_widget->setCurrentIndex(index);
  nav_btns->buttons()[index]->setChecked(true);
}

SettingsWindow::SettingsWindow(QWidget *parent) : QFrame(parent) {

  // setup two main layouts
  sidebar_widget = new QWidget;
  QVBoxLayout *sidebar_layout = new QVBoxLayout(sidebar_widget);
  panel_widget = new QStackedWidget();

  // close button
  QPushButton *close_btn = new QPushButton(tr("×"));
  close_btn->setStyleSheet(R"(
    QPushButton {
      font-size: 140px;
      padding-bottom: 20px;
      border-radius: 100px;
      background-color: #292929;
      font-weight: 400;
    }
    QPushButton:pressed {
      background-color: #3B3B3B;
    }
  )");
  close_btn->setFixedSize(200, 200);
  sidebar_layout->addSpacing(45);
  sidebar_layout->addWidget(close_btn, 0, Qt::AlignCenter);
  QObject::connect(close_btn, &QPushButton::clicked, this, &SettingsWindow::closeSettings);

  // setup panels
  DevicePanel *device = new DevicePanel(this);
  QObject::connect(device, &DevicePanel::reviewTrainingGuide, this, &SettingsWindow::reviewTrainingGuide);
  QObject::connect(device, &DevicePanel::showDriverView, this, &SettingsWindow::showDriverView);

  TogglesPanel *toggles = new TogglesPanel(this);
  QObject::connect(this, &SettingsWindow::expandToggleDescription, toggles, &TogglesPanel::expandToggleDescription);

  auto networking = new Networking(this);
  QObject::connect(uiState()->prime_state, &PrimeState::changed, networking, &Networking::setPrimeType);

  QList<QPair<QString, QWidget *>> panels = {
    {tr("Device"), device},
    {tr("Network"), networking},
    {tr("Toggles"), toggles},
  };
  if(Params().getBool("SoftwareMenu")) {
    panels.append({tr("Software"), new SoftwarePanel(this)});
  }
  if(false) {
    panels.append({tr("Firehose"), new FirehosePanel(this)});
  }
  panels.append({ tr("萝卜"), new CarrotPanel(this) });
  panels.append({ tr("开发"), new DeveloperPanel(this) });

  nav_btns = new QButtonGroup(this);
  for (auto &[name, panel] : panels) {
    QPushButton *btn = new QPushButton(name);
    btn->setCheckable(true);
    btn->setChecked(nav_btns->buttons().size() == 0);
    btn->setStyleSheet(R"(
      QPushButton {
        color: grey;
        border: none;
        background: none;
        font-size: 65px;
        font-weight: 500;
      }
      QPushButton:checked {
        color: white;
      }
      QPushButton:pressed {
        color: #ADADAD;
      }
    )");
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    nav_btns->addButton(btn);
    sidebar_layout->addWidget(btn, 0, Qt::AlignRight);

    const int lr_margin = name != tr("Network") ? 50 : 0;  // Network panel handles its own margins
    panel->setContentsMargins(lr_margin, 25, lr_margin, 25);

    ScrollView *panel_frame = new ScrollView(panel, this);
    panel_widget->addWidget(panel_frame);

    QObject::connect(btn, &QPushButton::clicked, [=, w = panel_frame]() {
      btn->setChecked(true);
      panel_widget->setCurrentWidget(w);
    });
  }
  sidebar_layout->setContentsMargins(50, 50, 100, 50);

  // main settings layout, sidebar + main panel
  QHBoxLayout *main_layout = new QHBoxLayout(this);

  sidebar_widget->setFixedWidth(500);
  main_layout->addWidget(sidebar_widget);
  main_layout->addWidget(panel_widget);

  setStyleSheet(R"(
    * {
      color: white;
      font-size: 50px;
    }
    SettingsWindow {
      background-color: black;
    }
    QStackedWidget, ScrollView {
      background-color: #292929;
      border-radius: 30px;
    }
  )");
}


#include <QScroller>
#include <QListWidget>

static QStringList get_list(const char* path) {
  QStringList stringList;
  QFile textFile(path);
  if (textFile.open(QIODevice::ReadOnly)) {
    QTextStream textStream(&textFile);
    while (true) {
      QString line = textStream.readLine();
      if (line.isNull()) {
        break;
      } else {
        stringList.append(line);
      }
    }
  }
  return stringList;
}

CarrotPanel::CarrotPanel(QWidget* parent) : QWidget(parent) {
  main_layout = new QStackedLayout(this);
  homeScreen = new QWidget(this);
  carrotLayout = new QVBoxLayout(homeScreen);
  carrotLayout->setMargin(10);

  QHBoxLayout* select_layout = new QHBoxLayout();
  select_layout->setSpacing(10);


  QPushButton* start_btn = new QPushButton(tr("开始"));
  start_btn->setObjectName("start_btn");
  QObject::connect(start_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 0;
    this->togglesCarrot(0);
    updateButtonStyles();
  });

  QPushButton* cruise_btn = new QPushButton(tr("巡航"));
  cruise_btn->setObjectName("cruise_btn");
  QObject::connect(cruise_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 1;
    this->togglesCarrot(1);
    updateButtonStyles();
  });

  QPushButton* speed_btn = new QPushButton(tr("速度"));
  speed_btn->setObjectName("speed_btn");
  QObject::connect(speed_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 2;
    this->togglesCarrot(2);
    updateButtonStyles();
  });

  QPushButton* latLong_btn = new QPushButton(tr("调节"));
  latLong_btn->setObjectName("latLong_btn");
  QObject::connect(latLong_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 3;
    this->togglesCarrot(3);
    updateButtonStyles();
  });

  QPushButton* disp_btn = new QPushButton(tr("显示"));
  disp_btn->setObjectName("disp_btn");
  QObject::connect(disp_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 4;
    this->togglesCarrot(4);
    updateButtonStyles();
  });

  QPushButton* path_btn = new QPushButton(tr("轨迹"));
  path_btn->setObjectName("path_btn");
  QObject::connect(path_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 5;
    this->togglesCarrot(5);
    updateButtonStyles();
  });


  updateButtonStyles();

  select_layout->addWidget(start_btn);
  select_layout->addWidget(cruise_btn);
  select_layout->addWidget(speed_btn);
  select_layout->addWidget(latLong_btn);
  select_layout->addWidget(disp_btn);
  select_layout->addWidget(path_btn);
  carrotLayout->addLayout(select_layout, 0);

  QWidget* toggles = new QWidget();
  QVBoxLayout* toggles_layout = new QVBoxLayout(toggles);

  cruiseToggles = new ListWidget(this);
  cruiseToggles->addItem(new CValueControl("CruiseButtonMode", "按钮：定速巡航模式", "0:普通,1:用户1,2:用户2", 0, 2, 1));
  cruiseToggles->addItem(new CValueControl("CancelButtonMode", "按钮：取消模式", "0:长按,1:长按+车道保持", 0, 1, 1));
  cruiseToggles->addItem(new CValueControl("LfaButtonMode", "按钮：LFA模式", "0:普通,1:减速&停车&前车准备", 0, 1, 1));
  cruiseToggles->addItem(new CValueControl("CruiseSpeedUnitBasic", "按钮：定速单位(基础)", "1:公里/小时, 2:英里/小时", 1, 20, 1));
  cruiseToggles->addItem(new CValueControl("CruiseSpeedUnit", "按钮：定速单位(高级)", "1:公里/小时, 2:英里/小时", 1, 20, 1));
  cruiseToggles->addItem(new CValueControl("CruiseEcoControl", "定速巡航：节能控制(4km/h)", "临时提高设定速度以提高燃油效率", 0, 10, 1));
  cruiseToggles->addItem(new CValueControl("AutoSpeedUptoRoadSpeedLimit", "定速巡航：可超道路限制的百分比(0%)", "巡航速度可超过道路限制的百分比x%", 0, 200, 10));
  cruiseToggles->addItem(new CValueControl("TFollowGap1", "跟车时间GAP1(110)x0.01s", "", 70, 300, 5));
  cruiseToggles->addItem(new CValueControl("TFollowGap2", "跟车时间GAP2(120)x0.01s", "", 70, 300, 5));
  cruiseToggles->addItem(new CValueControl("TFollowGap3", "跟车时间GAP3(160)x0.01s", "", 70, 300, 5));
  cruiseToggles->addItem(new CValueControl("TFollowGap4", "跟车时间GAP4(180)x0.01s", "", 70, 300, 5));
  cruiseToggles->addItem(new CValueControl("DynamicTFollow", "动态跟车GAP控制", "", 0, 100, 5));
  cruiseToggles->addItem(new CValueControl("DynamicTFollowLC", "动态跟车GAP控制(变道)", "", 0, 100, 5));
  cruiseToggles->addItem(new CValueControl("MyDrivingMode", "驾驶模式选择", "1:经济,2:安全,3:普通,4:激进", 1, 4, 1));
  cruiseToggles->addItem(new CValueControl("MyDrivingModeAuto", "驾驶模式自动", "0:关闭,1:开启(仅普通模式)", 0, 1, 1));
  cruiseToggles->addItem(new CValueControl("TrafficLightDetectMode", "红绿灯检测模式", "0:无,1:仅停止,2:停走模式", 0, 2, 1));

  //cruiseToggles->addItem(new CValueControl("CruiseSpeedMin", "CRUISE: Speed Lower limit(10)", "Cruise control MIN speed", 5, 50, 1));
  //cruiseToggles->addItem(new CValueControl("AutoResumeFromGas", "GAS CRUISE ON: Use", "Auto Cruise on when GAS pedal released, 60% Gas Cruise On automatically", 0, 3, 1));
  //cruiseToggles->addItem(new CValueControl("AutoResumeFromGasSpeed", "GAS CRUISE ON: Speed(30)", "Driving speed exceeds the set value, Cruise ON", 20, 140, 5));
  //cruiseToggles->addItem(new CValueControl("TFollowSpeedAddM", "GAP: Additional TFs 40km/h(0)x0.01s", "Speed-dependent additional max(100km/h) TFs", -100, 200, 5));
  //cruiseToggles->addItem(new CValueControl("TFollowSpeedAdd", "GAP: Additional TFs 100Km/h(0)x0.01s", "Speed-dependent additional max(100km/h) TFs", -100, 200, 5));
  //cruiseToggles->addItem(new CValueControl("MyEcoModeFactor", "DRIVEMODE: ECO Accel ratio(80%)", "Acceleration ratio in ECO mode", 10, 95, 5));
  //cruiseToggles->addItem(new CValueControl("MySafeModeFactor", "DRIVEMODE: SAFE ratio(60%)", "Accel/StopDistance/DecelRatio/Gap control ratio", 10, 90, 10));
  //cruiseToggles->addItem(new CValueControl("MyHighModeFactor", "DRIVEMODE: HIGH ratio(100%)", "AccelRatio control ratio", 100, 300, 10));

  latLongToggles = new ListWidget(this);
  latLongToggles->addItem(new CValueControl("UseLaneLineSpeed", "车道线模式速度(0)", "车道线模式，使用 lat_mpc 控制", 0, 200, 5));
  latLongToggles->addItem(new CValueControl("UseLaneLineCurveSpeed", "车道线模式弯道速度(0)", "车道线模式，仅在高速时生效", 0, 200, 5));
  latLongToggles->addItem(new CValueControl("AdjustLaneOffset", "车道偏移调整(0)cm", "", 0, 500, 5));
  latLongToggles->addItem(new CValueControl("LaneChangeNeedTorque", "变道扭矩需求", "-1:禁用变道, 0:不需要扭矩, 1:需要扭矩", -1, 1, 1));
  latLongToggles->addItem(new CValueControl("LaneChangeDelay", "变道延迟", "单位 x0.1秒", 0, 100, 5));
  latLongToggles->addItem(new CValueControl("LaneChangeBsd", "变道 BSD 设置", "-1:忽略BSD, 0:检测BSD, 1:阻止方向盘扭矩", -1, 1, 1));
  latLongToggles->addItem(new CValueControl("CustomSR", "横向: 自定义方向盘比x0.1(0)", "自定义转向比", 0, 300, 1));
  latLongToggles->addItem(new CValueControl("SteerRatioRate", "横向: 转向比应用速率x0.01(100)", "转向比应用速率", 30, 170, 1));
  latLongToggles->addItem(new CValueControl("PathOffset", "横向: 路径偏移", "(-)左偏, (+)右偏", -150, 150, 1));
  latLongToggles->addItem(new CValueControl("SteerActuatorDelay", "横向: 转向执行器延迟(30)", "x0.01, 0:实时延迟", 0, 100, 1));
  latLongToggles->addItem(new CValueControl("LateralTorqueCustom", "横向: 自定义扭矩模式(0)", "", 0, 2, 1));
  latLongToggles->addItem(new CValueControl("LateralTorqueAccelFactor", "横向: 扭矩加速度因子(2500)", "", 1000, 6000, 10));
  latLongToggles->addItem(new CValueControl("LateralTorqueFriction", "横向: 扭矩摩擦补偿(100)", "", 0, 1000, 10));
  latLongToggles->addItem(new CValueControl("CustomSteerMax", "横向: 自定义最大转向力(0)", "", 0, 30000, 5));
  latLongToggles->addItem(new CValueControl("CustomSteerDeltaUp", "横向: 转向增量上升(0)", "", 0, 50, 1));
  latLongToggles->addItem(new CValueControl("CustomSteerDeltaDown", "横向: 转向增量下降(0)", "", 0, 50, 1));
  latLongToggles->addItem(new CValueControl("LongTuningKpV", "纵向: P增益(100)", "", 0, 150, 5));
  latLongToggles->addItem(new CValueControl("LongTuningKiV", "纵向: I增益(0)", "", 0, 2000, 5));
  latLongToggles->addItem(new CValueControl("LongTuningKf", "纵向: FF增益(100)", "", 0, 200, 5));
  latLongToggles->addItem(new CValueControl("LongActuatorDelay", "纵向: 执行器延迟(20)", "", 0, 200, 5));
  latLongToggles->addItem(new CValueControl("VEgoStopping", "纵向: 车辆停止因子(50)", "停止因子", 1, 100, 5));
  latLongToggles->addItem(new CValueControl("RadarReactionFactor", "纵向: 雷达反应因子(100)", "", 0, 200, 10));
  latLongToggles->addItem(new CValueControl("StoppingAccel", "纵向: 停车启动加速度x0.01(-40)", "", -100, 0, 5));
  latLongToggles->addItem(new CValueControl("StopDistanceCarrot", "纵向: 停车距离 (600)cm", "", 300, 1000, 10));
  latLongToggles->addItem(new CValueControl("JLeadFactor3", "纵向: 加加速度前车因子(0)", "x0.01", 0, 100, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals0", "加速:0km/h(160)", "指定速度下所需加速度(x0.01m/s^2)", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals1", "加速:10km/h(160)", "指定速度下所需加速度(x0.01m/s^2)", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals2", "加速:40km/h(120)", "指定速度下所需加速度(x0.01m/s^2)", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals3", "加速:60km/h(100)", "指定速度下所需加速度(x0.01m/s^2)", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals4", "加速:80km/h(80)", "指定速度下所需加速度(x0.01m/s^2)", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals5", "加速:110km/h(70)", "指定速度下所需加速度(x0.01m/s^2)", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals6", "加速:140km/h(60)", "指定速度下所需加速度(x0.01m/s^2)", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("MaxAngleFrames", "最大转角帧数(89)", "89:默认, 仪表盘转向错误 85~87", 80, 100, 1));

  //latLongToggles->addItem(new CValueControl("AutoLaneChangeSpeed", "LaneChangeSpeed(20)", "", 1, 100, 5));
  //latLongToggles->addItem(new CValueControl("JerkStartLimit", "LONG: JERK START(10)x0.1", "Starting Jerk.", 1, 50, 1));
  //latLongToggles->addItem(new CValueControl("LongitudinalTuningApi", "LONG: ControlType", "0:velocity pid, 1:accel pid, 2:accel pid(comma)", 0, 2, 1));
  //latLongToggles->addItem(new CValueControl("StartAccelApply", "LONG: StartingAccel 2.0x(0)%", "정지->출발시 가속도의 가속율을 지정합니다 0: 사용안함.", 0, 100, 10));
  //latLongToggles->addItem(new CValueControl("StopAccelApply", "LONG: StoppingAccel -2.0x(0)%", "정지유지시 브레이크압을 조정합니다. 0: 사용안함. ", 0, 100, 10));
  //latLongToggles->addItem(new CValueControl("TraffStopDistanceAdjust", "LONG: TrafficStopDistance adjust(150)cm", "", -1000, 1000, 10));
  //latLongToggles->addItem(new CValueControl("CruiseMinVals", "DECEL:(120)", "Sets the deceleration rate.(x0.01m/s^2)", 50, 250, 5));

  dispToggles = new ListWidget(this);
  dispToggles->addItem(new CValueControl("ShowDebugUI", "调试信息", "", 0, 2, 1));
  dispToggles->addItem(new CValueControl("ShowTpms", "胎压信息", "", 0, 3, 1));
  dispToggles->addItem(new CValueControl("ShowDateTime", "时间信息", "0:无,1:时间/日期,2:仅时间,3:仅日期", 0, 3, 1));
  dispToggles->addItem(new CValueControl("ShowPathEnd", "轨迹终点", "0:无,1:显示", 0, 1, 1));
  dispToggles->addItem(new CValueControl("ShowDeviceState", "设备状态", "0:无,1:显示", 0, 1, 1));
  dispToggles->addItem(new CValueControl("ShowLaneInfo", "车道信息", "-1:无,0:轨迹,1:轨迹+车道线,2:轨迹+车道线+路沿", -1, 2, 1));
  dispToggles->addItem(new CValueControl("ShowRadarInfo", "雷达信息", "0:无,1:显示,2:相对位置,3:静止车辆", 0, 3, 1));
  dispToggles->addItem(new CValueControl("ShowRouteInfo", "路线信息", "0:无,1:显示", 0, 1, 1));
  dispToggles->addItem(new CValueControl("ShowPlotMode", "调试图表", "", 0, 10, 1));
  dispToggles->addItem(new CValueControl("ShowCustomBrightness", "亮度比例", "", 0, 100, 10));

  //dispToggles->addItem(new CValueControl("ShowHudMode", "Display Mode", "0:Frog,1:APilot,2:Bottom,3:Top,4:Left,5:Left-Bottom", 0, 5, 1));
  //dispToggles->addItem(new CValueControl("ShowSteerRotate", "Handle rotate", "0:None,1:Rotate", 0, 1, 1));
  //dispToggles->addItem(new CValueControl("ShowAccelRpm", "Accel meter", "0:None,1:Display,1:Accel+RPM", 0, 2, 1));
  //dispToggles->addItem(new CValueControl("ShowTpms", "TPMS", "0:None,1:Display", 0, 1, 1));
  //dispToggles->addItem(new CValueControl("ShowSteerMode", "Handle Display Mode", "0:Black,1:Color,2:None", 0, 2, 1));
  //dispToggles->addItem(new CValueControl("ShowConnInfo", "APM connection", "0:NOne,1:Display", 0, 1, 1));
  //dispToggles->addItem(new CValueControl("ShowBlindSpot", "BSD Info", "0:None,1:Display", 0, 1, 1));
  //dispToggles->addItem(new CValueControl("ShowGapInfo", "GAP Info", "0:None,1:Display", -1, 1, 1));
  //dispToggles->addItem(new CValueControl("ShowDmInfo", "DM Info", "0:None,1:Display,-1:Disable(Reboot)", -1, 1, 1));

  pathToggles = new ListWidget(this);
  pathToggles->addItem(new CValueControl("ShowPathColorCruiseOff", "轨迹颜色：未开启巡航", "(+10:描边)0:红,1:橙,2:黄,3:绿,4:蓝,5:靛青,6:紫,7:棕,8:白,9:黑", 0, 19, 1));
  pathToggles->addItem(new CValueControl("ShowPathMode", "轨迹模式：无车道线", "0:普通,1,2:推荐,3,4:^^,5,6:推荐,7,8:^^,9,10,11,12:平滑^^", 0, 15, 1));
  pathToggles->addItem(new CValueControl("ShowPathColor", "轨迹颜色：无车道线", "(+10:描边)0:红,1:橙,2:黄,3:绿,4:蓝,5:靛青,6:紫,7:棕,8:白,9:黑", 0, 19, 1));
  pathToggles->addItem(new CValueControl("ShowPathModeLane", "轨迹模式：有车道线", "0:普通,1,2:推荐,3,4:^^,5,6:推荐,7,8:^^,9,10,11,12:平滑^^", 0, 15, 1));
  pathToggles->addItem(new CValueControl("ShowPathColorLane", "轨迹颜色：有车道线", "(+10:描边)0:红,1:橙,2:黄,3:绿,4:蓝,5:靛青,6:紫,7:棕,8:白,9:黑", 0, 19, 1));
  pathToggles->addItem(new CValueControl("ShowPathWidth", "轨迹宽度比例(100%)", "", 10, 200, 10));


  startToggles = new ListWidget(this);
  QString selected = QString::fromStdString(Params().get("CarSelected3"));
  QPushButton* selectCarBtn = new QPushButton(selected.length() > 1 ? selected : tr("选择您的车辆"));
  selectCarBtn->setObjectName("selectCarBtn");
  selectCarBtn->setStyleSheet(R"(
    QPushButton {
      margin-top: 20px; margin-bottom: 20px; padding: 10px; height: 120px; border-radius: 15px;
      color: #FFFFFF; background-color: #2C2CE2;
    }
    QPushButton:pressed {
      background-color: #2424FF;
    }
  )");
  //selectCarBtn->setFixedSize(350, 100);
  connect(selectCarBtn, &QPushButton::clicked, [=]() {
    QString selected = QString::fromStdString(Params().get("CarSelected3"));


    QStringList all_items = get_list((QString::fromStdString(Params().getParamPath()) + "/SupportedCars").toStdString().c_str());
    all_items.append(get_list((QString::fromStdString(Params().getParamPath()) + "/SupportedCars_gm").toStdString().c_str()));
    all_items.append(get_list((QString::fromStdString(Params().getParamPath()) + "/SupportedCars_toyota").toStdString().c_str()));
    all_items.append(get_list((QString::fromStdString(Params().getParamPath()) + "/SupportedCars_mazda").toStdString().c_str()));
    all_items.append(get_list((QString::fromStdString(Params().getParamPath()) + "/SupportedCars_tesla").toStdString().c_str()));
    all_items.append(get_list((QString::fromStdString(Params().getParamPath()) + "/SupportedCars_honda").toStdString().c_str()));
    all_items.append(get_list((QString::fromStdString(Params().getParamPath()) + "/SupportedCars_volkswagen").toStdString().c_str()));
    QMap<QString, QStringList> car_groups;
    for (const QString& car : all_items) {
      QStringList parts = car.split(" ", QString::SkipEmptyParts);
      if (!parts.isEmpty()) {
        QString manufacturer = parts.first();
        car_groups[manufacturer].append(car);
      }
    }

        QStringList manufacturers = car_groups.keys();
    QString selectedManufacturer = MultiOptionDialog::getSelection("选择厂商", manufacturers, manufacturers.isEmpty() ? "" : manufacturers.first(), this);

    if (!selectedManufacturer.isEmpty()) {
      QStringList cars = car_groups[selectedManufacturer];
      QString selectedCar = MultiOptionDialog::getSelection("选择您的车辆", cars, selected, this);

      if (!selectedCar.isEmpty()) {
        if (selectedCar == "[ 未选择 ]") {
          Params().remove("CarSelected3");
        } else {
          printf("已选择车辆: %s\n", selectedCar.toStdString().c_str());
          Params().put("CarSelected3", selectedCar.toStdString());
          QTimer::singleShot(1000, []() {
            Params().putInt("SoftRestartTriggered", 1);
          });
          ConfirmationDialog::alert(selectedCar, this);
        }
        selected = QString::fromStdString(Params().get("CarSelected3"));
        selectCarBtn->setText((selected.isEmpty() || selected == "[ 未选择 ]") ? tr("选择您的车辆") : selected);
      }
    }
  });


  startToggles->addItem(selectCarBtn);
  startToggles->addItem(new CValueControl("HyundaiCameraSCC", "现代: 摄像头SCC", "1:连接SCC的CAN线到摄像头, 2:同步定速状态, 3:原厂长控，胜达设置为0", 0, 3, 1));
  startToggles->addItem(new CValueControl("CanfdHDA2", "CANFD: HDA2 模式", "1:HDA2, 2:HDA2+盲点监测，胜达设置为2", 0, 2, 1));
  startToggles->addItem(new CValueControl("EnableRadarTracks", "启用雷达追踪", "1:启用雷达追踪, -1,2:禁用 (始终使用HKG SCC雷达)，胜达设置为1", -1, 2, 1));
  startToggles->addItem(new CValueControl("AutoCruiseControl", "自动巡航控制", "自动巡航总开关,0-关,>1开,>1 softmode1 否则softmode2", 0, 3, 1));
  startToggles->addItem(new CValueControl("CruiseOnDist", "定速: 自动开启距离(0cm)", "当油门/刹车未踩下时，前车靠近自动开启定速", 0, 2500, 50));
  startToggles->addItem(new CValueControl("AutoEngage", "车辆启动时自动开启的功能", "1:车道保持启用, 2:车道保持+定速启用", 0, 2, 1));
  startToggles->addItem(new CValueControl("AutoGasTokSpeed", "轻踩油门开启巡航的速度", "当车速大于此速度时，轻点油门可自动开启巡航，前提是'自动巡航控制'必须要打开", 0, 200, 5));
  startToggles->addItem(new CValueControl("SpeedFromPCM", "从PCM读取定速速度(2)", "丰田必须设为1, 本田设为3，默认为2", 0, 3, 1));
  startToggles->addItem(new CValueControl("SoundVolumeAdjust", "提示音音量(100%)", "", 5, 200, 5));
  startToggles->addItem(new CValueControl("SoundVolumeAdjustEngage", "接管提示音音量(10%)", "", 5, 200, 5));
  startToggles->addItem(new CValueControl("MaxTimeOffroadMin", "熄屏时间 (分钟)", "", 1, 600, 10));
  startToggles->addItem(new CValueControl("EnableConnect", "启用远程连接", "您的设备可能会被 Comma 封禁", 0, 2, 1));
  startToggles->addItem(new CValueControl("MapboxStyle", "地图样式(0)", "", 0, 2, 1));
  startToggles->addItem(new CValueControl("RecordRoadCam", "记录前置摄像头(0)", "1:前置, 2:前置+广角前置", 0, 2, 1));
  startToggles->addItem(new CValueControl("HDPuse", "使用HDP(CCNC)(0)", "1:使用APN时, 2:始终启用", 0, 2, 1));
  startToggles->addItem(new CValueControl("NNFF", "NNFF", "Twilsonco的NNFF(需重启)", 0, 1, 1));
  startToggles->addItem(new CValueControl("NNFFLite", "NNFF精简版", "Twilsonco的NNFF-Lite(需重启)", 0, 1, 1));
  startToggles->addItem(new CValueControl("AutoGasSyncSpeed", "踩油门自动更新巡航速度", "0-关闭，1-开启，当开启此功能时，如果踩油门且当前车速高于巡航速度，巡航速度会自动调整为当前车速", 0, 1, 1));
  startToggles->addItem(new CValueControl("DisableMinSteerSpeed", "禁用最小转向速度限制", "", 0, 1, 1));
  startToggles->addItem(new CValueControl("DisableDM", "禁用疲劳监测(DM)", "", 0, 1, 1));
  startToggles->addItem(new CValueControl("HotspotOnBoot", "开机启用热点", "", 0, 1, 1));
  startToggles->addItem(new CValueControl("SoftwareMenu", "启用软件菜单", "", 0, 1, 1));
  startToggles->addItem(new CValueControl("IsLdwsCar", "是否LDWS车型", "", 0, 1, 1));

  //startToggles->addItem(new CValueControl("CarrotCountDownSpeed", "导航倒计时速度(10)", "", 0, 200, 5));
  //startToggles->addItem(new ParamControl("NoLogging", "禁用日志记录", "", this));
  //startToggles->addItem(new ParamControl("LaneChangeNeedTorque", "变道: 需要方向盘施力", "", this));
  //startToggles->addItem(new CValueControl("LaneChangeLaneCheck", "变道: 检查车道存在", "(0:否,1:车道,2:+路肩)", 0, 2, 1));

  speedToggles = new ListWidget(this);
  speedToggles->addItem(new CValueControl("AutoCurveSpeedLowerLimit", "弯道: 最低速度限制(30)", "接近弯道时降低车速, 最低速度", 30, 200, 5));
  speedToggles->addItem(new CValueControl("AutoCurveSpeedFactor", "弯道: 自动控制系数(100%)", "", 50, 300, 1));
  speedToggles->addItem(new CValueControl("AutoCurveSpeedAggressiveness", "弯道: 激进程度(100%)", "", 50, 300, 1));
  speedToggles->addItem(new CValueControl("SameSpiCamFilter", "相同测速数据过滤(0)", "0:关闭, 1:打开", 0, 1, 1));
  speedToggles->addItem(new CValueControl("AutoRoadSpeedLimitOffset", "道路限速偏移(-1)", "-1:不启用, 道路限速+偏移", -1, 100, 1));
  speedToggles->addItem(new CValueControl("AutoRoadSpeedAdjust", "自动调整道路限速(50%)", "当道路限速发生变化时，按此比例平滑调整到新限速,<0时，则用限速*测速点安全系数或限速+偏移", -1, 100, 5));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedCtrlEnd", "测速点减速结束点(6秒)", "设置减速完成点, 数值越大减速越提前完成", 3, 20, 1));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedCtrlMode", "导航限速控制模式(3)", "0:关闭, 1:测速摄像头, 2:+减速带, 3:+移动测速", 0, 3, 1));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedDecelRate", "测速点减速率x0.01m/s²(80)", "数值越小, 越早开始减速", 10, 200, 10));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedSafetyFactor", "测速点安全系数(105%)", "车速可超导航限速的比例", 80, 120, 1));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedBumpTime", "减速带时间距离(1秒)", "", 1, 50, 1));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedBumpSpeed", "减速带通过速度(35Km/h)", "", 10, 100, 5));
  speedToggles->addItem(new CValueControl("AutoNaviCountDownMode", "导航倒计时模式(2)", "0:关闭, 1:转向+摄像头, 2:转向+摄像头+减速带", 0, 2, 1));
  speedToggles->addItem(new CValueControl("TurnSpeedControlMode", "转弯速度控制模式(1)", "0:关闭, 1:视觉, 2:视觉+路线, 3:路线", 0, 3, 1));
  speedToggles->addItem(new CValueControl("MapTurnSpeedFactor", "地图转弯速度系数(100)", "", 50, 300, 5));
  speedToggles->addItem(new CValueControl("AutoTurnControl", "ATC: 自动转弯控制(0)", "0:无, 1:变道, 2:变道+减速, 3:减速", 0, 3, 1));
  speedToggles->addItem(new CValueControl("AutoTurnControlSpeedTurn", "ATC: 转弯速度(20)", "0:无, 转弯速度", 0, 100, 5));
  speedToggles->addItem(new CValueControl("AutoTurnControlTurnEnd", "ATC: 转弯控制距离时间(6)", "距离=速度*时间", 0, 30, 1));
  speedToggles->addItem(new CValueControl("AutoTurnMapChange", "ATC 自动地图切换(0)", "", 0, 1, 1));
  //new
  speedToggles->addItem(new CValueControl("AutoForkDistOffset", "ATC 自动变道距离偏移(0m)", "", 0, 500, 5));
  speedToggles->addItem(new CValueControl("AutoTurnDistOffset", "ATC 自动转弯距离偏移(0m)", "", -100, 200, 1));
  speedToggles->addItem(new CValueControl("AutoTurnInNotRoadEdge", "ATC 在中间车道允许自动变道(0)", "", 0, 1, 1));
  speedToggles->addItem(new CValueControl("ContinuousLaneChange", "ATC 允许自动连续变道(0)", "", 0, 1, 1));
  speedToggles->addItem(new CValueControl("AutoUpRoadLimit", "自动提高低于60km/h的公路限速(0)", "", 0, 1, 1));
  speedToggles->addItem(new CValueControl("AutoUpRoadLimit40KMH", "低于40km/h的公路提速偏移(15km/h)", "", 0, 50, 1));
  speedToggles->addItem(new CValueControl("AutoUpHighwayRoadLimit", "自动提高低于60km/h的匝道限速(0)", "", 0, 1, 1));
  speedToggles->addItem(new CValueControl("AutoUpHighwayRoadLimit40KMH", "低于40km/h的匝道提速偏移(15km/h)", "", 0, 50, 1));

  toggles_layout->addWidget(cruiseToggles);
  toggles_layout->addWidget(latLongToggles);
  toggles_layout->addWidget(dispToggles);
  toggles_layout->addWidget(pathToggles);
  toggles_layout->addWidget(startToggles);
  toggles_layout->addWidget(speedToggles);
  ScrollView* toggles_view = new ScrollView(toggles, this);
  carrotLayout->addWidget(toggles_view, 1);

  homeScreen->setLayout(carrotLayout);
  main_layout->addWidget(homeScreen);
  main_layout->setCurrentWidget(homeScreen);

  togglesCarrot(0);
}

void CarrotPanel::togglesCarrot(int widgetIndex) {
  startToggles->setVisible(widgetIndex == 0);
  cruiseToggles->setVisible(widgetIndex == 1);
  speedToggles->setVisible(widgetIndex == 2);
  latLongToggles->setVisible(widgetIndex == 3);
  dispToggles->setVisible(widgetIndex == 4);
  pathToggles->setVisible(widgetIndex == 5);
}

void CarrotPanel::updateButtonStyles() {
  QString styleSheet = R"(
      #start_btn, #cruise_btn, #speed_btn, #latLong_btn ,#disp_btn, #path_btn {
        height: 120px; border-radius: 15px; background-color: #393939;
      }
      #start_btn:pressed, #cruise_btn:pressed, #speed_btn:pressed, #latLong_btn:pressed, #disp_btn:pressed, #path_btn:pressed {
        background-color: #4a4a4a;
      }
  )";

  switch (currentCarrotIndex) {
  case 0:
    styleSheet += "#start_btn { background-color: #33ab4c; }";
    break;
  case 1:
    styleSheet += "#cruise_btn { background-color: #33ab4c; }";
    break;
  case 2:
    styleSheet += "#speed_btn { background-color: #33ab4c; }";
    break;
  case 3:
    styleSheet += "#latLong_btn { background-color: #33ab4c; }";
    break;
  case 4:
    styleSheet += "#disp_btn { background-color: #33ab4c; }";
    break;
  case 5:
    styleSheet += "#path_btn { background-color: #33ab4c; }";
    break;
  }

  setStyleSheet(styleSheet);
}


CValueControl::CValueControl(const QString& params, const QString& title, const QString& desc, int min, int max, int unit)
  : AbstractControl(title, desc), m_params(params), m_min(min), m_max(max), m_unit(unit) {

  label.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  QString btnStyle = R"(
    QPushButton {
      padding: 0;
      border-radius: 50px;
      font-size: 20px;
      font-weight: 300;
      color: #E4E4E4;
      background-color: #393939;
    }
    QPushButton:pressed {
      background-color: #4a4a4a;
    }
  )";

  btnminus.setStyleSheet(btnStyle);
  btnplus.setStyleSheet(btnStyle);
  btnminus.setFixedSize(100, 100);
  btnplus.setFixedSize(100, 100);
  btnminus.setText("－");
  btnplus.setText("＋");
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  connect(&btnminus, &QPushButton::released, this, &CValueControl::decreaseValue);
  connect(&btnplus, &QPushButton::released, this, &CValueControl::increaseValue);

  refresh();
}

void CValueControl::showEvent(QShowEvent* event) {
  AbstractControl::showEvent(event);
  refresh();
}

void CValueControl::refresh() {
  label.setText(QString::fromStdString(Params().get(m_params.toStdString())));
}

void CValueControl::adjustValue(int delta) {
  int value = QString::fromStdString(Params().get(m_params.toStdString())).toInt();
  value = qBound(m_min, value + delta, m_max);
  Params().putInt(m_params.toStdString(), value);
  refresh();
}

void CValueControl::increaseValue() {
  adjustValue(m_unit);
}

void CValueControl::decreaseValue() {
  adjustValue(-m_unit);
}
