/*
  progressmanager.h

  This file is part of libkdepim.

  SPDX-FileCopyrightText: 2004 Till Adam <adam@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kdepim_export.h"

#include <QHash>
#include <QMap>
#include <QObject>
#include <QPointer>
#include <QString>

namespace KPIM
{
class ProgressItem;
class ProgressManager;
using ProgressItemMap = QMap<ProgressItem *, bool>;
/**
 * @brief The ProgressItem class
 */
class KDEPIM_EXPORT ProgressItem : public QObject
{
    Q_OBJECT
    friend class ProgressManager;

public:
    enum CryptoStatus {
        Encrypted,
        Unencrypted,
        Unknown,
    };

    /**
     * @return The id string which uniquely identifies the operation
     *         represented by this item.
     */
    [[nodiscard]] const QString &id() const;

    /**
     * @return The parent item of this one, if there is one.
     */
    [[nodiscard]] ProgressItem *parent() const;

    /**
     * @return The user visible string to be used to represent this item.
     */
    [[nodiscard]] const QString &label() const;

    /**
     * @param v Set the user visible string identifying this item.
     */
    void setLabel(const QString &v);

    /**
     * @return The string to be used for showing this item's current status.
     */
    [[nodiscard]] const QString &status() const;
    /**
     * Set the string to be used for showing this item's current status.
     * @param v The status string.
     */
    void setStatus(const QString &v);

    /**
     * @return Whether this item can be canceled.
     */
    [[nodiscard]] bool canBeCanceled() const;

    /**
     * @param b Set if can be canceled
     */
    void setCanBeCanceled(bool b);

    /**
     * @return Whether this item uses secure communication
     * (Account uses ssl, for example.).
     */
    [[nodiscard]] CryptoStatus cryptoStatus() const;

    /**
     * Set whether this item uses encrypted communication, so listeners
     * can display a nice crypto icon.
     * @param v The value.
     */
    void setCryptoStatus(ProgressItem::CryptoStatus v);

    /**
     * @return whether this item uses a busy indicator instead of real progress display
     */
    [[nodiscard]] bool usesBusyIndicator() const;

    /**
     * Sets whether this item uses a busy indicator instead of real progress for its progress bar.
     * If it uses a busy indicator, you are still responsible for calling setProgress() from time to
     * time to update the busy indicator.
     */
    void setUsesBusyIndicator(bool useBusyIndicator);

    /**
     * @return The current progress value of this item in percent.
     */
    [[nodiscard]] unsigned int progress() const;

    /**
     * Set the progress (percentage of completion) value of this item.
     * @param v The percentage value.
     */
    void setProgress(unsigned int v);

    /**
     * Tell the item it has finished. This will Q_EMIT progressItemCompleted()
     * result in the destruction of the item after all slots connected to this
     * signal have executed. This is the only way to get rid of an item and
     * needs to be called even if the item is canceled. Don't use the item
     * after this has been called on it.
     */
    void setComplete();

    /**
     * Reset the progress value of this item to 0 and the status string to
     * the empty string.
     */
    void reset();

    void cancel();

    // Often needed values for calculating progress.
    void setTotalItems(unsigned int v);
    [[nodiscard]] unsigned int totalItems() const;
    void setCompletedItems(unsigned int v);
    void incCompletedItems(unsigned int v = 1);
    [[nodiscard]] unsigned int completedItems() const;

    /**
     * Recalculate progress according to total/completed items and update.
     */
    void updateProgress();

    void addChild(ProgressItem *kiddo);
    void removeChild(ProgressItem *kiddo);

    [[nodiscard]] bool canceled() const;

    unsigned int typeProgressItem() const;
    void setTypeProgressItem(unsigned int);

Q_SIGNALS:
    /**
     * Emitted when a new ProgressItem is added.
     * @param The ProgressItem that was added.
     */
    void progressItemAdded(KPIM::ProgressItem *);

    /**
     * Emitted when the progress value of an item changes.
     * @param  The item which got a new value.
     * @param  The value, for convenience.
     */
    void progressItemProgress(KPIM::ProgressItem *, unsigned int);

    /**
     * Emitted when a progress item was completed. The item will be
     * deleted afterwards, so slots connected to this are the last
     * chance to work with this item.
     * @param The completed item.
     */
    void progressItemCompleted(KPIM::ProgressItem *);

    /**
     * Emitted when an item was canceled. It will _not_ go away immediately,
     * only when the owner sets it complete, which will usually happen. Can be
     * used to visually indicate the canceled status of an item. Should be used
     * by the owner of the item to make sure it is set completed even if it is
     * canceled. There is a ProgressManager::slotStandardCancelHandler which
     * simply sets the item completed and can be used if no other work needs to
     * be done on cancel.
     * @param The canceled item;
     */
    void progressItemCanceled(KPIM::ProgressItem *);

