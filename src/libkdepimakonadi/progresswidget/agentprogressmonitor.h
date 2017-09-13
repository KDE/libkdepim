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

#ifndef KDEPIM_AGENTPROGRESSMONITOR_H
#define KDEPIM_AGENTPROGRESSMONITOR_H

#include <Libkdepim/ProgressManager> // ProgressItem

#include <AgentInstance>
namespace KPIM {
/**
 * @internal
 *
 * This class automatically updates a ProgressItem based on
 * Akonadi::AgentManager's signals, and places the abort() call if the
 * ProgressItem has been cancelled.
 */
class AgentProgressMonitor : public QObject
{
    Q_OBJECT
public:
    // used by our friend ProgressManager
    AgentProgressMonitor(const Akonadi::AgentInstance &mAgent, ProgressItem *mItem);
    ~AgentProgressMonitor();

private:
    void abort();
    void instanceProgressChanged(const Akonadi::AgentInstance &);
    void instanceStatusChanged(const Akonadi::AgentInstance &);
    void instanceRemoved(const Akonadi::AgentInstance &);
    void instanceNameChanged(const Akonadi::AgentInstance &);

    Akonadi::AgentInstance mAgent;
    QPointer<ProgressItem> const mItem;
};
}

#endif
