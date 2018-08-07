/*
   Copyright (C) 2017-2018 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef ADDRESSEELINEEDITINDEX_H
#define ADDRESSEELINEEDITINDEX_H

#include <QStringList>

namespace KPIM {
class AddresseeLineEditManager;
class AddresseeLineEditIndex
{
public:
    explicit AddresseeLineEditIndex(AddresseeLineEditManager *addressLineStatic);
    ~AddresseeLineEditIndex();

    int indexCompletionSource() const;
    void setIndexCompletionSource(int value);

    QStringList indexBlackList() const;
    QStringList domainExcludeList() const;

    void loadIndexBlackList();
    QStringList cleanupEmailList(const QStringList &inputList);
private:
    QStringList mIndexBlackList;
    QStringList mDomainExcludeList;

    int mIndexCompletionSource = 0;
    AddresseeLineEditManager *mAddressLineStatic = nullptr;
};
}

#endif // ADDRESSEELINEEDITINDEX_H
