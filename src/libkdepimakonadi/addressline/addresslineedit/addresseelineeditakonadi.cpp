/*
   Copyright (C) 2017 Laurent Montel <montel@kde.org>

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

#include "addresseelineeditakonadi.h"
#include <AkonadiCore/Session>

using namespace KPIM;

AddresseeLineEditAkonadi::AddresseeLineEditAkonadi(AddresseeLineEditManager *addressLineStatic)
    : m_akonadiSession(nullptr)
    , mAddressLineStatic(addressLineStatic)
{

}

AddresseeLineEditAkonadi::~AddresseeLineEditAkonadi()
{

}

Akonadi::Session *AddresseeLineEditAkonadi::akonadiSession() const
{
    if (!m_akonadiSession) {
        m_akonadiSession = new Akonadi::Session("contactsCompletionSession");
    }
    return m_akonadiSession;
}

Akonadi::Item::List AddresseeLineEditAkonadi::akonadiPendingItems() const
{
    return mAkonadiPendingItems;
}

Akonadi::Item::List::iterator AddresseeLineEditAkonadi::erasePendingItem(const Akonadi::Item::List::iterator &item)
{
    return mAkonadiPendingItems.erase(item);
}

void AddresseeLineEditAkonadi::appendPendingItem(const Akonadi::Item &item)
{
    mAkonadiPendingItems.append(item);
}
