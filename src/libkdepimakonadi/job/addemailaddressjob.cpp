/*
  Copyright 2010 Tobias Koenig <tokoe@kde.org>
  Copyright 2010 Nicolas Lécureuil <nicolas.lecureuil@free.fr>

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

#include "addemailaddressjob.h"
#include <Libkdepim/BroadcastStatus>
#include "akonadi/contact/selectaddressbookdialog.h"

#include <KContacts/ContactGroup>
#include <CollectionDialog>
#include <Akonadi/Contact/ContactSearchJob>
#include <Item>
#include <ItemCreateJob>
#include <CollectionFetchJob>
#include <CollectionFetchScope>
#include <Collection>
#include <Akonadi/Contact/ContactEditorDialog>
#include <AgentTypeDialog>
#include <AgentType>
#include <AgentFilterProxyModel>
#include <AgentInstanceCreateJob>

#include <KLocalizedString>
#include <KMessageBox>

#include <QPointer>

using namespace KPIM;

class Q_DECL_HIDDEN AddEmailAddressJob::Private
{
public:
    Private(AddEmailAddressJob *qq, const QString &emailString, QWidget *parentWidget)
        : q(qq)
        , mCompleteAddress(emailString)
        , mParentWidget(parentWidget)
        , mInteractive(true)
    {
        KContacts::Addressee::parseEmailAddress(emailString, mName, mEmail);
    }

    void slotResourceCreationDone(KJob *job)
    {
        if (job->error()) {
            q->setError(job->error());
            q->setErrorText(job->errorText());
            q->emitResult();
            return;
        }
        createContact();
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

        const KContacts::Addressee::List contacts = searchJob->contacts();
        if (!contacts.isEmpty()) {
            if (mInteractive) {
                const QString text
                    = xi18nc("@info",
                             "A contact with the email address <email>%1</email> "
                             "is already in your address book.", mCompleteAddress);

                KMessageBox::information(
                    mParentWidget,
                    text,
                    QString(),
                    QStringLiteral("alreadyInAddressBook"));
            }
            q->setError(UserDefinedError);
            q->emitResult();
            return;
        }
        createContact();
    }

    void createContact()
    {
        const QStringList mimeTypes(KContacts::Addressee::mimeType());

        Akonadi::CollectionFetchJob *const addressBookJob
            = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(),
                                              Akonadi::CollectionFetchJob::Recursive);

        addressBookJob->fetchScope().setContentMimeTypes(mimeTypes);
        q->connect(addressBookJob, &Akonadi::CollectionFetchJob::result, q, [this](KJob *job) {
            slotCollectionsFetched(job);
        });
    }

    void slotCollectionsFetched(KJob *job)
    {
        if (job->error()) {
            q->setError(job->error());
            q->setErrorText(job->errorText());
            q->emitResult();
            return;
        }

        const Akonadi::CollectionFetchJob *addressBookJob
            = qobject_cast<Akonadi::CollectionFetchJob *>(job);

        Akonadi::Collection::List canCreateItemCollections;

        const Akonadi::Collection::List lstColls = addressBookJob->collections();
        for (const Akonadi::Collection &collection : lstColls) {
            if (Akonadi::Collection::CanCreateItem &collection.rights()) {
                canCreateItemCollections.append(collection);
            }
        }

        Akonadi::Collection addressBook;

        const int nbItemCollection(canCreateItemCollections.size());
        if (nbItemCollection == 0) {
            if (KMessageBox::questionYesNo(
                    mParentWidget,
                    i18nc("@info",
                          "You must create an address book before adding a contact. Do you want to create an address book?"),
                    i18nc("@title:window", "No Address Book Available")) == KMessageBox::Yes) {
                QPointer<Akonadi::AgentTypeDialog> dlg = new Akonadi::AgentTypeDialog(mParentWidget);
                dlg->setWindowTitle(i18nc("@title:window", "Add Address Book"));
                dlg->agentFilterProxyModel()->addMimeTypeFilter(KContacts::Addressee::mimeType());
                dlg->agentFilterProxyModel()->addMimeTypeFilter(KContacts::ContactGroup::mimeType());
                dlg->agentFilterProxyModel()->addCapabilityFilter(QStringLiteral("Resource"));

                if (dlg->exec()) {
                    const Akonadi::AgentType agentType = dlg->agentType();

                    if (agentType.isValid()) {
                        Akonadi::AgentInstanceCreateJob *job = new Akonadi::AgentInstanceCreateJob(agentType, q);
                        q->connect(job, &Akonadi::AgentInstanceCreateJob::result, q, [this](KJob *job) {
                            slotResourceCreationDone(job);
                        });
                        job->configure(mParentWidget);
                        job->start();
                        delete dlg;
                        return;
                    } else { //if agent is not valid => return error and finish job
                        q->setError(UserDefinedError);
                        q->emitResult();
                        delete dlg;
                        return;
                    }
                } else { //Canceled create agent => return error and finish job
                    q->setError(UserDefinedError);
                    q->emitResult();
                    delete dlg;
                    return;
                }
            } else {
                q->setError(UserDefinedError);
                q->emitResult();
                return;
            }
        } else if (nbItemCollection == 1) {
            addressBook = canCreateItemCollections[0];
        } else {
            // ask user in which address book the new contact shall be stored
            QPointer<Akonadi::SelectAddressBookDialog> dlg = new Akonadi::SelectAddressBookDialog(mParentWidget);

            bool gotIt = true;
            if (dlg->exec()) {
                addressBook = dlg->selectedCollection();
            } else {
                q->setError(UserDefinedError);
                q->emitResult();
                gotIt = false;
            }
            delete dlg;
            if (!gotIt) {
                return;
            }
        }

        if (!addressBook.isValid()) {
            q->setError(UserDefinedError);
            q->emitResult();
            return;
        }
        KContacts::Addressee contact;
        contact.setNameFromString(mName);
        contact.insertEmail(mEmail, true);

        // create the new item
        Akonadi::Item item;
        item.setMimeType(KContacts::Addressee::mimeType());
        item.setPayload<KContacts::Addressee>(contact);

        // save the new item in akonadi storage
        Akonadi::ItemCreateJob *createJob = new Akonadi::ItemCreateJob(item, addressBook, q);
        q->connect(createJob, &Akonadi::ItemCreateJob::result, q, [this](KJob *job) {
            slotAddContactDone(job);
        });
    }

    void slotAddContactDone(KJob *job)
    {
        if (job->error()) {
            q->setError(job->error());
            q->setErrorText(job->errorText());
            q->emitResult();
            return;
        }

        const Akonadi::ItemCreateJob *createJob = qobject_cast<Akonadi::ItemCreateJob *>(job);
        mItem = createJob->item();

        if (mInteractive) {
            const QString text
                = xi18nc("@info",
                         "<para>A contact for \"%1\" was successfully added "
                         "to your address book.</para>"
                         "<para>Do you want to edit this new contact now?</para>",
                         mCompleteAddress);

            if (KMessageBox::questionYesNo(
                    mParentWidget,
                    text,
                    QString(),
                    KStandardGuiItem::yes(),
                    KStandardGuiItem::no(),
                    QStringLiteral("addedtokabc")) == KMessageBox::Yes) {
                QPointer<Akonadi::ContactEditorDialog> dlg
                    = new Akonadi::ContactEditorDialog(Akonadi::ContactEditorDialog::EditMode,
                                                       mParentWidget);
                dlg->setContact(mItem);
                connect(dlg.data(), &Akonadi::ContactEditorDialog::contactStored,
                        q, [this](const Akonadi::Item &item) {
                    contactStored(item);
                });
                connect(dlg.data(), &Akonadi::ContactEditorDialog::error,
                        q, [this](const QString &str) {
                    slotContactEditorError(str);
                });
                dlg->exec();
                delete dlg;
            }
        }
        q->emitResult();
    }

    void slotContactEditorError(const QString &error)
    {
        if (mInteractive) {
            KMessageBox::error(mParentWidget, i18n("Contact cannot be stored: %1", error), i18n("Failed to store contact"));
        }
    }

    void contactStored(const Akonadi::Item &)
    {
        if (mInteractive) {
            KPIM::BroadcastStatus::instance()->setStatusMsg(i18n("Contact created successfully"));
        }
    }

    AddEmailAddressJob *q = nullptr;
    QString mCompleteAddress;
    QString mEmail;
    QString mName;
    QWidget *mParentWidget = nullptr;
    Akonadi::Item mItem;
    bool mInteractive = false;
};

AddEmailAddressJob::AddEmailAddressJob(const QString &email, QWidget *parentWidget, QObject *parent)
    : KJob(parent)
    , d(new Private(this, email, parentWidget))
{
}

AddEmailAddressJob::~AddEmailAddressJob()
{
    delete d;
}

void AddEmailAddressJob::start()
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

Akonadi::Item AddEmailAddressJob::contact() const
{
    return d->mItem;
}

void AddEmailAddressJob::setInteractive(bool b)
{
    d->mInteractive = b;
}

#include "moc_addemailaddressjob.cpp"
