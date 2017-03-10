add_definitions(-DTRANSLATION_DOMAIN=\"libkdepim\")
add_definitions( -DQT_NO_CAST_FROM_ASCII )
add_definitions( -DQT_NO_CAST_TO_ASCII )

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif(BUILD_TESTING)

add_subdirectory(libkdepim)
add_subdirectory(libkdepimakonadi)
