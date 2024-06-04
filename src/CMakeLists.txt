# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_definitions(-DTRANSLATION_DOMAIN=\"libkdepim6\")
add_library(KPim6Libkdepim)
add_library(KPim6::Libkdepim ALIAS KPim6Libkdepim)
ecm_setup_version(PROJECT VARIABLE_PREFIX LIBKDEPIM
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/libkdepim_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim6LibkdepimConfigVersion.cmake"
    SOVERSION 6
    )

if (BUILD_TESTING)
    add_subdirectory(tests)
    add_subdirectory(progresswidget/autotests/)
    add_subdirectory(progresswidget/tests/)
endif()


########### next target ###############
target_sources(KPim6Libkdepim PRIVATE
    progresswidget/progressmanager.cpp
    progresswidget/progressdialog.cpp
    progresswidget/statusbarprogresswidget.cpp
    progresswidget/ssllabel.cpp
    progresswidget/progressstatusbarwidget.cpp
    widgets/kcheckcombobox.cpp
    widgets/kwidgetlister.cpp
    widgets/progressindicatorlabel.cpp
    multiplyingline/multiplyingline.cpp
    multiplyingline/multiplyinglineeditor.cpp
    multiplyingline/multiplyinglineview_p.cpp
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
ecm_qt_declare_logging_category(KPim6Libkdepim HEADER libkdepim_debug.h IDENTIFIER LIBKDEPIM_LOG CATEGORY_NAME org.kde.pim.libkdepim
        DESCRIPTION "libkdepim (libkdepim)"
        OLD_CATEGORY_NAMES log_libkdepim
        EXPORT LIBKDEPIM
    )


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6Libkdepim PROPERTIES UNITY_BUILD ON)
endif()
ecm_generate_export_header(KPim6Libkdepim
    BASE_NAME kdepim
    VERSION ${LIBKDEPIM_VERSION}
    DEPRECATED_BASE_VERSION 0
    USE_VERSION_HEADER \"libkdepim_version.h\"
)
target_include_directories(KPim6Libkdepim INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/Libkdepim/>")
target_include_directories(KPim6Libkdepim PUBLIC "$<BUILD_INTERFACE:${libkdepim_SOURCE_DIR}/src;${libkdepim_BINARY_DIR}/src;>")

target_link_libraries(KPim6Libkdepim
    PRIVATE
    KF6::Completion
    KF6::I18n
    KF6::WidgetsAddons
    )


if(MINGW)
    target_link_libraries(KPim6Libkdepim PRIVATE oleaut32)
endif()


set_target_properties(KPim6Libkdepim PROPERTIES
    VERSION ${LIBKDEPIM_VERSION}
    SOVERSION ${LIBKDEPIM_SOVERSION}
    EXPORT_NAME Libkdepim
    )

install(TARGETS
    KPim6Libkdepim
    EXPORT KPim6LibkdepimTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
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

install(FILES
    ${libkdepim_CamelcaseCamelCase_HEADERS}
    ${libkdepim_Camelcasewidgets_HEADERS}
    ${libkdepim_Camelcaseprogresswidget_HEADERS}
    ${libkdepim_Camelcasemultiline_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/Libkdepim/Libkdepim
    COMPONENT Devel
    )

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kdepim_export.h
    ${libkdepim_CamelCase_HEADERS}
    ${libkdepim_progresswidget_HEADERS}
    ${libkdepim_widgets_HEADERS}
    ${libkdepim_multiline_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/Libkdepim/libkdepim
    COMPONENT Devel
    )

if(BUILD_DESIGNERPLUGIN)
    add_subdirectory(designer)
endif()
########### install files ###############

if (Qt6::DBus)
    install(FILES interfaces/org.kde.addressbook.service.xml interfaces/org.kde.mailtransport.service.xml DESTINATION ${KDE_INSTALL_DBUSINTERFACEDIR})
endif()

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libkdepim_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/Libkdepim COMPONENT Devel
    )

if (BUILD_QCH)
    ecm_add_qch(
        KPim6Libkdepim_QCH
        NAME KPim6Libkdepim
        BASE_NAME KPim6Libkdepim
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${libkdepim_CamelCase_HEADERS}
            ${libkdepim_progresswidget_HEADERS}
            ${libkdepim_widgets_HEADERS}
            ${libkdepim_multiline_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt6Core_QCH
            Qt6Gui_QCH
            Qt6Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KDEPIM_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
