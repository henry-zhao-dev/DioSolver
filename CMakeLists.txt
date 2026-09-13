cmake_minimum_required(VERSION 3.21)

project(DioSolver
    VERSION 1.1.0
    DESCRIPTION "A linear Diophantine equation solver"
    LANGUAGES C CXX
)

include(GNUInstallDirs)

set(DIOSOLVER_FORMAT_SOURCES
    include/diosolver/diophantine.h
    include/diosolver/extended_euclidean.h
    include/diosolver/inequality.h
    include/diosolver/interval.h
    include/diosolver/string_utils.h
    src/cli/main.c
    src/core/diophantine.c
    src/core/extended_euclidean.c
    src/core/inequality.c
    src/core/interval.c
    src/core/string_utils.c
    src/gui/dialog.cpp
    src/gui/dialog.h
    src/gui/main.cpp
    src/gui/main_window.cpp
    src/gui/main_window.h
    tests/diophantine_tests.c
    tests/extended_euclidean_tests.c
    tests/inequality_tests.c
    tests/interval_tests.c
    tests/string_utils_tests.c
)

find_program(DIOSOLVER_CLANG_FORMAT clang-format)
if(DIOSOLVER_CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${DIOSOLVER_CLANG_FORMAT} -i --style=file ${DIOSOLVER_FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting DioSolver C and C++ sources"
        VERBATIM
    )

    add_custom_target(format-check
        COMMAND ${DIOSOLVER_CLANG_FORMAT} --dry-run --Werror --style=file ${DIOSOLVER_FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Checking DioSolver C and C++ formatting"
        VERBATIM
    )
else()
    message(STATUS "clang-format not found; format and format-check targets are unavailable")
endif()

option(DIOSOLVER_BUILD_CLI "Build the command-line application" ON)
option(DIOSOLVER_BUILD_GUI "Build the Qt Widgets application" ON)
option(DIOSOLVER_BUILD_TESTS "Build the core test executables" ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external/c-storage-kit)

set(DIOSOLVER_CORE_SOURCES
    src/core/string_utils.c
    src/core/diophantine.c
    src/core/extended_euclidean.c
    src/core/inequality.c
    src/core/interval.c
)

add_library(diosolver_core STATIC ${DIOSOLVER_CORE_SOURCES})
add_library(DioSolver::Core ALIAS diosolver_core)

target_include_directories(diosolver_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_options(diosolver_core PRIVATE
    $<$<COMPILE_LANG_AND_ID:C,GNU,Clang,AppleClang>:-Wall;-Wextra;-Wpedantic>
    $<$<COMPILE_LANG_AND_ID:C,MSVC>:/W4>
)

target_link_libraries(diosolver_core PUBLIC CStorageKit::Core)

if(NOT WIN32)
    target_link_libraries(diosolver_core PUBLIC m)
endif()

if(DIOSOLVER_BUILD_CLI)
    add_executable(diosolver_cli src/cli/main.c)
    set_target_properties(diosolver_cli PROPERTIES OUTPUT_NAME diosolver-cli)
    target_link_libraries(diosolver_cli PRIVATE DioSolver::Core)
endif()

if(DIOSOLVER_BUILD_TESTS)
    enable_testing()

    function(diosolver_add_core_test module)
        set(target diosolver_test_${module})

        add_executable(${target} tests/${module}_tests.c)
        set_target_properties(${target} PROPERTIES
            OUTPUT_NAME diosolver-test-${module}
        )
        target_link_libraries(${target} PRIVATE DioSolver::Core)
        add_test(NAME diosolver.${module} COMMAND ${target})
    endfunction()

    diosolver_add_core_test(string_utils)
    diosolver_add_core_test(extended_euclidean)
    diosolver_add_core_test(interval)
    diosolver_add_core_test(inequality)
    diosolver_add_core_test(diophantine)
endif()

if(DIOSOLVER_BUILD_GUI)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)

    add_executable(diosolver_gui
        src/gui/dialog.cpp
        src/gui/dialog.h
        src/gui/main.cpp
        src/gui/main_window.cpp
        src/gui/main_window.h
        src/gui/resources/resources.qrc
    )
    add_executable(DioSolver::Gui ALIAS diosolver_gui)

    set_target_properties(diosolver_gui PROPERTIES
        OUTPUT_NAME diosolver-gui
        AUTOMOC ON
        AUTORCC ON
    )

    target_compile_features(diosolver_gui PRIVATE cxx_std_20)
    target_link_libraries(diosolver_gui PRIVATE
        DioSolver::Core
        Qt6::Widgets
    )

    # Provide a short, explicit build target for the GUI while keeping the
    # executable target name useful to IDEs and run configurations.
    add_custom_target(gui DEPENDS diosolver_gui)
endif()

install(TARGETS diosolver_core
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(DIOSOLVER_BUILD_CLI)
    install(TARGETS diosolver_cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(DIOSOLVER_BUILD_GUI)
    install(TARGETS diosolver_gui RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
