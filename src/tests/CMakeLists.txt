# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
########### next target ###############
add_executable(kcheckcomboboxtest_gui kcheckcomboboxtest_gui.cpp)

target_link_libraries(kcheckcomboboxtest_gui KPim${KF_MAJOR_VERSION}::Libkdepim KF${KF_MAJOR_VERSION}::I18n KF${KF_MAJOR_VERSION}::Completion)
