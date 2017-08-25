/*
    Copyright (c) 2009 Constantin Berzan <exit3219@gmail.com>

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

#include "agentprogressmonitor.h"

#include <AgentManager>

#include <QWeakPointer>

using namespace Akonadi;
using namespace KPIM;

AgentProgressMonitor::AgentProgressMonitor(const AgentInstance &agent, ProgressItem *item)
    : QObject(item)
    , mAgent(agent)
    , mItem(item)
{
    connect(AgentManager::self(), &AgentManager::instanceProgressChanged,
            this, &AgentProgressMonitor::instanceProgressChanged);
    connect(AgentManager::self(), &AgentManager::instanceStatusChanged,
            this, &AgentProgressMonitor::instanceStatusChanged);
    connect(Akonadi::AgentManager::self(), &AgentManager::instanceRemoved,
            this, &AgentProgressMonitor::instanceRemoved);
    connect(Akonadi::AgentManager::self(), &AgentManager::instanceNameChanged,
            this, &AgentProgressMonitor::instanceNameChanged);
    // TODO connect to instanceError, instanceWarning, instanceOnline ?
    // and do what?

    connect(item, &ProgressItem::progressItemCanceled,
            this, &AgentProgressMonitor::abort);

    // TODO handle offline case
}

AgentProgressMonitor::~AgentProgressMonitor()
{
}

void AgentProgressMonitor::abort()
{
    mAgent.abortCurrentTask();
}

void AgentProgressMonitor::instanceRemoved(const Akonadi::AgentInstance &instance)
{
    Q_UNUSED(instance);

    if (!mItem.data()) {
        return;
    }

    mItem.data()->disconnect(this);   // avoid abort call
    mItem.data()->cancel();
    if (mItem.data()) {
        mItem.data()->setComplete();
    }
}

void AgentProgressMonitor::instanceProgressChanged(const AgentInstance &instance)
{
    if (!mItem.data()) {
        return;
    }

    if (mAgent == instance) { // compares identifiers
        mAgent = instance; // keeps copy of current status
        const int progress = mAgent.progress();
        if (progress >= 0) {
            mItem.data()->setProgress(progress);
        }
    }
}

void AgentProgressMonitor::instanceStatusChanged(const AgentInstance &instance)
{
    if (!mItem.data()) {
        return;
    }

    if (mAgent == instance) { // compares identifiers
        mAgent = instance; // keeps copy of current status
        mItem.data()->setStatus(mAgent.statusMessage());
        switch (mAgent.status()) {
        case AgentInstance::Idle:
            if (mItem.data()) {
                mItem.data()->setComplete();
            }
            break;
        case AgentInstance::Running:
            break;
        case AgentInstance::Broken:
            mItem.data()->disconnect(this);   // avoid abort call
            mItem.data()->cancel();
            if (mItem.data()) {
                mItem.data()->setComplete();
            }
            break;
        default:
            Q_ASSERT(false);
        }
    }
}

void AgentProgressMonitor::instanceNameChanged(const Akonadi::AgentInstance &instance)
{
    if (!mItem.data()) {
        return;
    }
    mItem.data()->setLabel(instance.name());
}
