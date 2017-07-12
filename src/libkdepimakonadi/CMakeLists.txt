ecm_setup_version(PROJECT VARIABLE_PREFIX LIBKDEPIM
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/libkdepimakonadi_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5LibkdepimAkonadiConfigVersion.cmake"
    SOVERSION 5
    )

if(BUILD_TESTING)
    add_subdirectory(tests)
    add_subdirectory(addressline/blacklistbaloocompletion/autotests)
    add_subdirectory(addressline/autotests/)
endif()


add_subdirectory(designer)

set(kdepim_addressline_LIB_SRCS
    addressline/completionorder/completionordereditor.cpp
    addressline/completionorder/completionorderwidget.cpp
    addressline/addresslineedit/addresseelineedit.cpp
    addressline/addresslineedit/addresseelineedit_p.cpp
    addressline/addresslineedit/addresseelineeditstatic.cpp
    addressline/addresslineedit/addresseelineeditldap.cpp
    addressline/addresslineedit/addresseelineeditakonadi.cpp
    addressline/addresslineedit/addresseelineeditbaloo.cpp
    addressline/addresslineedit/kmailcompletion.cpp
    addressline/addresslineedit/baloocompletionemail.cpp
    addressline/completionconfiguredialog/completionconfiguredialog.cpp
    )

set(kdepim_addressline_blacklist_baloo_LIB_SRCS
    addressline/blacklistbaloocompletion/blacklistbalooemaillist.cpp
    addressline/blacklistbaloocompletion/blacklistbalooemailsearchjob.cpp
    addressline/blacklistbaloocompletion/blacklistbalooemailutil.cpp
    addressline/blacklistbaloocompletion/blacklistbalooemailcompletionwidget.cpp
    addressline/blacklistbaloocompletion/blacklistbalooemailwarning.cpp
    )

set(kdepim_addressline_plugin_LIB_SRCS
    addressline/plugins/addressesslineeditabstractplugin.cpp
    addressline/plugins/addressesslineeditpluginmanager.cpp
    )

############ next target ###############
set(kdepimakonadi_progresswidget_LIB_SRCS
    progresswidget/progressmanagerakonadi.cpp
    progresswidget/agentprogressmonitor.cpp
    )

set(kdepim_job_LIB_SRCS
    job/addemailaddressjob.cpp
    job/addcontactjob.cpp
    job/openemailaddressjob.cpp
    job/addemaildisplayjob.cpp
    job/collectionsearchjob.cpp
    job/personsearchjob.cpp
    )

set(kdepimakonadi_widgets_LIB_SRCS
    widgets/tagwidgets.cpp
    widgets/tagselectioncombo.cpp
    )



set(kdepimakonadi_LIB_SRCS
    ${kdepim_job_LIB_SRCS}
    ${kdepimakonadi_widgets_LIB_SRCS}
    ${kdepim_addressline_LIB_SRCS}
    ${kdepim_addressline_blacklist_baloo_LIB_SRCS}
    ${kdepimakonadi_progresswidget_LIB_SRCS}
    ${kdepim_addressline_plugin_LIB_SRCS}
    )


ecm_qt_declare_logging_category(kdepimakonadi_LIB_SRCS HEADER libkdepimakonadi_debug.h IDENTIFIER LIBKDEPIMAKONADI_LOG CATEGORY_NAME org.kde.pim.libkdepimakonadi)


add_library(KF5LibkdepimAkonadi ${kdepimakonadi_LIB_SRCS})
generate_export_header(KF5LibkdepimAkonadi BASE_NAME kdepimakonadi)
add_library(KF5::LibkdepimAkonadi ALIAS KF5LibkdepimAkonadi)
target_include_directories(KF5LibkdepimAkonadi INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/Libkdepim/;${KDE_INSTALL_INCLUDEDIR_KF5}/libkdepim>")
target_include_directories(KF5LibkdepimAkonadi PUBLIC "$<BUILD_INTERFACE:${libkdepim_SOURCE_DIR}/src;${libkdepim_BINARY_DIR}/src;>")



target_link_libraries(KF5LibkdepimAkonadi
    PUBLIC
    KF5::AkonadiCore
    KF5::AkonadiContact
    KF5::Libkdepim
    PRIVATE
    KF5::AkonadiWidgets
    KF5::Ldap
    KF5::Contacts
    KF5::Codecs
    KF5::Completion
    KF5::I18n
    KF5::ItemViews
    KF5::AkonadiSearchPIM
    KF5::KIOCore
    KF5::KIOWidgets
    Qt5::DBus
    Qt5::Network
    KF5::ConfigWidgets
    )

if(MINGW)
    target_link_libraries(KF5LibkdepimAkonadi oleaut32)
endif()

set_target_properties(KF5LibkdepimAkonadi PROPERTIES
    VERSION ${LIBKDEPIM_VERSION_STRING}
    SOVERSION ${LIBKDEPIM_SOVERSION}
    EXPORT_NAME LibkdepimAkonadi
    )

install(TARGETS
    KF5LibkdepimAkonadi
    EXPORT KF5LibkdepimAkonadiTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS} ${LIBRARY_NAMELINK}
    )

