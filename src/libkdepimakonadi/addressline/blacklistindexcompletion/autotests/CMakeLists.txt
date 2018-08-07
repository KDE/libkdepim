macro(add_libkdepim_blacklistindexcompletion_unittest _name)
    ecm_add_test(${ARGN}
        TEST_NAME ${_name}
        NAME_PREFIX "libkdepim-addessline-blaclistindexcompletion-"
        LINK_LIBRARIES Qt5::Test KF5::LibkdepimAkonadi KF5::WidgetsAddons KF5::Completion KF5::I18n KF5::ConfigCore KF5::ItemViews KF5::Codecs
    )
endmacro()

add_libkdepim_blacklistindexcompletion_unittest(blacklistindexemailcompletionwidgettest
    blacklistindexemailcompletionwidgettest.cpp
)
add_libkdepim_blacklistindexcompletion_unittest(blacklistindexemaillisttest
    blacklistindexemaillisttest.cpp
)
add_libkdepim_blacklistindexcompletion_unittest(blacklistindexemailsearchjobtest
    blacklistindexemailsearchjobtest.cpp
)
add_libkdepim_blacklistindexcompletion_unittest(blacklistindexemailutiltest
    blacklistindexemailutiltest.cpp
)
add_libkdepim_blacklistindexcompletion_unittest(blacklistindexemailwarningtest
    blacklistindexemailwarningtest.cpp
)
