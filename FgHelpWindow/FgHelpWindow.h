// （C） FACEGOOD, Inc. All Rights Reserved.
/** @file FgEventTracking.cpp
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
