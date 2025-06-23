/*
    This file is part of libkdepim.

    SPDX-FileCopyrightText: 2010 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QApplication>
using namespace Qt::Literals::StringLiterals;

#include <QCommandLineParser>
#include <QStandardPaths>

#include "widgets/kcheckcombobox.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    QStandardPaths::setTestModeEnabled(true);
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    auto combo = new KPIM::KCheckComboBox(nullptr);
    combo->addItems(QStringList() << u"KDE"_s << u"Mac OSX"_s << QStringLiteral("Windows") << QStringLiteral("XFCE") << u"FVWM"_s << u"TWM"_s);
    combo->setCheckedItems(QStringList() << u"KDE"_s << u"Mac OSX"_s << QStringLiteral("Windows"));
    combo->resize(400, 20);
    combo->setSqueezeText(true);
    combo->setDefaultText(u"Default text"_s);
    combo->show();

    return app.exec();
}
