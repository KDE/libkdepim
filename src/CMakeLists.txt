add_definitions(-DTRANSLATION_DOMAIN=\"libkdepim\")

if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif()

add_subdirectory(libkdepim)
add_subdirectory(libkdepimakonadi)
