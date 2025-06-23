/*
 *
 *  This file is part of KMail, the KDE mail client.
 *
 *  SPDX-FileCopyrightText: 2003 Zack Rusin <zack@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "ssllabel.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

#include <QIcon>
#include <QStyle>

using namespace KPIM;

SSLLabel::SSLLabel(QWidget *parent)
    : QLabel(parent)
{
    setState(Done);
}

void SSLLabel::setEncrypted(SSLLabel::State state)
{
    m_lastEncryptionState = state;
}

SSLLabel::State SSLLabel::lastState() const
{
    return m_lastEncryptionState;
}

void SSLLabel::setState(State state)
{
    switch (state) {
    case Encrypted:
        setToolTip(i18nc("@info:tooltip", "Connection is encrypted"));
        setPixmap(QIcon::fromTheme(u"security-high"_s).pixmap(style()->pixelMetric(QStyle::PM_SmallIconSize)));
        show();
        break;
    case Unencrypted:
        setToolTip(i18nc("@info:tooltip", "Connection is unencrypted"));
        setPixmap(QIcon::fromTheme(u"security-low"_s).pixmap(style()->pixelMetric(QStyle::PM_SmallIconSize)));
        show();
        break;
    case Unknown:
    case Done:
        setToolTip(QString());
        hide();
        break;
    case Clean:
        setToolTip(QString());
        hide();
        // we return because we do not save the state as the only
        // action we want to perform is to hide ourself
        return;
    }
    m_lastEncryptionState = state;
}

#include "moc_ssllabel.cpp"
