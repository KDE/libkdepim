/*  -*- c++ -*-

  kwidgetlister.h

  This file is part of libkdepim
  SPDX-FileCopyrightText: 2001 Marc Mutz <mutz@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kdepim_export.h"
#include <QWidget>

namespace KPIM
{
/*!
  \brief Widget that manages a list of other widgets (incl. 'more', 'fewer' and 'clear' buttons).

  Simple widget that nonetheless does a lot of the dirty work for
  the filter edit widgets (KMSearchPatternEdit and
  KMFilterActionEdit). It provides a growable and shrinkable area
  where widget may be displayed in rows. Widgets can be added by
  hitting the provided 'More' button, removed by the 'Fewer' button
  and cleared (e.g. reset, if an derived class implements that and
  removed for all but \ mMinWidgets).

  To use this widget, derive from it with the template changed to
  the type of widgets this class should list. Then reimplement \
  addWidgetAtEnd, \ removeLastWidget, calling the original
  implementation as necessary. Instantiate an object of the class and
  put it in your dialog.

  \author Marc Mutz <Marc@Mutz.com>
  \sa KMSearchPatternEdit::WidgetLister KMFilterActionEdit::WidgetLister

*/

class KDEPIM_EXPORT KWidgetLister : public QWidget
{
    Q_OBJECT

public:
    /*!
     * Creates a new widget lister.
     * \a fewerMoreButton Add or Not fewerMoreButton
     * \a minWidgets The minimum number of widgets to stay on the screen.
     * \a maxWidgets The maximum number of widgets to stay on the screen.
     * \a parent The parent widget.
     */
    explicit KWidgetLister(bool fewerMoreButton, int minWidgets = 1, int maxWidgets = 8, QWidget *parent = nullptr);

    /*!
     * Destroys the widget lister.
     */
    ~KWidgetLister() override;

protected Q_SLOTS:
    /*!
     * Called whenever the user clicks on the 'more' button.
     * Reimplementations should call this method, because this
     * implementation does all the dirty work with adding the widgets
     * to the layout (through \ addWidgetAtEnd) and enabling/disabling
     * the control buttons.
     */
    virtual void slotMore();

    /*!
     * Called whenever the user clicks on the 'fewer' button.
     * Reimplementations should call this method, because this
     * implementation does all the dirty work with removing the widgets
     * from the layout (through \ removeLastWidget) and
     * enabling/disabling the control buttons.
     */
    virtual void slotFewer();

    /*!
     * Called whenever the user clicks on the 'clear' button.
     * Reimplementations should call this method, because this
     * implementation does all the dirty work with removing all but
     * \ mMinWidgets widgets from the layout and enabling/disabling
     * the control buttons.
     */
    virtual void slotClear();

protected:
    /*!
     * Adds a single widget. Doesn't care if there are already \
     * mMaxWidgets on screen and whether it should enable/disable any
     * controls. It simply does what it is asked to do.  You want to
     * reimplement this method if you want to initialize the widget
     * when showing it on screen. Make sure you call this
     * implementation, though, since you cannot put the widget on screen
     * from derived classes (\a mLayout is private).
     * Make sure the parent of the QWidget to add is this KWidgetLister.
     */
    virtual void addWidgetAtEnd(QWidget *widget = nullptr);

    /*!
     * Removes a single (always the last) widget. Doesn't care if there
     * are still only \ mMinWidgets left on screen and whether it
     * should enable/disable any controls. It simply does what it is
     * asked to do. You want to reimplement this method if you want to
     * save the widget's state before removing it from screen. Make
     * sure you call this implementation, though, since you should not
     * remove the widget from screen from derived classes.
     */
    virtual void removeLastWidget();

    /*!
     * Called to clear a given widget. The default implementation does
     * nothing.
     */
    virtual void clearWidget(QWidget *w);

    /*!
     * Returns a new widget that shall be added to the lister.
     *
     * \a parent The parent widget of the new widget.
     */
    virtual QWidget *createWidget(QWidget *parent);

    /*!
     * Sets the number of widgets on screen to exactly \a count. Doesn't
     * check if \a count is inside the range \a [mMinWidgets,mMaxWidgets].
     */
    virtual void setNumberOfShownWidgetsTo(int count);

    /*!
     * Returns the list of widgets.
     */
    QList<QWidget *> widgets() const;

    /*!
     * The minimum number of widgets that are to stay on screen.
     */
    int widgetsMinimum() const;

    /*!
     * The maximum number of widgets that are to be shown on screen.
     */
    int widgetsMaximum() const;

    /*!
     * Remove specific widget
     */
    virtual void removeWidget(QWidget *widget);
    /*!
     * Add widget after specific widget
     */
    virtual void addWidgetAfterThisWidget(QWidget *currentWidget, QWidget *widget = nullptr);

private:
    KDEPIM_NO_EXPORT void init(bool fewerMoreButton = true);

Q_SIGNALS:
    /*!
     * This signal is emitted whenever a widget was added.
     */
    void widgetAdded();

    /*!
     * This signal is emitted whenever a widget was added.
     *
     * \a widget The added widget.
     */
    void widgetAdded(QWidget *widget);

    /*!
     * This signal is emitted whenever a widget was removed.
     */
    void widgetRemoved();

    /*!
     * This signal is emitted whenever a widget was removed.
     */
    void widgetRemoved(QWidget *widget);

    /*!
     * This signal is emitted whenever the clear button is clicked.
     */
    void clearWidgets();

private:
    class KWidgetListerPrivate;
    std::unique_ptr<KWidgetListerPrivate> const d;
};
}