ecm_generate_headers(libkdepimakonadi_Camelcasewidgets_HEADERS
    HEADER_NAMES
    TagSelectionCombo
    TagWidgets
    REQUIRED_HEADERS libkdepimakonadi_widgets_HEADERS
    PREFIX LibkdepimAkonadi
    RELATIVE widgets
    )

ecm_generate_headers(libkdepimakonadi_Camelcaseprogresswidget_HEADERS
    HEADER_NAMES
    ProgressManagerAkonadi
    REQUIRED_HEADERS libkdepimakonadi_progresswidget_HEADERS
    PREFIX LibkdepimAkonadi
    RELATIVE progresswidget
    )

ecm_generate_headers(libkdepim_Camelcasejob_HEADERS
    HEADER_NAMES
    AddContactJob
    AddEmailAddressJob
    AddEmailDisplayJob
    CollectionSearchJob
    OpenEmailAddressJob
    PersonSearchJob
    Person

    REQUIRED_HEADERS libkdepim_job_HEADERS
    PREFIX LibkdepimAkonadi
    RELATIVE job
    )

ecm_generate_headers(libkdepim_Camelcaseaddressline_HEADERS
    HEADER_NAMES
    AddresseeLineEdit
    REQUIRED_HEADERS libkdepim_addressline_HEADERS
    PREFIX LibkdepimAkonadi
    RELATIVE addressline/addresslineedit/
    )

ecm_generate_headers(libkdepim_Camelcasecompletionorder_HEADERS
    HEADER_NAMES
    CompletionOrderEditor
    REQUIRED_HEADERS libkdepim_completionorder_HEADERS
    PREFIX LibkdepimAkonadi
    RELATIVE addressline/completionorder
    )

ecm_generate_headers(libkdepim_Camelcasecompletionconfiguredialog_HEADERS
    HEADER_NAMES
    CompletionConfigureDialog
    REQUIRED_HEADERS libkdepim_completionconfiguredialog_HEADERS
    PREFIX LibkdepimAkonadi
    RELATIVE addressline/completionconfiguredialog/
    )

ecm_generate_pri_file(BASE_NAME LibkdepimAkonadi
    LIB_NAME KF5LibkdepimAkonadi
    DEPS "AkonadiCore AkonadiContact Libkdepim" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/LibkdepimAkonadi
    )

install(FILES
    ${libkdepim_Camelcasecompletionconfiguredialog_HEADERS}
    ${libkdepim_Camelcasecompletionorder_HEADERS}
    ${libkdepimakonadi_Camelcaseprogresswidget_HEADERS}
    ${libkdepimakonadi_Camelcasewidgets_HEADERS}
    ${libkdepim_Camelcasejob_HEADERS}
    ${libkdepim_Camelcaseaddressline_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/LibkdepimAkonadi
    COMPONENT Devel
    )

install(FILES
    ${libkdepim_completionconfiguredialog_HEADERS}
    ${libkdepim_completionorder_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kdepimakonadi_export.h
    ${libkdepimakonadi_progresswidget_HEADERS}
    ${libkdepimakonadi_widgets_HEADERS}
    ${libkdepim_job_HEADERS}
    ${libkdepim_addressline_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/libkdepimakonadi
    COMPONENT Devel
    )

install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})


set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5LibkdepimAkonadi")

install(EXPORT KF5LibkdepimAkonadiTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5LibkdepimAkonadiTargets.cmake NAMESPACE KF5::)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5LibkdepimAkonadiConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5LibkdepimAkonadiConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5LibkdepimAkonadiConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5LibkdepimAkonadiConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
    )


install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libkdepimakonadi_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel
    )
