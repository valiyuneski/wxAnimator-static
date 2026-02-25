cmake_minimum_required(VERSION 3.16)

project(wxAnimator-static VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "" FORCE)
endif()

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_library(wxAnimator-static STATIC src/wxStaticTextAnimated.cpp src/wxButtonAnimated.cpp src/wxImageAnimated.cpp src/wxCheckBoxAnimated.cpp src/wxModesDlgAnimated.cpp)
target_include_directories(wxAnimator-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(wxAnimator-static PUBLIC ${wxWidgets_LIBRARIES})
set_target_properties(wxAnimator-static PROPERTIES OUTPUT_NAME wxAnimator-static)

add_library(wxAnimator_shared SHARED src/wxStaticTextAnimated.cpp src/wxButtonAnimated.cpp src/wxImageAnimated.cpp src/wxCheckBoxAnimated.cpp src/wxModesDlgAnimated.cpp)
target_include_directories(wxAnimator_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(wxAnimator_shared PUBLIC ${wxWidgets_LIBRARIES})
set_target_properties(wxAnimator_shared PROPERTIES
    OUTPUT_NAME wxAnimator-static
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    INSTALL_NAME_DIR "@rpath"
    BUILD_WITH_INSTALL_RPATH TRUE
)

install(TARGETS wxAnimator-static wxAnimator_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

# Example executable
add_executable(example example.cpp)
target_link_libraries(example PRIVATE wxAnimator-static ${wxWidgets_LIBRARIES})

# Copy images to build directory for example
add_custom_command(TARGET example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${CMAKE_SOURCE_DIR}/images ${CMAKE_BINARY_DIR}/images
)
