# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
########### next target ###############
add_executable(kcheckcomboboxtest_gui kcheckcomboboxtest_gui.cpp)

target_link_libraries(kcheckcomboboxtest_gui KF5::Libkdepim KF5::I18n KF5::Completion KF5::CoreAddons)
