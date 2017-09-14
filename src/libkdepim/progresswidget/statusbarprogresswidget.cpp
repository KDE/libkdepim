/*
  statusbarprogresswidget.cpp

  (C) 2004 Till Adam <adam@kde.org>
           Don Sanders
           David Faure <dfaure@kde.org>

  Copyright 2004 David Faure <faure@kde.org>
  Includes StatusbarProgressWidget which is based on KIOLittleProgressDlg
  by Matt Koss <koss@miesto.sk>

  KMail is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2 or above,
  as published by the Free Software Foundation.

  KMail is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  In addition, as a special exception, the copyright holders give
  permission to link the code of this program with any edition of
  the Qt library by Trolltech AS, Norway (or with modified versions
  of Qt that use the same license as Qt), and distribute linked
  combinations including the two.  You must obey the GNU General
  Public License in all respects for all of the code used other than
  Qt.  If you modify this file, you may extend this exception to
  your version of the file, but you are not obligated to do so.  If
  you do not wish to do so, delete this exception statement from
  your version.
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
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QProgressBar>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>

using namespace KPIM;

//-----------------------------------------------------------------------------
StatusbarProgressWidget::StatusbarProgressWidget(ProgressDialog *progressDialog, QWidget *parent, bool button)
    : QFrame(parent)
    , mShowButton(button)
    , mProgressDialog(progressDialog)
{
    int w = fontMetrics().width(QStringLiteral(" 999.9 kB/s 00:00:01 ")) + 8;
    QHBoxLayout *boxLayout = new QHBoxLayout(this);
    boxLayout->setObjectName(QStringLiteral("boxLayout"));
    boxLayout->setMargin(0);
    boxLayout->setSpacing(0);

    mButton = new QPushButton(this);
    mButton->setObjectName(QStringLiteral("button"));
    mButton->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,
                                         QSizePolicy::Minimum));
    mButton->setIcon(QIcon::fromTheme(QStringLiteral("go-up")));
    boxLayout->addWidget(mButton);
    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QStringLiteral("stackedwidget"));
    int maximumHeight = qMax(mButton->iconSize().height(), fontMetrics().height());
    mStackedWidget->setMaximumHeight(maximumHeight);
    boxLayout->addWidget(mStackedWidget);

    mSslLabel = new SSLLabel(this);
    mSslLabel->setObjectName(QStringLiteral("ssllabel"));
    boxLayout->addWidget(mSslLabel);

    mButton->setToolTip(i18n("Open detailed progress dialog"));

    mProgressBar = new QProgressBar(this);
    mProgressBar->setObjectName(QStringLiteral("progressbar"));
    mProgressBar->installEventFilter(this);
    mProgressBar->setMinimumWidth(w);
    mStackedWidget->insertWidget(1, mProgressBar);

    mLabel = new QLabel(QString(), this);
    mLabel->setObjectName(QStringLiteral("emptylabel"));
    mLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mLabel->installEventFilter(this);
    mLabel->setMinimumWidth(w);
    mStackedWidget->insertWidget(2, mLabel);
    mButton->setMaximumHeight(maximumHeight);
    setMinimumWidth(minimumSizeHint().width());

    mMode = None;
    setMode();

    connect(mButton, &QAbstractButton::clicked,
            this, &StatusbarProgressWidget::slotProgressButtonClicked);

    connect(ProgressManager::instance(), &ProgressManager::progressItemAdded,
            this, &StatusbarProgressWidget::slotProgressItemAdded);
    connect(ProgressManager::instance(), &ProgressManager::progressItemCompleted,
            this, &StatusbarProgressWidget::slotProgressItemCompleted);
    connect(ProgressManager::instance(), &ProgressManager::progressItemUsesBusyIndicator,
            this, &StatusbarProgressWidget::updateBusyMode);

    connect(progressDialog, &ProgressDialog::visibilityChanged,
            this, &StatusbarProgressWidget::slotProgressDialogVisible);

    mDelayTimer = new QTimer(this);
    mDelayTimer->setSingleShot(true);
    connect(mDelayTimer, &QTimer::timeout,
            this, &StatusbarProgressWidget::slotShowItemDelayed);

    mCleanTimer = new QTimer(this);
    mCleanTimer->setSingleShot(true);
    connect(mCleanTimer, &QTimer::timeout,
            this, &StatusbarProgressWidget::slotClean);
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
        if (mCurrentItem) {   // Exactly one item
            delete mBusyTimer;
            mBusyTimer = nullptr;
            mDelayTimer->start(1000);
        } else { // N items
            if (!mBusyTimer) {
                mBusyTimer = new QTimer(this);
                connect(mBusyTimer, &QTimer::timeout,
                        this, &StatusbarProgressWidget::slotBusyIndicator);
                mDelayTimer->start(1000);
            }
        }
    }
}

void StatusbarProgressWidget::slotProgressItemAdded(ProgressItem *item)
{
    if (item->parent()) {
        return;    // we are only interested in top level items
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
    if (ProgressManager::instance()->isEmpty()) {   // No item
        // Done. In 5s the progress-widget will close, then we can clean up the statusbar
        mCleanTimer->start(5000);
    } else if (mCurrentItem) {   // Exactly one item
        delete mBusyTimer;
        mBusyTimer = nullptr;
        activateSingleItemMode();
    }
}

void StatusbarProgressWidget::connectSingleItem()
{
    if (mCurrentItem) {
        disconnect(mCurrentItem, &ProgressItem::progressItemProgress,
                   this, &StatusbarProgressWidget::slotProgressItemProgress);
        mCurrentItem = nullptr;
    }
    mCurrentItem = ProgressManager::instance()->singleItem();
    if (mCurrentItem) {
        connect(mCurrentItem, &ProgressItem::progressItemProgress,
                this, &StatusbarProgressWidget::slotProgressItemProgress);
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
    } else if (!noItems) {   // N items
        mProgressBar->setMaximum(0);
        mProgressBar->setTextVisible(false);
        Q_ASSERT(mBusyTimer);
        if (mBusyTimer) {
            mBusyTimer->start(100);
        }
    }

    if (!noItems && mMode == None) {
        mMode = Progress;
        setMode();
    }
}

void StatusbarProgressWidget::slotBusyIndicator()
{
    const int p = mProgressBar->value();
    mProgressBar->setValue(p + 10);
}

void StatusbarProgressWidget::slotProgressItemProgress(ProgressItem *item, unsigned int value)
{
    Q_ASSERT(item == mCurrentItem);  // the only one we should be connected to
    Q_UNUSED(item);
    mProgressBar->setValue(value);
}

void StatusbarProgressWidget::setMode()
{
    switch (mMode) {
    case None:
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
        //m_pLabel->clear();
        mMode = None;
        setMode();
    }
}

bool StatusbarProgressWidget::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::MouseButtonPress) {
        QMouseEvent *e = (QMouseEvent *)ev;

        if (e->button() == Qt::LeftButton && mMode != None) {      // toggle view on left mouse button
            // Consensus seems to be that we should show/hide the fancy dialog when the user
            // clicks anywhere in the small one.
            slotProgressButtonClicked();
            return true;
        }
    }
    return QFrame::eventFilter(obj, ev);
}

void StatusbarProgressWidget::slotProgressButtonClicked()
{
    if (mShowDetailedProgress) {
        mShowDetailedProgress = false;
        mButton->setIcon(QIcon::fromTheme(QStringLiteral("go-up")));
        mButton->setToolTip(i18n("Show detailed progress window"));
    } else {
        mShowDetailedProgress = true;
        mButton->setIcon(QIcon::fromTheme(QStringLiteral("go-down")));
        mButton->setToolTip(i18n("Hide detailed progress window"));
    }
    mProgressDialog->slotToggleVisibility();
}

void StatusbarProgressWidget::slotProgressDialogVisible(bool b)
{
    // Update the hide/show button when the detailed one is shown/hidden
    if (b) {
        setMode();
    }
}
