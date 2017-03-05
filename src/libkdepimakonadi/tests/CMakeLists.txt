########### next target ###############
set(testaddresseelineedit_SRCS testaddresseelineedit.cpp)

add_executable(testaddresseelineedit ${testaddresseelineedit_SRCS})

target_link_libraries(testaddresseelineedit KF5::LibkdepimAkonadi KF5::I18n KF5::Completion)


########### next target ###############
set(testcompletionconfiguredialog_SRCS completionconfiguredialog_gui.cpp)

add_executable(completionconfiguredialog_gui ${testcompletionconfiguredialog_SRCS})
target_link_libraries(completionconfiguredialog_gui KF5::LibkdepimAkonadi KF5::I18n KF5::Completion)