    /**
     * Emitted when the status message of an item changed. Should be used by
     * progress dialogs to update the status message for an item.
     * @param  The updated item.
     * @param  The new message.
     */
    void progressItemStatus(KPIM::ProgressItem *, const QString &);

    /**
     * Emitted when the label of an item changed. Should be used by
     * progress dialogs to update the label of an item.
     * @param  The updated item.
     * @param  The new label.
     */
    void progressItemLabel(KPIM::ProgressItem *, const QString &);

    /**
     * Emitted when the crypto status of an item changed. Should be used by
     * progress dialogs to update the crypto indicator of an item.
     * @param  The updated item.
     * @param  The new state.
     */
    void progressItemCryptoStatus(KPIM::ProgressItem *, KPIM::ProgressItem::CryptoStatus);

    /**
     * Emitted when the busy indicator state of an item changes. Should be used
     * by progress dialogs so that they can adjust the display of the progress bar
     * to the new mode.
     * @param item The updated item
     * @param value True if the item uses a busy indicator now, false otherwise
     */
    void progressItemUsesBusyIndicator(KPIM::ProgressItem *item, bool value);

protected:
    /* Only to be used by our good friend the ProgressManager */
    ProgressItem(ProgressItem *parent, const QString &id, const QString &label, const QString &status, bool isCancellable, CryptoStatus cryptoStatus);
    ~ProgressItem() override;

private:
    QString mId;
    QString mLabel;
    QString mStatus;
    QPointer<ProgressItem> mParent;
    bool mCanBeCanceled = false;
    unsigned int mProgress = 0;
    ProgressItemMap mChildren;
    unsigned int mTotal = 0;
    unsigned int mCompleted = 0;
    CryptoStatus mCryptoStatus;
    unsigned int mType = 0;
    bool mWaitingForKids = false;
    bool mCanceled = false;
    bool mUsesBusyIndicator = false;
    bool mCompletedCalled = false;
};

struct ProgressManagerPrivate;

/**
 * The ProgressManager singleton keeps track of all ongoing transactions
 * and notifies observers (progress dialogs) when their progress percent value
 * changes, when they are completed (by their owner), and when they are canceled.
 * Each ProgressItem emits those signals individually and the singleton
 * broadcasts them. Use the ::createProgressItem() statistics to acquire an item
 * and then call ->setProgress( int percent ) on it every time you want to
 * update the item and ->setComplete() when the operation is done. This will
 * delete the item. Connect to the item's progressItemCanceled() signal to be
 * notified when the user cancels the transaction using one of the observing
 * progress dialogs or by calling item->cancel() in some other way. The owner
 * is responsible for calling setComplete() on the item, even if it is canceled.
 * Use the standardCancelHandler() slot if that is all you want to do on cancel.
 *
 * Note that if you request an item with a certain id and there is already
 * one with that id, there will not be a new one created but the existing
 * one will be returned. This is convenient for accessing items that are
 * needed regularly without the to store a pointer to them or to add child
 * items to parents by id.
 */
class KDEPIM_EXPORT ProgressManager : public QObject
{
    Q_OBJECT

    friend struct ProgressManagerPrivate;

public:
    ~ProgressManager() override;

    /**
     * @return The singleton instance of this class.
     */
    static ProgressManager *instance();

    /**
     * Use this to acquire a unique id number which can be used to discern
     * an operation from all others going on at the same time. Use that
     * number as the id string for your progressItem to ensure it is unique.
     * @return
     */
    [[nodiscard]] static QString getUniqueID();

    /**
     * Creates a ProgressItem with a unique id and the given label.
     * This is the simplest way to acquire a progress item. It will not
     * have a parent and will be set to be cancellable and not using crypto.
     */
    static ProgressItem *createProgressItem(unsigned int progressType, const QString &label);

    /**
     * Creates a ProgressItem with a unique id and the given label.
     * This is the simplest way to acquire a progress item. It will not
     * have a parent and will be set to be cancellable and not using crypto.
     */
    static ProgressItem *createProgressItem(const QString &label);

