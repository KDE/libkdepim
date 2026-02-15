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
  \class KPIM::KWidgetLister
  \inmodule Libkdepim
  \inheaderfile KPIM/KWidgetLister

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
     * \brief Constructor for KWidgetLister.
     * \param fewerMoreButton if true, shows 'more' and 'fewer' buttons
     * \param minWidgets the minimum number of widgets to keep visible
     * \param maxWidgets the maximum number of widgets to show
     * \param parent the parent widget
     */
    explicit KWidgetLister(bool fewerMoreButton, int minWidgets = 1, int maxWidgets = 8, QWidget *parent = nullptr);

    /*!
     * \brief Destructor for KWidgetLister.
     */
    ~KWidgetLister() override;

protected Q_SLOTS:
    /*!
     * \brief Slot called when the 'more' button is clicked.
     * This adds a new widget to the lister. Subclasses should override this
     * to perform initialization on the new widget.
     */
    virtual void slotMore();

    /*!
     * \brief Slot called when the 'fewer' button is clicked.
     * This removes the last widget from the lister. Subclasses should override
     * this to save the state of the removed widget.
     */
    virtual void slotFewer();

    /*!
     * \brief Slot called when the 'clear' button is clicked.
     * This removes all widgets except the minimum number. Subclasses should override
     * to reset widgets to their default state.
     */
    virtual void slotClear();

protected:
    /*!
     * \brief Adds a single widget to the lister.
     * Override this method to initialize the new widget when it appears on screen.
     * Make sure to call the parent implementation.
     * \param widget the widget to add, or nullptr to create a default widget
     */
    virtual void addWidgetAtEnd(QWidget *widget = nullptr);

    /*!
     * \brief Removes the last widget from the lister.
     * Override this method to save the state of the removed widget before removal.
     * Make sure to call the parent implementation.
     */
    virtual void removeLastWidget();

    /*!
     * \brief Clears the contents of a widget.
     * Default implementation does nothing. Override to reset widget to default state.
     * \param w the widget to clear
     */
    virtual void clearWidget(QWidget *w);

    /*!
     * \brief Creates a new widget for the lister.
     * Override this method to create and return your custom widget.
     * \param parent the parent widget for the new widget
     * \return the newly created widget
     */
    virtual QWidget *createWidget(QWidget *parent);

    /*!
     * \brief Sets the exact number of widgets visible on screen.
     * \param count the desired number of widgets
     */
    virtual void setNumberOfShownWidgetsTo(int count);

    /*!
     * \brief Returns the list of all widgets in the lister.
     * \return list of QWidget pointers
     */
    QList<QWidget *> widgets() const;

    /*!
     * \brief Returns the minimum number of widgets that must stay visible.
     * \return the minimum widget count
     */
    int widgetsMinimum() const;

    /*!
     * \brief Returns the maximum number of widgets that can be shown.
     * \return the maximum widget count
     */
    int widgetsMaximum() const;

    /*!
     * \brief Removes a specific widget from the lister.
     * \param widget the widget to remove
     */
    virtual void removeWidget(QWidget *widget);
    /*!
     * \brief Adds a widget after a specific existing widget.
     * \param currentWidget the widget after which to insert
     * \param widget the widget to add, or nullptr to create a default widget
     */
    virtual void addWidgetAfterThisWidget(QWidget *currentWidget, QWidget *widget = nullptr);

private:
    KDEPIM_NO_EXPORT void init(bool fewerMoreButton = true);

Q_SIGNALS:
    /*!
     * \brief Signal emitted when a widget is added.
     */
    void widgetAdded();

    /*!
     * \brief Signal emitted when a widget is added.
     * \param widget the widget that was added
     */
    void widgetAdded(QWidget *widget);

    /*!
     * \brief Signal emitted when a widget is removed.
     */
    void widgetRemoved();

    /*!
     * \brief Signal emitted when a widget is removed.
     * \param widget the widget that was removed
     */
    void widgetRemoved(QWidget *widget);

    /*!
     * \brief Signal emitted when the clear button is clicked.
     */
    void clearWidgets();

private:
    class KWidgetListerPrivate;
    std::unique_ptr<KWidgetListerPrivate> const d;
};
}
