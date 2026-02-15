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
  \class KPIM::MultiplyingLineFactory
  \inmodule Libkdepim
  \inheaderfile KPIM/MultiplyingLineFactory

  \brief An Abstract Base Class used to create MultiplyingLines
  Subclass this class and MultiplyingLine, then implement newLine() such that it allocates
  and returns an instance of your MultiplyingLine.
 */
class KDEPIM_EXPORT MultiplyingLineFactory : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor for MultiplyingLineFactory.
     * \param parent the parent QObject
     */
    explicit MultiplyingLineFactory(QObject *parent)
        : QObject(parent)
    {
    }

    /*!
     * \brief Destructor for MultiplyingLineFactory.
     */
    ~MultiplyingLineFactory() override = default;

    /*!
     * \brief Creates a new MultiplyingLine instance.
     * Subclasses must implement this to return their custom MultiplyingLine subclass.
     * \param parent the parent widget for the new line
     * \return a new MultiplyingLine instance
     */
    virtual MultiplyingLine *newLine(QWidget *parent) = 0;
    /*!
     * \brief Returns the maximum number of recipients this factory can create.
     * \return the maximum number of recipients, or -1 for unlimited
     */
    virtual int maximumRecipients()
    {
        return -1;
    }
};

/*!
  \class KPIM::MultiplyingLineEditor
  \inmodule Libkdepim
  \inheaderfile KPIM/MultiplyingLineEditor

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
    /*!
     * \brief Constructor for MultiplyingLineEditor.
     * \param factory the line factory that creates MultiplyingLine instances (ownership is transferred)
     * \param parent the parent widget
     */
    explicit MultiplyingLineEditor(MultiplyingLineFactory *factory, QWidget *parent = nullptr);

    /*!
     * \brief Destructor for MultiplyingLineEditor.
     */
    ~MultiplyingLineEditor() override;

    /*!
     * \brief Returns the line factory for this editor.
     * \return the MultiplyingLineFactory instance
     */
    MultiplyingLineFactory *factory() const;

    /*!
     * \brief Retrieves all data from all lines.
     * \return a list of MultiplyingLineData pointers
     */
    [[nodiscard]] QList<MultiplyingLineData::Ptr> allData() const;

    /*!
     * \brief Retrieves the data from the currently active line.
     * \return the active line's data
     */
    MultiplyingLineData::Ptr activeData() const;

    /*!
     * \brief Clears all lines from the editor.
     */
    void clear();

    /*!
     * \brief Returns whether any modifications have been made.
     * \return true if the user has modified the editor, false otherwise
     */
    [[nodiscard]] bool isModified() const;

    /*!
     * \brief Resets the modified flag.
     */
    void clearModified();

    /*!
     * \brief Adds a line with the given data to the editor.
     * \param data the data for the new line (nullptr creates an empty line)
     * \param showDialogBox whether to show a confirmation dialog
     * \return true if the line was added successfully
     */
    bool addData(const MultiplyingLineData::Ptr &data = MultiplyingLineData::Ptr(), bool showDialogBox = true);

    /*!
     * \brief Removes the line containing the given data.
     * \param data the data to remove
     */
    void removeData(const MultiplyingLineData::Ptr &data);

    /*!
     * \brief Sets the width of the first column.
     * This allows alignment with other widgets.
     * \param w the desired width in pixels
     * \return the actual width used
     */
    int setFirstColumnWidth(int w);

    /*!
     * \brief Sets the completion mode for all lines.
     * \param mode the KCompletion::CompletionMode to use
     */
    void setCompletionMode(KCompletion::CompletionMode mode);

    /*!
     * \brief Returns the list of all lines in the editor.
     * \return list of MultiplyingLine pointers
     */
    [[nodiscard]] QList<MultiplyingLine *> lines() const;

Q_SIGNALS:
    /*!
     * \brief Signal emitted when focus should move up.
     */
    void focusUp();
    /*!
     * \brief Signal emitted when focus should move down.
     */
    void focusDown();
    /*!
     * \brief Signal emitted when the completion mode changes.
     * \param mode the new completion mode
     */
    void completionModeChanged(KCompletion::CompletionMode);
    /*!
     * \brief Signal emitted when the size hint changes.
     */
    void sizeHintChanged();
    /*!
     * \brief Signal emitted when a line is deleted.
     * \param pos the position of the deleted line
     */
    void lineDeleted(int pos);
    /*!
     * \brief Signal emitted when a line is added.
     * \param line the newly added MultiplyingLine
     */
    void lineAdded(KPIM::MultiplyingLine *);

public Q_SLOTS:
    /*!
     * \brief Sets focus to the editor.
     */
    void setFocus();
    /*!
     * \brief Sets focus to the top line.
     */
    void setFocusTop();
    /*!
     * \brief Sets focus to the bottom line.
     */
    void setFocusBottom();

protected:
    bool mModified = false;

private:
    MultiplyingLineFactory *const mMultiplyingLineFactory;
    MultiplyingLineView *mView = nullptr;
};
}