    /**
     * Creates a new progressItem with the given parent, id, label and initial
     * status.
     *
     * @param parent Specify an already existing item as the parent of this one.
     * @param id Used to identify this operation for cancel and progress info.
     * @param label The text to be displayed by progress handlers
     * @param status Additional text to be displayed for the item.
     * @param canBeCanceled can the user cancel this operation?
     * @param usesCrypto does the operation use secure transports (SSL)
     * Cancelling the parent will cancel the children as well (if they can be
     * canceled) and ongoing children prevent parents from finishing.
     * @return The ProgressItem representing the operation.
     */
    static ProgressItem *createProgressItem(ProgressItem *parent,
                                            const QString &id,
                                            const QString &label,
                                            const QString &status = QString(),
                                            bool canBeCanceled = true,
                                            KPIM::ProgressItem::CryptoStatus cryptoStatus = KPIM::ProgressItem::Unencrypted);

    /**
     * Use this version if you have the id string of the parent and want to
     * add a subjob to it.
     */
    static ProgressItem *createProgressItem(const QString &parent,
                                            const QString &id,
                                            const QString &label,
                                            const QString &status = QString(),
                                            bool canBeCanceled = true,
                                            KPIM::ProgressItem::CryptoStatus cryptoStatus = KPIM::ProgressItem::Unencrypted);

    /**
     * Version without a parent.
     */
    static ProgressItem *createProgressItem(const QString &id,
                                            const QString &label,
                                            const QString &status = QString(),
                                            bool canBeCanceled = true,
                                            KPIM::ProgressItem::CryptoStatus cryptoStatus = KPIM::ProgressItem::Unencrypted);

    /**
     * @return true when there are no more progress items.
     */
    [[nodiscard]] bool isEmpty() const;

    /**
     * @return the only top level progressitem when there's only one.
     * Returns 0 if there is no item, or more than one top level item.
     * Since this is used to calculate the overall progress, it will also return
     * 0 if there is an item which uses a busy indicator, since that will invalidate
     * the overall progress.
     */
    ProgressItem *singleItem() const;

    /**
     * Ask all listeners to show the progress dialog, because there is
     * something that wants to be shown.
     */
    static void emitShowProgressDialog();

    ProgressItem *progressItem(const QString &id) const;

Q_SIGNALS:
    /** @see ProgressItem::progressItemAdded() */
    void progressItemAdded(KPIM::ProgressItem *);
    /** @see ProgressItem::progressItemProgress() */
    void progressItemProgress(KPIM::ProgressItem *, unsigned int);
    /** @see ProgressItem::progressItemCompleted() */
    void progressItemCompleted(KPIM::ProgressItem *);
    /** @see ProgressItem::progressItemCanceled() */
    void progressItemCanceled(KPIM::ProgressItem *);
    /** @see ProgressItem::progressItemStatus() */
    void progressItemStatus(KPIM::ProgressItem *, const QString &);
    /** @see ProgressItem::progressItemLabel() */
    void progressItemLabel(KPIM::ProgressItem *, const QString &);
    /** @see ProgressItem::progressItemCryptoStatus() */
    void progressItemCryptoStatus(KPIM::ProgressItem *, KPIM::ProgressItem::CryptoStatus);
    /** @see ProgressItem::progressItemUsesBusyIndicator */
    void progressItemUsesBusyIndicator(KPIM::ProgressItem *, bool);

    /**
     * Emitted when an operation requests the listeners to be shown.
     * Use emitShowProgressDialog() to trigger it.
     */
    void showProgressDialog();

public Q_SLOTS:

    /**
     * Calls setCompleted() on the item, to make sure it goes away.
     * Provided for convenience.
     * @param item the canceled item.
     */
    void slotStandardCancelHandler(KPIM::ProgressItem *item);

    /**
     * Aborts all running jobs. Bound to "Esc"
     */
    void slotAbortAll();

private Q_SLOTS:
    void slotTransactionCompleted(KPIM::ProgressItem *item);

private:
    KDEPIM_NO_EXPORT ProgressManager();
    // prevent unsolicited copies
    KDEPIM_NO_EXPORT ProgressManager(const ProgressManager &);

    KDEPIM_NO_EXPORT ProgressItem *createProgressItemImpl(ProgressItem *parent,
                                                          const QString &id,
                                                          const QString &label,
                                                          const QString &status,
                                                          bool cancellable,
                                                          ProgressItem::CryptoStatus cryptoStatus,
                                                          unsigned int progressType = 0);

    KDEPIM_NO_EXPORT ProgressItem *createProgressItemImpl(const QString &parent,
                                                          const QString &id,
                                                          const QString &label,
                                                          const QString &status,
                                                          bool cancellable,
                                                          ProgressItem::CryptoStatus cryptoStatus,
                                                          unsigned int progressType = 0);
    KDEPIM_NO_EXPORT void emitShowProgressDialogImpl();

    QHash<QString, ProgressItem *> mTransactions;
    static unsigned int uID;
};
}
