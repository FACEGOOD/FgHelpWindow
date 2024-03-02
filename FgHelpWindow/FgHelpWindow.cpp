// （C） FACEGOOD, Inc. All Rights Reserved.
/** @file FgEventTracking.cpp
 *  @brief 
 *
 *  @author Nero
 */
#include "FgHelpWindow.h"

#include <QLayout>
#include <QWebEngineSettings>
#include <QWebEngineView>

#include "framelesswindowsmanager.h"
#include "ui_TitleBar.h"

FgHelpWindow::FgHelpWindow(QWidget *parent)
    : QMainWindow(parent),
      titleBar(new Ui::TitleBar) {
  ui.setupUi(this);

  //this->setWindowFlags(Qt::FramelessWindowHint);
  // setAttribute(Qt::WA_TranslucentBackground);  //����͸��

  initTitleBar();
  this->setMinimumSize(800, 600);
  initWidgets();
}

FgHelpWindow::~FgHelpWindow() { /*delete ui;*/ }

void FgHelpWindow::setDocumentUrl(const QString &url) {
  m_docView->load(QUrl(url));
}

void FgHelpWindow::setFAQSUrl(const QString &url) {
  m_fapsView->load(QUrl(url));
}

void FgHelpWindow::setShortcutKeys(QMap<QString, QString> &keys) {
  QMapIterator<QString, QString> iter(keys);
  while (iter.hasNext()) {
    iter.next();
    int rowCount = ui.shortcutTableWidget->rowCount();
    ui.shortcutTableWidget->insertRow(rowCount);

    QTableWidgetItem *commandItem = new QTableWidgetItem(iter.key());
    QTableWidgetItem *shortcutItem = new QTableWidgetItem(iter.value());
    commandItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    shortcutItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui.shortcutTableWidget->setItem(rowCount, 0, commandItem);
    ui.shortcutTableWidget->setItem(rowCount, 1, shortcutItem);
  }
}
bool FgHelpWindow::eventFilter(QObject *obj, QEvent *event) {
  return QWidget::eventFilter(obj, event);
}

void FgHelpWindow::onCloseWindow(bool flag) {
  delete m_docView;
  delete m_fapsView;
  this->close();
}

void FgHelpWindow::showMinimized(bool flag) {}

void FgHelpWindow::changeEvent(QEvent *e) {
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

void FgHelpWindow::initWidgets() {
  connect(titleBar->closeButton, &QPushButton::clicked, this,
          &FgHelpWindow::onCloseWindow);

  m_docView = new QWebEngineView;
  m_docView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled,
                                      false);
  m_docView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
  ui.documentWidget->layout()->addWidget(m_docView);

  m_fapsView = new QWebEngineView;
  m_fapsView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled,
                                       false);
  m_fapsView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled,
                                       true);
  ui.faqsWidget->layout()->addWidget(m_fapsView);

  ui.shortcutTableWidget->verticalHeader()->hide();
  ui.shortcutTableWidget->horizontalHeader()->setStretchLastSection(true);
  ui.shortcutTableWidget->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
}

void FgHelpWindow::initTitleBar() {
  QWidget *widget = new QWidget(this);
  titleBar->setupUi(widget);
  this->setMenuWidget(widget);

  connect(titleBar->minimizeButton, &QPushButton::clicked, this,
          &FgHelpWindow::showMinimized);
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
