/** -*- c++ -*-
 * progressdialog.cpp
 *
 *  SPDX-FileCopyrightText: 2004 Till Adam <adam@kde.org>
 *  SPDX-FileCopyrightText: 2004 David Faure <faure@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "progressdialog.h"
#include "ssllabel.h"

#include <KLocalizedString>
#include <QHBoxLayout>

#include <QCloseEvent>
#include <QFrame>
#include <QLabel>
#include <QLayout>
#include <QObject>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollBar>
#include <QTimer>
#include <QVBoxLayout>
#include <chrono>

using namespace std::chrono_literals;

using namespace KPIM;
static const int MAX_LABEL_WIDTH = 650;

class KPIM::OverlayWidgetPrivate
{
public:
    OverlayWidgetPrivate() = default;

    QWidget *mAlignWidget = nullptr;
};

OverlayWidget::OverlayWidget(QWidget *alignWidget, QWidget *parent)
    : QFrame(parent)
    , d(new KPIM::OverlayWidgetPrivate)
{
    setAlignWidget(alignWidget);
    setLayout(new QHBoxLayout(this));
}

OverlayWidget::~OverlayWidget() = default;

QWidget *OverlayWidget::alignWidget() const
{
    return d->mAlignWidget;
}

void OverlayWidget::reposition()
{
    if (!d->mAlignWidget) {
        return;
    }
    // p is in the alignWidget's coordinates
    QPoint p;
    // We are always above the alignWidget, right-aligned with it for
    // LTR locales, and left-aligned for RTL locales (default value=0).
    if (layoutDirection() == Qt::LeftToRight) {
        p.setX(d->mAlignWidget->width() - width());
    }
    p.setY(-height());
    // Position in the toplevelwidget's coordinates
    QPoint pTopLevel = d->mAlignWidget->mapTo(topLevelWidget(), p);
    // Position in the widget's parentWidget coordinates
    QPoint pParent = parentWidget()->mapFrom(topLevelWidget(), pTopLevel);
    // Move 'this' to that position.
    move(pParent);
}

void OverlayWidget::setAlignWidget(QWidget *w)
{
    if (w == d->mAlignWidget) {
        return;
    }

    if (d->mAlignWidget) {
        d->mAlignWidget->removeEventFilter(this);
    }

    d->mAlignWidget = w;

    if (d->mAlignWidget) {
        d->mAlignWidget->installEventFilter(this);
    }

    reposition();
}

bool OverlayWidget::eventFilter(QObject *o, QEvent *e)
{
    if (o == d->mAlignWidget && (e->type() == QEvent::Move || e->type() == QEvent::Resize)) {
        reposition();
    }
    return QFrame::eventFilter(o, e);
}

void OverlayWidget::resizeEvent(QResizeEvent *ev)
{
    reposition();
    QFrame::resizeEvent(ev);
}

TransactionItemView::TransactionItemView(QWidget *parent, const QString &name)
    : QScrollArea(parent)
    , mBigBox(new QWidget(this))
{
    setObjectName(name);
    setFrameStyle(NoFrame);
    auto mBigBoxVBoxLayout = new QVBoxLayout(mBigBox);
    mBigBoxVBoxLayout->setContentsMargins(0, 0, 0, 0);
    setWidget(mBigBox);
    setWidgetResizable(true);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

TransactionItem *TransactionItemView::addTransactionItem(ProgressItem *item, bool first)
{
    auto ti = new TransactionItem(mBigBox, item, first);
    mBigBox->layout()->addWidget(ti);

    resize(mBigBox->width(), mBigBox->height());

    return ti;
}

void TransactionItemView::resizeEvent(QResizeEvent *event)
{
    // Tell the layout in the parent (progressdialog) that our size changed
    updateGeometry();

    const QSize sz = parentWidget()->sizeHint();
    int currentWidth = parentWidget()->width();

    // Don't resize to sz.width() every time when it only reduces a little bit
    if (currentWidth < sz.width() || currentWidth > sz.width() + 100) {
        currentWidth = sz.width();
    }
    parentWidget()->resize(currentWidth, sz.height());

    QScrollArea::resizeEvent(event);
}

QSize TransactionItemView::sizeHint() const
{
    return minimumSizeHint();
}

QSize TransactionItemView::minimumSizeHint() const
{
    const int f = 2 * frameWidth();
    // Make room for a vertical scrollbar in all cases, to avoid a horizontal one
    const int vsbExt = verticalScrollBar()->sizeHint().width();
    const int minw = topLevelWidget()->width() / 3;
    const int maxh = topLevelWidget()->height() / 2;
    QSize sz(mBigBox->minimumSizeHint());
    sz.setWidth(qMax(sz.width(), minw) + f + vsbExt);
    sz.setHeight(qMin(sz.height(), maxh) + f);
    return sz;
}

void TransactionItemView::slotLayoutFirstItem()
{
    // This slot is called whenever a TransactionItem is deleted, so this is a
    // good place to call updateGeometry(), so our parent takes the new size
    // into account and resizes.
    updateGeometry();

    /*
     The below relies on some details in Qt's behaviour regarding deleting
     objects. This slot is called from the destroyed signal of an item just
     going away. That item is at that point still in the  list of children, but
     since the vtable is already gone, it will have type QObject. The first
     one with both the right name and the right class therefore is what will
     be the first item very shortly. That's the one we want to remove the
     hline for.
    */
    auto ti = mBigBox->findChild<KPIM::TransactionItem *>(QStringLiteral("TransactionItem"));
    if (ti) {
        ti->hideHLine();
    }
}

