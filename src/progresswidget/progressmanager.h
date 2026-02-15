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
/*!
 * \class KPIM::ProgressItem
 * \inmodule Libkdepim
 * \inheaderfile KPIM/ProgressItem
 *
 * \brief The ProgressItem class
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

    /*!
     * \brief Returns the unique ID for this progress item.
     * \return the unique ID string
     */
    [[nodiscard]] const QString &id() const;

    /*!
     * \brief Returns the parent progress item.
     * \return the parent ProgressItem, or nullptr if this is a top-level item
     */
    [[nodiscard]] ProgressItem *parent() const;

    /*!
     * \brief Returns the user-visible label for this item.
     * \return the label string
     */
    [[nodiscard]] const QString &label() const;

    /*!
     * \brief Sets the user-visible label for this item.
     * \param v the new label string
     */
    void setLabel(const QString &v);

    /*!
     * \brief Returns the current status message for this item.
     * \return the status string
     */
    [[nodiscard]] const QString &status() const;
    /*!
     * \brief Sets the status message for this item.
     * \param v the new status string
     */
    void setStatus(const QString &v);

    /*!
     * \brief Returns whether this item can be canceled by the user.
     * \return true if the item can be canceled, false otherwise
     */
    [[nodiscard]] bool canBeCanceled() const;

    /*!
     * \brief Sets whether this item can be canceled.
     * \param b true to allow cancellation, false otherwise
     */
    void setCanBeCanceled(bool b);

    /*!
     * \brief Returns the encryption status for this operation.
     * \return the CryptoStatus value
     */
    [[nodiscard]] CryptoStatus cryptoStatus() const;

    /*!
     * \brief Sets the encryption status for this operation.
     * \param v the new CryptoStatus value
     */
    void setCryptoStatus(ProgressItem::CryptoStatus v);

    /*!
     * \brief Returns whether this item uses a busy indicator instead of precise progress.
     * \return true if using a busy indicator, false otherwise
     */
    [[nodiscard]] bool usesBusyIndicator() const;

    /*!
     * \brief Sets whether this item uses a busy indicator.
     * \param useBusyIndicator true to use a busy indicator, false for precise progress
     */
    void setUsesBusyIndicator(bool useBusyIndicator);

    /*!
     * \brief Returns the current progress percentage.
     * \return the progress value (0-100)
     */
    [[nodiscard]] unsigned int progress() const;

    /*!
     * \brief Sets the progress percentage.
     * \param v the new progress value (0-100)
     */
    void setProgress(unsigned int v);

    /*!
     * \brief Marks this item as complete.
     * This will emit progressItemCompleted() and eventually delete the item.
     */
    void setComplete();

    /*!
     * \brief Resets the progress to 0 and clears the status message.
     */
    void reset();

    /*!
     * \brief Cancels this progress item.
     */
    void cancel();

    /*!
     * \brief Sets the total number of items for progress calculation.
     * \param v the total item count
     */
    void setTotalItems(unsigned int v);
    /*!
     * \brief Returns the total number of items.
     * \return the total item count
     */
    [[nodiscard]] unsigned int totalItems() const;
    /*!
     * \brief Sets the number of completed items.
     * \param v the completed item count
     */
    void setCompletedItems(unsigned int v);
    /*!
     * \brief Increments the completed items counter.
     * \param v the amount to increment (default 1)
     */
    void incCompletedItems(unsigned int v = 1);
    /*!
     * \brief Returns the number of completed items.
     * \return the completed item count
     */
    [[nodiscard]] unsigned int completedItems() const;

    /*!
     * \brief Recalculates and updates the progress based on item counts.
     */
    void updateProgress();

    /*!
     * \brief Adds a child progress item.
     * \param kiddo the child item to add
     */
    void addChild(ProgressItem *kiddo);
    /*!
     * \brief Removes a child progress item.
     * \param kiddo the child item to remove
     */
    void removeChild(ProgressItem *kiddo);

    /*!
     * \brief Returns whether this item has been canceled.
     * \return true if canceled, false otherwise
     */
    [[nodiscard]] bool canceled() const;

    /*!
     * \brief Returns the progress item type.
     * \return the progress item type
     */
    unsigned int typeProgressItem() const;
    /*!
     * \brief Sets the progress item type.
     * \param type the progress item type
     */
    void setTypeProgressItem(unsigned int);

