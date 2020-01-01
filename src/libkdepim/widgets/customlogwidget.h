/*
   Copyright (C) 2012-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef CUSTOMLOGWIDGET_H
#define CUSTOMLOGWIDGET_H

#include "kdepim_export.h"
#include <QStyledItemDelegate>
#include <QListWidget>

class QTextDocument;

namespace KPIM {
class LogItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit LogItemDelegate(QObject *parent);
    ~LogItemDelegate() override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const override;

private:
    QTextDocument *document(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class KDEPIM_EXPORT CustomLogWidget : public QListWidget
{
public:
    explicit CustomLogWidget(QWidget *parent = nullptr);
    ~CustomLogWidget();

    void addInfoLogEntry(const QString &log);
    void addErrorLogEntry(const QString &log);
    void addTitleLogEntry(const QString &log);
    void addEndLineLogEntry();

    Q_REQUIRED_RESULT QString toHtml() const;
    Q_REQUIRED_RESULT QString toPlainText() const;
    Q_REQUIRED_RESULT bool isEmpty() const;

private:
    enum ItemType {
        ItemLogType = Qt::UserRole + 1
    };

    enum LogType {
        Title = 0,
        Error,
        Info,
        EndLine
    };
};
}

#endif /* CUSTOMLOGWIDGET_H */
