/*
    SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "progressindicatorlabel.h"
#include <KBusyIndicatorWidget>

#include <QHBoxLayout>
#include <QLabel>

using namespace KPIM;
class KPIM::ProgressIndicatorLabelPrivate
{
public:
    ProgressIndicatorLabelPrivate(const QString &_label, ProgressIndicatorLabel *qq)
        : labelStr(_label)
        , q(qq)
    {
        QHBoxLayout *lay = new QHBoxLayout(q);
        lay->setContentsMargins(0, 0, 0, 0);
        indicator = new KBusyIndicatorWidget;
        lay->addWidget(indicator);
        label = new QLabel;
        lay->addWidget(label);
    }

    ~ProgressIndicatorLabelPrivate()
    {
    }

    void setActiveLabel(const QString &str)
    {
        if (!indicator->isHidden()) {
            label->setText(str);
        }
    }

    void start()
    {
        indicator->show();
        label->setText(labelStr);
    }

    void stop()
    {
        indicator->hide();
        label->clear();
    }

    QString labelStr;
    QLabel *label = nullptr;
    KBusyIndicatorWidget *indicator = nullptr;
    ProgressIndicatorLabel *q;
};

ProgressIndicatorLabel::ProgressIndicatorLabel(const QString &label, QWidget *parent)
    : QWidget(parent)
    , d(new ProgressIndicatorLabelPrivate(label, this))
{
    d->stop();
}

ProgressIndicatorLabel::ProgressIndicatorLabel(QWidget *parent)
    : QWidget(parent)
    , d(new ProgressIndicatorLabelPrivate(QString(), this))
{
    d->stop();
}

ProgressIndicatorLabel::~ProgressIndicatorLabel()
{
    delete d;
}

void ProgressIndicatorLabel::start()
{
    d->start();
}

void ProgressIndicatorLabel::stop()
{
    d->stop();
}

void ProgressIndicatorLabel::setActiveLabel(const QString &label)
{
    d->setActiveLabel(label);
}