Q_SIGNALS:
    /*!
     * Emitted when a new ProgressItem is added.
     * \a The ProgressItem that was added.
     */
    void progressItemAdded(KPIM::ProgressItem *);

    /*!
     * Emitted when the progress value of an item changes.
     * \a  The item which got a new value.
     * \a  The value, for convenience.
     */
    void progressItemProgress(KPIM::ProgressItem *, unsigned int);

    /*!
     * Emitted when a progress item was completed. The item will be
     * deleted afterwards, so slots connected to this are the last
     * chance to work with this item.
     * \a The completed item.
     */
    void progressItemCompleted(KPIM::ProgressItem *);

    /*!
     * Emitted when an item was canceled. It will _not_ go away immediately,
     * only when the owner sets it complete, which will usually happen. Can be
     * used to visually indicate the canceled status of an item. Should be used
     * by the owner of the item to make sure it is set completed even if it is
     * canceled. There is a ProgressManager::slotStandardCancelHandler which
     * simply sets the item completed and can be used if no other work needs to
     * be done on cancel.
     * \a The canceled item;
     */
    void progressItemCanceled(KPIM::ProgressItem *);

    /*!
     * Emitted when the status message of an item changed. Should be used by
     * progress dialogs to update the status message for an item.
     * \a  The updated item.
     * \a  The new message.
     */
    void progressItemStatus(KPIM::ProgressItem *, const QString &);

    /*!
     * Emitted when the label of an item changed. Should be used by
     * progress dialogs to update the label of an item.
     * \a  The updated item.
     * \a  The new label.
     */
    void progressItemLabel(KPIM::ProgressItem *, const QString &);

    /*!
     * Emitted when the crypto status of an item changed. Should be used by
     * progress dialogs to update the crypto indicator of an item.
     * \a  The updated item.
     * \a  The new state.
     */
    void progressItemCryptoStatus(KPIM::ProgressItem *, KPIM::ProgressItem::CryptoStatus);

    /*!
     * Emitted when the busy indicator state of an item changes. Should be used
     * by progress dialogs so that they can adjust the display of the progress bar
     * to the new mode.
     * \a item The updated item
     * \a value True if the item uses a busy indicator now, false otherwise
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

/*!
 * \class KPIM::ProgressManager
 * \inmodule Libkdepim
 * \inheaderfile KPIM/ProgressManager
 *
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
    /*!
      \brief Destructor for ProgressManager.
    */
    ~ProgressManager() override;

    /*!
     * \brief Returns the singleton instance of ProgressManager.
     * \return the ProgressManager instance
     */
    static ProgressManager *instance();

    /*!
     * \brief Generates a unique ID string for a new progress item.
     * \return a unique ID string
     */
    [[nodiscard]] static QString getUniqueID();

    /*!
     * \brief Creates a simple progress item with a unique ID.
     * \param progressType the type of progress item
     * \param label the user-visible label for the item
     * \return the created ProgressItem
     */
    static ProgressItem *createProgressItem(unsigned int progressType, const QString &label);

    /*!
     * \brief Creates a simple progress item with a unique ID.
     * \param label the user-visible label for the item
     * \return the created ProgressItem
     */
    static ProgressItem *createProgressItem(const QString &label);

    /*!
     * \brief Creates a progress item with specified parent and properties.
     * \param parent the parent ProgressItem (or nullptr for top-level)
     * \param id the unique identifier for this operation
     * \param label the user-visible label
     * \param status optional status message
     * \param canBeCanceled whether the user can cancel this operation
     * \param cryptoStatus the encryption status of this operation
     * \return the created ProgressItem
     */
    static ProgressItem *createProgressItem(ProgressItem *parent,
                                            const QString &id,
                                            const QString &label,
                                            const QString &status = QString(),
                                            bool canBeCanceled = true,
                                            KPIM::ProgressItem::CryptoStatus cryptoStatus = KPIM::ProgressItem::Unencrypted);

    /*!
     * \brief Creates a progress item as a child of an item identified by parent ID.
     * \param parent the ID of the parent ProgressItem
     * \param id the unique identifier for this operation
     * \param label the user-visible label
     * \param status optional status message
     * \param canBeCanceled whether the user can cancel this operation
     * \param cryptoStatus the encryption status of this operation
     * \return the created ProgressItem
     */
    static ProgressItem *createProgressItem(const QString &parent,
                                            const QString &id,
                                            const QString &label,
                                            const QString &status = QString(),
                                            bool canBeCanceled = true,
                                            KPIM::ProgressItem::CryptoStatus cryptoStatus = KPIM::ProgressItem::Unencrypted);

    /*!
     * Version without a parent.
     */
    static ProgressItem *createProgressItem(const QString &id,
                                            const QString &label,
                                            const QString &status = QString(),
                                            bool canBeCanceled = true,
                                            KPIM::ProgressItem::CryptoStatus cryptoStatus = KPIM::ProgressItem::Unencrypted);

    /*!
     * \brief Checks if there are any active progress items.
     * \return true if no progress items are active, false otherwise
     */
    [[nodiscard]] bool isEmpty() const;

    /*!
     * \brief Returns the single top-level progress item if there is only one.
     * \return the single ProgressItem, or nullptr if none or more than one top-level item exists
     */
    ProgressItem *singleItem() const;

    /*!
     * \brief Requests all listeners to show the progress dialog.
     */
    static void emitShowProgressDialog();

    /*!
     * \brief Retrieves a progress item by its ID.
     * \param id the unique identifier of the item
     * \return the ProgressItem with the given ID, or nullptr if not found
     */
    ProgressItem *progressItem(const QString &id) const;

