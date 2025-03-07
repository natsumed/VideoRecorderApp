cmake_minimum_required(VERSION 3.10)

project(VideoReceiverApp)

set(CMAKE_CXX_STANDARD 11)

# Add -fPIC flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Find the Qt5 package
find_package(Qt5Widgets REQUIRED)

# Find the GStreamer package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-video-1.0)

# Include nlohmann JSON library
find_package(nlohmann_json 3.2.0 REQUIRED)

# Include directories
include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${GST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/VideoReceiver
    ${CMAKE_SOURCE_DIR}/Manager
    ${nlohmann_json_INCLUDE_DIRS}
)

# Link directories
link_directories(
    ${Qt5Widgets_LIBRARY_DIRS}
    ${GST_LIBRARY_DIRS}
)

# Add the VideoReceiver library
add_subdirectory(VideoReceiver)

# Add the Manager library
add_subdirectory(Manager)

# Add the source files
set(SOURCES
    main.cpp
    VideoWindow.cpp
)

# Add the header files
set(HEADERS
    VideoWindow.h
)

# Generate moc files
qt5_wrap_cpp(MOC_SOURCES ${HEADERS})

# Add the executable
add_executable(VideoReceiverApp ${SOURCES} ${MOC_SOURCES})

# Link the libraries
target_link_libraries(VideoReceiverApp
    Qt5::Widgets
    ${GST_LIBRARIES}
    VideoReceiver
    Manager
    nlohmann_json::nlohmann_json
)
