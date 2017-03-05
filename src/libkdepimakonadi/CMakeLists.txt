
add_definitions( -DQT_NO_CAST_FROM_ASCII )
add_definitions( -DQT_NO_CAST_TO_ASCII )



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
   addressline/addresslineedit/kmailcompletion.cpp
   addressline/addresslineedit/baloocompletionemail.cpp
   addressline/completionconfiguredialog/completionconfiguredialog.cpp
   addressline/recentaddress/recentaddressdialog.cpp
   addressline/recentaddress/recentaddresswidget.cpp
   addressline/recentaddress/recentaddresses.cpp
   addressline/addresslineedit/addresseelineeditutil.cpp
)

set(kdepim_addressline_blacklist_baloo_LIB_SRCS
   addressline/blacklistbaloocompletion/blacklistbalooemailcompletiondialog.cpp
   addressline/blacklistbaloocompletion/blacklistbalooemaillist.cpp
   addressline/blacklistbaloocompletion/blacklistbalooemailsearchjob.cpp
   addressline/blacklistbaloocompletion/blacklistbalooemailutil.cpp
   addressline/blacklistbaloocompletion/blacklistbalooemailcompletionwidget.cpp
   addressline/blacklistbaloocompletion/blacklistbalooemailwarning.cpp
)


############ next target ###############
#set(kdepim_progresswidget_LIB_SRCS
#   progresswidget/progressmanager.cpp
#   progresswidget/progressmanager_akonadiagent.cpp
#   progresswidget/agentprogressmonitor.cpp
#   progresswidget/progressdialog.cpp
#   progresswidget/statusbarprogresswidget.cpp
#   progresswidget/ssllabel.cpp
#   progresswidget/progressstatusbarwidget.cpp
#)

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
  KF5::KCMUtils
  KF5::Wallet
  KF5::ItemViews
  KF5::AkonadiSearchPIM
  KF5::IconThemes
  KF5::KIOCore
  KF5::KIOWidgets
  Qt5::DBus
  Qt5::Network
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
    EXPORT KF5LibkdepimTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS} ${LIBRARY_NAMELINK}
)

ecm_generate_headers(libkdepimakonadi_Camelcasewidgets_HEADERS
  HEADER_NAMES
  TagSelectionCombo
  TagWidgets
  REQUIRED_HEADERS libkdepimakonadi_widgets_HEADERS
  PREFIX LibkdepimAkonadi
  RELATIVE widgets
)

#ecm_generate_headers(libkdepim_Camelcaseprogresswidget_HEADERS
#  HEADER_NAMES
#  ProgressStatusBarWidget
#  StatusbarProgressWidget
#  ProgressDialog
#  ProgressManager
#  REQUIRED_HEADERS libkdepim_progresswidget_HEADERS
#  PREFIX Libkdepim
#  RELATIVE progresswidget
#)

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

#ecm_generate_headers(libkdepim_Camelcasemultiline_HEADERS
#  HEADER_NAMES
#  MultiplyingLine
#  MultiplyingLineEditor
#  REQUIRED_HEADERS libkdepim_multiline_HEADERS
#  PREFIX Libkdepim
#  RELATIVE  multiplyingline
#)


ecm_generate_headers(libkdepim_Camelcaseaddressline_HEADERS
  HEADER_NAMES
  AddresseeLineEdit
  REQUIRED_HEADERS libkdepim_addressline_HEADERS
  PREFIX Libkdepim
  RELATIVE  addressline/addresslineedit/
)

ecm_generate_headers(libkdepim_Camelcaserecentaddress_HEADERS
  HEADER_NAMES
  RecentAddresses
  REQUIRED_HEADERS libkdepim_recentaddress_HEADERS
  PREFIX Libkdepim
  RELATIVE addressline/recentaddress/
)

ecm_generate_headers(libkdepim_Camelcasecompletionorder_HEADERS
  HEADER_NAMES
  CompletionOrderEditor
  REQUIRED_HEADERS libkdepim_completionorder_HEADERS
  PREFIX Libkdepim
  RELATIVE addressline/completionorder
)

ecm_generate_headers(libkdepim_Camelcasecompletionconfiguredialog_HEADERS
  HEADER_NAMES
  CompletionConfigureDialog
  REQUIRED_HEADERS libkdepim_completionconfiguredialog_HEADERS
  PREFIX Libkdepim
  RELATIVE addressline/completionconfiguredialog/
)

ecm_generate_headers(libkdepim_Camelcaseblacklistbaloocompletion_HEADERS
  HEADER_NAMES
  BlackListBalooEmailCompletionDialog
  REQUIRED_HEADERS libkdepim_blacklistbaloocompletion_HEADERS
  PREFIX Libkdepim
  RELATIVE addressline/blacklistbaloocompletion/
)

ecm_generate_pri_file(BASE_NAME LibkdepimAkonadi
    LIB_NAME KF5LibkdepimAkonadi
    DEPS "AkonadiCore AkonadiContact" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/Libkdepim
)

install(FILES
    ${libkdepim_Camelcaseblacklistbaloocompletion_HEADERS}
    ${libkdepim_Camelcasecompletionconfiguredialog_HEADERS}
    ${libkdepim_Camelcasecompletionorder_HEADERS}
#    ${libkdepim_CamelcaseCamelCase_HEADERS}
    ${libkdepimakonadi_Camelcasewidgets_HEADERS}
#    ${libkdepim_Camelcaseprogresswidget_HEADERS}
#    ${libkdepim_Camelcasemisc_HEADERS}
    ${libkdepim_Camelcasejob_HEADERS}
#    ${libkdepim_Camelcasepref_HEADERS}
#    ${libkdepim_Camelcasemultiline_HEADERS}
    ${libkdepim_Camelcaseaddressline_HEADERS}
    ${libkdepim_Camelcaserecentaddress_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/LibkdepimAkonadi
    COMPONENT Devel
)

install(FILES
    ${libkdepim_blacklistbaloocompletion_HEADERS}
    ${libkdepim_completionconfiguredialog_HEADERS}
    ${libkdepim_completionorder_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kdepimakonadi_export.h
#    ${libkdepim_CamelCase_HEADERS}
#    ${libkdepim_progresswidget_HEADERS}
#    ${libkdepim_misc_HEADERS}
    ${libkdepimakonadi_widgets_HEADERS}
    ${libkdepim_job_HEADERS}
#    ${libkdepim_pref_HEADERS}
#    ${libkdepim_multiline_HEADERS}
    ${libkdepim_addressline_HEADERS}
    ${libkdepim_recentaddress_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/libkdepimakonadi
    COMPONENT Devel
)

install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

