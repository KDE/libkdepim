/*
  Copyright (c) 2015-2019 Montel Laurent <montel@kde.org>

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

#ifndef BLACKLISTBALOOEMAILCOMPLETIONWIDGET_H
#define BLACKLISTBALOOEMAILCOMPLETIONWIDGET_H

#include <QWidget>
#include "libkdepimakonadi_private_export.h"
class QPushButton;
class KLineEdit;
class QLabel;
class KListWidgetSearchLine;
namespace KPIM {
class BlackListBalooEmailList;
class BlackListBalooEmailWarning;
class LIBKDEPIMAKONADI_TESTS_EXPORT BlackListBalooEmailCompletionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BlackListBalooEmailCompletionWidget(QWidget *parent = nullptr);
    ~BlackListBalooEmailCompletionWidget();

    void save();
    void load();
    void setEmailBlackList(const QStringList &list);
private:
    void slotSelectionChanged();
    void slotUnselectEmails();
    void slotSelectEmails();
    void slotSearchLineEditChanged(const QString &text);
    void slotSearch();
    void slotLinkClicked(const QString &link);
    void slotEmailFound(const QStringList &list);
    void slotCheckIfUpdateBlackListIsNeeded();
    void slotSaveChanges();
    void slotShowAllBlacklistedEmail();
    void hideMoreResultAndChangeLimit();
    QStringList mOriginalExcludeDomain;
    QLabel *mNumberOfEmailsFound = nullptr;
    KLineEdit *mSearchLineEdit = nullptr;
    KLineEdit *mExcludeDomainLineEdit = nullptr;
    BlackListBalooEmailList *mEmailList = nullptr;
    QPushButton *mSearchButton = nullptr;
    QPushButton *mSelectButton = nullptr;
    QPushButton *mUnselectButton = nullptr;
    QPushButton *mShowAllBlackListedEmails = nullptr;
    QLabel *mMoreResult = nullptr;
    KListWidgetSearchLine *mSearchInResultLineEdit = nullptr;
    BlackListBalooEmailWarning *mBlackListWarning = nullptr;
    int mLimit = 500;
};
}

#endif // BLACKLISTBALOOEMAILCOMPLETIONWIDGET_H
