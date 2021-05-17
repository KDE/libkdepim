macro(add_libkdepim_progresswidget_unittest _name)
    ecm_add_test(${ARGN}
        TEST_NAME ${_name}
        NAME_PREFIX "libkdepim-progresswidget-"
        LINK_LIBRARIES Qt::Test
        Qt::Widgets
        KF5::Libkdepim
    )
endmacro()
add_libkdepim_progresswidget_unittest(statusbarprogresswidgettest statusbarprogresswidgettest.cpp)
add_libkdepim_progresswidget_unittest(progressstatusbarwidgettest progressstatusbarwidgettest.cpp)
