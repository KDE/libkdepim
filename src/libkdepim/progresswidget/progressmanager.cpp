/*
  progressmanager.cpp

  This file is part of libkdepim.

  SPDX-FileCopyrightText: 2004 Till Adam <adam@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "progressmanager.h"

#include "libkdepim_debug.h"
#include <KLocalizedString>

using namespace KPIM;
unsigned int KPIM::ProgressManager::uID = 42;

ProgressItem::ProgressItem(ProgressItem *parent, const QString &id, const QString &label, const QString &status, bool canBeCanceled, CryptoStatus cryptoStatus)
    : mId(id)
    , mLabel(label)
    , mStatus(status)
    , mParent(parent)
    , mCanBeCanceled(canBeCanceled)
    , mCryptoStatus(cryptoStatus)
{
}

ProgressItem::~ProgressItem() = default;

void ProgressItem::setComplete()
{
    //   qCDebug(LIBKDEPIM_LOG) << label();
    if (mChildren.isEmpty()) {
        if (mCompletedCalled) {
            return;
        }
        if (!mCanceled) {
            setProgress(100);
        }
        mCompletedCalled = true;
        if (parent()) {
            parent()->removeChild(this);
        }
        Q_EMIT progressItemCompleted(this);
    } else {
        mWaitingForKids = true;
    }
}

void ProgressItem::reset()
{
    setProgress(0);
    setStatus(QString());
    mCompleted = 0;
}

void ProgressItem::addChild(ProgressItem *kiddo)
{
    mChildren.insert(kiddo, true);
}

void ProgressItem::removeChild(ProgressItem *kiddo)
{
    if (mChildren.isEmpty()) {
        mWaitingForKids = false;
        return;
    }

    if (mChildren.remove(kiddo) == 0) {
        // do nothing if the specified item is not in the map
        return;
    }

    // in case we were waiting for the last kid to go away, now is the time
    if (mChildren.isEmpty() && mWaitingForKids) {
        Q_EMIT progressItemCompleted(this);
    }
}

bool ProgressItem::canceled() const
{
    return mCanceled;
}

unsigned int ProgressItem::typeProgressItem() const
{
    return mType;
}

void ProgressItem::setTypeProgressItem(unsigned int type)
{
    mType = type;
}

void ProgressItem::cancel()
{
    if (mCanceled || !mCanBeCanceled) {
        return;
    }

    qCDebug(LIBKDEPIM_LOG) << label();
    mCanceled = true;
    // Cancel all children.
    for (auto it = mChildren.cbegin(), end = mChildren.cend(); it != end; ++it) {
        ProgressItem *kid = it.key();
        if (kid->canBeCanceled()) {
            kid->cancel();
        }
    }
    setStatus(i18n("Aborting..."));
    Q_EMIT progressItemCanceled(this);
}

void ProgressItem::updateProgress()
{
    setProgress(mTotal ? mCompleted * 100 / mTotal : 0);
}

void ProgressItem::setProgress(unsigned int v)
{
    mProgress = v;
    // qCDebug(LIBKDEPIM_LOG) << label() << " :" << v;
    Q_EMIT progressItemProgress(this, mProgress);
}

const QString &ProgressItem::id() const
{
    return mId;
}

ProgressItem *ProgressItem::parent() const
{
    return mParent.data();
}

const QString &ProgressItem::label() const
{
    return mLabel;
}

void ProgressItem::setLabel(const QString &v)
{
    mLabel = v;
    Q_EMIT progressItemLabel(this, mLabel);
}

const QString &ProgressItem::status() const
{
    return mStatus;
}

void ProgressItem::setStatus(const QString &v)
{
    mStatus = v;
    Q_EMIT progressItemStatus(this, mStatus);
}

bool ProgressItem::canBeCanceled() const
{
    return mCanBeCanceled;
}

void ProgressItem::setCanBeCanceled(bool b)
{
    mCanBeCanceled = b;
}

ProgressItem::CryptoStatus ProgressItem::cryptoStatus() const
{
    return mCryptoStatus;
}

void ProgressItem::setCryptoStatus(ProgressItem::CryptoStatus v)
{
    mCryptoStatus = v;
    Q_EMIT progressItemCryptoStatus(this, v);
}

bool ProgressItem::usesBusyIndicator() const
{
    return mUsesBusyIndicator;
}

void ProgressItem::setUsesBusyIndicator(bool useBusyIndicator)
{
    mUsesBusyIndicator = useBusyIndicator;
    Q_EMIT progressItemUsesBusyIndicator(this, useBusyIndicator);
}

unsigned int ProgressItem::progress() const
{
    return mProgress;
}

// ======================================

struct KPIM::ProgressManagerPrivate {
    ProgressManager instance;
};

Q_GLOBAL_STATIC(KPIM::ProgressManagerPrivate, progressManagerPrivate)

ProgressManager::ProgressManager()
    : QObject()
{
}

ProgressManager::~ProgressManager() = default;

ProgressManager *ProgressManager::instance()
{
    return progressManagerPrivate.isDestroyed() ? nullptr : &progressManagerPrivate->instance;
}

QString ProgressManager::getUniqueID()
{
    return QString::number(++uID);
}

bool ProgressManager::isEmpty() const
{
    return mTransactions.isEmpty();
}

ProgressItem *
ProgressManager::createProgressItem(const QString &id, const QString &label, const QString &status, bool canBeCanceled, ProgressItem::CryptoStatus cryptoStatus)
{
    return instance()->createProgressItemImpl(nullptr, id, label, status, canBeCanceled, cryptoStatus);
}

ProgressItem *ProgressManager::createProgressItem(const QString &parent,
                                                  const QString &id,
                                                  const QString &label,
                                                  const QString &status,
                                                  bool canBeCanceled,
                                                  ProgressItem::CryptoStatus cryptoStatus)
{
    return instance()->createProgressItemImpl(parent, id, label, status, canBeCanceled, cryptoStatus);
}

ProgressItem *ProgressManager::createProgressItem(ProgressItem *parent,
                                                  const QString &id,
                                                  const QString &label,
                                                  const QString &status,
                                                  bool canBeCanceled,
                                                  ProgressItem::CryptoStatus cryptoStatus)
{
    return instance()->createProgressItemImpl(parent, id, label, status, canBeCanceled, cryptoStatus);
}

ProgressItem *ProgressManager::createProgressItem(const QString &label)
{
    return instance()->createProgressItemImpl(nullptr, getUniqueID(), label, QString(), true, KPIM::ProgressItem::Unencrypted);
}

ProgressItem *ProgressManager::createProgressItem(unsigned int progressType, const QString &label)
{
    return instance()->createProgressItemImpl(nullptr, getUniqueID(), label, QString(), true, KPIM::ProgressItem::Unencrypted, progressType);
}

ProgressItem *ProgressManager::createProgressItemImpl(ProgressItem *parent,
                                                      const QString &id,
                                                      const QString &label,
                                                      const QString &status,
                                                      bool cancellable,
                                                      ProgressItem::CryptoStatus cryptoStatus,
                                                      unsigned int progressType)
{
    ProgressItem *t = nullptr;
    if (!mTransactions.value(id)) {
        t = new ProgressItem(parent, id, label, status, cancellable, cryptoStatus);
        t->setTypeProgressItem(progressType);
        mTransactions.insert(id, t);
        if (parent) {
            ProgressItem *p = mTransactions.value(parent->id());
            if (p) {
                p->addChild(t);
            }
        }
        // connect all signals
        connect(t, &ProgressItem::progressItemCompleted, this, &ProgressManager::slotTransactionCompleted);
        connect(t, &ProgressItem::progressItemProgress, this, &ProgressManager::progressItemProgress);
        connect(t, &ProgressItem::progressItemAdded, this, &ProgressManager::progressItemAdded);
        connect(t, &ProgressItem::progressItemCanceled, this, &ProgressManager::progressItemCanceled);
        connect(t, &ProgressItem::progressItemStatus, this, &ProgressManager::progressItemStatus);
        connect(t, &ProgressItem::progressItemLabel, this, &ProgressManager::progressItemLabel);
        connect(t, &ProgressItem::progressItemCryptoStatus, this, &ProgressManager::progressItemCryptoStatus);
        connect(t, &ProgressItem::progressItemUsesBusyIndicator, this, &ProgressManager::progressItemUsesBusyIndicator);

        Q_EMIT progressItemAdded(t);
    } else {
        // Hm, is this what makes the most sense?
        t = mTransactions.value(id);
    }
    return t;
}

ProgressItem *ProgressManager::createProgressItemImpl(const QString &parent,
                                                      const QString &id,
                                                      const QString &label,
                                                      const QString &status,
                                                      bool canBeCanceled,
                                                      ProgressItem::CryptoStatus cryptoStatus,
                                                      unsigned int progressType)
{
    ProgressItem *p = mTransactions.value(parent);
    return createProgressItemImpl(p, id, label, status, canBeCanceled, cryptoStatus, progressType);
}

void ProgressManager::emitShowProgressDialogImpl()
{
    Q_EMIT showProgressDialog();
}

// slots

void ProgressManager::slotTransactionCompleted(ProgressItem *item)
{
    mTransactions.remove(item->id());
    Q_EMIT progressItemCompleted(item);
}

void ProgressManager::slotStandardCancelHandler(ProgressItem *item)
{
    item->setComplete();
}

ProgressItem *ProgressManager::singleItem() const
{
    ProgressItem *item = nullptr;
    QHash<QString, ProgressItem *>::const_iterator it = mTransactions.constBegin();
    QHash<QString, ProgressItem *>::const_iterator end = mTransactions.constEnd();
    while (it != end) {
        // No single item for progress possible, as one of them is a busy indicator one.
        if ((*it)->usesBusyIndicator()) {
            return nullptr;
        }

        if (!(*it)->parent()) { // if it's a top level one, only those count
            if (item) {
                return nullptr; // we found more than one
            } else {
                item = (*it);
            }
        }
        ++it;
    }
    return item;
}

void ProgressManager::emitShowProgressDialog()
{
    instance()->emitShowProgressDialogImpl();
}

ProgressItem *ProgressManager::progressItem(const QString &id) const
{
    return mTransactions.value(id);
}

void ProgressManager::slotAbortAll()
{
    QHashIterator<QString, ProgressItem *> it(mTransactions);
    while (it.hasNext()) {
        it.next().value()->cancel();
    }
}

void KPIM::ProgressItem::setTotalItems(unsigned int v)
{
    mTotal = v;
}

unsigned int ProgressItem::totalItems() const
{
    return mTotal;
}

void ProgressItem::setCompletedItems(unsigned int v)
{
    mCompleted = v;
}

void ProgressItem::incCompletedItems(unsigned int v)
{
    mCompleted += v;
}

unsigned int ProgressItem::completedItems() const
{
    return mCompleted;
}
