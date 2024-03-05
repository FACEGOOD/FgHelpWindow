/*
 * Copyright (C) 2023-2033 FACEGOOD, Inc. All Rights Reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/** @file main.cpp
 *  @brief 
 *
 *  @author Nero
 */
#include <QDebug>
#include <QTranslator>
#include <QtWidgets/QApplication>
#include <QFileInfo>
#include <fstream>
#include <vector>
#include "FgHelpWidget.h"
#include "json.hpp"
#include "FgEventTracking.h"

void loadConfig(const std::string& path, nlohmann::json& config) {
  std::ifstream in(path);
  if (in.is_open()) {
    in >> config;
  }
  in.close();
}

void parseConfig(const nlohmann::json& config, std::string& language,
                 QString& docURL, QString& faqURL, std::vector<std::pair< QString, QString >> &keyMap) {
  if (!config.contains(language)) {
    language = "en-US";
  }
  nlohmann::json subConfig = config[language];
  if (subConfig.contains("doc")) {
    std::string tempStr = subConfig["doc"].get<std::string>();
    docURL = QString::fromStdString(tempStr);
  }

  if (subConfig.contains("faq")) {
    std::string tempStr = subConfig["faq"].get<std::string>();
    faqURL = QString::fromStdString(tempStr);
  }

  if (subConfig.contains("doc")) {
    std::vector<std::pair<std::string, std::string>> tempKeys =
        subConfig["shortcutKeys"]
            .get<std::vector<std::pair<std::string, std::string>>>();

    for (int i = 0; i < tempKeys.size(); ++i) {
      keyMap.push_back({QString::fromStdString(tempKeys[i].first),QString::fromStdString(tempKeys[i].second)});
    }
  }
}

int main(int argc, char* argv[]) {
  //:/FgHelpWindow/icons/fghelpwindow_zh.qm

  QApplication a(argc, argv);

  QFont font;
  font.setPointSize(16);
  font.setFamily("Microsoft YaHei");
  a.setFont(font);

  //if (argc != 3) return -1;

  std::string configPath = argv[1];
  //  "D:/Repositories_Projects/nero/FgHelpWindow/x64/Debug/helpconfig.json";

  QFileInfo fileInfo(configPath.c_str());
  std::string filename = fileInfo.fileName().toStdString();
  FgEventTracking::init(filename);
  std::string language = argv[2];

  QTranslator trans;

  if (language.compare("zh-CN") == 0) {
    qDebug() << "Chinese";
    if (trans.load(":/FgHelpWindow/icons/fghelpwindow_zh.qm")) {
      a.installTranslator(&trans);
    } else {
      qDebug() << "install translator failed" << '\n';
    }
  } else if (language.compare("ja-JP") == 0) {
    if (trans.load(":/FgHelpWindow/icons/fghelpwindow_ja.qm")) {
      qDebug() << "install translator:" << a.installTranslator(&trans);
    } else {
      qDebug() << "install translator failed" << '\n';
    }
  } else if (language.compare("en-US") == 0) {
    qDebug() << "English";
  }

  nlohmann::json config;
  loadConfig(configPath, config);

  QString strFAQURL;
  QString strDocURL;
  std::vector<std::pair<QString, QString>> keyMap;

  parseConfig(config, language, strDocURL, strFAQURL, keyMap);

  FgHelpWidget helpWind;

  helpWind.setDocumentUrl(strDocURL);
  helpWind.setFAQSUrl(strFAQURL);
  helpWind.setShortcutKeys(keyMap);
  helpWind.show();
  return a.exec();
}
