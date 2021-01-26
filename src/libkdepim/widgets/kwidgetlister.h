/*  -*- c++ -*-

  kwidgetlister.h

  This file is part of libkdepim
  SPDX-FileCopyrightText: 2001 Marc Mutz <mutz@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KDEPIM_KWIDGETLISTER_H
#define KDEPIM_KWIDGETLISTER_H

#include "kdepim_export.h"
#include <QWidget>

namespace KPIM
{
/**
  @short Widget that manages a list of other widgets (incl. 'more', 'fewer' and 'clear' buttons).

  Simple widget that nonetheless does a lot of the dirty work for
  the filter edit widgets (KMSearchPatternEdit and
  KMFilterActionEdit). It provides a growable and shrinkable area
  where widget may be displayed in rows. Widgets can be added by
  hitting the provided 'More' button, removed by the 'Fewer' button
  and cleared (e.g. reset, if an derived class implements that and
  removed for all but @ref mMinWidgets).

  To use this widget, derive from it with the template changed to
  the type of widgets this class should list. Then reimplement @ref
  addWidgetAtEnd, @ref removeLastWidget, calling the original
  implementation as necessary. Instantiate an object of the class and
  put it in your dialog.

  @author Marc Mutz <Marc@Mutz.com>
  @see KMSearchPatternEdit::WidgetLister KMFilterActionEdit::WidgetLister

*/

class KDEPIM_EXPORT KWidgetLister : public QWidget
{
    Q_OBJECT

public:
    /**
     * Creates a new widget lister.
     * @param fewerMoreButton Add or Not fewerMoreButton
     * @param minWidgets The minimum number of widgets to stay on the screen.
     * @param maxWidgets The maximum number of widgets to stay on the screen.
     * @param parent The parent widget.
     */
    explicit KWidgetLister(bool fewerMoreButton, int minWidgets = 1, int maxWidgets = 8, QWidget *parent = nullptr);

    /**
     * Destroys the widget lister.
     */
    ~KWidgetLister() override;

protected Q_SLOTS:
    /**
     * Called whenever the user clicks on the 'more' button.
     * Reimplementations should call this method, because this
     * implementation does all the dirty work with adding the widgets
     * to the layout (through @ref addWidgetAtEnd) and enabling/disabling
     * the control buttons.
     */
    virtual void slotMore();

    /**
     * Called whenever the user clicks on the 'fewer' button.
     * Reimplementations should call this method, because this
     * implementation does all the dirty work with removing the widgets
     * from the layout (through @ref removeLastWidget) and
     * enabling/disabling the control buttons.
     */
    virtual void slotFewer();

    /**
     * Called whenever the user clicks on the 'clear' button.
     * Reimplementations should call this method, because this
     * implementation does all the dirty work with removing all but
     * @ref mMinWidgets widgets from the layout and enabling/disabling
     * the control buttons.
     */
    virtual void slotClear();

protected:
    /**
     * Adds a single widget. Doesn't care if there are already @ref
     * mMaxWidgets on screen and whether it should enable/disable any
     * controls. It simply does what it is asked to do.  You want to
     * reimplement this method if you want to initialize the widget
     * when showing it on screen. Make sure you call this
     * implementaion, though, since you cannot put the widget on screen
     * from derived classes (@p mLayout is private).
     * Make sure the parent of the QWidget to add is this KWidgetLister.
     */
    virtual void addWidgetAtEnd(QWidget *widget = nullptr);

    /**
     * Removes a single (always the last) widget. Doesn't care if there
     * are still only @ref mMinWidgets left on screen and whether it
     * should enable/disable any controls. It simply does what it is
     * asked to do. You want to reimplement this method if you want to
     * save the widget's state before removing it from screen. Make
     * sure you call this implementaion, though, since you should not
     * remove the widget from screen from derived classes.
     */
    virtual void removeLastWidget();

    /**
     * Called to clear a given widget. The default implementation does
     * nothing.
     */
    virtual void clearWidget(QWidget *w);

    /**
     * Returns a new widget that shall be added to the lister.
     *
     * @param parent The parent widget of the new widget.
     */
    virtual QWidget *createWidget(QWidget *parent);

    /**
     * Sets the number of widgets on scrren to exactly @p count. Doesn't
     * check if @p count is inside the range @p [mMinWidgets,mMaxWidgets].
     */
    virtual void setNumberOfShownWidgetsTo(int count);

    /**
     * Returns the list of widgets.
     */
    QList<QWidget *> widgets() const;

    /**
     * The minimum number of widgets that are to stay on screen.
     */
    int widgetsMinimum() const;

    /**
     * The maximum number of widgets that are to be shown on screen.
     */
    int widgetsMaximum() const;

    /**
     * Remove specific widget
     */
    virtual void removeWidget(QWidget *widget);
    /**
     * Add widget after specific widget
     */
    virtual void addWidgetAfterThisWidget(QWidget *currentWidget, QWidget *widget = nullptr);

private:
    void init(bool fewerMoreButton = true);

Q_SIGNALS:
    /**
     * This signal is emitted whenever a widget was added.
     */
    void widgetAdded();

    /**
     * This signal is emitted whenever a widget was added.
     *
     * @param widget The added widget.
     */
    void widgetAdded(QWidget *widget);

    /**
     * This signal is emitted whenever a widget was removed.
     */
    void widgetRemoved();

    /**
     * This signal is emitted whenever a widget was removed.
     */
    void widgetRemoved(QWidget *widget);

    /**
     * This signal is emitted whenever the clear button is clicked.
     */
    void clearWidgets();

private:
    //@cond PRIVATE
    class Private;
    Private *const d;
    //@endcond
};
}

#endif /* _KWIDGETLISTER_H_ */
