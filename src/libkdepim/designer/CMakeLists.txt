if (USE_NEW_DESIGNERPLUGIN_MACRO)
    include(ECMAddQtDesignerPlugin)
    ecm_qtdesignerplugin_widget(KPIM::KCheckComboBox
        INCLUDE_FILE "Libkdepim/KCheckComboBox"
        TOOLTIP "Combobox with checkable items (KDE-PIM)"
        WHATSTHIS "A combobox that allows the user to check the items in the drop down menu"
        GROUP "Input (KDE-PIM)"
        )

    ecm_add_qtdesignerplugin(kdepimwidgets
        NAME KDEPimWidgets
        OUTPUT_NAME kdepimwidgets
        WIDGETS
        KPIM::KCheckComboBox
        LINK_LIBRARIES
        KF5::Libkdepim
        KF5::Completion
        INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
        COMPONENT Devel
        )
else()

    if (KF5DesignerPlugin_FOUND)
        kf5designerplugin_add_plugin(kdepimwidgets kdepim.widgets)

        target_link_libraries(kdepimwidgets
            KF5::Libkdepim KF5::Completion
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
            kdepimwidgets
            DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/designer
            )
    endif()

endif()
