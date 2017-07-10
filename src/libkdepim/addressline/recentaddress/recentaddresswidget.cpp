/*
  Copyright (c) 2015-2017 Montel Laurent <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.

*/

#include "recentaddresswidget.h"
#include "recentaddresses.h"

#include <KConfig>
#include <KLineEdit>
#include <QPushButton>
#include <KMessageBox>
#include <KLocalizedString>

#include <QVBoxLayout>
#include <QListWidget>
#include <QKeyEvent>
#include <QToolButton>

using namespace KPIM;
RecentAddressWidget::RecentAddressWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);


    QHBoxLayout *lineLayout = new QHBoxLayout;
    layout->addLayout(lineLayout);

    mLineEdit = new KLineEdit(this);
    mLineEdit->setObjectName(QStringLiteral("line_edit"));
    mLineEdit->setTrapReturnKey(true);
    mLineEdit->installEventFilter(this);
    //connect(mLineEdit, &KLineEdit::textChanged, this, &RecentAddressWidget::slotTypedSomething);
    connect(mLineEdit, &KLineEdit::returnPressed, this, &RecentAddressWidget::slotAddItem);

    lineLayout->addWidget(mLineEdit);

    mNewButton = new QToolButton(this);
    mNewButton->setToolTip(i18n("Add"));
    mNewButton->setObjectName(QStringLiteral("new_button"));
    mNewButton->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    connect(mNewButton, &QPushButton::clicked, this, &RecentAddressWidget::slotAddItem);
    lineLayout->addWidget(mNewButton);

    mRemoveButton = new QToolButton(this);
    mRemoveButton->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    mRemoveButton->setToolTip(i18n("Remove"));
    mRemoveButton->setObjectName(QStringLiteral("remove_button"));
    mRemoveButton->setEnabled(false);
    lineLayout->addWidget(mRemoveButton);
    connect(mRemoveButton, &QPushButton::clicked, this, &RecentAddressWidget::slotRemoveItem);

    mListView = new QListWidget(this);
    mListView->setObjectName(QStringLiteral("list_view"));
    mListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mListView->setSortingEnabled(true);
    layout->addWidget(mListView);
    mDirty = false;
}

RecentAddressWidget::~RecentAddressWidget()
{
}

void RecentAddressWidget::slotAddItem()
{
    mListView->insertItem(0, mLineEdit->text());
    mListView->setCurrentRow(0, QItemSelectionModel::ClearAndSelect);
    mLineEdit->clear();
    mLineEdit->setFocus();
    mDirty = true;
    updateButtonState();
}

void RecentAddressWidget::slotRemoveItem()
{
    const QList<QListWidgetItem *> selectedItems = mListView->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18np("Do you want to remove this email address?", "Do you want to remove %1 email addresses?", selectedItems.count()), i18n("Remove"))) {
        for (QListWidgetItem *item : selectedItems) {
            delete mListView->takeItem(mListView->row(item));
        }
        mDirty = true;
        updateButtonState();
    }
}

void RecentAddressWidget::updateButtonState()
{
    QList<QListWidgetItem *> selectedItems = mListView->selectedItems();
    const int numberOfElementSelected(selectedItems.count());
    mRemoveButton->setEnabled(numberOfElementSelected);
    bool enableElement = (numberOfElementSelected <= 1);
    mNewButton->setEnabled(enableElement);
    mLineEdit->setEnabled(enableElement);
}

void RecentAddressWidget::slotSelectionChanged()
{
    updateButtonState();
}

void RecentAddressWidget::setAddresses(const QStringList &addrs)
{
    mListView->clear();
    mListView->addItems(addrs);
}

bool RecentAddressWidget::eventFilter(QObject *o, QEvent *e)
{
    if (o == mLineEdit && e->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = (QKeyEvent *)e;
        if (keyEvent->key() == Qt::Key_Down
            || keyEvent->key() == Qt::Key_Up) {
            return ((QObject *)mListView)->event(e);
        }
    }

    return false;
}

void RecentAddressWidget::storeAddresses(KConfig *config)
{
    const int numberOfItem(mListView->count());
    for (int i = 0; i < numberOfItem; ++i) {
        KPIM::RecentAddresses::self(config)->add(mListView->item(i)->text());
    }
}

bool RecentAddressWidget::wasChanged() const
{
    return mDirty;
}
