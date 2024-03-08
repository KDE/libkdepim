/*
  statusbarprogresswidget.cpp

  SPDX-FileCopyrightText: 2004 Till Adam <adam@kde.org>
  SPDX-FileCopyrightText: 2004 Don Sanders
  SPDX-FileCopyrightText: 2004 David Faure <faure@kde.org>

  Includes StatusbarProgressWidget which is based on KIOLittleProgressDlg
  by SPDX-FileCopyrightText: Matt Koss <koss@miesto.sk>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "statusbarprogresswidget.h"
#include "progressdialog.h"
#include "ssllabel.h"
using KPIM::SSLLabel;
#include "progressmanager.h"
using KPIM::ProgressItem;
using KPIM::ProgressManager;

#include <KLocalizedString>

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QProgressBar>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <chrono>

using namespace std::chrono_literals;

using namespace KPIM;

//-----------------------------------------------------------------------------
StatusbarProgressWidget::StatusbarProgressWidget(ProgressDialog *progressDialog, QWidget *parent, bool button)
    : QFrame(parent)
    , mButton(new QPushButton(this))
    , mShowButton(button)
    , mProgressDialog(progressDialog)
{
    int w = fontMetrics().boundingRect(QStringLiteral(" 999.9 kB/s 00:00:01 ")).width() + 8;
    auto boxLayout = new QHBoxLayout(this);
    boxLayout->setObjectName(QLatin1StringView("boxLayout"));
    boxLayout->setContentsMargins(0, 0, 0, 0);
    boxLayout->setSpacing(0);

    mButton->setObjectName(QLatin1StringView("button"));
    mButton->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    mButton->setIcon(QIcon::fromTheme(QStringLiteral("go-up")));
    boxLayout->addWidget(mButton);
    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QLatin1StringView("stackedwidget"));
    int maximumHeight = qMax(mButton->iconSize().height(), fontMetrics().height());
    mStackedWidget->setMaximumHeight(maximumHeight);
    boxLayout->addWidget(mStackedWidget);

    mSslLabel = new SSLLabel(this);
    mSslLabel->setObjectName(QLatin1StringView("ssllabel"));
    boxLayout->addWidget(mSslLabel);

    mButton->setToolTip(i18n("Open detailed progress dialog"));

    mProgressBar = new QProgressBar(this);
    mProgressBar->setObjectName(QLatin1StringView("progressbar"));
    mProgressBar->installEventFilter(this);
    mProgressBar->setMinimumWidth(w);
    mProgressBar->setFormat(i18nc("Percent value; %p is the value, % is the percent sign", "%p%"));
    mStackedWidget->insertWidget(1, mProgressBar);

    mLabel = new QLabel(QString(), this);
    mLabel->setObjectName(QLatin1StringView("emptylabel"));
    mLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mLabel->installEventFilter(this);
    mLabel->setMinimumWidth(w);
    mStackedWidget->insertWidget(2, mLabel);
    mButton->setMaximumHeight(maximumHeight);
    setFixedWidth(600);

    mMode = Progress; // so the call below works
    setMode(Clean);

    connect(mButton, &QAbstractButton::clicked, this, &StatusbarProgressWidget::slotProgressButtonClicked);

    connect(ProgressManager::instance(), &ProgressManager::progressItemAdded, this, &StatusbarProgressWidget::slotProgressItemAdded);
    connect(ProgressManager::instance(), &ProgressManager::progressItemCompleted, this, &StatusbarProgressWidget::slotProgressItemCompleted);
    connect(ProgressManager::instance(), &ProgressManager::progressItemUsesBusyIndicator, this, &StatusbarProgressWidget::updateBusyMode);

    connect(progressDialog, &ProgressDialog::visibilityChanged, this, &StatusbarProgressWidget::slotProgressDialogVisible);

    mDelayTimer = new QTimer(this);
    mDelayTimer->setSingleShot(true);
    connect(mDelayTimer, &QTimer::timeout, this, &StatusbarProgressWidget::slotShowItemDelayed);

    mCleanTimer = new QTimer(this);
    mCleanTimer->setSingleShot(true);
    connect(mCleanTimer, &QTimer::timeout, this, &StatusbarProgressWidget::slotClean);
}

void StatusbarProgressWidget::setShowTypeProgressItem(unsigned int type)
{
    mShowTypeProgressItem = type;
}

// There are three cases: no progressitem, one progressitem (connect to it directly),
// or many progressitems (display busy indicator). Let's call them 0,1,N.
// In slot..Added we can only end up in 1 or N.
// In slot..Removed we can end up in 0, 1, or we can stay in N if we were already.

void StatusbarProgressWidget::updateBusyMode(KPIM::ProgressItem *item)
{
    if (item->typeProgressItem() == mShowTypeProgressItem) {
        connectSingleItem(); // if going to 1 item
        if (mCurrentItem) { // Exactly one item
            delete mBusyTimer;
            mBusyTimer = nullptr;
            mDelayTimer->start(1s);
        } else { // N items
            if (!mBusyTimer) {
                mBusyTimer = new QTimer(this);
                connect(mBusyTimer, &QTimer::timeout, this, &StatusbarProgressWidget::slotBusyIndicator);
                mDelayTimer->start(1s);
            }
        }
    }
}

void StatusbarProgressWidget::slotProgressItemAdded(ProgressItem *item)
{
    if (item->parent()) {
        return; // we are only interested in top level items
    }

    updateBusyMode(item);
}

void StatusbarProgressWidget::slotProgressItemCompleted(ProgressItem *item)
{
    if (item->parent()) {
        item->deleteLater();
        item = nullptr;
        return; // we are only interested in top level items
    }
    item->deleteLater();
    item = nullptr;
    connectSingleItem(); // if going back to 1 item
    if (ProgressManager::instance()->isEmpty()) { // No item
        // Done. In 5s the progress-widget will close, then we can clean up the statusbar
        mCleanTimer->start(5s);
    } else if (mCurrentItem) { // Exactly one item
        delete mBusyTimer;
        mBusyTimer = nullptr;
        activateSingleItemMode();
    }
}

void StatusbarProgressWidget::connectSingleItem()
{
    if (mCurrentItem) {
        disconnect(mCurrentItem, &ProgressItem::progressItemProgress, this, &StatusbarProgressWidget::slotProgressItemProgress);
        mCurrentItem = nullptr;
    }
    mCurrentItem = ProgressManager::instance()->singleItem();
    if (mCurrentItem) {
        connect(mCurrentItem, &ProgressItem::progressItemProgress, this, &StatusbarProgressWidget::slotProgressItemProgress);
    }
}

void StatusbarProgressWidget::activateSingleItemMode()
{
    mProgressBar->setMaximum(100);
    mProgressBar->setValue(mCurrentItem->progress());
    mProgressBar->setTextVisible(true);
}

void StatusbarProgressWidget::slotShowItemDelayed()
{
    bool noItems = ProgressManager::instance()->isEmpty();
    if (mCurrentItem) {
        activateSingleItemMode();
    } else if (!noItems) { // N items
        mProgressBar->setMaximum(0);
        mProgressBar->setTextVisible(false);
        Q_ASSERT(mBusyTimer);
        if (mBusyTimer) {
            mBusyTimer->start(100ms);
        }
    }

    if (!noItems) {
        setMode(Progress);
    }
}

void StatusbarProgressWidget::slotBusyIndicator()
{
    const int p = mProgressBar->value();
    mProgressBar->setValue(p + 10);
}

void StatusbarProgressWidget::slotProgressItemProgress(ProgressItem *item, unsigned int value)
{
    Q_ASSERT(item == mCurrentItem); // the only one we should be connected to
    Q_UNUSED(item)
    mProgressBar->setValue(value);
}

void StatusbarProgressWidget::setMode(Mode mode)
{
    if (mMode == mode) {
        return;
    }
    mMode = mode;
    switch (mMode) {
    case Clean:
        if (mShowButton) {
            mButton->hide();
        }
        mSslLabel->setState(SSLLabel::Done);
        // show the empty label in order to make the status bar look better
        mStackedWidget->show();
        mStackedWidget->setCurrentWidget(mLabel);
        break;
    case Progress:
        mStackedWidget->show();
        mStackedWidget->setCurrentWidget(mProgressBar);
        if (mShowButton) {
            mShowDetailedProgress = mProgressDialog->wasLastShown();
            updateProgressButton(mShowDetailedProgress);
            mButton->show();
        }
        mSslLabel->setState(mSslLabel->lastState());
        break;
    }
}

void StatusbarProgressWidget::slotClean()
{
    // check if a new item showed up since we started the timer. If not, clear
    if (ProgressManager::instance()->isEmpty()) {
        mProgressBar->setValue(0);
        setMode(Clean);
    }
}

bool StatusbarProgressWidget::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::MouseButtonPress) {
        auto e = static_cast<QMouseEvent *>(ev);

        if (e->button() == Qt::LeftButton && mMode == Progress) { // toggle view on left mouse button
            // Consensus seems to be that we should show/hide the fancy dialog when the user
            // clicks anywhere in the small one.
            slotProgressButtonClicked();
            return true;
        }
    }
    return QFrame::eventFilter(obj, ev);
}

void StatusbarProgressWidget::updateProgressButton(bool showingProgress)
{
    if (!showingProgress) {
        mButton->setIcon(QIcon::fromTheme(QStringLiteral("go-up")));
        mButton->setToolTip(i18n("Show detailed progress window"));
    } else {
        mButton->setIcon(QIcon::fromTheme(QStringLiteral("go-down")));
        mButton->setToolTip(i18n("Hide detailed progress window"));
    }
}

void StatusbarProgressWidget::slotProgressButtonClicked()
{
    mProgressDialog->slotToggleVisibility();
    mShowDetailedProgress = !mProgressDialog->isHidden();
    setFixedWidth(qMax(600, mProgressDialog->width()));
}

void StatusbarProgressWidget::slotProgressDialogVisible(bool b)
{
    // Show the hide/show button (mButton) as soon as the progress dialog is shown
    // (StatusbarProgressWidget::slotShowItemDelayed happens later)
    if (b) {
        setMode(Progress);
    }

    updateProgressButton(b);
}

#include "moc_statusbarprogresswidget.cpp"
