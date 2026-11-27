
cmake_minimum_required(VERSION 3.21)

project(DebtNote
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_executable(DebtNote
    ${PROJECT_SOURCES}
    resources.qrc
)

target_include_directories(DebtNote PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DebtNote PRIVATE
    Qt6::Widgets
    Qt6::Sql
)

# Deployment
include(GNUInstallDirs)

install(TARGETS DebtNote
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET DebtNote
    OUTPUT_SCRIPT deploy_script
)

install(SCRIPT ${deploy_script})
