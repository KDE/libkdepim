/*
    SPDX-FileCopyrightText: 2010 Casey Link <unnamedrambler@gmail.com>
    SPDX-FileCopyrightText: 2009-2010 Klaralvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>

    Refactored from earlier code by:
    SPDX-FileCopyrightText: 2010 Volker Krause <vkrause@kde.org>
    SPDX-FileCopyrightText: 2004 Cornelius Schumacher <schumacher@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kdepim_export.h"

#include "multiplyingline.h"

#include <KCompletion>
#include <QObject>
#include <QWidget>

namespace KPIM
{
class MultiplyingLineView;

/*!
  \brief An Abstract Base Class used to create MultiplyingLines
  Subclass this class and MultiplyingLine, then implement newLine() such that it allocates
  and returns an instance of your MultiplyingLine.
 */
class KDEPIM_EXPORT MultiplyingLineFactory : public QObject
{
    Q_OBJECT
public:
    explicit MultiplyingLineFactory(QObject *parent)
        : QObject(parent)
    {
    }

    ~MultiplyingLineFactory() override = default;

    virtual MultiplyingLine *newLine(QWidget *parent) = 0;
    virtual int maximumRecipients()
    {
        return -1;
    }
};

/*!
  \brief An editor that adds rows (lines) of widgets and deletes them as the user edits

  Line widgets in the MultiplyingLineEditor are usually composed of multiple
  basic widgets. An example is below:

  -------------------------------------------------
  | ComboBox|   Line edit             |  Checkbox |  <-- 1 line
  -------------------------------------------------
  | ComboBox|   Line edit             |  Checkbox | <-- another line

  Default behavior is one line with default settings, and when
  the user edits it, another line is automatically added.
  Lines are added and deleted on demand.

  Implement this class and MultiplyingLineData. Then implement
  MultiplyingLineFactory to return instances of your line.
*/
class KDEPIM_EXPORT MultiplyingLineEditor : public QWidget
{
    Q_OBJECT

public:
    // We take ownership of factory
    explicit MultiplyingLineEditor(MultiplyingLineFactory *factory, QWidget *parent = nullptr);

    ~MultiplyingLineEditor() override;

    /*! Get the current line factory for this instance of the widget.
     */
    MultiplyingLineFactory *factory() const;

    /*! Retrieve the data from the editor */
    [[nodiscard]] QList<MultiplyingLineData::Ptr> allData() const;

    /*! Retrieve the data of the active line */
    MultiplyingLineData::Ptr activeData() const;

    /*! Clear all lines from the widget.
     */
    void clear();

    /*! Returns true if the user has made any modifications to the list of
        recipients.
    */
    [[nodiscard]] bool isModified() const;

    /*! Resets the modified flag to false.
     */
    void clearModified();

    /*! Adds data to one line of the editor.
        \a data The data you want to add.
        Can be used to add an empty/default  line.
    */
    bool addData(const MultiplyingLineData::Ptr &data = MultiplyingLineData::Ptr(), bool showDialogBox = true);

    /*! Removes data provided it can be found. The Data class must support operator==
        \a data The data you want to add.
    */
    void removeData(const MultiplyingLineData::Ptr &data);

    /*!
      Set the width of the left most column to be the argument width.
      This method allows other widgets to align their label/combobox column with ours
      by communicating how many pixels that first column is for them.
      \a w what the left most column width should be
      Returns the width that is actually being used.
      */
    int setFirstColumnWidth(int w);

    /*!
      Set completion mode for all lines
      \a mode the completion mode
      */
    void setCompletionMode(KCompletion::CompletionMode mode);

    [[nodiscard]] QList<MultiplyingLine *> lines() const;

Q_SIGNALS:
    void focusUp();
    void focusDown();
    void completionModeChanged(KCompletion::CompletionMode);
    void sizeHintChanged();
    void lineDeleted(int pos);
    void lineAdded(KPIM::MultiplyingLine *);

public Q_SLOTS:
    void setFocus();
    void setFocusTop();
    void setFocusBottom();

protected:
    bool mModified = false;

private:
    MultiplyingLineFactory *const mMultiplyingLineFactory;
    MultiplyingLineView *mView = nullptr;
};
}
