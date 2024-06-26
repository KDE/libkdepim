/*
    SPDX-FileCopyrightText: 2010 Casey Link <unnamedrambler@gmail.com>
    SPDX-FileCopyrightText: 2009-2010 Klaralvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>

    Refactored from earlier code by:
    SPDX-FileCopyrightText: 2010 Volker Krause <vkrause@kde.org>
    SPDX-FileCopyrightText: 2004 Cornelius Schumacher <schumacher@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "multiplyingline.h"
#include "multiplyinglineeditor.h"

#include <QPointer>
#include <QScrollArea>

namespace KPIM
{
class MultiplyingLineView : public QScrollArea
{
    Q_OBJECT
public:
    explicit MultiplyingLineView(MultiplyingLineFactory *factory, MultiplyingLineEditor *parent);
    ~MultiplyingLineView() override = default;

    [[nodiscard]] QSize minimumSizeHint() const override;
    [[nodiscard]] QSize sizeHint() const override;

    [[nodiscard]] MultiplyingLine *activeLine() const;

    [[nodiscard]] MultiplyingLine *emptyLine() const;

    [[nodiscard]] QList<MultiplyingLineData::Ptr> allData() const;

    /** Removes data provided it can be found. The Data class must support operator==
        @param data The data you want to remove.
    */
    void removeData(const MultiplyingLineData::Ptr &data);

    /** Returns true if the user has made any modifications to the list of
        recipients.
        @return whether the view is modified or not.
    */
    [[nodiscard]] bool isModified() const;

    /** Resets the modified flag to false.
     */
    void clearModified();

    /** Activates the line */
    void activateLine(MultiplyingLine *line);

    /**QScrollArea
     * Set the width of the left most column to be the argument width.
     * This method allows other widgets to align their label/combobox column with ours
     * by communicating how many pixels that first column is for them.
     * Returns the width that is actually being used.
     */
    int setFirstColumnWidth(int);

    [[nodiscard]] QList<MultiplyingLine *> lines() const;
    MultiplyingLine *addLine(bool showDialogBox);

public Q_SLOTS:
    void setCompletionMode(KCompletion::CompletionMode mode);

    void setFocus();
    void setFocusTop();
    void setFocusBottom();

Q_SIGNALS:
    void focusUp();
    void focusDown();
    void focusRight();
    void completionModeChanged(KCompletion::CompletionMode);
    void sizeHintChanged();
    void lineDeleted(int pos);
    void lineAdded(KPIM::MultiplyingLine *);

protected:
    void resizeEvent(QResizeEvent *) override;
    void resizeView();

protected Q_SLOTS:
    void slotReturnPressed(KPIM::MultiplyingLine *);
    void slotDownPressed(KPIM::MultiplyingLine *);
    void slotUpPressed(KPIM::MultiplyingLine *);
    void slotDecideLineDeletion(KPIM::MultiplyingLine *);
    void slotDeleteLine();
    void moveScrollBarToEnd();

private:
    QList<MultiplyingLine *> mLines;
    QPointer<MultiplyingLine> mCurDelLine = nullptr;
    QWidget *mPage = nullptr;
    QLayout *mTopLayout = nullptr;
    MultiplyingLineFactory *mMultiplyingLineFactory = nullptr;
    int mLineHeight = 0;
    int mFirstColumnWidth = 0;
    KCompletion::CompletionMode mCompletionMode = KCompletion::CompletionNone;
    bool mModified = false;
};
}
