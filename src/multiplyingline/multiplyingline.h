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

#include <KCompletion>
#include <QSharedPointer>
#include <QWidget>

namespace KPIM
{
/*!
  \class KPIM::MultiplyingLineData
  \inmodule Libkdepim
  \inheaderfile KPIM/MultiplyingLineData

  \brief ABC representing line data
  \author Casey Link
*/
class KDEPIM_EXPORT MultiplyingLineData
{
public:
    using Ptr = QSharedPointer<MultiplyingLineData>;
    /*!
      \brief Destructor for MultiplyingLineData.
    */
    virtual ~MultiplyingLineData() = default;

    /*!
      Clear data, reset to defaults
    */
    virtual void clear() = 0;
    /*!
      Is the data empty?
    */
    virtual bool isEmpty() const = 0;
};

/*!
 \class KPIM::MultiplyingLineData
 \inmodule Libkdepim
 \inheaderfile KPIM/MultiplyingLineData

 \brief Abstract Base Class representing a line in the Multiplying line widget.
 This class (and its subclasses) represent the lines in the MultiplyingLineEditor. Users of the
 MultiplyingLineEditor widget should subclass this class, and add their own input widgets as members,
 then implement the pure virtual methods and connect all the appropriate slots.
 \author Casey Link
*/
class KDEPIM_EXPORT MultiplyingLine : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor for MultiplyingLine.
     * \param parent the parent widget
     */
    explicit MultiplyingLine(QWidget *parent);
    /*!
      \brief Destructor for MultiplyingLine.
    */
    ~MultiplyingLine() override;

    /*!
      This line is being activated. Focus should be set
      on the first or most important widget in the line.
      */
    virtual void activate() = 0;
    /*!
     Check if whatever receives focus in activate()
     currently has focus.
     Returns true if this line is active
     */
    virtual bool isActive() const = 0;

    /*!
      Determine if this line was modified.
      Returns true if the user has made any modifications to this
      MultiplyingLine.
    */
    virtual bool isModified() const = 0;

    /*!
      Resets the modified flag to false.
    */
    virtual void clearModified() = 0;

    /*!
      Retrieve the data.
      Returns the data associated with this line.
    */
    virtual MultiplyingLineData::Ptr data() const = 0;
    /*!
      Set the data of this line. The containing widgets should be
      populated accordingly.
      \a data the data to populate this line with
    */
    virtual void setData(const MultiplyingLineData::Ptr &data) = 0;

    /*!
      Whether this line is empty or not. Usually there is a primary widget
      that can be tested (such as a line edit).
      Returns true if this line is empty, false otherwise.
    */
    virtual bool isEmpty() const = 0;

    /*!
      Determine if this line can be deleted.
      \return true if this line can be deleted by the user, false otherwise
    */
    virtual bool canDeleteLineEdit() const = 0;

    /*!
      Set the width of the left most column to be the argument width.
      This method allows other widgets to align their label/combobox column with ours
      by communicating how many pixels that first column is for them.
      \a w the width to set the left most column to.
      Returns the width that is actually being used.
    */
    virtual int setColumnWidth(int w) = 0;

    /*!
      Used to set setup the correct chain of widgets to focus on
      when the user presses tab.
      \a previous the previous widget (probably from the preceding line)

      Example with a 3 widget line:

      void YourLine::fixTabOrder( QWidget *previous ) {
        setTabOrder( previous, mLeftMost );
        setTabOrder( mLeftMost, mMiddle);
        setTabOrder( mMiddle, mRightMost);
      }
      */
    virtual void fixTabOrder(QWidget *previous) = 0;

    /*!
      Returns The final widget in this line on which if the user presses
      tab focus should be given to the next line. This will commonly
      be used as the parameter of fixTabOrder( QWidget *previous ).
      \sa fixTabOrder( QWidget *previous )
      */
    virtual QWidget *tabOut() const = 0;

    /*!
      Clear the contents of this line. Reset to default state
     */
    virtual void clear() = 0;

    /*!
      Sets the type of completion to be used for KLineEdits in this line
      \a mode the completion mode
      */
    virtual void setCompletionMode(KCompletion::CompletionMode mode) = 0;

    /*!
     * \brief Called just before this line is deleted.
     *
     * Re implement this method if you need to do something before a line is deleted.
     * Default implementation does nothing.
     */
    virtual void aboutToBeDeleted();

Q_SIGNALS:
    /*!
      \brief Signal emitted when the return/enter key is pressed.
      \param line the MultiplyingLine that emitted the signal
    */
    void returnPressed(KPIM::MultiplyingLine *);
    /*!
      \brief Signal emitted when the down key is pressed.
      \param line the MultiplyingLine that emitted the signal
    */
    void downPressed(KPIM::MultiplyingLine *);
    /*!
      \brief Signal emitted when the up key is pressed.
      \param line the MultiplyingLine that emitted the signal
    */
    void upPressed(KPIM::MultiplyingLine *);
    /*!
      \brief Signal emitted when the right key is pressed.
    */
    void rightPressed();
    /*!
      \brief Signal emitted when the line should be deleted.
      \param line the MultiplyingLine to be deleted
    */
    void deleteLine(KPIM::MultiplyingLine *);
    /*!
      \brief Signal emitted when the completion mode changes.
      \param mode the new completion mode
    */
    void completionModeChanged(KCompletion::CompletionMode);
public Q_SLOTS:
    /*!
      \brief Slot called to propagate deletion of this line.
    */
    void slotPropagateDeletion();

protected Q_SLOTS:
    /*!
      \brief Slot called when the return/enter key is pressed.
    */
    void slotReturnPressed();
    /*!
      \brief Slot called when the focus should move up.
    */
    void slotFocusUp();
    /*!
      \brief Slot called when the focus should move down.
    */
    void slotFocusDown();

protected:
    /*!
      Handles key press events on this line.
      Default behavior handles Up and Down presses.
    */
    void keyPressEvent(QKeyEvent *) override;
};
}
