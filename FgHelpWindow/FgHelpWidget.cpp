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
/** @file FgHelpWidget.cpp
 *  @brief 
 *
 *  @author Nero
 */
#include "FgHelpWidget.h"

#include <QLayout>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <QScrollBar>
#include <QEvent>
#include <QFont>
#include <iostream>
#include "framelesswindowsmanager.h"
#include "ui_FgHelpWidget.h"
#include "ui_TitleBar.h"
#include <format>
#include <chrono>
#include "FgEventTracking.h"

std::chrono::system_clock::time_point pageTimeStart;

int lastPageIndex = 0;

FgHelpWidget::FgHelpWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::FgHelpWidget), titleBar(new Ui::TitleBar) {
  ui->setupUi(this);

  setWindowIcon(QIcon(":/FgHelpWindow/icons/help.png"));

  initTitleBar();
  this->setMinimumSize(800, 600);
  initWidgets();
}

FgHelpWidget::~FgHelpWidget() { delete ui; }

void FgHelpWidget::setDocumentUrl(const QString &url) {
  m_docView->load(QUrl(url));
}

void FgHelpWidget::setFAQSUrl(const QString &url) {
  m_faqsView->load(QUrl(url));
}

void FgHelpWidget::setShortcutKeys(
    std::vector<std::pair<QString, QString>> &keys) {
  auto iter =  keys.begin();
  while (iter!=keys.end()) {

    int rowCount = ui->shortcutTableWidget->rowCount();
    ui->shortcutTableWidget->insertRow(rowCount);

    QTableWidgetItem *commandItem = new QTableWidgetItem(iter->first);
    QTableWidgetItem *shortcutItem = new QTableWidgetItem(iter->second);
    commandItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    shortcutItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->shortcutTableWidget->setItem(rowCount, 0, commandItem);
    ui->shortcutTableWidget->setItem(rowCount, 1, shortcutItem);
    iter++;
  }
}
bool FgHelpWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->shortcutTableWidget->viewport())
    {
    if (event->type() == QEvent::Wheel) {
        return true;
      } 

  }
  return QWidget::eventFilter(obj, event);
}

void FgHelpWidget::onPrevPageHits() {
  int maxValue = ui->shortcutTableWidget->verticalScrollBar()
                     ->maximum();  // ��ǰSCROLLER�����ʾֵ
  m_curScroller = ui->shortcutTableWidget->verticalScrollBar()->value();

  if (m_curScroller > 0)
    ui->shortcutTableWidget->verticalScrollBar()->setSliderPosition(
        m_curScroller - m_pageValue);
  else
    ui->shortcutTableWidget->verticalScrollBar()->setSliderPosition(maxValue);
}

void FgHelpWidget::onNextPageHits() {
  int maxValue = ui->shortcutTableWidget->verticalScrollBar()
                     ->maximum();  // ��ǰSCROLLER�����ʾֵ
  m_curScroller = ui->shortcutTableWidget->verticalScrollBar()
                     ->value();  //��õ�ǰscrollerֵ

  if (m_curScroller < maxValue)
    ui->shortcutTableWidget->verticalScrollBar()->setSliderPosition(
        m_pageValue + m_curScroller);
  else
    ui->shortcutTableWidget->verticalScrollBar()->setSliderPosition(0);
}

void FgHelpWidget::onTabWidgetChanged(int i)
{
    auto duration = std::chrono::system_clock::now()- pageTimeStart;
    //std::chrono::hh_mm_ss<std::chrono::seconds> tod{ std::chrono::duration_cast<std::chrono::seconds>(duration)};
    //std::string strTime = std::format("{:%T}", tod);
    switch (lastPageIndex) {
    case 0:
        FgEventTracking::Event_ReadDocmentTab(std::chrono::duration_cast<std::chrono::seconds>(duration).count());
        break;
    case 1:
        FgEventTracking::Event_ReadFAQTab(std::chrono::duration_cast<std::chrono::seconds>(duration).count());

        break;
    case 2:
        FgEventTracking::Event_ReadShortCutTab(std::chrono::duration_cast<std::chrono::seconds>(duration).count());
        break;
    default:
        break;
    }
    //qDebug() << "read " << lastPageIndex << " " << strTime.c_str();

    pageTimeStart = std::chrono::system_clock::now();
    lastPageIndex = i;
} 

