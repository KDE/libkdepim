#! /bin/sh
# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause

$XGETTEXT `find . -name '*.h' -o -name '*.cpp' | grep -v '/tests/' | grep -v '/autotests/'` -o $podir/libkdepim.pot