// ----------------------------------------------------------------------------

TransactionItem::TransactionItem(QWidget *parent, ProgressItem *item, bool first)
    : QWidget(parent)
    , mItem(item)
{
    auto vboxLayout = new QVBoxLayout(this);
    vboxLayout->setSpacing(2);
    vboxLayout->setContentsMargins(2, 2, 2, 2);
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));

    mFrame = new QFrame(this);
    mFrame->setFrameShape(QFrame::HLine);
    mFrame->setFrameShadow(QFrame::Raised);
    mFrame->show();
    layout()->addWidget(mFrame);

    auto h = new QWidget(this);
    auto hHBoxLayout = new QHBoxLayout(h);
    hHBoxLayout->setContentsMargins(0, 0, 0, 0);
    hHBoxLayout->setSpacing(5);
    layout()->addWidget(h);

    mItemLabel = new QLabel(fontMetrics().elidedText(item->label(), Qt::ElideRight, MAX_LABEL_WIDTH), h);
    h->layout()->addWidget(mItemLabel);
    h->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));

    mProgress = new QProgressBar(h);
    hHBoxLayout->addWidget(mProgress);
    mProgress->setMaximum(100);
    mProgress->setValue(item->progress());
    h->layout()->addWidget(mProgress);

    if (item->canBeCanceled()) {
        mCancelButton = new QPushButton(QIcon::fromTheme(QStringLiteral("dialog-cancel")), QString(), h);
        hHBoxLayout->addWidget(mCancelButton);
        mCancelButton->setToolTip(i18n("Cancel this operation."));
        connect(mCancelButton, &QAbstractButton::clicked, this, &TransactionItem::slotItemCanceled);
        h->layout()->addWidget(mCancelButton);
    }

    h = new QWidget(this);
    hHBoxLayout = new QHBoxLayout(h);
    hHBoxLayout->setContentsMargins(0, 0, 0, 0);
    hHBoxLayout->setSpacing(5);
    h->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    layout()->addWidget(h);
    mSSLLabel = new SSLLabel(h);
    hHBoxLayout->addWidget(mSSLLabel);
    mSSLLabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    h->layout()->addWidget(mSSLLabel);
    mItemStatus = new QLabel(h);
    hHBoxLayout->addWidget(mItemStatus);
    mItemStatus->setTextFormat(Qt::RichText);
    mItemStatus->setText(fontMetrics().elidedText(item->status(), Qt::ElideRight, MAX_LABEL_WIDTH));
    h->layout()->addWidget(mItemStatus);
    setCryptoStatus(item->cryptoStatus());
    if (first) {
        hideHLine();
    }
}

