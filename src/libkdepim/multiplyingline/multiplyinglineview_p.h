/*
    SPDX-FileCopyrightText: 2010 Casey Link <unnamedrambler@gmail.com>
    SPDX-FileCopyrightText: 2009-2010 Klaralvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>

    Refactored from earlier code by:
    SPDX-FileCopyrightText: 2010 Volker Krause <vkrause@kde.org>
    SPDX-FileCopyrightText: 2004 Cornelius Schumacher <schumacher@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MULTIPLYINGLINEVIEW_P_H
#define MULTIPLYINGLINEVIEW_P_H

#include "multiplyingline.h"
#include "multiplyinglineeditor.h"

#include <QPointer>
#include <QScrollArea>

namespace KPIM {
class MultiplyingLineView : public QScrollArea
{
    Q_OBJECT
public:
    MultiplyingLineView(MultiplyingLineFactory *factory, MultiplyingLineEditor *parent);
    ~MultiplyingLineView() override
    {
    }

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    MultiplyingLine *activeLine() const;

    MultiplyingLine *emptyLine() const;

    QVector<MultiplyingLineData::Ptr> allData() const;

    /** Removes data provided it can be found. The Data class must support operator==
        @param data The data you want to remove.
    */
    void removeData(const MultiplyingLineData::Ptr &data);

    /** Returns true if the user has made any modifications to the list of
        recipients.
        @return whether the view is modified or not.
    */
    bool isModified() const;

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

    /**
     Make this widget follow it's children's size
     @param resize turn on or off this behavior of auto resizing
     */
    void setAutoResize(bool resize);
    bool autoResize();

    /**
     * Sets whether the size hint of the editor shall be calculated
     * dynamically by the number of lines. Default is @c true.
     */
    void setDynamicSizeHint(bool dynamic);
    bool dynamicSizeHint() const;

    QList<MultiplyingLine *> lines() const;

public Q_SLOTS:
    void setCompletionMode(KCompletion::CompletionMode mode);
    MultiplyingLine *addLine();

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
    bool mAutoResize = false;
    bool mDynamicSizeHint = true;
    bool mModified = false;
};
}

#endif //MULTIPLYINGLINEVIEW_P_H
