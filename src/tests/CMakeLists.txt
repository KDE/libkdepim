# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
########### next target ###############
add_executable(kcheckcomboboxtest_gui kcheckcomboboxtest_gui.cpp)

target_link_libraries(kcheckcomboboxtest_gui KF5::Libkdepim KF${KF_MAJOR_VERSION}::I18n KF${KF_MAJOR_VERSION}::Completion KF${KF_MAJOR_VERSION}::CoreAddons)
