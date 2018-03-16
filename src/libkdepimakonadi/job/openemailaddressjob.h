/*
  Copyright 2010 Tobias Koenig <tokoe@kde.org>

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

#ifndef OPENEMAILADDRESSJOB_H
#define OPENEMAILADDRESSJOB_H

#include "kdepimakonadi_export.h"

#include <kjob.h>

namespace Akonadi {
}

namespace KPIM {
/**
 * @short A job to open the contact editor for a contact with a given email address.
 *
 * The job will check whether a contact with the given email address already
 * exists in Akonadi. If not, it will add a new contact with the email address
 * to Akonadi and then opens the contact editor.
 */
class KDEPIMAKONADI_EXPORT OpenEmailAddressJob : public KJob
{
    Q_OBJECT

public:
    /**
     * Creates a new open email address job.
     *
     * @param email The email address to open.
     * @param parentWidget The widget that will be used as parent for dialog.
     * @param parent The parent object.
     */
    OpenEmailAddressJob(const QString &email, QWidget *parentWidget, QObject *parent = nullptr);

    /**
     * Destroys the open email address job.
     */
    ~OpenEmailAddressJob() override;

    /**
     * Starts the job.
     */
    void start() override;

private:
    //@cond PRIVATE
    class Private;
    Private *const d;
    //@endcond
};
}

#endif
