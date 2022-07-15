# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
macro(add_libkdepim_progresswidget_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h
        TEST_NAME ${_name}
        NAME_PREFIX "libkdepim-progresswidget-"
        LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test
        Qt${QT_MAJOR_VERSION}::Widgets
        KF5::Libkdepim
    )
endmacro()
add_libkdepim_progresswidget_unittest(statusbarprogresswidgettest.cpp)
add_libkdepim_progresswidget_unittest(progressstatusbarwidgettest.cpp)
