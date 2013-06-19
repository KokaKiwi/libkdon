cmake_minimum_required(VERSION 2.8)

project(libkdon C)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

set(OFLAGS "-O4")

set(CMAKE_C_FLAGS "${OFLAGS} --std=c99")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(JANSSON jansson)
pkg_check_modules(MSGPACK msgpack)
pkg_check_modules(LIBXML2 libxml-2.0)
pkg_check_modules(LIBYAML yaml-0.1)

include_directories(include)
include_directories(${PROJECT_BINARY_DIR})
include_directories(${GLIB2_INCLUDE_DIRS})

file(GLOB libkdon_SOURCES src/*.c)

set(libkdon_LIBRARIES
    ${GLIB2_LIBRARIES}
)

if (JANSSON_FOUND)
    set(HAVE_JANSSON ON)

    include_directories(${JANSSON_INCLUDE_DIRS})
    list(APPEND libkdon_SOURCES src/encode/json.c)

    list(APPEND libkdon_LIBRARIES
        ${JANSSON_LIBRARIES}
    )
endif (JANSSON_FOUND)

if (MSGPACK_FOUND)
    set(HAVE_MSGPACK ON)

    list(APPEND libkdon_SOURCES src/encode/msgpack.c)
    include_directories(${MSGPACK_INCLUDE_DIRS})

    list(APPEND libkdon_LIBRARIES
        ${MSGPACK_LIBRARIES}
    )
endif (MSGPACK_FOUND)

if (LIBXML2_FOUND)
    set(HAVE_LIBXML2 ON)

    list(APPEND libkdon_SOURCES src/encode/xml.c)
    include_directories(${LIBXML2_INCLUDE_DIRS})

    list(APPEND libkdon_LIBRARIES
        ${LIBXML2_LIBRARIES}
    )
endif (LIBXML2_FOUND)

if (LIBYAML_FOUND)
    set(HAVE_LIBYAML ON)

    list(APPEND libkdon_SOURCES src/encode/yaml.c)
    include_directories(${LIBYAML_INCLUDE_DIRS})

    list(APPEND libkdon_LIBRARIES
        ${LIBYAML_LIBRARIES}
    )
endif (LIBYAML_FOUND)

add_library(kdon STATIC ${libkdon_SOURCES})
target_link_libraries(kdon ${libkdon_LIBRARIES})

add_executable(kdon_test main.c)
target_link_libraries(kdon_test kdon)

configure_file(config.cmake.h kdon_config.h @ONLY)

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR lib)
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)
set(prefix      ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(libdir      ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(VERSION     "0.1.0")
configure_file(kdon.pc.in kdon.pc @ONLY)

install(TARGETS kdon
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION bin
)

install(FILES
    include/kdon.h
    DESTINATION include
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kdon.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
