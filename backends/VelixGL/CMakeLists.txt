cmake_minimum_required(VERSION 3.16)
project(VelixGL)

set(INCLUDE_DIR include)
set(SRC_DIR src)

set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
set(GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include)

file(GLOB_RECURSE PROJECT_HEADERS ${INCLUDE_DIR}/*.h ${INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES ${SRC_DIR}/*.cpp)

find_package(OpenGL REQUIRED)

if(NOT TARGET OpenGL::OpenGL)
    add_library(OpenGL::OpenGL INTERFACE IMPORTED)

    if(WIN32)
        set_target_properties(OpenGL::OpenGL PROPERTIES
            INTERFACE_LINK_LIBRARIES "opengl32"
            INTERFACE_INCLUDE_DIRECTORIES "${OPENGL_INCLUDE_DIR}"
        )
    elseif(UNIX OR APPLE)
        set_target_properties(OpenGL::OpenGL PROPERTIES
            INTERFACE_LINK_LIBRARIES "${OPENGL_gl_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${OPENGL_INCLUDE_DIR}"
        )
    endif()
endif()

if(APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)

    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
endif()


add_library(glad STATIC ${GLAD_DIR}/src/glad.c)

target_include_directories(glad
        PUBLIC
        $<BUILD_INTERFACE:${GLAD_DIR}/include>
)

set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)


file(GLOB SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert"
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag")

set(EMBEDDED_SHADER_SOURCES "")

foreach(SHADER ${SHADERS})
    get_filename_component(FILENAME ${SHADER} NAME)              # e.g., "text.vert"
    get_filename_component(FILENAME_WE ${SHADER} NAME_WE)        # e.g., "text"

    string(REPLACE "." "_" VAR_NAME ${FILENAME})                 # e.g., "text_vert"

    set(OUTPUT_CPP "${CMAKE_CURRENT_BINARY_DIR}/embedded_shaders/${FILENAME}.cpp")

    add_custom_command(
            OUTPUT ${OUTPUT_CPP}
            COMMAND ${CMAKE_COMMAND}
            -DINPUT=${SHADER}
            -DOUTPUT=${OUTPUT_CPP}
            -DVAR_NAME=${VAR_NAME}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_shader.cmake
            DEPENDS ${SHADER}
            COMMENT "Embedding shader: ${FILENAME}"
    )

    list(APPEND EMBEDDED_SHADER_SOURCES ${OUTPUT_CPP})
endforeach()

set(HEADER_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/EmbeddedShaders.hpp")

file(WRITE ${HEADER_OUTPUT} "// Auto-generated header for embedded shaders\n")
file(APPEND ${HEADER_OUTPUT} "#pragma once\n\n")

foreach(SHADER_CPP ${EMBEDDED_SHADER_SOURCES})
    get_filename_component(FILENAME ${SHADER_CPP} NAME)

    string(REPLACE ".cpp" "" BASENAME ${FILENAME})

    string(REPLACE "." "_" VAR_NAME ${BASENAME})

    file(APPEND ${HEADER_OUTPUT} "extern const char* shader_${VAR_NAME};\n")
endforeach()

add_library(${PROJECT_NAME} SHARED ${PROJECT_HEADERS} ${PROJECT_SOURCES}  ${EMBEDDED_SHADER_SOURCES})
        
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> ${OPENGL_INCLUDE_DIRS})

set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${GLAD_INCLUDE_DIR}
    ${GLFW_ROOT}/include/
    ${GLM_ROOT}/include/
)

if(UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        OpenGL::GL
        glad
        VelixFlow
    )
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        OpenGL::GL
        glad
        VelixFlow
    )

elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC 
        ${OPENGL_LIBRARY}
        glad
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
        VelixFlow
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${INCLUDE_DIR}/
    DESTINATION include
)