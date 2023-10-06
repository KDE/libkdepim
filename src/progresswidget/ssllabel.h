/*
 *
 *  This file is part of KMail, the KDE mail client.
 *
 *  SPDX-FileCopyrightText: 2003 Zack Rusin <zack@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QLabel>

namespace KPIM
{
class SSLLabel : public QLabel
{
public:
    enum State { Encrypted, Unencrypted, Clean, Done, Unknown };
    explicit SSLLabel(QWidget *parent);

    void setEncrypted(State enc = Encrypted);
    void setState(State state);
    [[nodiscard]] State lastState() const;

private:
    State m_lastEncryptionState;
};
}
