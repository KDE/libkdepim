/*
  Copyright (c) 2015-2020 Laurent Montel <montel@kde.org>

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

#ifndef BLACKLISTBALOOEMAILLIST_H
#define BLACKLISTBALOOEMAILLIST_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QHash>
#include "libkdepimakonadi_private_export.h"

namespace KPIM {
class LIBKDEPIMAKONADI_TESTS_EXPORT BlackListBalooEmailListItem : public QListWidgetItem
{
public:
    explicit BlackListBalooEmailListItem(QListWidget *parent = nullptr);
    ~BlackListBalooEmailListItem();

    Q_REQUIRED_RESULT bool initializeStatus() const;
    void setInitializeStatus(bool initializeStatus);

private:
    bool mInitializeStatus;
};

class LIBKDEPIMAKONADI_TESTS_EXPORT BlackListBalooEmailList : public QListWidget
{
    Q_OBJECT
public:
    explicit BlackListBalooEmailList(QWidget *parent = nullptr);
    ~BlackListBalooEmailList() override;

    void setEmailBlackList(const QStringList &list);

    Q_REQUIRED_RESULT QHash<QString, bool> blackListItemChanged() const;

    void setExcludeDomain(const QStringList &domain);
    Q_REQUIRED_RESULT int setEmailFound(const QStringList &);
protected:
    void paintEvent(QPaintEvent *event) override;

    void changeEvent(QEvent *event) override;

private:
    void generalPaletteChanged();
    QStringList mEmailBlackList;
    QStringList mExcludeDomain;
    QColor mTextColor;
    bool mFirstResult = false;
};
}

#endif // BLACKLISTBALOOEMAILLIST_H
