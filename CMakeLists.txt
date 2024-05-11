# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "6.1.0")

project(libkdepim VERSION ${PIM_VERSION})

set(KF_MIN_VERSION "6.0.0")
set(QT_REQUIRED_VERSION "6.6.0")
find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMGenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)

include(FeatureSummary)
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(ECMFeatureSummary)
include(ECMAddTests)
include(ECMAddQch)


option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

set(LIBKDEPIM_LIB_VERSION ${PIM_VERSION})
find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets Test DBus Network)

find_package(KF6Completion ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6I18n ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6WidgetsAddons ${KF_MIN_VERSION} CONFIG REQUIRED)

option(BUILD_DESIGNERPLUGIN "Build plugin for Qt Designer" ON)
add_feature_info(DESIGNERPLUGIN ${BUILD_DESIGNERPLUGIN} "Build plugin for Qt Designer")

add_definitions(-DQT_NO_CONTEXTLESS_CONNECT)
ecm_set_disabled_deprecation_versions(QT 6.7.0 KF 6.1.0)

if(BUILD_TESTING)
    add_definitions(-DBUILD_TESTING)
endif()

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6Libkdepim")



configure_package_config_file(KPimMailTransportDBusServiceConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/KPim6MailTransportDBusServiceConfig.cmake
  PATH_VARS KDE_INSTALL_DBUSINTERFACEDIR
  INSTALL_DESTINATION "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6MailTransportDBusService")

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/KPim6MailTransportDBusServiceConfig.cmake
  DESTINATION "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6MailTransportDBusService")

option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()

add_subdirectory(src)

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6Libkdepim")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KPim6Libkdepim_QCH
        FILE KPim6LibkdepimQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim6LibkdepimQchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimLibkdepimConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6LibkdepimConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6LibkdepimConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/src/KPim6LibkdepimConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
    )

install(EXPORT KPim6LibkdepimTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KPim6LibkdepimTargets.cmake NAMESPACE KPim6::)


ecm_qt_install_logging_categories(
        EXPORT LIBKDEPIM
        FILE libkdepim.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
        )

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)



ecm_feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
