/*
   Copyright (C) 2017-2020 Laurent Montel <montel@kde.org>

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
#include "kdepimakonadi_export.h"

namespace KPIM {
class AddresseeLineEdit;
/**
 * @brief The AddressessLineEditAbstractPluginInfo struct
 * @author Laurent Montel <montel@kde.org>
 */
struct KDEPIMAKONADI_EXPORT AddressessLineEditAbstractPluginInfo
{
    QString name;
    QString identifier;
    QString description;
};

/**
 * @brief The AddressessLineEditAbstractPlugin class
 * @author Laurent Montel <montel@kde.org>
 */
class KDEPIMAKONADI_EXPORT AddressessLineEditAbstractPlugin : public QObject
{
    Q_OBJECT
public:
    explicit AddressessLineEditAbstractPlugin(QObject *parent = nullptr);
    ~AddressessLineEditAbstractPlugin();

    void setLineEdit(KPIM::AddresseeLineEdit *linedit);

    virtual QVector<AddressessLineEditAbstractPluginInfo> names() const = 0;
    virtual void start() = 0;
    virtual void cancel() = 0;
    virtual void stop() = 0;

Q_SIGNALS:
    void wasUpdated();

private:
    KPIM::AddresseeLineEdit *mLinedit = nullptr;
    bool mEnabled = true;
};
}

#endif // ADDRESSESSLINEEDITABSTRACTPLUGIN_H
