/*
  Copyright (c) 2015-2018 Montel Laurent <montel@kde.org>

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

#include "blacklistindexemailsearchjob.h"

#include <AkonadiSearch/ContactCompleter>

using namespace KPIM;

BlackListIndexEmailSearchJob::BlackListIndexEmailSearchJob(QObject *parent)
    : QObject(parent)
    , mLimit(500)
{
}

BlackListIndexEmailSearchJob::~BlackListIndexEmailSearchJob()
{
}

bool BlackListIndexEmailSearchJob::start()
{
    const QString trimmedString = mSearchEmail.trimmed();
    if (trimmedString.isEmpty()) {
        deleteLater();
        return false;
    }

    auto completer = new Akonadi::Search::ContactCompleter(trimmedString, mLimit);
    connect(completer, &Akonadi::Search::ContactCompleter::finished,
            this, [this](const QStringList &results) {
                Q_EMIT emailsFound(results);
                deleteLater();
            });
    completer->start();
    return true;
}

void BlackListIndexEmailSearchJob::setSearchEmail(const QString &searchEmail)
{
    mSearchEmail = searchEmail;
}

void BlackListIndexEmailSearchJob::setLimit(int limit)
{
    mLimit = qMax(10, limit);
}
