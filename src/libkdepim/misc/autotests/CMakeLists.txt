macro(add_libkdepim_misc_unittest _name)
    ecm_add_test(${ARGN}
        TEST_NAME ${_name}
        NAME_PREFIX "libkdepim-misc-"
        LINK_LIBRARIES Qt5::Test
        Qt5::Widgets
        KF5::Libkdepim
    )
endmacro()
add_libkdepim_misc_unittest(emailvalidatortest emailvalidatortest.cpp)
