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
