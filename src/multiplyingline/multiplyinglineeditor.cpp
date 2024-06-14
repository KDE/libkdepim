/*
    SPDX-FileCopyrightText: 2010 Casey Link <unnamedrambler@gmail.com>
    SPDX-FileCopyrightText: 2009-2010 Klaralvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>

    Refactored from earlier code by:
    SPDX-FileCopyrightText: 2010 Volker Krause <vkrause@kde.org>
    SPDX-FileCopyrightText: 2004 Cornelius Schumacher <schumacher@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "multiplyinglineeditor.h"
#include "multiplyinglineview_p.h"

#include <QHBoxLayout>
using namespace KPIM;
MultiplyingLineEditor::MultiplyingLineEditor(MultiplyingLineFactory *factory, QWidget *parent)
    : QWidget(parent)
    , mMultiplyingLineFactory(factory)
{
    auto topLayout = new QHBoxLayout(this);
    topLayout->setContentsMargins(0, 0, 0, 0);

    mView = new MultiplyingLineView(mMultiplyingLineFactory, this);
    topLayout->addWidget(mView);
    connect(mView, &MultiplyingLineView::focusUp, this, &MultiplyingLineEditor::focusUp);
    connect(mView, &MultiplyingLineView::focusDown, this, &MultiplyingLineEditor::focusDown);
    connect(mView, &MultiplyingLineView::completionModeChanged, this, &MultiplyingLineEditor::completionModeChanged);
    connect(mView, &MultiplyingLineView::lineDeleted, this, &MultiplyingLineEditor::lineDeleted);
    connect(mView, &MultiplyingLineView::lineAdded, this, &MultiplyingLineEditor::lineAdded);
    connect(mView, &MultiplyingLineView::sizeHintChanged, this, &MultiplyingLineEditor::sizeHintChanged);
}

MultiplyingLineEditor::~MultiplyingLineEditor()
{
    delete mMultiplyingLineFactory;
}

bool MultiplyingLineEditor::addData(const MultiplyingLineData::Ptr &data, bool showDialogBox)
{
    MultiplyingLine *line = mView->emptyLine();
    bool tooManyAddress = false;
    if (!line) {
        line = mView->addLine(showDialogBox);
    }
    if (!line) {
        tooManyAddress = true;
    }
    if (line && data) {
        line->setData(data);
    }
    return tooManyAddress;
}

void MultiplyingLineEditor::removeData(const MultiplyingLineData::Ptr &data)
{
    mView->removeData(data);
}

void MultiplyingLineEditor::clear()
{
    const auto lstLines = mView->lines();
    for (MultiplyingLine *line : lstLines) {
        line->slotPropagateDeletion();
    }
}

bool MultiplyingLineEditor::isModified() const
{
    return mModified || mView->isModified();
}

void MultiplyingLineEditor::clearModified()
{
    mModified = false;
    mView->clearModified();
}

void MultiplyingLineEditor::setFocus()
{
    mView->setFocus();
}

void MultiplyingLineEditor::setFocusTop()
{
    mView->setFocusTop();
}

void MultiplyingLineEditor::setFocusBottom()
{
    mView->setFocusBottom();
}

int MultiplyingLineEditor::setFirstColumnWidth(int w)
{
    return mView->setFirstColumnWidth(w);
}

void MultiplyingLineEditor::setCompletionMode(KCompletion::CompletionMode mode)
{
    mView->setCompletionMode(mode);
}

MultiplyingLineFactory *MultiplyingLineEditor::factory() const
{
    return mMultiplyingLineFactory;
}

QList<MultiplyingLineData::Ptr> MultiplyingLineEditor::allData() const
{
    return mView->allData();
}

MultiplyingLineData::Ptr MultiplyingLineEditor::activeData() const
{
    return mView->activeLine()->data();
}

QList<MultiplyingLine *> MultiplyingLineEditor::lines() const
{
    return mView->lines();
}

MultiplyingLine *MultiplyingLineEditor::activeLine() const
{
    return mView->activeLine();
}

void MultiplyingLineEditor::setFrameStyle(int shape)
{
    mView->setFrameStyle(shape);
}

void MultiplyingLineEditor::setAutoResizeView(bool resize)
{
    mView->setAutoResize(resize);
}

bool MultiplyingLineEditor::autoResizeView()
{
    return mView->autoResize();
}

void MultiplyingLineEditor::setDynamicSizeHint(bool dynamic)
{
    mView->setDynamicSizeHint(dynamic);
}

bool MultiplyingLineEditor::dynamicSizeHint() const
{
    return mView->dynamicSizeHint();
}

#include "moc_multiplyinglineeditor.cpp"