KPIM::TransactionItem::~TransactionItem() = default;

void TransactionItem::hideHLine()
{
    mFrame->hide();
}

void TransactionItem::setProgress(int progress)
{
    mProgress->setValue(progress);
}

void TransactionItem::setLabel(const QString &label)
{
    mItemLabel->setText(fontMetrics().elidedText(label, Qt::ElideRight, MAX_LABEL_WIDTH));
}

void TransactionItem::setStatus(const QString &status)
{
    mItemStatus->setText(fontMetrics().elidedText(status, Qt::ElideRight, MAX_LABEL_WIDTH));
}

void TransactionItem::setCryptoStatus(KPIM::ProgressItem::CryptoStatus status)
{
    switch (status) {
    case KPIM::ProgressItem::Encrypted:
        mSSLLabel->setEncrypted(SSLLabel::Encrypted);
        break;
    case KPIM::ProgressItem::Unencrypted:
        mSSLLabel->setEncrypted(SSLLabel::Unencrypted);
        break;
    case KPIM::ProgressItem::Unknown:
        mSSLLabel->setEncrypted(SSLLabel::Unknown);
        break;
    }
    mSSLLabel->setState(mSSLLabel->lastState());
}

void TransactionItem::setTotalSteps(int totalSteps)
{
    mProgress->setMaximum(totalSteps);
}

void TransactionItem::slotItemCanceled()
{
    if (mItem) {
        mItem->cancel();
    }
}

void TransactionItem::addSubTransaction(ProgressItem *item)
{
    Q_UNUSED(item)
}

// ---------------------------------------------------------------------------

ProgressDialog::ProgressDialog(QWidget *alignWidget, QWidget *parent)
    : OverlayWidget(alignWidget, parent)
{
    // Qt Bug: Sunken is not applied for RTL layouts correctly (is not mirrored).
    // For now let's just use Plain, which is fine for this.
    if (layoutDirection() == Qt::LeftToRight) {
        setFrameStyle(QFrame::Panel | QFrame::Sunken); // QFrame
    } else {
        setFrameStyle(QFrame::Panel | QFrame::Plain); // QFrame
    }

    setAutoFillBackground(true);

    mScrollView = new TransactionItemView(this, QStringLiteral("ProgressScrollView"));
    layout()->addWidget(mScrollView);
    /*
     * Get the singleton ProgressManager item which will inform us of
     * appearing and vanishing items.
     */
    ProgressManager *pm = ProgressManager::instance();
    connect(pm, &ProgressManager::progressItemAdded, this, &ProgressDialog::slotTransactionAdded);
    connect(pm, &ProgressManager::progressItemCompleted, this, &ProgressDialog::slotTransactionCompleted);
    connect(pm, &ProgressManager::progressItemProgress, this, &ProgressDialog::slotTransactionProgress);
    connect(pm, &ProgressManager::progressItemStatus, this, &ProgressDialog::slotTransactionStatus);
    connect(pm, &ProgressManager::progressItemLabel, this, &ProgressDialog::slotTransactionLabel);
    connect(pm, &ProgressManager::progressItemCryptoStatus, this, &ProgressDialog::slotTransactionCryptoStatus);
    connect(pm, &ProgressManager::progressItemUsesBusyIndicator, this, &ProgressDialog::slotTransactionUsesBusyIndicator);
    connect(pm, &ProgressManager::showProgressDialog, this, &ProgressDialog::slotShow);
}

void ProgressDialog::closeEvent(QCloseEvent *e)
{
    e->accept();
    hide();
}

bool ProgressDialog::wasLastShown() const
{
    return mWasLastShown;
}

/*
 *  Destructor
 */
