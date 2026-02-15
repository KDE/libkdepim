/* -*- c++ -*-
 * progressdialog.h
 *
 *  SPDX-FileCopyrightText: 2004 Till Adam <adam@kde.org>
 *  based on imapprogressdialog.cpp ,which is
 *  SPDX-FileCopyrightText: 2002-2003 Klarälvdalens Datakonsult AB
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "kdepim_export.h"

#include "progressmanager.h"

#include <QMap>
#include <QScrollArea>

class QProgressBar;
class QFrame;
class QLabel;
class QPushButton;

namespace KPIM
{
class TransactionItem;
class SSLLabel;

/*!
 * \class KPIM::OverlayWidget
 * \inmodule Libkdepim
 * \inheaderfile KPIM/OverlayWidget
 *
 * This is a widget that can align itself with another one, without using a layout,
 * so that it can actually be on top of other widgets.
 * Currently the only supported type of alignment is "right aligned, on top of the other widget".
 *
 */
class OverlayWidgetPrivate;
class KDEPIM_EXPORT OverlayWidget : public QFrame
{
    Q_OBJECT

public:
    /*!
      \brief Constructor for OverlayWidget.
      \param alignWidget the widget to align with
      \param parent the parent widget
    */
    OverlayWidget(QWidget *alignWidget, QWidget *parent);
    /*!
      \brief Destructor for OverlayWidget.
    */
    ~OverlayWidget() override;

    /*!
      \brief Returns the widget this overlay is aligned with.
      \return the aligned widget
    */
    [[nodiscard]] QWidget *alignWidget() const;
    /*!
      \brief Sets the widget to align this overlay with.
      \param alignWidget the widget to align with
    */
    void setAlignWidget(QWidget *alignWidget);

protected:
    void resizeEvent(QResizeEvent *ev) override;
    [[nodiscard]] bool eventFilter(QObject *o, QEvent *e) override;

private:
    KDEPIM_NO_EXPORT void reposition();

private:
    std::unique_ptr<OverlayWidgetPrivate> const d;
};

class TransactionItemView : public QScrollArea
{
    Q_OBJECT
public:
    explicit TransactionItemView(QWidget *parent = nullptr, const QString &name = QString());

    ~TransactionItemView() override;

    TransactionItem *addTransactionItem(ProgressItem *item, bool first);

    [[nodiscard]] QSize sizeHint() const override;
    [[nodiscard]] QSize minimumSizeHint() const override;

public Q_SLOTS:
    void slotLayoutFirstItem();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QWidget *mBigBox = nullptr;
};

class TransactionItem : public QWidget
{
    Q_OBJECT
public:
    TransactionItem(QWidget *parent, ProgressItem *item, bool first);

    ~TransactionItem() override;

    void hideHLine();

    void setProgress(int progress);
    void setLabel(const QString &);

    // the given text is interpreted as RichText, so you might need to
    // .toHtmlEscaped() it before passing
    void setStatus(const QString &);

    void setCryptoStatus(KPIM::ProgressItem::CryptoStatus);
    void setTotalSteps(int totalSteps);

    [[nodiscard]] ProgressItem *item() const
    {
        return mItem;
    }

    void addSubTransaction(ProgressItem *item);

    // The progressitem is deleted immediately, we take 5s to go out,
    // so better not use mItem during this time.
    void setItemComplete()
    {
        mItem = nullptr;
    }

public Q_SLOTS:
    void slotItemCanceled();

protected:
    QProgressBar *mProgress = nullptr;
    QPushButton *mCancelButton = nullptr;
    QLabel *mItemLabel = nullptr;
    QLabel *mItemStatus = nullptr;
    QFrame *mFrame = nullptr;
    SSLLabel *mSSLLabel = nullptr;
    ProgressItem *mItem = nullptr;
};

class KDEPIM_EXPORT ProgressDialog : public OverlayWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor for ProgressDialog.
     * \param alignWidget the widget to align with
     * \param parent the parent widget
     */
    ProgressDialog(QWidget *alignWidget, QWidget *parent);
    /*!
      \brief Destructor for ProgressDialog.
    */
    ~ProgressDialog() override;
    /*!
      \brief Sets the visibility of the progress dialog.
      \param b true to show the dialog, false to hide it
    */
    void setVisible(bool b) override;

    /*!
      \brief Sets which progress items types should be shown.
      \param type the progress item type to show
    */
    void setShowTypeProgressItem(unsigned int type);

    /*!
      \brief Returns whether this dialog was last shown to the user.
      \return true if the dialog was last shown, false otherwise
    */
    [[nodiscard]] bool wasLastShown() const;

public Q_SLOTS:
    /*!
      \brief Toggles the visibility of the progress dialog.
    */
    void slotToggleVisibility();

protected Q_SLOTS:
    /*!
      \brief Slot called when a new progress item is added.
      \param item the progress item that was added
    */
    void slotTransactionAdded(KPIM::ProgressItem *item);
    /*!
      \brief Slot called when a progress item is completed.
      \param item the progress item that was completed
    */
    void slotTransactionCompleted(KPIM::ProgressItem *item);
    /*!
      \brief Slot called when a progress item is canceled.
      \param item the progress item that was canceled
    */
    void slotTransactionCanceled(KPIM::ProgressItem *item);
    /*!
      \brief Slot called when the progress of an item changes.
      \param item the progress item
      \param progress the new progress value
    */
    void slotTransactionProgress(KPIM::ProgressItem *item, unsigned int progress);
    /*!
      \brief Slot called when the status of a progress item changes.
      \param item the progress item
      \param status the new status text
    */
    void slotTransactionStatus(KPIM::ProgressItem *item, const QString &);
    /*!
      \brief Slot called when the label of a progress item changes.
      \param item the progress item
      \param label the new label text
    */
    void slotTransactionLabel(KPIM::ProgressItem *item, const QString &);
    /*!
      \brief Slot called when the crypto status of a progress item changes.
      \param item the progress item
      \param cryptoStatus the new crypto status
    */
    void slotTransactionCryptoStatus(KPIM::ProgressItem *item, KPIM::ProgressItem::CryptoStatus);
    /*!
      \brief Slot called when a progress item's busy indicator setting changes.
      \param item the progress item
      \param usesBusyIndicator true if using busy indicator
    */
    void slotTransactionUsesBusyIndicator(KPIM::ProgressItem *, bool);

    /*!
      \brief Slot called to close the progress dialog.
    */
    void slotClose();
    /*!
      \brief Slot called to show the progress dialog.
    */
    void slotShow();
    /*!
      \brief Slot called to hide the progress dialog.
    */
    void slotHide();

Q_SIGNALS:
    /*!
      \brief Signal emitted when the visibility of the dialog changes.
      \param visible true if the dialog is now visible, false if hidden
    */
    void visibilityChanged(bool);

protected:
    void closeEvent(QCloseEvent *) override;

    TransactionItemView *mScrollView = nullptr;
    QList<QMetaObject::Connection> mConnections;
    QMap<const ProgressItem *, TransactionItem *> mTransactionsToListviewItems;
    unsigned int mShowTypeProgressItem = 0;
    bool mWasLastShown = false;
};
} // namespace KPIM
