/*
  Copyright 2013-2018 Laurent Montel <montel@kde.org>

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

#ifndef ADDEMAILDISPLAYJOB_H
#define ADDEMAILDISPLAYJOB_H

#include "kdepimakonadi_export.h"
#include <AkonadiCore/Item>
#include <kjob.h>

namespace Akonadi {
class Item;
}

namespace KPIM {
class KDEPIMAKONADI_EXPORT AddEmailDiplayJob : public KJob
{
    Q_OBJECT

public:
    explicit AddEmailDiplayJob(const QString &email, QWidget *parentWidget, QObject *parent = nullptr);

    ~AddEmailDiplayJob() override;
    void setShowAsHTML(bool html);
    void setRemoteContent(bool b);
    void setContact(const Akonadi::Item &contact);
    void setMessageId(Akonadi::Item::Id id);

    void start() override;

Q_SIGNALS:
    void contactUpdated(const Akonadi::Item &contact, Akonadi::Item::Id id);

private:
    //@cond PRIVATE
    class Private;
    Private *const d;
    //@endcond
};
}

#endif
