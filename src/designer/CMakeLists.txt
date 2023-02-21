# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
include(ECMAddQtDesignerPlugin)
ecm_qtdesignerplugin_widget(KPIM::KCheckComboBox
    INCLUDE_FILE "Libkdepim/KCheckComboBox"
    TOOLTIP "Combobox with checkable items (KDE-PIM)"
    WHATSTHIS "A combobox that allows the user to check the items in the drop down menu"
    GROUP "Input (KDE-PIM)"
    )

ecm_add_qtdesignerplugin(kdepimwidgets
    NAME KDEPimWidgets
    OUTPUT_NAME kdepim${QT_MAJOR_VERSION}widgets
    WIDGETS
    KPIM::KCheckComboBox
    LINK_LIBRARIES
    KPim${KF_MAJOR_VERSION}::Libkdepim
    KF${KF_MAJOR_VERSION}::Completion
    INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
    COMPONENT Devel
    )
