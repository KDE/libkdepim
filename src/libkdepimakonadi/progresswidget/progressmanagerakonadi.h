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


#ifndef PROGRESSMANAGERAKONADI_H
#define PROGRESSMANAGERAKONADI_H
#include "kdepimakonadi_export.h"
#include <Libkdepim/ProgressManager>

#include <AkonadiCore/AgentInstance>

namespace KPIM
{
namespace ProgressManagerAkonadi
{
KDEPIMAKONADI_EXPORT ProgressItem *createProgressItem(ProgressItem *parent, const Akonadi::AgentInstance &agent, const QString &id,
                                                      const QString &label, const QString &status, bool canBeCanceled,
                                                      ProgressItem::CryptoStatus cryptoStatus);
}
}
#endif // PROGRESSMANAGERAKONADI_H