Q_SIGNALS:
    /*!
     * \brief Signal emitted when a new progress item is added.
     * \param item the ProgressItem that was added
     */
    void progressItemAdded(KPIM::ProgressItem *);

    /*!
     * \brief Signal emitted when the progress value of an item changes.
     * \param item the ProgressItem with updated progress
     * \param progress the new progress value
     */
    void progressItemProgress(KPIM::ProgressItem *, unsigned int);

    /*!
     * \brief Signal emitted when a progress item is completed.
     * \param item the completed ProgressItem (will be deleted after signal processing)
     */
    void progressItemCompleted(KPIM::ProgressItem *);

    /*!
     * \brief Signal emitted when a progress item is canceled.
     * \param item the canceled ProgressItem
     */
    void progressItemCanceled(KPIM::ProgressItem *);

    /*!
     * \brief Signal emitted when the status message of an item changes.
     * \param item the ProgressItem with updated status
     * \param status the new status message
     */
    void progressItemStatus(KPIM::ProgressItem *, const QString &);

    /*!
     * \brief Signal emitted when the label of an item changes.
     * \param item the ProgressItem with updated label
     * \param label the new label
     */
    void progressItemLabel(KPIM::ProgressItem *, const QString &);

    /*!
     * \brief Signal emitted when the crypto status of an item changes.
     * \param item the ProgressItem with updated crypto status
     * \param cryptoStatus the new crypto status
     */
    void progressItemCryptoStatus(KPIM::ProgressItem *, KPIM::ProgressItem::CryptoStatus);

    /*!
     * \brief Signal emitted when the busy indicator setting of an item changes.
     * \param item the ProgressItem
     * \param value true if using a busy indicator, false otherwise
     */
    void progressItemUsesBusyIndicator(KPIM::ProgressItem *, bool);

    /*!
     * \brief Signal emitted when an operation requests to be shown.
     */
    void showProgressDialog();

public Q_SLOTS:

    /*!
     * \brief Convenience slot that cancels and completes a progress item.
     * \param item the ProgressItem to cancel and complete
     */
    void slotStandardCancelHandler(KPIM::ProgressItem *item);

    /*!
     * \brief Aborts all running progress items.
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