void FgHelpWidget::onCloseWindow(bool flag) {
    auto duration = std::chrono::system_clock::now() - pageTimeStart;
    int currentIdx = ui->tabWidget->currentIndex();
    switch (currentIdx) {
    case 0:
        FgEventTracking::Event_ReadDocmentTab(std::chrono::duration_cast<std::chrono::seconds>(duration).count());
        break;
    case 1:
        FgEventTracking::Event_ReadFAQTab(std::chrono::duration_cast<std::chrono::seconds>(duration).count());

        break;
    case 2:
        FgEventTracking::Event_ReadShortCutTab(std::chrono::duration_cast<std::chrono::seconds>(duration).count());
        break;
    default:
        break;
    }

  FgEventTracking::storage();
  delete m_docView;
  delete m_faqsView;
  this->close();
}

//void FgHelpWidget::showMinimized(bool flag) {}

void FgHelpWidget::changeEvent(QEvent *e) {
  if (e->type() == QEvent::WindowStateChange) {
    if (this->isMaximized()) {
      titleBar->maximizeButton->setToolTip(tr("Restore"));
      titleBar->maximizeButton->setIcon(
          QIcon(":/FgHelpWindow/icons/restore.png"));
    } else {
      titleBar->maximizeButton->setToolTip(tr("Maximize"));
      titleBar->maximizeButton->setIcon(QIcon(":/FgHelpWindow/icons/max.png"));
    }
  }
  QWidget::changeEvent(e);
}

void FgHelpWidget::initWidgets() {
  connect(titleBar->closeButton, &QPushButton::clicked, this,
          &FgHelpWidget::onCloseWindow);

  m_docView = new QWebEngineView;
  m_docView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled,
                                      false);
  m_docView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
  QFont font;
  font.setFamily("Microsoft YaHei");
  m_docView->settings()->setFontFamily(QWebEngineSettings::StandardFont,
                                       font.family());
  ui->documentWidget->layout()->addWidget(m_docView);

  m_faqsView = new QWebEngineView;
  m_faqsView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled,
                                       false);
  m_faqsView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled,
                                       true);
  m_faqsView->settings()->setFontFamily(QWebEngineSettings::StandardFont,
                                        font.family());

  ui->faqsWidget->layout()->addWidget(m_faqsView);

  ui->shortcutTableWidget->verticalHeader()->hide();
  ui->shortcutTableWidget->horizontalHeader()->setStretchLastSection(true);
  ui->shortcutTableWidget->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);

  ui->shortcutTableWidget->horizontalScrollBar()->setDisabled(true);
  ui->shortcutTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->shortcutTableWidget->viewport()->installEventFilter(this);
  ui->shortcutTableWidget->setObjectName("shortcutTableWidget");
  ui->shortcutTableWidget->setMouseTracking(false);

  connect(ui->prevPushButton, &QPushButton::clicked, this,
          &FgHelpWidget::onPrevPageHits);
  connect(ui->nextPushButton, &QPushButton::clicked, this,
          &FgHelpWidget::onNextPageHits);
  ui->tabWidget->setCurrentIndex(0);

  connect(ui->tabWidget, &QTabWidget::currentChanged, this, &FgHelpWidget::onTabWidgetChanged);
  pageTimeStart = std::chrono::system_clock::now();
}

void FgHelpWidget::initTitleBar() {
  QWidget *widget = new QWidget(this);
  titleBar->setupUi(widget);
  ui->titleWidget->layout()->addWidget(widget);

  connect(titleBar->minimizeButton, &QPushButton::clicked, this,
          &QWidget::showMinimized);
  connect(titleBar->maximizeButton, &QPushButton::clicked, [this]() {
    if (this->isMaximized()) {
      this->showNormal();
      titleBar->maximizeButton->setToolTip(QObject::tr("Maximize"));
    } else {
      this->showMaximized();
      titleBar->maximizeButton->setToolTip(QObject::tr("Restore"));
    }
  });

  this->createWinId();
  const QWindow *win = this->windowHandle();
  FramelessWindowsManager::addWindow(win);
  FramelessWindowsManager::addIgnoreObject(win, titleBar->minimizeButton);
  FramelessWindowsManager::addIgnoreObject(win, titleBar->maximizeButton);
  FramelessWindowsManager::addIgnoreObject(win, titleBar->closeButton);
}
