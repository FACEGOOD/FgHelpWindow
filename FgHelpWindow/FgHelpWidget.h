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
/** @file FgHelpWidget.h
 *  @brief 
 *
 *  @author Nero
 */
#ifndef FGHELPWIDGET_H
#define FGHELPWIDGET_H

#include <QWidget>

class QWebEngineView;

namespace Ui {
class FgHelpWidget;
class TitleBar;
}  // namespace Ui

class FgHelpWidget : public QWidget {
  Q_OBJECT
 public:
  explicit FgHelpWidget(QWidget* parent = nullptr);
  ~FgHelpWidget();

  void setDocumentUrl(const QString& url);
  void setFAQSUrl(const QString& url);
  void setShortcutKeys(std::vector<std::pair<QString, QString>>& keys);

  bool eventFilter(QObject* obj, QEvent* event);

 public slots:
  void onCloseWindow(bool flag = false);
  void onPrevPageHits();
  void onNextPageHits();
  void onTabWidgetChanged(int i);
  //void showMinimized(bool flag = false);

 protected:
  void changeEvent(QEvent* e);
  void initWidgets();

 private:
  void initTitleBar();

 private:
  Ui::FgHelpWidget* ui;
  Ui::TitleBar* titleBar = nullptr;

  QWebEngineView* m_docView = nullptr;
  QWebEngineView* m_faqsView = nullptr;

  int m_curScroller = 0;
  int m_pageValue = 10;
};

#endif  // FGHELPWIDGET_H
