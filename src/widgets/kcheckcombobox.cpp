/*
  This file is part of libkdepim.

  SPDX-FileCopyrightText: 2008 Thomas Thrainer <tom_t@gmx.at>
  SPDX-FileCopyrightText: 2010 Bertjan Broeksema <broeksema@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#include "kcheckcombobox.h"

#include "libkdepim_debug.h"

#include <QAbstractItemView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QStandardItemModel>

using namespace KPIM;

/// Class KCheckComboBox::KCheckComboBoxPrivate

namespace KPIM
{
class Q_DECL_HIDDEN KCheckComboBox::KCheckComboBoxPrivate
{
public:
    KCheckComboBoxPrivate(KCheckComboBox *qq)
        : mSeparator(QLatin1Char(','))
        , q(qq)
    {
    }

    void makeInsertedItemsCheckable(const QModelIndex &, int start, int end);
    Q_REQUIRED_RESULT QString squeeze(const QString &text);
    void updateCheckedItems(const QModelIndex &topLeft = QModelIndex(), const QModelIndex &bottomRight = QModelIndex(), int role = Qt::DisplayRole);
    void toggleCheckState();

public:
    QString mSeparator;
    QString mDefaultText;
    bool mSqueezeText = false;
    bool mIgnoreHide = false;
    bool mAlwaysShowDefaultText = false;

private:
    KCheckComboBox *const q;
};
}

void KCheckComboBox::KCheckComboBoxPrivate::makeInsertedItemsCheckable(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
    auto model = qobject_cast<QStandardItemModel *>(q->model());
    if (model) {
        for (int r = start; r <= end; ++r) {
            QStandardItem *item = model->item(r, 0);
            item->setCheckable(true);
        }
    } else {
        qCWarning(LIBKDEPIM_LOG) << "KCheckComboBox: model is not a QStandardItemModel but a" << q->model() << ". Cannot proceed.";
    }
}

QString KCheckComboBox::KCheckComboBoxPrivate::squeeze(const QString &text)
{
    QFontMetrics fm(q->fontMetrics());
    // The 4 pixels is 2 * horizontalMargin from QLineEdit.
    // The rest is code from QLineEdit::paintEvent, where it determines whether to scroll the text
    // (on my machine minLB=2 and minRB=2, so this removes 8 pixels in total)
    const int minLB = qMax(0, -fm.minLeftBearing());
    const int minRB = qMax(0, -fm.minRightBearing());
    const int lineEditWidth = q->lineEdit()->width() - 4 - minLB - minRB;
    const int textWidth = fm.boundingRect(text).width();
    if (textWidth > lineEditWidth) {
        return fm.elidedText(text, Qt::ElideMiddle, lineEditWidth);
    }

    return text;
}

void KCheckComboBox::KCheckComboBoxPrivate::updateCheckedItems(const QModelIndex &topLeft, const QModelIndex &bottomRight, int role)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)

    const QStringList items = q->checkedItems(role);
    QString text;
    if (items.isEmpty() || mAlwaysShowDefaultText) {
        text = mDefaultText;
    } else {
        text = items.join(mSeparator);
    }

    if (mSqueezeText) {
        text = squeeze(text);
    }

    q->lineEdit()->setText(text);

    Q_EMIT q->checkedItemsChanged(items);
}

void KCheckComboBox::KCheckComboBoxPrivate::toggleCheckState()
{
    if (q->view()->isVisible()) {
        const QModelIndex index = q->view()->currentIndex();
        const QVariant value = index.data(Qt::CheckStateRole);
        if (value.isValid()) {
            const auto state = static_cast<Qt::CheckState>(value.toInt());
            q->model()->setData(index, state == Qt::Unchecked ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
        }
    }
}

/// Class KCheckComboBox

KCheckComboBox::KCheckComboBox(QWidget *parent)
    : QComboBox(parent)
    , d(new KCheckComboBox::KCheckComboBoxPrivate(this))
{
    connect(this, &QComboBox::activated, this, [this]() {
        d->toggleCheckState();
    });
    connect(model(), &QAbstractItemModel::rowsInserted, this, [this](const QModelIndex &index, int start, int end) {
        d->makeInsertedItemsCheckable(index, start, end);
    });
    connect(model(), &QAbstractItemModel::dataChanged, this, [this](const QModelIndex &topLeft, const QModelIndex &bottomRight) {
        d->updateCheckedItems(topLeft, bottomRight);
    });

    // read-only contents
    setEditable(true);

    lineEdit()->setAlignment(Qt::AlignLeft);
    connect(lineEdit(), &QLineEdit::textChanged, this, [this](const QString &text) {
        if (text.isEmpty()) {
            // Clear checked items
            setCheckedItems(QStringList());
        }
    });

    view()->installEventFilter(this);
    view()->viewport()->installEventFilter(this);

    lineEdit()->installEventFilter(this);

    d->updateCheckedItems();
}

KCheckComboBox::~KCheckComboBox() = default;

void KCheckComboBox::hidePopup()
{
    if (!d->mIgnoreHide) {
        QComboBox::hidePopup();
    }
    d->mIgnoreHide = false;
}

Qt::CheckState KCheckComboBox::itemCheckState(int index) const
{
    return static_cast<Qt::CheckState>(itemData(index, Qt::CheckStateRole).toInt());
}

void KCheckComboBox::setItemCheckState(int index, Qt::CheckState state)
{
    setItemData(index, state, Qt::CheckStateRole);
}

QStringList KCheckComboBox::checkedItems(int role) const
{
    QStringList items;
    if (model()) {
        const QModelIndex index = model()->index(0, modelColumn(), rootModelIndex());
        const QModelIndexList indexes = model()->match(index, Qt::CheckStateRole, Qt::Checked, -1, Qt::MatchExactly);
        items.reserve(indexes.count());
        for (const QModelIndex &index : indexes) {
            items += index.data(role).toString();
        }
    }
    return items;
}

void KCheckComboBox::setCheckedItems(const QStringList &items, int role)
{
    for (int r = 0; r < model()->rowCount(rootModelIndex()); ++r) {
        const QModelIndex indx = model()->index(r, modelColumn(), rootModelIndex());

        const QString text = indx.data(role).toString();
        const bool found = items.contains(text);
        model()->setData(indx, found ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
    }
    d->updateCheckedItems(QModelIndex(), QModelIndex(), role);
}

QString KCheckComboBox::defaultText() const
{
    return d->mDefaultText;
}

void KCheckComboBox::setDefaultText(const QString &text)
{
    if (d->mDefaultText != text) {
        d->mDefaultText = text;
        d->updateCheckedItems();
    }
}

bool KCheckComboBox::squeezeText() const
{
    return d->mSqueezeText;
}

void KCheckComboBox::setSqueezeText(bool squeeze)
{
    if (d->mSqueezeText != squeeze) {
        d->mSqueezeText = squeeze;
        d->updateCheckedItems();
    }
}

bool KCheckComboBox::itemEnabled(int index)
{
    Q_ASSERT(index >= 0 && index <= count());

    auto itemModel = qobject_cast<QStandardItemModel *>(model());
    Q_ASSERT(itemModel);

    QStandardItem *item = itemModel->item(index, 0);
    return item->isEnabled();
}

void KCheckComboBox::setItemEnabled(int index, bool enabled)
{
    Q_ASSERT(index >= 0 && index <= count());

    auto itemModel = qobject_cast<QStandardItemModel *>(model());
    Q_ASSERT(itemModel);

    QStandardItem *item = itemModel->item(index, 0);
    item->setEnabled(enabled);
}

QString KCheckComboBox::separator() const
{
    return d->mSeparator;
}

void KCheckComboBox::setSeparator(const QString &separator)
{
    if (d->mSeparator != separator) {
        d->mSeparator = separator;
        d->updateCheckedItems();
    }
}

void KCheckComboBox::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_Down:
        showPopup();
        event->accept();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Escape:
        hidePopup();
        event->accept();
        break;
    default:
        break;
    }
    // don't call base class implementation, we don't need all that stuff in there
}

#ifndef QT_NO_WHEELEVENT
void KCheckComboBox::wheelEvent(QWheelEvent *event)
{
    // discard mouse wheel events on the combo box
    event->accept();
}

#endif

void KCheckComboBox::resizeEvent(QResizeEvent *event)
{
    QComboBox::resizeEvent(event);
    if (d->mSqueezeText) {
        d->updateCheckedItems();
    }
}

bool KCheckComboBox::eventFilter(QObject *receiver, QEvent *event)
{
    switch (event->type()) {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::ShortcutOverride:
        switch (static_cast<QKeyEvent *>(event)->key()) {
        case Qt::Key_Space:
            if (event->type() == QEvent::KeyPress && view()->isVisible()) {
                d->toggleCheckState();
            }
            // Always eat the event: don't let QItemDelegate toggle the current index when the view is hidden.
            return true;
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Escape:
            // ignore Enter keys, they would normally select items.
            // but we select with Space, because multiple selection is possible
            // we simply close the popup on Enter/Escape
            hidePopup();
            return true;
        }
        break;
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
        d->mIgnoreHide = true;
        if (receiver == lineEdit()) {
            showPopup();
            return true;
        }
        break;
    default:
        break;
    }
    return QComboBox::eventFilter(receiver, event);
}

bool KCheckComboBox::alwaysShowDefaultText() const
{
    return d->mAlwaysShowDefaultText;
}

void KCheckComboBox::setAlwaysShowDefaultText(bool always)
{
    if (always != d->mAlwaysShowDefaultText) {
        d->mAlwaysShowDefaultText = always;
        d->updateCheckedItems();
    }
}

void KCheckComboBox::updateCheckedItems(const QModelIndex &topLeft, const QModelIndex &bottomRight, int role)
{
    d->updateCheckedItems(topLeft, bottomRight, role);
}

#include "moc_kcheckcombobox.cpp"
