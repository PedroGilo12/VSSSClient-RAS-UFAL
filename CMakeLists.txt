cmake_minimum_required(VERSION 3.10.0)
project(vsss_ras_ufal VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(behaviortree_cpp REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Network Gui)

add_subdirectory(proto)

set(SOURCES
    clients/actuator/actuatorclient.cpp
    clients/client.cpp
    clients/referee/refereeclient.cpp
    clients/replacer/replacerclient.cpp
    clients/vision/visionclient.cpp
    utils/timer/timer.cpp
)

add_executable(vsss_ras_ufal
    main.cpp
    ${SOURCES}
)

target_include_directories(vsss_ras_ufal
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(vsss_ras_ufal
    behaviortree_cpp
    Qt5::Core
    Qt5::Network
    Qt5::Gui
    pb_msgs_lib
)
