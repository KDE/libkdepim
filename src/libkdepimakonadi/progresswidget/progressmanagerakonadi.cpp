/*
  progressmanager.cpp

  This file is part of libkdepim.

  Copyright (c) 2004 Till Adam <adam@kde.org>

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

#include "progressmanagerakonadi.h"
#include "agentprogressmonitor.h"

KPIM::ProgressItem *KPIM::ProgressManagerAkonadi::createProgressItem(ProgressItem *parent, const Akonadi::AgentInstance &agent, const QString &id, const QString &label, const QString &status,
                                                                     bool canBeCanceled, ProgressItem::CryptoStatus cryptoStatus)
{
    const bool itemAlreadyExists = (KPIM::ProgressManager::instance()->progressItem(id) != nullptr);
    KPIM::ProgressItem *t = KPIM::ProgressManager::instance()->createProgressItem(parent, id, label,
                                                                                  status, canBeCanceled, cryptoStatus);
    t->setTypeProgressItem(0);
    if (!itemAlreadyExists) {
        //    qCDebug(LIBKDEPIM_LOG) << "Created ProgressItem for agent" << instance.name();
        new AgentProgressMonitor(agent, t);
    }
    return t;
}
