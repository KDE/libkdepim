# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
########### next target ###############
add_executable(kcheckcomboboxtest_gui kcheckcomboboxtest_gui.cpp)

target_link_libraries(
    kcheckcomboboxtest_gui
    KPim6::Libkdepim
    KF6::I18n
    KF6::Completion
)
