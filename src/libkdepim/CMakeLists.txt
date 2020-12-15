
ecm_setup_version(PROJECT VARIABLE_PREFIX LIBKDEPIM
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/libkdepim_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5LibkdepimConfigVersion.cmake"
    SOVERSION 5
    )

if (BUILD_TESTING)
    add_subdirectory(tests)
    add_subdirectory(progresswidget/autotests/)
    add_subdirectory(progresswidget/tests/)
endif()


########### next target ###############
set(kdepim_progresswidget_LIB_SRCS
    progresswidget/progressmanager.cpp
    progresswidget/progressdialog.cpp
    progresswidget/statusbarprogresswidget.cpp
    progresswidget/ssllabel.cpp
    progresswidget/progressstatusbarwidget.cpp
    )

set(kdepim_widgets_LIB_SRCS
    widgets/kcheckcombobox.cpp
    widgets/kwidgetlister.cpp
    widgets/progressindicatorlabel.cpp
    )

set(kdepim_misc_LIB_SRCS
    misc/lineeditcatchreturnkey.cpp
    )

set(kdepim_multiplyingline_LIB_SRCS
    multiplyingline/multiplyingline.cpp
    multiplyingline/multiplyinglineeditor.cpp
    multiplyingline/multiplyinglineview_p.cpp
    )

set(kdepim_LIB_SRCS
    ${kdepim_progresswidget_LIB_SRCS}
    ${kdepim_widgets_LIB_SRCS}
    ${kdepim_misc_LIB_SRCS}
    ${kdepim_multiplyingline_LIB_SRCS}
    )
ecm_qt_declare_logging_category(kdepim_LIB_SRCS HEADER libkdepim_debug.h IDENTIFIER LIBKDEPIM_LOG CATEGORY_NAME org.kde.pim.libkdepim
        DESCRIPTION "libkdepim (libkdepim)"
        OLD_CATEGORY_NAMES log_libkdepim
        EXPORT LIBKDEPIM
    )



add_library(KF5Libkdepim ${kdepim_LIB_SRCS})
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5Libkdepim PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5Libkdepim BASE_NAME kdepim)
add_library(KF5::Libkdepim ALIAS KF5Libkdepim)
target_include_directories(KF5Libkdepim INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/Libkdepim/;${KDE_INSTALL_INCLUDEDIR_KF5}/libkdepim>")
target_include_directories(KF5Libkdepim PUBLIC "$<BUILD_INTERFACE:${libkdepim_SOURCE_DIR}/src;${libkdepim_BINARY_DIR}/src;>")

target_link_libraries(KF5Libkdepim
    PRIVATE
    KF5::Completion
    KF5::I18n
    KF5::ConfigGui
    KF5::WidgetsAddons
    )


if(MINGW)
    target_link_libraries(KF5Libkdepim oleaut32)
endif()


set_target_properties(KF5Libkdepim PROPERTIES
    VERSION ${LIBKDEPIM_VERSION_STRING}
    SOVERSION ${LIBKDEPIM_SOVERSION}
    EXPORT_NAME Libkdepim
    )

install(TARGETS
    KF5Libkdepim
    EXPORT KF5LibkdepimTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
    )

ecm_generate_headers(libkdepim_Camelcasewidgets_HEADERS
    HEADER_NAMES
    KCheckComboBox
    KWidgetLister
    ProgressIndicatorLabel
    REQUIRED_HEADERS libkdepim_widgets_HEADERS
    PREFIX Libkdepim
    RELATIVE widgets
    )

ecm_generate_headers(libkdepim_Camelcasemisc_HEADERS
    HEADER_NAMES
    LineEditCatchReturnKey
    REQUIRED_HEADERS libkdepim_misc_HEADERS
    PREFIX Libkdepim
    RELATIVE misc
    )

ecm_generate_headers(libkdepim_Camelcaseprogresswidget_HEADERS
    HEADER_NAMES
    ProgressStatusBarWidget
    StatusbarProgressWidget
    ProgressDialog
    ProgressManager
    REQUIRED_HEADERS libkdepim_progresswidget_HEADERS
    PREFIX Libkdepim
    RELATIVE progresswidget
    )

ecm_generate_headers(libkdepim_Camelcasemultiline_HEADERS
    HEADER_NAMES
    MultiplyingLine
    MultiplyingLineEditor
    REQUIRED_HEADERS libkdepim_multiline_HEADERS
    PREFIX Libkdepim
    RELATIVE multiplyingline
    )

ecm_generate_pri_file(BASE_NAME Libkdepim
    LIB_NAME KF5Libkdepim
    DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/Libkdepim
    )

install(FILES
    ${libkdepim_CamelcaseCamelCase_HEADERS}
    ${libkdepim_Camelcasewidgets_HEADERS}
    ${libkdepim_Camelcaseprogresswidget_HEADERS}
    ${libkdepim_Camelcasemisc_HEADERS}
    ${libkdepim_Camelcasemultiline_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/Libkdepim
    COMPONENT Devel
    )

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kdepim_export.h
    ${libkdepim_CamelCase_HEADERS}
    ${libkdepim_progresswidget_HEADERS}
    ${libkdepim_misc_HEADERS}
    ${libkdepim_widgets_HEADERS}
    ${libkdepim_multiline_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/libkdepim
    COMPONENT Devel
    )

install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

if(BUILD_DESIGNERPLUGIN)
    add_subdirectory(designer)
endif()
########### install files ###############

install(FILES interfaces/org.kde.addressbook.service.xml interfaces/org.kde.mailtransport.service.xml DESTINATION ${KDE_INSTALL_DBUSINTERFACEDIR})

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Libkdepim")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5LibkdepimConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5LibkdepimConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5LibkdepimConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5LibkdepimConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
    )

install(EXPORT KF5LibkdepimTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5LibkdepimTargets.cmake NAMESPACE KF5::)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libkdepim_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel
    )
