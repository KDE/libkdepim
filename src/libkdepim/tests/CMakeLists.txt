########### next target ###############
set(testkcheckcomboboxtest_gui_SRCS kcheckcomboboxtest_gui.cpp)

add_executable(kcheckcomboboxtest_gui ${testkcheckcomboboxtest_gui_SRCS})

target_link_libraries(kcheckcomboboxtest_gui KF5::Libkdepim KF5::I18n KF5::Completion KF5::CoreAddons)
