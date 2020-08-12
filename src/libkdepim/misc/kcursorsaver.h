/*
    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#ifndef kcursorsaver_h
#define kcursorsaver_h

#include <QCursor>
#include <QApplication>

namespace KPIM {
/**
 * @short sets a cursor and makes sure it's restored on destruction
 * Create a KCursorSaver object when you want to set the cursor.
 * As soon as it gets out of scope, it will restore the original
 * cursor.
 */
class KCursorSaver
{
public:
    /// constructor taking QCursor shapes
    explicit KCursorSaver(Qt::CursorShape shape)
    {
        QApplication::setOverrideCursor(QCursor(shape));
        inited = true;
    }

    /// copy constructor. The right side won't restore the cursor
    KCursorSaver(const KCursorSaver &rhs)
    {
        *this = rhs;
    }

    /// restore the cursor
    ~KCursorSaver()
    {
        if (inited) {
            QApplication::restoreOverrideCursor();
        }
    }

    /// call this to explicitly restore the cursor
    inline void restoreCursor()
    {
        QApplication::restoreOverrideCursor();
        inited = false;
    }

protected:
    void operator=(const KCursorSaver &rhs)
    {
        inited = rhs.inited;
        rhs.inited = false;
    }

private:
    mutable bool inited;
};

/**
 * convenience functions
 */
namespace KBusyPtr {
inline KCursorSaver idle()
{
    return KCursorSaver(Qt::ArrowCursor);
}

inline KCursorSaver busy()
{
    return KCursorSaver(Qt::WaitCursor);
}
}
}

#endif /*kbusyptr_h_*/
