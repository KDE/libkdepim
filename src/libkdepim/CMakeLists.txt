
ecm_setup_version(PROJECT VARIABLE_PREFIX LIBKDEPIM
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/libkdepim_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5LibkdepimConfigVersion.cmake"
    SOVERSION 5
    )

add_subdirectory(pics)
if (BUILD_TESTING)
    add_subdirectory(tests)
    add_subdirectory(misc/autotests)
    add_subdirectory(addressline/autotests/)
    add_subdirectory(progresswidget/autotests/)
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
    widgets/customlogwidget.cpp
    widgets/kdatepickerpopup.cpp
    widgets/kcheckcombobox.cpp
    widgets/kweekdaycheckcombo.cpp
    widgets/kwidgetlister.cpp
    widgets/pimmessagebox.cpp
    widgets/overlaywidget.cpp
    widgets/progressindicatorlabel.cpp
    widgets/progressindicatorwidget.cpp
    )

set(kdepim_ldap_LIB_SRCS
    ldap/ldapclient.cpp
    ldap/ldapsearchdialog.cpp
    ldap/ldapclientsearch.cpp
    ldap/ldapclientsearchconfig.cpp
    ldap/addhostdialog.cpp
    )
ecm_qt_declare_logging_category(kdepim_ldap_LIB_SRCS HEADER ldapclient_debug.h IDENTIFIER LDAPCLIENT_LOG CATEGORY_NAME org.kde.pim.ldapclient)

set(kdepim_misc_LIB_SRCS
    misc/broadcaststatus.cpp
    misc/emailvalidator.cpp
    misc/maillistdrag.cpp
    misc/uistatesaver.cpp
    )

set(kdepim_multiplyingline_LIB_SRCS
    multiplyingline/multiplyingline.cpp
    multiplyingline/multiplyinglineeditor.cpp
    multiplyingline/multiplyinglineview_p.cpp
    )

set(kdepim_addressline_LIB_SRCS
    addressline/recentaddress/recentaddresswidget.cpp
    addressline/recentaddress/recentaddresses.cpp
    addressline/addresslineedit/addresseelineeditutil.cpp
    )


set(kdepim_LIB_SRCS
    ${kdepim_progresswidget_LIB_SRCS}
    ${kdepim_widgets_LIB_SRCS}
    ${kdepim_ldap_LIB_SRCS}
    ${kdepim_misc_LIB_SRCS}
    ${kdepim_multiplyingline_LIB_SRCS}
    ${kdepim_addressline_LIB_SRCS}
    prefs/kprefsdialog.cpp
    )
ecm_qt_declare_logging_category(kdepim_LIB_SRCS HEADER libkdepim_debug.h IDENTIFIER LIBKDEPIM_LOG CATEGORY_NAME org.kde.pim.libkdepim)


add_library(KF5Libkdepim ${kdepim_LIB_SRCS})
generate_export_header(KF5Libkdepim BASE_NAME kdepim)
add_library(KF5::Libkdepim ALIAS KF5Libkdepim)
target_include_directories(KF5Libkdepim INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/Libkdepim/;${KDE_INSTALL_INCLUDEDIR_KF5}/libkdepim>")
target_include_directories(KF5Libkdepim PUBLIC "$<BUILD_INTERFACE:${libkdepim_SOURCE_DIR}/src;${libkdepim_BINARY_DIR}/src;>")

target_link_libraries(KF5Libkdepim
    PRIVATE
    KF5::Ldap
    KF5::Contacts
    KF5::Codecs
    KF5::Completion
    KF5::I18n
    KF5::KCMUtils
    KF5::Wallet
    KF5::IconThemes
    KF5::KIOCore
    KF5::KIOWidgets
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
    EXPORT KF5LibkdepimTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS} ${LIBRARY_NAMELINK}
    )

ecm_generate_headers(libkdepim_Camelcaseldap_HEADERS
    HEADER_NAMES
    LdapClientSearchConfig
    LdapClientSearch
    AddHostDialog
    LdapSearchDialog
    LdapClient
    REQUIRED_HEADERS libkdepim_ldap_HEADERS
    PREFIX Libkdepim
    RELATIVE ldap
    )

