/* -*- c++ -*-
 * progressdialog.h
 *
 *  SPDX-FileCopyrightText: 2004 Till Adam <adam@kde.org>
 *  based on imapprogressdialog.cpp ,which is
 *  SPDX-FileCopyrightText: 2002-2003 Klarälvdalens Datakonsult AB
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  In addition, as a special exception, the copyright holders give
 *  permission to link the code of this program with any edition of
 *  the Qt library by Trolltech AS, Norway (or with modified versions
 *  of Qt that use the same license as Qt), and distribute linked
 *  combinations including the two.  You must obey the GNU General
 *  Public License in all respects for all of the code used other than
 *  Qt.  If you modify this file, you may extend this exception to
 *  your version of the file, but you are not obligated to do so.  If
 *  you do not wish to do so, delete this exception statement from
 *  your version.
 */

#ifndef KDEPIM_PROGRESSDIALOG_H
#define KDEPIM_PROGRESSDIALOG_H

#include "kdepim_export.h"

#include "progressmanager.h"

#include <QScrollArea>
#include <QMap>

class QProgressBar;
class QFrame;
class QLabel;
class QPushButton;

namespace KPIM {
class TransactionItem;
class SSLLabel;

/**
 * This is a widget that can align itself with another one, without using a layout,
 * so that it can actually be on top of other widgets.
 * Currently the only supported type of alignment is "right aligned, on top of the other widget".
 *
 */
class OverlayWidgetPrivate;
class OverlayWidget : public QFrame
{
    Q_OBJECT

public:
    OverlayWidget(QWidget *alignWidget, QWidget *parent);
    ~OverlayWidget() override;

    Q_REQUIRED_RESULT QWidget *alignWidget() const;
    void setAlignWidget(QWidget *alignWidget);

protected:
    void resizeEvent(QResizeEvent *ev) override;
    bool eventFilter(QObject *o, QEvent *e) override;

private:
    void reposition();

private:
    OverlayWidgetPrivate *const d;
};

class TransactionItemView : public QScrollArea
{
    Q_OBJECT
public:
    explicit TransactionItemView(QWidget *parent = nullptr, const QString &name = QString());

    ~TransactionItemView() override
    {
    }

    TransactionItem *addTransactionItem(ProgressItem *item, bool first);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

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

    ~TransactionItem();

    void hideHLine();

    void setProgress(int progress);
    void setLabel(const QString &);

    // the given text is interpreted as RichText, so you might need to
    // .toHtmlEscaped() it before passing
    void setStatus(const QString &);

    void setCryptoStatus(KPIM::ProgressItem::CryptoStatus);
    void setTotalSteps(int totalSteps);

    ProgressItem *item() const
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
    ProgressDialog(QWidget *alignWidget, QWidget *parent);
    ~ProgressDialog() override;
    void setVisible(bool b) override;

    void setShowTypeProgressItem(unsigned int type);

    bool wasLastShown() const;

public Q_SLOTS:
    void slotToggleVisibility();

protected Q_SLOTS:
    void slotTransactionAdded(KPIM::ProgressItem *item);
    void slotTransactionCompleted(KPIM::ProgressItem *item);
    void slotTransactionCanceled(KPIM::ProgressItem *item);
    void slotTransactionProgress(KPIM::ProgressItem *item, unsigned int progress);
    void slotTransactionStatus(KPIM::ProgressItem *item, const QString &);
    void slotTransactionLabel(KPIM::ProgressItem *item, const QString &);
    void slotTransactionCryptoStatus(KPIM::ProgressItem *item, KPIM::ProgressItem::CryptoStatus);
    void slotTransactionUsesBusyIndicator(KPIM::ProgressItem *, bool);

    void slotClose();
    void slotShow();
    void slotHide();

Q_SIGNALS:
    void visibilityChanged(bool);

protected:
    void closeEvent(QCloseEvent *) override;

    TransactionItemView *mScrollView = nullptr;
    QMap<const ProgressItem *, TransactionItem *> mTransactionsToListviewItems;
    unsigned int mShowTypeProgressItem = 0;
    bool mWasLastShown = false;
};
} // namespace KPIM

#endif // __KPIM_PROGRESSDIALOG_H__
