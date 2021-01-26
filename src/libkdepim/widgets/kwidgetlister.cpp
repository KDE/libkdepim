/*  -*- c++ -*-

  kwidgetlister.cpp

  This file is part of libkdepim.
  SPDX-FileCopyrightText: 2001 Marc Mutz <mutz@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kwidgetlister.h"

#include <KGuiItem>
#include <KLocalizedString>
#include <QHBoxLayout>

#include <QPushButton>
#include <QVBoxLayout>

#include <KStandardGuiItem>
#include <assert.h>

using namespace KPIM;

class Q_DECL_HIDDEN KWidgetLister::Private
{
public:
    Private(KWidgetLister *qq)
        : q(qq)
    {
    }

    ~Private()
    {
        qDeleteAll(mWidgetList);
        mWidgetList.clear();
    }

    void enableControls();

    KWidgetLister *q = nullptr;
    QPushButton *mBtnMore = nullptr;
    QPushButton *mBtnFewer = nullptr;
    QPushButton *mBtnClear = nullptr;
    QVBoxLayout *mLayout = nullptr;
    QWidget *mButtonBox = nullptr;
    QList<QWidget *> mWidgetList;
    int mMinWidgets = 0;
    int mMaxWidgets = 0;
};

void KWidgetLister::Private::enableControls()
{
    const int count = mWidgetList.count();
    const bool isMaxWidgets = (count >= mMaxWidgets);
    const bool isMinWidgets = (count <= mMinWidgets);
    if (mBtnMore) {
        mBtnMore->setEnabled(!isMaxWidgets);
    }
    if (mBtnFewer) {
        mBtnFewer->setEnabled(!isMinWidgets);
    }
}

KWidgetLister::KWidgetLister(bool fewerMoreButton, int minWidgets, int maxWidgets, QWidget *parent)
    : QWidget(parent)
    , d(new Private(this))
{
    d->mMinWidgets = qMax(minWidgets, 1);
    d->mMaxWidgets = qMax(maxWidgets, d->mMinWidgets + 1);
    init(fewerMoreButton);
}

KWidgetLister::~KWidgetLister()
{
    delete d;
}

void KWidgetLister::init(bool fewerMoreButton)
{
    //--------- the button box
    d->mLayout = new QVBoxLayout(this);
    d->mLayout->setContentsMargins(0, 0, 0, 0);
    d->mLayout->setSpacing(4);

    d->mButtonBox = new QWidget(this);
    auto mButtonBoxHBoxLayout = new QHBoxLayout(d->mButtonBox);
    mButtonBoxHBoxLayout->setContentsMargins(0, 0, 0, 0);
    d->mLayout->addWidget(d->mButtonBox);

    if (fewerMoreButton) {
        d->mBtnMore = new QPushButton(d->mButtonBox);
        mButtonBoxHBoxLayout->addWidget(d->mBtnMore);
        KGuiItem::assign(d->mBtnMore, KGuiItem(i18nc("more widgets", "More"), QStringLiteral("list-add")));
        mButtonBoxHBoxLayout->setStretchFactor(d->mBtnMore, 0);

        d->mBtnFewer = new QPushButton(d->mButtonBox);
        mButtonBoxHBoxLayout->addWidget(d->mBtnFewer);
        KGuiItem::assign(d->mBtnFewer, KGuiItem(i18nc("fewer widgets", "Fewer"), QStringLiteral("list-remove")));
        mButtonBoxHBoxLayout->setStretchFactor(d->mBtnFewer, 0);
    }
    auto spacer = new QWidget(d->mButtonBox);
    mButtonBoxHBoxLayout->addWidget(spacer);
    mButtonBoxHBoxLayout->setStretchFactor(spacer, 1);

    d->mBtnClear = new QPushButton(d->mButtonBox);
    mButtonBoxHBoxLayout->addWidget(d->mBtnClear);
    KGuiItem::assign(d->mBtnClear, KStandardGuiItem::clear());
    // FIXME a useful whats this. KStandardGuiItem::clear() returns a text with an edit box
    d->mBtnClear->setWhatsThis(QString());
    mButtonBoxHBoxLayout->setStretchFactor(d->mBtnClear, 0);

    //---------- connect everything
    if (fewerMoreButton) {
        connect(d->mBtnMore, &QPushButton::clicked, this, &KWidgetLister::slotMore);
        connect(d->mBtnFewer, &QPushButton::clicked, this, &KWidgetLister::slotFewer);
    }

    connect(d->mBtnClear, &QPushButton::clicked, this, &KWidgetLister::slotClear);

    d->enableControls();
}

void KWidgetLister::slotMore()
{
    // the class should make certain that slotMore can't
    // be called when mMaxWidgets are on screen.
    assert(d->mWidgetList.count() < d->mMaxWidgets);

    addWidgetAtEnd();
    //  adjustSize();
    d->enableControls();
}

void KWidgetLister::slotFewer()
{
    // the class should make certain that slotFewer can't
    // be called when mMinWidgets are on screen.
    assert(d->mWidgetList.count() > d->mMinWidgets);

    removeLastWidget();
    //  adjustSize();
    d->enableControls();
}

void KWidgetLister::slotClear()
{
    setNumberOfShownWidgetsTo(d->mMinWidgets);

    // clear remaining widgets
    for (QWidget *widget : qAsConst(d->mWidgetList)) {
        clearWidget(widget);
    }

    //  adjustSize();
    d->enableControls();
    Q_EMIT clearWidgets();
}

void KWidgetLister::addWidgetAtEnd(QWidget *widget)
{
    if (!widget) {
        widget = this->createWidget(this);
    }

    d->mLayout->insertWidget(d->mLayout->indexOf(d->mButtonBox), widget);
    d->mWidgetList.append(widget);
    widget->show();

    d->enableControls();
    Q_EMIT widgetAdded();
    Q_EMIT widgetAdded(widget);
}

void KWidgetLister::removeLastWidget()
{
    // The layout will take care that the
    // widget is removed from screen, too.
    delete d->mWidgetList.takeLast();
    d->enableControls();
    Q_EMIT widgetRemoved();
}

void KWidgetLister::clearWidget(QWidget *widget)
{
    Q_UNUSED(widget)
}

QWidget *KWidgetLister::createWidget(QWidget *parent)
{
    return new QWidget(parent);
}

void KWidgetLister::setNumberOfShownWidgetsTo(int aNum)
{
    int superfluousWidgets = qMax(d->mWidgetList.count() - aNum, 0);
    int missingWidgets = qMax(aNum - d->mWidgetList.count(), 0);

    // remove superfluous widgets
    for (; superfluousWidgets; superfluousWidgets--) {
        removeLastWidget();
    }

    // add missing widgets
    for (; missingWidgets; missingWidgets--) {
        addWidgetAtEnd();
    }
}

QList<QWidget *> KWidgetLister::widgets() const
{
    return d->mWidgetList;
}

int KWidgetLister::widgetsMinimum() const
{
    return d->mMinWidgets;
}

int KWidgetLister::widgetsMaximum() const
{
    return d->mMaxWidgets;
}

void KWidgetLister::removeWidget(QWidget *widget)
{
    // The layout will take care that the
    // widget is removed from screen, too.

    if (d->mWidgetList.count() <= widgetsMinimum()) {
        return;
    }

    const int index = d->mWidgetList.indexOf(widget);
    QWidget *w = d->mWidgetList.takeAt(index);
    w->deleteLater();
    w = nullptr;
    d->enableControls();
    Q_EMIT widgetRemoved(widget);
    Q_EMIT widgetRemoved();
}

void KWidgetLister::addWidgetAfterThisWidget(QWidget *currentWidget, QWidget *widget)
{
    if (!widget) {
        widget = this->createWidget(this);
    }

    int index = d->mLayout->indexOf(currentWidget ? currentWidget : d->mButtonBox) + 1;
    d->mLayout->insertWidget(index, widget);
    if (currentWidget) {
        index = d->mWidgetList.indexOf(currentWidget);
        d->mWidgetList.insert(index + 1, widget);
    } else {
        d->mWidgetList.append(widget);
    }
    widget->show();

    d->enableControls();
    Q_EMIT widgetAdded();
    Q_EMIT widgetAdded(widget);
}
