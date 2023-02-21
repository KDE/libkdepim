# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_executable(statusbarprogresswidgetgui statusbarprogresswidget_gui.cpp)
target_link_libraries(statusbarprogresswidgetgui
  Qt::Widgets
  Qt::Core
  KF${KF_MAJOR_VERSION}::I18n
  KPim${KF_MAJOR_VERSION}::Libkdepim
)
