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

#include "openemailaddressjob.h"

#include "job/addemailaddressjob.h"

#include <collectiondialog.h>
#include <Akonadi/Contact/ContactEditorDialog>
#include <Akonadi/Contact/ContactSearchJob>
#include <item.h>
#include <itemcreatejob.h>
#include <kcontacts/addressee.h>
#include <QPointer>
#include <KMessageBox>

using namespace KPIM;

class Q_DECL_HIDDEN OpenEmailAddressJob::Private
{
public:
    Private(OpenEmailAddressJob *qq, const QString &emailString, QWidget *parentWidget)
        : q(qq)
        , mCompleteAddress(emailString)
        , mParentWidget(parentWidget)
    {
        KContacts::Addressee::parseEmailAddress(emailString, mName, mEmail);
    }

    void slotSearchDone(KJob *job)
    {
        if (job->error()) {
            q->setError(job->error());
            q->setErrorText(job->errorText());
            q->emitResult();
            return;
        }

        const Akonadi::ContactSearchJob *searchJob = qobject_cast<Akonadi::ContactSearchJob *>(job);

        const Akonadi::Item::List contacts = searchJob->items();
        if (!contacts.isEmpty()) {
            // open the editor with the matching item
            QPointer<Akonadi::ContactEditorDialog> dlg = new Akonadi::ContactEditorDialog(Akonadi::ContactEditorDialog::EditMode, mParentWidget);
            dlg->setContact(contacts.first());
            dlg->exec();
            delete dlg;

            q->emitResult();
            return;
        }

        AddEmailAddressJob *createJob = new AddEmailAddressJob(mCompleteAddress, mParentWidget, q);
        q->connect(createJob, &AddEmailAddressJob::result, q, [this](KJob *job) {
            slotAddContactDone(job);
        });
        createJob->start();
    }

    void slotAddContactDone(KJob *job)
    {
        if (job->error()) {
            q->setError(job->error());
            q->setErrorText(job->errorText());
            q->emitResult();
            return;
        }

        const AddEmailAddressJob *createJob = qobject_cast<AddEmailAddressJob *>(job);

        // open the editor with the matching item
        QPointer<Akonadi::ContactEditorDialog> dlg = new Akonadi::ContactEditorDialog(Akonadi::ContactEditorDialog::EditMode, mParentWidget);
        dlg->setContact(createJob->contact());
        dlg->exec();
        delete dlg;

        q->emitResult();
    }

    OpenEmailAddressJob *q = nullptr;
    QString mCompleteAddress;
    QString mEmail;
    QString mName;
    QWidget *mParentWidget = nullptr;
};

OpenEmailAddressJob::OpenEmailAddressJob(const QString &email, QWidget *parentWidget, QObject *parent)
    : KJob(parent)
    , d(new Private(this, email, parentWidget))
{
}

OpenEmailAddressJob::~OpenEmailAddressJob()
{
    delete d;
}

void OpenEmailAddressJob::start()
{
    // first check whether a contact with the same email exists already
    Akonadi::ContactSearchJob *searchJob = new Akonadi::ContactSearchJob(this);
    searchJob->setLimit(1);
    searchJob->setQuery(Akonadi::ContactSearchJob::Email, d->mEmail.toLower(),
                        Akonadi::ContactSearchJob::ExactMatch);
    connect(searchJob, &Akonadi::ContactSearchJob::result, this, [this](KJob *job) {
        d->slotSearchDone(job);
    });
}

#include "moc_openemailaddressjob.cpp"
