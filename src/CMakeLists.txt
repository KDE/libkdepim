# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_library(KPim${KF_MAJOR_VERSION}Libkdepim)
add_library(KPim${KF_MAJOR_VERSION}::Libkdepim ALIAS KPim${KF_MAJOR_VERSION}Libkdepim)
add_library(KF5::Libkdepim ALIAS KPim${KF_MAJOR_VERSION}Libkdepim)
ecm_setup_version(PROJECT VARIABLE_PREFIX LIBKDEPIM
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/libkdepim_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}LibkdepimConfigVersion.cmake"
    SOVERSION 5
    )

if (BUILD_TESTING)
    add_subdirectory(tests)
    add_subdirectory(progresswidget/autotests/)
    add_subdirectory(progresswidget/tests/)
endif()


########### next target ###############
target_sources(KPim${KF_MAJOR_VERSION}Libkdepim PRIVATE
    progresswidget/progressmanager.cpp
    progresswidget/progressdialog.cpp
    progresswidget/statusbarprogresswidget.cpp
    progresswidget/ssllabel.cpp
    progresswidget/progressstatusbarwidget.cpp
    widgets/kcheckcombobox.cpp
    widgets/kwidgetlister.cpp
    widgets/progressindicatorlabel.cpp
    misc/lineeditcatchreturnkey.cpp
    multiplyingline/multiplyingline.cpp
    multiplyingline/multiplyinglineeditor.cpp
    multiplyingline/multiplyinglineview_p.cpp
    misc/lineeditcatchreturnkey.h
    progresswidget/statusbarprogresswidget.h
    progresswidget/ssllabel.h
    progresswidget/progressmanager.h
    progresswidget/progressdialog.h
    progresswidget/progressstatusbarwidget.h
    multiplyingline/multiplyinglineeditor.h
    multiplyingline/multiplyingline.h
    multiplyingline/multiplyinglineview_p.h
    libkdepim_private_export.h
    widgets/kwidgetlister.h
    widgets/kcheckcombobox.h
    widgets/progressindicatorlabel.h
    )
ecm_qt_declare_logging_category(KPim${KF_MAJOR_VERSION}Libkdepim HEADER libkdepim_debug.h IDENTIFIER LIBKDEPIM_LOG CATEGORY_NAME org.kde.pim.libkdepim
        DESCRIPTION "libkdepim (libkdepim)"
        OLD_CATEGORY_NAMES log_libkdepim
        EXPORT LIBKDEPIM
    )


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim${KF_MAJOR_VERSION}Libkdepim PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KPim${KF_MAJOR_VERSION}Libkdepim BASE_NAME kdepim)
target_include_directories(KPim${KF_MAJOR_VERSION}Libkdepim INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/Libkdepim/>")
target_include_directories(KPim${KF_MAJOR_VERSION}Libkdepim PUBLIC "$<BUILD_INTERFACE:${libkdepim_SOURCE_DIR}/src;${libkdepim_BINARY_DIR}/src;>")

target_link_libraries(KPim${KF_MAJOR_VERSION}Libkdepim
    PRIVATE
    KF${KF_MAJOR_VERSION}::Completion
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::ConfigGui
    KF${KF_MAJOR_VERSION}::WidgetsAddons
    )


if(MINGW)
    target_link_libraries(KPim${KF_MAJOR_VERSION}Libkdepim oleaut32)
endif()


set_target_properties(KPim${KF_MAJOR_VERSION}Libkdepim PROPERTIES
    VERSION ${LIBKDEPIM_VERSION}
    SOVERSION ${LIBKDEPIM_SOVERSION}
    EXPORT_NAME Libkdepim
    )

install(TARGETS
    KPim${KF_MAJOR_VERSION}Libkdepim
    EXPORT KPim${KF_MAJOR_VERSION}LibkdepimTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS}
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
    LIB_NAME KF${KF_MAJOR_VERSION}Libkdepim
    DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/Libkdepim
    )

install(FILES
    ${libkdepim_CamelcaseCamelCase_HEADERS}
    ${libkdepim_Camelcasewidgets_HEADERS}
    ${libkdepim_Camelcaseprogresswidget_HEADERS}
    ${libkdepim_Camelcasemisc_HEADERS}
    ${libkdepim_Camelcasemultiline_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/Libkdepim/Libkdepim
    COMPONENT Devel
    )

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kdepim_export.h
    ${libkdepim_CamelCase_HEADERS}
    ${libkdepim_progresswidget_HEADERS}
    ${libkdepim_misc_HEADERS}
    ${libkdepim_widgets_HEADERS}
    ${libkdepim_multiline_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/Libkdepim/libkdepim
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

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim${KF_MAJOR_VERSION}Libkdepim")
set(LIBKDEPIM_KF5_COMPAT FALSE)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimLibkdepimConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}LibkdepimConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}LibkdepimConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}LibkdepimConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
    )

install(EXPORT KPim${KF_MAJOR_VERSION}LibkdepimTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KPim${KF_MAJOR_VERSION}LibkdepimTargets.cmake NAMESPACE KPim${KF_MAJOR_VERSION}::)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libkdepim_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/Libkdepim COMPONENT Devel
    )

if (BUILD_QCH)
    ecm_add_qch(
        KPim${KF_MAJOR_VERSION}Libkdepim_QCH
        NAME KPim${KF_MAJOR_VERSION}Libkdepim
        BASE_NAME KPim${KF_MAJOR_VERSION}Libkdepim
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${libkdepim_CamelCase_HEADERS}
            ${libkdepim_progresswidget_HEADERS}
            ${libkdepim_misc_HEADERS}
            ${libkdepim_widgets_HEADERS}
            ${libkdepim_multiline_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt${QT_MAJOR_VERSION}Core_QCH
            Qt${QT_MAJOR_VERSION}Gui_QCH
            Qt${QT_MAJOR_VERSION}Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KDEPIM_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
