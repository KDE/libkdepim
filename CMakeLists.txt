cmake_minimum_required(VERSION 3.5)
set(PIM_VERSION "5.12.40")

project(libkdepim VERSION ${PIM_VERSION})

set(KF5_MIN_VERSION "5.60.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(LIBRARY_NAMELINK)
include(GenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)
include(ECMAddTests)


set(LIBKDEPIM_LIB_VERSION ${PIM_VERSION})
set(AKONADICONTACT_LIB_VERSION "5.12.40")
set(QT_REQUIRED_VERSION "5.11.0")
set(KLDAP_LIB_VERSION "5.12.40")
set(KCONTACTS_LIB_VERSION "5.12.40")
set(AKONADISEARCH_LIB_VERSION "5.12.40")
set(AKONADI_VERSION "5.12.40")

find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets Test DBus Network)

find_package(KF5Codecs ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Completion ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Config ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5ConfigWidgets ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5CoreAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5IconThemes ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5ItemViews ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5JobWidgets ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5KCMUtils ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5KIO ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Wallet ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5WidgetsAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)

find_package(KF5Akonadi ${AKONADI_VERSION} CONFIG REQUIRED)
find_package(KF5AkonadiContact ${AKONADICONTACT_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Contacts ${KCONTACTS_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Ldap ${KLDAP_LIB_VERSION} CONFIG REQUIRED)

option(FORCE_DISABLE_AKONADI_SEARCH "Disable features and API that require akonadi-search, e.g. because xapian isn't available" OFF)
if (NOT FORCE_DISABLE_AKONADI_SEARCH)
find_package(KF5AkonadiSearch ${AKONADISEARCH_LIB_VERSION} CONFIG REQUIRED)
set_package_properties(KF5AkonadiSearch PROPERTIES DESCRIPTION "The Akonadi Search libraries" URL "https://www.kde.org" TYPE REQUIRED PURPOSE "Provides search capabilities in KMail and Akonadi")
endif()

set(DISABLE_AKONADI_SEARCH ${FORCE_DISABLE_AKONADI_SEARCH})
configure_file(config-akonadi-search.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/config-akonadi-search.h)

find_package(KF5DesignerPlugin ${KF5_MIN_VERSION} CONFIG)
set_package_properties(KF5DesignerPlugin PROPERTIES DESCRIPTION "KF5 designer plugin" TYPE OPTIONAL)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DQT_NO_FOREACH)
set(CMAKE_CXX_STANDARD 14)
########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Libkdepim")


configure_package_config_file(MailTransportDBusServiceConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/MailTransportDBusServiceConfig.cmake
  PATH_VARS KDE_INSTALL_DBUSINTERFACEDIR
  INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_PREFIX}/MailTransportDBusService")

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/MailTransportDBusServiceConfig.cmake
  DESTINATION "${CMAKECONFIG_INSTALL_PREFIX}/MailTransportDBusService")


add_subdirectory(src)

install(FILES libkdepim.categories libkdepim.renamecategories  DESTINATION  ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