ecm_generate_headers(libkdepim_Camelcasewidgets_HEADERS
    HEADER_NAMES
    CustomLogWidget
    KCheckComboBox
    KDatePickerPopup
    KWeekdayCheckCombo
    KWidgetLister
    OverlayWidget
    PIMMessageBox
    ProgressIndicatorLabel
    ProgressIndicatorWidget
    REQUIRED_HEADERS libkdepim_widgets_HEADERS
    PREFIX Libkdepim
    RELATIVE widgets
    )

ecm_generate_headers(libkdepim_Camelcasemisc_HEADERS
    HEADER_NAMES
    EmailValidator
    BroadcastStatus
    MaillistDrag
    UiStateSaver
    KCursorSaver
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

ecm_generate_headers(libkdepim_Camelcasepref_HEADERS
    HEADER_NAMES
    KPrefsDialog
    REQUIRED_HEADERS libkdepim_pref_HEADERS
    PREFIX Libkdepim
    RELATIVE prefs
    )

ecm_generate_headers(libkdepim_Camelcasemultiline_HEADERS
    HEADER_NAMES
    MultiplyingLine
    MultiplyingLineEditor
    REQUIRED_HEADERS libkdepim_multiline_HEADERS
    PREFIX Libkdepim
    RELATIVE multiplyingline
    )

ecm_generate_headers(libkdepim_Camelcaserecentaddress_HEADERS
    HEADER_NAMES
    RecentAddresses
    REQUIRED_HEADERS libkdepim_recentaddress_HEADERS
    PREFIX Libkdepim
    RELATIVE addressline/recentaddress/
    )

ecm_generate_headers(libkdepim_Camelcaseaddressline_HEADERS
    HEADER_NAMES
    AddresseeLineEditUtil
    REQUIRED_HEADERS libkdepim_addressline_HEADERS
    PREFIX Libkdepim
    RELATIVE addressline/addresslineedit/
    )




ecm_generate_pri_file(BASE_NAME Libkdepim
    LIB_NAME KF5Libkdepim
    DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/Libkdepim
    )

install(FILES
    ${libkdepim_CamelcaseCamelCase_HEADERS}
    ${libkdepim_Camelcaseldap_HEADERS}
    ${libkdepim_Camelcasewidgets_HEADERS}
    ${libkdepim_Camelcaseprogresswidget_HEADERS}
    ${libkdepim_Camelcasemisc_HEADERS}
    ${libkdepim_Camelcasepref_HEADERS}
    ${libkdepim_Camelcasemultiline_HEADERS}
    ${libkdepim_Camelcaserecentaddress_HEADERS}
    ${libkdepim_Camelcaseaddressline_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/Libkdepim
    COMPONENT Devel
    )

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kdepim_export.h
    ${libkdepim_CamelCase_HEADERS}
    ${libkdepim_ldap_HEADERS}
    ${libkdepim_progresswidget_HEADERS}
    ${libkdepim_misc_HEADERS}
    ${libkdepim_widgets_HEADERS}
    ${libkdepim_pref_HEADERS}
    ${libkdepim_multiline_HEADERS}
    ${libkdepim_recentaddress_HEADERS}
    ${libkdepim_addressline_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/libkdepim
    COMPONENT Devel
    )

install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

add_subdirectory(designer)
########### next target ###############


set( kcm_ldap_SRCS
    ldap/kcmldap.cpp
    )

add_library(kcm_ldap MODULE ${kcm_ldap_SRCS})


target_link_libraries(kcm_ldap KF5::Libkdepim KF5::IconThemes KF5::KCMUtils KF5::I18n KF5::Ldap Qt5::Core)

kcoreaddons_desktop_to_json(kcm_ldap ldap/kcmldap.desktop DEFAULT_SERVICE_TYPE)

install(TARGETS kcm_ldap DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(FILES ldap/kcmldap.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

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
