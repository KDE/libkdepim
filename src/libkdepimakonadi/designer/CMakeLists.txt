if (USE_NEW_DESIGNERPLUGIN_MACRO)
    include(ECMAddQtDesignerPlugin)
    ecm_qtdesignerplugin_widget(KPIM::AddresseeLineEdit
        INCLUDE_FILE "LibkdepimAkonadi/AddresseeLineEdit"
        TOOLTIP "Addressee Line Edit (KDE-PIM)"
        WHATSTHIS "A line edit which lets you select addressees from the addressbook"
        GROUP "Input (KDE-PIM)"
        )

    ecm_add_qtdesignerplugin(kdepimakonadiwidgets
        NAME KDEPimAkonadiWidgets
        OUTPUT_NAME kdepimakonadiwidgets
        WIDGETS
        KPIM::AddresseeLineEdit
        LINK_LIBRARIES
        KF5::LibkdepimAkonadi
        KF5::Completion
        INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
        COMPONENT Devel
        )
else()
    if (KF5DesignerPlugin_FOUND)
        kf5designerplugin_add_plugin(kdepimakonadiwidgets kdepimakonadi.widgets)

        target_link_libraries(kdepimakonadiwidgets
            KF5::LibkdepimAkonadi KF5::Completion
            )
        if(NOT WIN32)
            # Since there are no libraries provided by this module,
            # there is no point including the build tree in RPath,
            # and then having to edit it at install time.
            set_target_properties(kdepimwidgets PROPERTIES
                SKIP_BUILD_RPATH TRUE
                BUILD_WITH_INSTALL_RPATH TRUE
                )
        endif()

        install(TARGETS
            kdepimakonadiwidgets
            DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/designer
            )
    endif()

endif()
