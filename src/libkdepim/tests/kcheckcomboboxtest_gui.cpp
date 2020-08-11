/*
    This file is part of libkdepim.

    SPDX-FileCopyrightText: 2010 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QApplication>
#include <QCommandLineParser>
#include <QStandardPaths>

#include "../src/libkdepim/widgets/kcheckcombobox.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    QStandardPaths::setTestModeEnabled(true);
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    KPIM::KCheckComboBox *combo = new KPIM::KCheckComboBox(nullptr);
    combo->addItems(QStringList() << QStringLiteral("KDE") << QStringLiteral("Mac OSX") << QStringLiteral("Windows") << QStringLiteral("XFCE") << QStringLiteral("FVWM") << QStringLiteral("TWM"));
    combo->setCheckedItems(QStringList() << QStringLiteral("KDE") << QStringLiteral("Mac OSX") << QStringLiteral("Windows"));
    combo->resize(400, 20);
    combo->setSqueezeText(true);
    combo->setDefaultText(QStringLiteral("Default text"));
    combo->show();

    return app.exec();
}
