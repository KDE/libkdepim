#pragma once
/*
  statusbarprogresswidget.h

  SPDX-FileCopyrightText: 2004 Till Adam <adam@kde.org>
  SPDX-FileCopyrightText: 2004 Don Sanders
  SPDX-FileCopyrightText: 2004 David Faure <faure@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
/*!
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
/*!
 * \class KPIM::StatusbarProgressWidget
 * \inmodule Libkdepim
 * \inheaderfile KPIM/StatusbarProgressWidget
 */
class KDEPIM_EXPORT StatusbarProgressWidget : public QFrame
{
    Q_OBJECT

public:
    /*!
     * \brief Constructor for StatusbarProgressWidget.
     * \param progressDialog the associated ProgressDialog
     * \param parent the parent widget
     * \param button whether to show a progress button
     */
    explicit StatusbarProgressWidget(ProgressDialog *progressDialog, QWidget *parent, bool button = true);

    /*!
     * \brief Sets which progress item types should be shown.
     * \param type the progress item type to display
     */
    void setShowTypeProgressItem(unsigned int type);
public Q_SLOTS:

    /*!
     * \brief Slot to clean up the progress display.
     */
    void slotClean();

    /*!
     * \brief Slot called when a progress item is added.
     * \param i the added ProgressItem
     */
    void slotProgressItemAdded(KPIM::ProgressItem *i);
    /*!
     * \brief Slot called when a progress item is completed.
     * \param i the completed ProgressItem
     */
    void slotProgressItemCompleted(KPIM::ProgressItem *i);
    /*!
     * \brief Slot called when a progress item's value changes.
     * \param i the ProgressItem with updated progress
     * \param value the new progress value
     */
    void slotProgressItemProgress(KPIM::ProgressItem *i, unsigned int value);
    /*!
     * \brief Slot called when the progress button is clicked.
     */
    void slotProgressButtonClicked();

protected Q_SLOTS:
    /*!
     * \brief Slot called when the progress dialog visibility changes.
     * \param visible true if the dialog is now visible
     */
    void slotProgressDialogVisible(bool);
    /*!
     * \brief Slot called to show a delayed item.
     */
    void slotShowItemDelayed();
    /*!
     * \brief Slot to update the busy indicator.
     */
    void slotBusyIndicator();
    /*!
     * \brief Slot to update the busy mode for a progress item.
     * \param item the ProgressItem to update
     */
    void updateBusyMode(KPIM::ProgressItem *);

protected:
    bool eventFilter(QObject *, QEvent *) override;

private:
    enum Mode {
        Progress,
        Clean,
    };
    KDEPIM_NO_EXPORT void setMode(Mode mode);
    KDEPIM_NO_EXPORT void updateProgressButton(bool showingProgress);
    KDEPIM_NO_EXPORT void connectSingleItem();
    KDEPIM_NO_EXPORT void activateSingleItemMode();

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
