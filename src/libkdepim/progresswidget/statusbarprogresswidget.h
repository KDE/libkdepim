#pragma once
/*
  statusbarprogresswidget.h

  SPDX-FileCopyrightText: 2004 Till Adam <adam@kde.org>
  SPDX-FileCopyrightText: 2004 Don Sanders
  SPDX-FileCopyrightText: 2004 David Faure <faure@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
/**
 *  A specialized progress widget class, heavily based on
 *  kio_littleprogress_dlg (it looks similar)
 */

#include "kdepim_export.h"

#include <QFrame>

class QEvent;
class QProgressBar;
class QPushButton;
class QStackedWidget;
class QLabel;
class QTimer;

namespace KPIM
{
class SSLLabel;
class ProgressItem;
class ProgressDialog;

class KDEPIM_EXPORT StatusbarProgressWidget : public QFrame
{
    Q_OBJECT

public:
    explicit StatusbarProgressWidget(ProgressDialog *progressDialog, QWidget *parent, bool button = true);

    void setShowTypeProgressItem(unsigned int type);
public Q_SLOTS:

    void slotClean();

    void slotProgressItemAdded(KPIM::ProgressItem *i);
    void slotProgressItemCompleted(KPIM::ProgressItem *i);
    void slotProgressItemProgress(KPIM::ProgressItem *i, unsigned int value);
    void slotProgressButtonClicked();

protected Q_SLOTS:
    void slotProgressDialogVisible(bool);
    void slotShowItemDelayed();
    void slotBusyIndicator();
    void updateBusyMode(KPIM::ProgressItem *);

protected:
    bool eventFilter(QObject *, QEvent *) override;

private:
    enum Mode { Progress, Clean };
    void setMode(Mode mode);
    void updateProgressButton(bool showingProgress);
    void connectSingleItem();
    void activateSingleItemMode();

    unsigned int mShowTypeProgressItem = 0;
    QProgressBar *mProgressBar = nullptr;
    QLabel *mLabel = nullptr;
    SSLLabel *mSslLabel = nullptr;
    QPushButton *mButton = nullptr;

    Mode mMode = Clean;
    const bool mShowButton = false;
    bool mShowDetailedProgress = false;

    QStackedWidget *mStackedWidget = nullptr;
    ProgressItem *mCurrentItem = nullptr;
    ProgressDialog *const mProgressDialog;
    QTimer *mDelayTimer = nullptr;
    QTimer *mBusyTimer = nullptr;
    QTimer *mCleanTimer = nullptr;
};
} // namespace
