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

#ifndef ADDRESSESSLINEEDITABSTRACTPLUGIN_H
#define ADDRESSESSLINEEDITABSTRACTPLUGIN_H

#include <QObject>
#include "kdepim_export.h"

namespace KPIM {
class KDEPIM_EXPORT AddressessLineEditAbstractPlugin : public QObject
{
    Q_OBJECT
public:
    explicit AddressessLineEditAbstractPlugin(QObject *parent = nullptr);
    ~AddressessLineEditAbstractPlugin();

    virtual QString identifier() const = 0;
    virtual void start() = 0;
    virtual void cancel() = 0;
    virtual void stop() = 0;

Q_SIGNALS:
    void wasUpdated();
};
}

#endif // ADDRESSESSLINEEDITABSTRACTPLUGIN_H
