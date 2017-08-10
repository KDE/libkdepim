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

#include "addressesslineeditpluginmanager.h"
#include "addressesslineeditabstractplugin.h"
#include "libkdepimakonadi_debug.h"

#include <kpluginmetadata.h>
#include <KPluginLoader>
#include <KPluginFactory>

#include <QFileInfo>
#include <QSet>

using namespace KPIM;

class AddressessLineEditPluginInfo
{
public:
    AddressessLineEditPluginInfo()
        : plugin(nullptr)
    {
    }

    QString metaDataFileNameBaseName;
    QString metaDataFileName;
    KPIM::AddressessLineEditAbstractPlugin *plugin;
};

namespace {
QString pluginVersion()
{
    return QStringLiteral("1.0");
}
}

class KPIM::AddressessLineEditPluginManagerPrivate
{
public:
    AddressessLineEditPluginManagerPrivate(AddressessLineEditPluginManager *qq)
        : q(qq)
    {
        initializePlugins();
    }

    void loadPlugin(AddressessLineEditPluginInfo *item);
    QVector<KPIM::AddressessLineEditAbstractPlugin *> pluginsList() const;
    QVector<AddressessLineEditPluginInfo> mPluginList;
    bool initializePlugins();
private:
    AddressessLineEditPluginManager *q;
};

bool AddressessLineEditPluginManagerPrivate::initializePlugins()
{
    if (!mPluginList.isEmpty()) {
        return true;
    }
    const QVector<KPluginMetaData> plugins = KPluginLoader::findPlugins(QStringLiteral("addressline"), [](const KPluginMetaData &md) {
        return md.serviceTypes().contains(QStringLiteral("AddressLineEdit/Plugin"));
    });

    QVectorIterator<KPluginMetaData> i(plugins);
    i.toBack();
    QSet<QString> unique;
    while (i.hasPrevious()) {
        AddressessLineEditPluginInfo info;
        const KPluginMetaData data = i.previous();

        info.metaDataFileNameBaseName = QFileInfo(data.fileName()).baseName();
        info.metaDataFileName = data.fileName();
        if (pluginVersion() == data.version()) {
            // only load plugins once, even if found multiple times!
            if (unique.contains(info.metaDataFileNameBaseName)) {
                continue;
            }
            info.plugin = nullptr;
            mPluginList.push_back(info);
            unique.insert(info.metaDataFileNameBaseName);
        } else {
            qCWarning(LIBKDEPIMAKONADI_LOG) << "Plugin " << data.name() << " doesn't have correction plugin version. It will not be loaded.";
        }
    }
    const QVector<AddressessLineEditPluginInfo>::iterator end(mPluginList.end());
    for (QVector<AddressessLineEditPluginInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
        loadPlugin(&(*it));
    }
    return true;
}

void AddressessLineEditPluginManagerPrivate::loadPlugin(AddressessLineEditPluginInfo *item)
{
    KPluginLoader pluginLoader(item->metaDataFileName);
    if (pluginLoader.factory()) {
        item->plugin = pluginLoader.factory()->create<KPIM::AddressessLineEditAbstractPlugin>(q, QVariantList() << item->metaDataFileNameBaseName);
    }
}

QVector<KPIM::AddressessLineEditAbstractPlugin *> AddressessLineEditPluginManagerPrivate::pluginsList() const
{
    QVector<KPIM::AddressessLineEditAbstractPlugin *> lst;
    const QVector<AddressessLineEditPluginInfo>::ConstIterator end(mPluginList.constEnd());
    for (QVector<AddressessLineEditPluginInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

AddressessLineEditPluginManager::AddressessLineEditPluginManager(QObject *parent)
    : QObject(parent)
    , d(new AddressessLineEditPluginManagerPrivate(this))
{
}

AddressessLineEditPluginManager::~AddressessLineEditPluginManager()
{
    delete d;
}

AddressessLineEditPluginManager *AddressessLineEditPluginManager::self()
{
    static AddressessLineEditPluginManager s_self;
    return &s_self;
}

QVector<KPIM::AddressessLineEditAbstractPlugin *> AddressessLineEditPluginManager::pluginsList() const
{
    return d->pluginsList();
}

KPIM::AddressessLineEditAbstractPlugin *AddressessLineEditPluginManager::plugin(const QString &identifier)
{
    for (KPIM::AddressessLineEditAbstractPlugin *p : pluginsList()) {
        for (const AddressessLineEditAbstractPluginInfo &info : p->names()) {
            if (info.identifier == identifier) {
                return p;
            }
        }
    }
    return {};
}
