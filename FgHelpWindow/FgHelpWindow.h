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
 * You should have received a copy of GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/** @file FgHelpWindow.h
 *  @brief 
 *
 *  @author Nero
 */
#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_FgHelpWindow.h"

class QWebEngineView;

namespace Ui {
class FgHelpWindowClass;
class TitleBar;
}  // namespace Ui

class FgHelpWindow : public QMainWindow {
  Q_OBJECT

 public:
  FgHelpWindow(QWidget* parent = Q_NULLPTR);
  ~FgHelpWindow();
  void setDocumentUrl(const QString& url);
  void setFAQSUrl(const QString& url);
  void setShortcutKeys(QMap<QString, QString>& keys);

  bool eventFilter(QObject* obj, QEvent* event);

 public slots:
  void onCloseWindow(bool flag = false);
  void showMinimized(bool flag = false);
  // void onCloseWindow(bool flag = false);

 protected:
  void changeEvent(QEvent* e);
  void initWidgets();

 private:
  void initTitleBar();

 private:
  Ui::FgHelpWindowClass ui;

  Ui::TitleBar* titleBar = nullptr;

  QWebEngineView* m_docView = nullptr;
  QWebEngineView* m_fapsView = nullptr;
};
