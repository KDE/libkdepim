set(statusbarprogresswidgetgui_SRCS statusbarprogresswidget_gui.cpp)
add_executable(statusbarprogresswidgetgui ${statusbarprogresswidgetgui_SRCS})
target_link_libraries(statusbarprogresswidgetgui
  Qt5::Widgets
  Qt5::Core
  KF5::I18n
  KF5::Libkdepim
)
