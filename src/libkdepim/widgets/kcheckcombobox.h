/*
  This file is part of libkdepim.

  SPDX-FileCopyrightText: 2008 Thomas Thrainer <tom_t@gmx.at>
  SPDX-FileCopyrightText: 2010 Bertjan Broeksema <broeksema@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0

*/

#pragma once

#include "kdepim_export.h"

#include <QComboBox>
#include <QModelIndex>

namespace KPIM
{
/**
 * A combobox that shows its items in such a way that they can be checked in the
 * drop menu. It provides methods to set the default text when no items are selected
 * and the separator that is used to show the items that are selected in the line
 * edit.
 */
class KDEPIM_EXPORT KCheckComboBox : public QComboBox
{
    Q_OBJECT

    Q_PROPERTY(QString separator READ separator WRITE setSeparator)
    Q_PROPERTY(QString defaultText READ defaultText WRITE setDefaultText)
    Q_PROPERTY(bool squeezeText READ squeezeText WRITE setSqueezeText)
    Q_PROPERTY(QStringList checkedItems READ checkedItems WRITE setCheckedItems)

public:
    /**
     * Creates a new checkable combobox.
     *
     * @param parent The parent widget.
     */
    explicit KCheckComboBox(QWidget *parent = nullptr);

    /**
     * Destroys the time zone combobox.
     */
    ~KCheckComboBox() override;

    /**
     * Hides the popup list if it is currently shown.
     */
    void hidePopup() override;

    /**
     * Returns the default text that is shown when no items are selected.
     */
    Q_REQUIRED_RESULT QString defaultText() const;

    /**
     * Sets the default text that is shown when no items are selected.
     *
     * @param text The new default text
     */
    void setDefaultText(const QString &text);

    /**
     * Returns whether the default text is always shown, even if there are
     * no checked items.
     */
    bool alwaysShowDefaultText() const;

    /**
     * Sets if the default text should always be shown even if there are
     * no checked items.
     *
     * Default is false.
     */
    void setAlwaysShowDefaultText(bool always);

    /**
     * Returns whether or not the text will be squeezed to fit in the combo's line
     * edit. This property is false by default.
     *
     * @see KSqueezedTextLabel
     */
    Q_REQUIRED_RESULT bool squeezeText() const;

    /**
     * Sets whether or not the text must be squeezed.
     *
     * @param squeeze The new squeeze status
     */
    void setSqueezeText(bool squeeze);

    /**
     * Return whether or not the item at @param index is enabled, i.e. if the
     * user can (un)check the item.
     */
    Q_REQUIRED_RESULT bool itemEnabled(int index);

    /**
     * Set the item at @param index to @param enabled, i.e. if the
     * user can (un)check the item.
     */
    void setItemEnabled(int index, bool enabled = true);

    /**
     * Returns the check state of item at given index.
     *
     * @param index The index for which to return the check state.
     */
    Q_REQUIRED_RESULT Qt::CheckState itemCheckState(int index) const;

    /**
     * Changes the check state of the given index to the given state.
     *
     * @param index The index of which the state needs to be changed
     * @param state The new state
     */
    void setItemCheckState(int index, Qt::CheckState state);

    /**
     * Returns the current separator used to separate the selected items in the
     * line edit of the combo box.
     */
    Q_REQUIRED_RESULT QString separator() const;

    /**
     * Sets the separator used to separate items in the line edit.
     *
     * @param separator The new separator
     */
    void setSeparator(const QString &separator);

    /**
     * Returns The currently selected items.
     * @param role The role the returned values belong to.
     */
    Q_REQUIRED_RESULT QStringList checkedItems(int role = Qt::DisplayRole) const;

    /**
     * Update the KCheckComboBox text with Item belonging to the role.
     * @param topLeft Currently unused.
     * @param bottomRight Currently unused.
     * @param role The role the updated values belong to.
     */
    void updateCheckedItems(const QModelIndex &topLeft = QModelIndex(), const QModelIndex &bottomRight = QModelIndex(), int role = Qt::DisplayRole);

public Q_SLOTS:
    /**
     * Sets the currently selected items. Items that are not found in the model
     * are silently ignored.
     *
     * @param items The items that will be set to checked.
     * @param role The role @p items belong to.
     */
    void setCheckedItems(const QStringList &items, int role = Qt::DisplayRole);

Q_SIGNALS:
    /**
     * Signal to notify listeners that the current selections has changed.
     *
     * @param items The new selection.
     */
    void checkedItemsChanged(const QStringList &items);

protected:
    bool eventFilter(QObject *receiver, QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) override;
#endif

private:
    //@cond PRIVATE
    class KCheckComboBoxPrivate;
    std::unique_ptr<KCheckComboBoxPrivate> const d;
    //@endcond
};
}