ProgressDialog::~ProgressDialog()
{
    // no need to delete child widgets.
}

void ProgressDialog::setShowTypeProgressItem(unsigned int type)
{
    mShowTypeProgressItem = type;
}

void ProgressDialog::slotTransactionAdded(ProgressItem *item)
{
    if (item->typeProgressItem() == mShowTypeProgressItem) {
        if (item->parent()) {
            if (TransactionItem *parent = mTransactionsToListviewItems.value(item->parent())) {
                parent->addSubTransaction(item);
            }
        } else {
            const bool first = mTransactionsToListviewItems.empty();
            TransactionItem *ti = mScrollView->addTransactionItem(item, first);
            if (ti) {
                mTransactionsToListviewItems.insert(item, ti);
            }
            if (first && mWasLastShown) {
                QTimer::singleShot(1s, this, &ProgressDialog::slotShow);
            }
        }
    }
}

void ProgressDialog::slotTransactionCompleted(ProgressItem *item)
{
    if (TransactionItem *ti = mTransactionsToListviewItems.value(item)) {
        mTransactionsToListviewItems.remove(item);
        ti->setItemComplete();
        QTimer::singleShot(3s, ti, &QObject::deleteLater);
        // see the slot for comments as to why that works
        connect(ti, &QObject::destroyed, mScrollView, &TransactionItemView::slotLayoutFirstItem);
    }
    // This was the last item, hide.
    if (mTransactionsToListviewItems.empty()) {
        QTimer::singleShot(3s, this, &ProgressDialog::slotHide);
    }
}

void ProgressDialog::slotTransactionCanceled(ProgressItem *)
{
}

void ProgressDialog::slotTransactionProgress(ProgressItem *item, unsigned int progress)
{
    if (TransactionItem *ti = mTransactionsToListviewItems.value(item)) {
        ti->setProgress(progress);
    }
}

void ProgressDialog::slotTransactionStatus(ProgressItem *item, const QString &status)
{
    if (TransactionItem *ti = mTransactionsToListviewItems.value(item)) {
        ti->setStatus(status);
    }
}

void ProgressDialog::slotTransactionLabel(ProgressItem *item, const QString &label)
{
    if (TransactionItem *ti = mTransactionsToListviewItems.value(item)) {
        ti->setLabel(label);
    }
}

void ProgressDialog::slotTransactionCryptoStatus(ProgressItem *item, KPIM::ProgressItem::CryptoStatus value)
{
    if (TransactionItem *ti = mTransactionsToListviewItems.value(item)) {
        ti->setCryptoStatus(value);
    }
}

void ProgressDialog::slotTransactionUsesBusyIndicator(KPIM::ProgressItem *item, bool value)
{
    if (TransactionItem *ti = mTransactionsToListviewItems.value(item)) {
        if (value) {
            ti->setTotalSteps(0);
        } else {
            ti->setTotalSteps(100);
        }
    }
}

void ProgressDialog::slotShow()
{
    setVisible(true);
}

void ProgressDialog::slotHide()
{
    // check if a new item showed up since we started the timer. If not, hide
    if (mTransactionsToListviewItems.isEmpty()) {
        setVisible(false);
    }
}

void ProgressDialog::slotClose()
{
    mWasLastShown = false;
    setVisible(false);
}

void ProgressDialog::setVisible(bool b)
{
    OverlayWidget::setVisible(b);
    Q_EMIT visibilityChanged(b);
}

void ProgressDialog::slotToggleVisibility()
{
    /* Since we are only hiding with a timeout, there is a short period of
     * time where the last item is still visible, but clicking on it in
     * the statusbarwidget should not display the dialog, because there
     * are no items to be shown anymore. Guard against that.
     */
    if (!isHidden() || !mTransactionsToListviewItems.isEmpty()) {
        const bool showNow = isHidden();
        setVisible(showNow);
        mWasLastShown = showNow;
    }
}

#include "moc_progressdialog.cpp"
