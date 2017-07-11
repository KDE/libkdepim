/*
  Copyright (c) 2015-2016 Montel Laurent <montel@kde.org>

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

#ifndef RECENTADDRESSWIDGET_H
#define RECENTADDRESSWIDGET_H

#include "kdepim_export.h"
#include <QWidget>
#include <QStringList>
class KConfig;
class QPushButton;
class QListWidget;
class KLineEdit;
class QToolButton;

namespace KPIM {
class KDEPIM_EXPORT RecentAddressWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RecentAddressWidget(QWidget *parent = nullptr);
    ~RecentAddressWidget();

    void setAddresses(const QStringList &addrs);
    void storeAddresses(KConfig *config);
    bool wasChanged() const;

protected:
    void updateButtonState();
    bool eventFilter(QObject *o, QEvent *e) override;

private:
    void slotAddItem();
    void slotRemoveItem();
    void slotUpdateAddButton(const QString &str);
    QToolButton *mNewButton;
    QToolButton *mRemoveButton;
    QListWidget *mListView;
    KLineEdit *mLineEdit;
    bool mDirty;
};
}
#endif // RECENTADDRESSWIDGET_H
