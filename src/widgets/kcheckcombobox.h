/*
  This file is part of libkdepim.

  SPDX-FileCopyrightText: 2008 Thomas Thrainer <tom_t@gmx.at>
  SPDX-FileCopyrightText: 2010 Bertjan Broeksema <broeksema@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH LicenseRef-Qt-Commercial-exception-1.0

*/

#pragma once

#include "kdepim_export.h"

#include <QComboBox>

namespace KPIM
{
/*!
 * \class KPIM::KCheckComboBox
 * \inmodule Libkdepim
 * \inheaderfile KPIM/KCheckComboBox
 *
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
    /*!
     * \brief Constructor for KCheckComboBox.
     * \param parent the parent widget
     */
    explicit KCheckComboBox(QWidget *parent = nullptr);

    /*!
     * \brief Destructor for KCheckComboBox.
     */
    ~KCheckComboBox() override;

    /*!
     * \brief Hides the dropdown popup.
     */
    void hidePopup() override;

    /*!
     * \brief Returns the default text shown when no items are selected.
     * \return the default text
     */
    [[nodiscard]] QString defaultText() const;

    /*!
     * \brief Sets the default text to show when no items are selected.
     * \param text the new default text
     */
    void setDefaultText(const QString &text);

    /*!
     * \brief Returns whether the default text is always shown.
     * \return true if default text is always shown
     */
    [[nodiscard]] bool alwaysShowDefaultText() const;

    /*!
     * \brief Sets whether the default text should always be shown.
     * \param always true to always show default text, false to hide when items are checked
     */
    void setAlwaysShowDefaultText(bool always);

    /*!
     * \brief Returns whether the text is squeezed to fit.
     * \return true if text is squeezed
     */
    [[nodiscard]] bool squeezeText() const;

    /*!
     * \brief Sets whether the display text should be squeezed to fit.
     * \param squeeze true to squeeze text, false otherwise
     */
    void setSqueezeText(bool squeeze);

    /*!
     * \brief Returns whether the item at the given index is enabled.
     * \param index the item index
     * \return true if the item is enabled (can be checked/unchecked)
     */
    [[nodiscard]] bool itemEnabled(int index);

    /*!
     * \brief Sets whether the item at the given index is enabled.
     * \param index the item index
     * \param enabled true to enable the item, false to disable it
     */
    void setItemEnabled(int index, bool enabled = true);

    /*!
     * \brief Returns the check state of the item at the given index.
     * \param index the item index
     * \return the Qt::CheckState of the item
     */
    [[nodiscard]] Qt::CheckState itemCheckState(int index) const;

    /*!
     * \brief Sets the check state of the item at the given index.
     * \param index the item index
     * \param state the new Qt::CheckState
     */
    void setItemCheckState(int index, Qt::CheckState state);

    /*!
     * \brief Returns the separator string used between checked items.
     * \return the separator string
     */
    [[nodiscard]] QString separator() const;

    /*!
     * \brief Sets the separator string used between checked items.
     * \param separator the new separator string
     */
    void setSeparator(const QString &separator);

    /*!
     * \brief Returns the list of currently checked items.
     * \param role the data role to retrieve (default is Qt::DisplayRole)
     * \return list of checked item strings
     */
    [[nodiscard]] QStringList checkedItems(int role = Qt::DisplayRole) const;

public Q_SLOTS:
    /*!
     * \brief Sets which items should be checked.
     * \param items the list of items to check
     * \param role the data role (default is Qt::DisplayRole)
     */
    void setCheckedItems(const QStringList &items, int role = Qt::DisplayRole);

Q_SIGNALS:
    /*!
     * \brief Signal emitted when the checked items have changed.
     * \param items the new list of checked items
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
    class KCheckComboBoxPrivate;
    std::unique_ptr<KCheckComboBoxPrivate> const d;
};
}
