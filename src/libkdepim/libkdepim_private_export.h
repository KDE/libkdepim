/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2017-2020 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef LIBKDEPIMPRIVATE_EXPORT_H
#define LIBKDEPIMPRIVATE_EXPORT_H

#include "kdepim_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
# ifndef LIBKDEPIM_TESTS_EXPORT
#  define LIBKDEPIM_TESTS_EXPORT KDEPIM_EXPORT
# endif
#else /* not compiling tests */
# define LIBKDEPIM_TESTS_EXPORT
#endif

#endif
