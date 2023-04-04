/*
    SPDX-FileCopyrightText: 2010 Casey Link <unnamedrambler@gmail.com>
    SPDX-FileCopyrightText: 2009-2010 Klaralvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>

    Refactored from earlier code by:
    SPDX-FileCopyrightText: 2010 Volker Krause <vkrause@kde.org>
    SPDX-FileCopyrightText: 2004 Cornelius Schumacher <schumacher@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "multiplyinglineview_p.h"

#include "libkdepim_debug.h"
#include <KLocalizedString>
#include <KMessageBox>

#include <QResizeEvent>
#include <QScrollBar>
#include <QTimer>
#include <QVBoxLayout>

using namespace KPIM;

MultiplyingLineView::MultiplyingLineView(MultiplyingLineFactory *factory, MultiplyingLineEditor *parent)
    : QScrollArea(parent)
    , mPage(new QWidget(this))
    , mTopLayout(new QVBoxLayout(this))
    , mMultiplyingLineFactory(factory)
{
    setWidgetResizable(true);
    setFrameStyle(QFrame::NoFrame);

    mPage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setWidget(mPage);

    mTopLayout->setContentsMargins(0, 0, 0, 0);
    mTopLayout->setSpacing(0);
    mPage->setLayout(mTopLayout);
}

MultiplyingLine *MultiplyingLineView::activeLine() const
{
    return mLines.last();
}

MultiplyingLine *MultiplyingLineView::emptyLine() const
{
    for (MultiplyingLine *line : std::as_const(mLines)) {
        if (line->isEmpty()) {
            return line;
        }
    }
    return nullptr;
}

MultiplyingLine *MultiplyingLineView::addLine(bool showDialogBox)
{
    const int maximumRecipients = mMultiplyingLineFactory->maximumRecipients();
    if (maximumRecipients != -1) {
        int numberOfLine = mLines.count();
        if (numberOfLine++ >= maximumRecipients) {
            if (showDialogBox) {
                KMessageBox::error(this, i18n("We can not add more recipients. We have reached maximum recipients"));
            }

            return nullptr;
        }
    }
    MultiplyingLine *line = mMultiplyingLineFactory->newLine(widget());

    mTopLayout->addWidget(line);
    line->setCompletionMode(mCompletionMode);
    line->show();
    connect(line, &MultiplyingLine::returnPressed, this, &MultiplyingLineView::slotReturnPressed);
    connect(line, &MultiplyingLine::upPressed, this, &MultiplyingLineView::slotUpPressed);
    connect(line, &MultiplyingLine::downPressed, this, &MultiplyingLineView::slotDownPressed);
    connect(line, &MultiplyingLine::rightPressed, this, &MultiplyingLineView::focusRight);
    connect(line, &MultiplyingLine::deleteLine, this, &MultiplyingLineView::slotDecideLineDeletion);
    connect(line, &MultiplyingLine::completionModeChanged, this, &MultiplyingLineView::setCompletionMode);

    if (!mLines.isEmpty()) {
        line->fixTabOrder(mLines.last()->tabOut());
    }
    mLines.append(line);
    mFirstColumnWidth = line->setColumnWidth(mFirstColumnWidth);
    mLineHeight = line->minimumSizeHint().height();
    line->resize(viewport()->width(), mLineHeight);
    resizeView();
    ensureVisible(0, mLines.count() * mLineHeight, 0, 0);

    QTimer::singleShot(0, this, &MultiplyingLineView::moveScrollBarToEnd);

    Q_EMIT lineAdded(line);
    return line;
}

void MultiplyingLineView::moveScrollBarToEnd()
{
    // scroll to bottom
    verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
}

void MultiplyingLineView::slotReturnPressed(MultiplyingLine *line)
{
    if (!line->data()->isEmpty()) {
        MultiplyingLine *empty = emptyLine();
        if (!empty) {
            empty = addLine(true);
        }
        if (!empty) {
            return;
        }
        activateLine(empty);
    }
}

void MultiplyingLineView::slotDownPressed(MultiplyingLine *line)
{
    const int pos = mLines.indexOf(line);
    if (pos >= (mLines.count() - 1)) {
        Q_EMIT focusDown();
    } else if (pos >= 0) {
        activateLine(mLines.at(pos + 1));
    }
}

void MultiplyingLineView::slotUpPressed(MultiplyingLine *line)
{
    const int pos = mLines.indexOf(line);
    if (pos > 0) {
        activateLine(mLines.at(pos - 1));
    } else {
        Q_EMIT focusUp();
    }
}

void MultiplyingLineView::slotDecideLineDeletion(MultiplyingLine *line)
{
    if (!line->isEmpty()) {
        mModified = true;
    }
    if (mLines.count() == 1) {
        line->clear();
    } else if (!line->canDeleteLineEdit()) {
        line->clear();
    } else if (mLines.indexOf(line) != mLines.count() - 1) {
        mCurDelLine = line;
        slotDeleteLine();
    }
}

void MultiplyingLineView::slotDeleteLine()
{
    if (!mCurDelLine) {
        return;
    }

    MultiplyingLine *line = mCurDelLine;
    line->aboutToBeDeleted();
    const int pos = mLines.indexOf(line);

    if (mCurDelLine->isActive()) {
        int newPos;
        if (pos == 0) {
            newPos = pos + 1;
        } else {
            newPos = pos - 1;
        }

        // if there is something left to activate, do so
        if (mLines.at(newPos)) {
            mLines.at(newPos)->activate();
        }
    }

    mLines.removeAll(line);
    line->hide();
    line->setParent(nullptr);
    line->deleteLater();

    if (pos > 0) {
        Q_EMIT lineDeleted(pos);
    }

    resizeView();
}

void MultiplyingLineView::resizeView()
{
    if (mDynamicSizeHint) {
        if (!mAutoResize) {
            if (mLines.count() < 6) {
                setMinimumHeight(mLineHeight * mLines.count());
            } else {
                setMinimumHeight(mLineHeight * 5);
                setMaximumHeight(mLineHeight * mLines.count());
            }
        } else {
            setMinimumHeight(mLineHeight * mLines.count());
        }
    }

    parentWidget()->layout()->activate();
    Q_EMIT sizeHintChanged();
}

void MultiplyingLineView::activateLine(MultiplyingLine *line)
{
    line->activate();
    ensureWidgetVisible(line);
}

void MultiplyingLineView::resizeEvent(QResizeEvent *ev)
{
    QScrollArea::resizeEvent(ev);
    const int numberLine(mLines.count());
    for (int i = 0; i < numberLine; ++i) {
        mLines.at(i)->resize(ev->size().width(), mLineHeight);
    }
    ensureVisible(0, numberLine * mLineHeight, 0, 0);
}

QSize MultiplyingLineView::sizeHint() const
{
    if (mDynamicSizeHint) {
        return {200, mLineHeight * static_cast<int>(mLines.count())};
    } else {
        return QScrollArea::sizeHint();
    }
}

QSize MultiplyingLineView::minimumSizeHint() const
{
    if (mDynamicSizeHint) {
        int height;
        int numLines = 5;
        if (mLines.count() < numLines) {
            height = mLineHeight * mLines.count();
        } else {
            height = mLineHeight * numLines;
        }
        return {200, height};
    } else {
        return QScrollArea::minimumSizeHint();
    }
}

QList<MultiplyingLineData::Ptr> MultiplyingLineView::allData() const
{
    QList<MultiplyingLineData::Ptr> data;

    QListIterator<MultiplyingLine *> it(mLines);
    while (it.hasNext()) {
        MultiplyingLine *line = it.next();
        if (!line->data()->isEmpty()) {
            data.append(line->data());
        }
    }

    return data;
}

void MultiplyingLineView::setCompletionMode(KCompletion::CompletionMode mode)
{
    if (mCompletionMode == mode) {
        return;
    }
    mCompletionMode = mode;

    QListIterator<MultiplyingLine *> it(mLines);
    while (it.hasNext()) {
        MultiplyingLine *line = it.next();
        line->blockSignals(true);
        line->setCompletionMode(mode);
        line->blockSignals(false);
    }
    Q_EMIT completionModeChanged(mode); // report change to MultiplyingLineEditor
}

void MultiplyingLineView::removeData(const MultiplyingLineData::Ptr &data)
{
    // search a line which matches recipient and type
    QListIterator<MultiplyingLine *> it(mLines);
    MultiplyingLine *line = nullptr;
    while (it.hasNext()) {
        line = it.next();
        if (line->data() == data) {
            break;
        }
    }
    if (line) {
        line->slotPropagateDeletion();
    }
}

bool MultiplyingLineView::isModified() const
{
    if (mModified) {
        return true;
    }

    QListIterator<MultiplyingLine *> it(mLines);
    MultiplyingLine *line = nullptr;
    while (it.hasNext()) {
        line = it.next();
        if (line->isModified()) {
            return true;
        }
    }

    return false;
}

void MultiplyingLineView::clearModified()
{
    mModified = false;

    QListIterator<MultiplyingLine *> it(mLines);
    while (it.hasNext()) {
        MultiplyingLine *line = it.next();
        line->clearModified();
    }
}

void MultiplyingLineView::setFocus()
{
    if (!mLines.empty() && mLines.constLast()->isActive()) {
        setFocusBottom();
    } else {
        setFocusTop();
    }
}

void MultiplyingLineView::setFocusTop()
{
    if (!mLines.empty()) {
        MultiplyingLine *line = mLines.constFirst();
        if (line) {
            line->activate();
        } else {
            qCWarning(LIBKDEPIM_LOG) << "No first";
        }
    } else {
        qCWarning(LIBKDEPIM_LOG) << "No first";
    }
}

void MultiplyingLineView::setFocusBottom()
{
    MultiplyingLine *line = mLines.constLast();
    if (line) {
        ensureWidgetVisible(line);
        line->activate();
    } else {
        qCWarning(LIBKDEPIM_LOG) << "No last";
    }
}

int MultiplyingLineView::setFirstColumnWidth(int w)
{
    mFirstColumnWidth = w;

    QListIterator<MultiplyingLine *> it(mLines);
    while (it.hasNext()) {
        MultiplyingLine *line = it.next();
        mFirstColumnWidth = line->setColumnWidth(mFirstColumnWidth);
    }

    resizeView();
    return mFirstColumnWidth;
}

QList<MultiplyingLine *> MultiplyingLineView::lines() const
{
    return mLines;
}

void MultiplyingLineView::setAutoResize(bool resize)
{
    mAutoResize = resize;

    if (mAutoResize) {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setMaximumHeight(QWIDGETSIZE_MAX);
    } else {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
}

bool MultiplyingLineView::autoResize() const
{
    return mAutoResize;
}

void MultiplyingLineView::setDynamicSizeHint(bool dynamic)
{
    mDynamicSizeHint = dynamic;
}

bool MultiplyingLineView::dynamicSizeHint() const
{
    return mDynamicSizeHint;
}
