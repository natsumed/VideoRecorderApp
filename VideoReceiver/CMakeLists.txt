add_library(VideoReceiver STATIC
    VideoReceiver.cpp
    VideoReceiver.h
)

find_package(Qt5 REQUIRED COMPONENTS Core Gui)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0 gstreamer-video-1.0)

qt5_wrap_cpp(MOC_SOURCES VideoReceiver.h)

target_include_directories(VideoReceiver PRIVATE
    ${GSTREAMER_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
)

target_sources(VideoReceiver PRIVATE ${MOC_SOURCES})

target_link_libraries(VideoReceiver
    Qt5::Core
    Qt5::Gui
    ${GSTREAMER_LIBRARIES}
)
