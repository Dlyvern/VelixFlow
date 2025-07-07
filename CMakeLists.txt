cmake_minimum_required(VERSION 3.16)
project(VelixFlow)

if(WIN32)
    set(VELIX_PLATFORM "windows")
    set(VELIX_PLATFORM_WINDOWS ON)
    message(STATUS "Building for Windows platform")
elseif(UNIX)
    set(VELIX_PLATFORM "linux")
    set(VELIX_PLATFORM_LINUX ON)
    message(STATUS "Building for Linux platform")
elseif(APPLE)
    set(VELIX_PLATFORM "macos")
    set(VELIX_PLATFORM_MACOS ON)
    message(STATUS "Building for macOS platform")
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()


if(MSVC)
    add_compile_options(/Zc:preprocessor)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Paths ===
set(INCLUDE_DIR include/VelixFlow)
set(SRC_DIR src)
set(LIBRARIES_DIR libraries)
set(EXTERNAL_DIR external)
set(SHADERS_DST_DIR ${CMAKE_BINARY_DIR}/resources/shaders)

set(GLM_ROOT ${CMAKE_SOURCE_DIR}/external/glm)
set(PHYSX_ROOT ${CMAKE_SOURCE_DIR}/external/physx)
set(GLFW_ROOT ${CMAKE_SOURCE_DIR}/external/glfw)
set(ASSIMP_ROOT ${CMAKE_SOURCE_DIR}/external/assimp)
set(GLAD_ROOT ${CMAKE_SOURCE_DIR}/external/glad)
set(FREETYPE_ROOT ${CMAKE_SOURCE_DIR}/external/freetype)
set(LIBZ_ROOT ${CMAKE_SOURCE_DIR}/external/libz)
set(FMOD_ROOT ${CMAKE_SOURCE_DIR}/external/fmod)


option(VELIX_USE_PHYSX "Enable NVIDIA PhysX support" ON)

if(WIN32)
    message(WARNING "PhysX is not supported for windows build. Disabling PhysX")
    set(VELIX_USE_PHYSX OFF)
endif()

if(VELIX_PLATFORM_MACOS)
    add_compile_options(-std=c++23)
    add_definitions(-DGL_SILENCE_DEPRECATION)

    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
endif()


set(PHYSX_BUILD_TYPE "checked" CACHE STRING "The build type of PhysX")

set_property(CACHE PHYSX_BUILD_TYPE PROPERTY STRINGS debug checked profile release)

# === Build Mode ===
if(VELIX_USE_PHYSX)
    set(PHYSX_BUILD_TYPE "checked" CACHE STRING "The build type of PhysX")
    set_property(CACHE PHYSX_BUILD_TYPE PROPERTY STRINGS debug checked profile release)
endif()

# === Source Files ===
file(GLOB_RECURSE PROJECT_HEADERS ${INCLUDE_DIR}/*.h ${INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES ${SRC_DIR}/*.cpp)

# === Build Mode ===
if(NOT CMAKE_BUILD_TYPE)
    if(VELIX_USE_PHYSX)
        if(PHYSX_BUILD_TYPE STREQUAL "debug" OR PHYSX_BUILD_TYPE STREQUAL "checked")
            set(CMAKE_BUILD_TYPE "Debug")
        else()
            set(CMAKE_BUILD_TYPE "Release")
        endif()
    else()
        set(CMAKE_BUILD_TYPE "Release")
    endif()
endif()

# === Output Messages ===
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(VELIX_USE_PHYSX)
        message("Building in Debug with PhysX ${PHYSX_BUILD_TYPE}")
    else()
        message("Building in Debug without PhysX")
    endif()
    add_compile_definitions(_DEBUG)
else()
    if(VELIX_USE_PHYSX)
        message("Building in Release with PhysX ${PHYSX_BUILD_TYPE}")
    else()
        message("Building in Release without PhysX")
    endif()
    add_compile_definitions(NDEBUG)
endif()

include(FetchContent)

function(use_or_fetch name root_dir lib_path git_repo git_tag)
    if(EXISTS "${root_dir}/${lib_path}")
        message(STATUS "Using prebuilt ${name} from ${root_dir}/${lib_path}")
        add_library(${name} STATIC IMPORTED)
        set_target_properties(${name} PROPERTIES
                IMPORTED_LOCATION "${root_dir}/${lib_path}"
                INTERFACE_INCLUDE_DIRECTORIES "${root_dir}/include"
        )
    else()
        message(STATUS "${name} not found locally, fetching via FetchContent")
        FetchContent_Declare(
                ${name}
                GIT_REPOSITORY ${git_repo}
                GIT_TAG ${git_tag}
        )
        FetchContent_MakeAvailable(${name})
    endif()
endfunction()


if(WIN32)
    use_or_fetch(assimp "${ASSIMP_ROOT}" "lib/windows/assimp.lib" "https://github.com/assimp/assimp.git" "v5.3.1")
elseif(APPLE OR UNIX)
    use_or_fetch(assimp "${ASSIMP_ROOT}" "lib/linux/libassimp.a" "https://github.com/assimp/assimp.git" "v5.3.1")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()


# === External Dependencies ===
find_package(Threads REQUIRED)
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

if(VELIX_PLATFORM_WINDOWS)
    add_definitions(-DNOMINMAX)
endif()

# === Baking shaders ===

file(GLOB SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/*.vert"
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/*.frag")

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

# === End baking shaders ===

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include>
        $<INSTALL_INTERFACE:include/VelixFlow/glad>
)

set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)

# === Create Static Library ===
add_library(${PROJECT_NAME} STATIC ${PROJECT_HEADERS} ${PROJECT_SOURCES}  ${EMBEDDED_SHADER_SOURCES})
add_library(Velix::VelixFlow ALIAS VelixFlow)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-volatile)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (APPLE)
        target_compile_options(${PROJECT_NAME} PRIVATE -Wno-deprecated-volatile)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wno-volatile)
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # MSVC (Windows), do nothing
endif()


target_compile_definitions(${PROJECT_NAME} PUBLIC VELIX_USE_PHYSX=$<BOOL:${VELIX_USE_PHYSX}>)

# === Public Includes ===
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARIES_DIR}>

        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/VelixFlow>
        $<INSTALL_INTERFACE:include>

        $<BUILD_INTERFACE:${GLFW_ROOT}/include/>
        $<INSTALL_INTERFACE:include/VelixFlow/glfw>

        $<BUILD_INTERFACE:${GLAD_ROOT}/include/>
        $<INSTALL_INTERFACE:include/VelixFlow/glad>

        $<BUILD_INTERFACE:${FREETYPE_ROOT}/include/>
        $<INSTALL_INTERFACE:include/VelixFlow/freetype>

        $<BUILD_INTERFACE:${FMOD_ROOT}/include/>
        $<INSTALL_INTERFACE:include/VelixFlow/fmod>

        $<BUILD_INTERFACE:${ASSIMP_ROOT}/include/>
        $<INSTALL_INTERFACE:include/VelixFlow/assimp>

        ${OPENGL_INCLUDE_DIRS}
)

#Just to supress warnings on Macos and Windows
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    $<BUILD_INTERFACE:${GLM_ROOT}/include/>
    $<INSTALL_INTERFACE:include/VelixFlow/glm>
)

#Just to supress warnings on Macos and Windows
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
        $<BUILD_INTERFACE:${PHYSX_ROOT}/include/>
        $<INSTALL_INTERFACE:include/VelixFlow/PhysX>
)

target_link_libraries(${PROJECT_NAME} PRIVATE glad)

# === Link Libraries ===
if(VELIX_PLATFORM_WINDOWS)
    target_link_libraries(${PROJECT_NAME} PUBLIC
#            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXExtensions_static_64.lib
#            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysX_static_64.lib
#            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXPvdSDK_static_64.lib
#            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXCommon_static_64.lib
#            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXFoundation_static_64.lib
#            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXCharacterKinematic_static_64.lib
            OpenGL::OpenGL
#            ${LIBZ_ROOT}/lib/windows/zlib.lib
            ${GLFW_ROOT}/lib/windows/glfw3.lib
            assimp
            ${FREETYPE_ROOT}/lib/windows/freetype.lib
            Threads::Threads
#            opengl32.lib
    )
elseif(VELIX_PLATFORM_MACOS)
    target_link_libraries(${PROJECT_NAME} PUBLIC
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXExtensions_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysX_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXPvdSDK_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXCommon_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXFoundation_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXCharacterKinematic_static_64.a
            ${GLFW_ROOT}/lib/linux/libglfw3.a
            ${ASSIMP_ROOT}/lib/linux/libassimp.a
            assimp
            ${FMOD_ROOT}/lib/linux/libfmod.so.14.8
            ${FREETYPE_ROOT}/lib/linux/libfreetype.a
            ${LIBZ_ROOT}/lib/linux/libz.a
            ${COCOA_LIBRARY}
            ${OPENGL_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
            Threads::Threads
    )
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC
            -Wl,--start-group
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXExtensions_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysX_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXPvdSDK_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXCommon_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXFoundation_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXCharacterKinematic_static_64.a
            -Wl,--end-group
            OpenGL::OpenGL
            ${GLFW_ROOT}/lib/linux/libglfw3.a
            ${FMOD_ROOT}/lib/linux/libfmod.so.14.8
#            ${ASSIMP_ROOT}/lib/linux/libassimp.a
            assimp
            ${FREETYPE_ROOT}/lib/linux/libfreetype.a
            Threads::Threads
            ${LIBZ_ROOT}/lib/linux/libz.a
    )
endif()


set(HEADER_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/EmbeddedShaders.hpp")

file(WRITE ${HEADER_OUTPUT} "// Auto-generated header for embedded shaders\n")
file(APPEND ${HEADER_OUTPUT} "#pragma once\n\n")

foreach(SHADER_CPP ${EMBEDDED_SHADER_SOURCES})
    get_filename_component(FILENAME ${SHADER_CPP} NAME)

    string(REPLACE ".cpp" "" BASENAME ${FILENAME})

    string(REPLACE "." "_" VAR_NAME ${BASENAME})

    file(APPEND ${HEADER_OUTPUT} "extern const char* shader_${VAR_NAME};\n")
endforeach()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS glad
        EXPORT VelixFlowTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(TARGETS VelixFlow
        EXPORT VelixFlowTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

if(VELIX_PLATFORM_WINDOWS)
    install(FILES
        ${GLFW_ROOT}/lib/windows/glfw3.lib
#        ${LIBZ_ROOT}/lib/windows/zlib.lib
#        ${ASSIMP_ROOT}/lib/windows/assimp.lib
        ${FREETYPE_ROOT}/lib/windows/freetype.lib
#        ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXExtensions_static_64.lib
#        ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysX_static_64.lib
#        ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXPvdSDK_static_64.lib
#        ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXCommon_static_64.lib
#        ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXFoundation_static_64.lib
#        ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXCharacterKinematic_static_64.lib

        DESTINATION lib
    )

#    install(FILES
#            ${LIBZ_ROOT}/lib/windows/zlib1.dll
#            DESTINATION bin
#    )

endif()

if(VELIX_PLATFORM_LINUX)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_RPATH "${FMOD_ROOT}/lib/linux"
        INSTALL_RPATH "$ORIGIN"
    )
endif()

install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY ${PHYSX_ROOT}/include/ DESTINATION include/VelixFlow/PhysX)
install(DIRECTORY ${GLM_ROOT}/include/ DESTINATION include/VelixFlow/glm)
install(DIRECTORY ${GLFW_ROOT}/include/ DESTINATION include/VelixFlow/glfw)
install(DIRECTORY ${ASSIMP_ROOT}/include/ DESTINATION include/VelixFlow/assimp)
install(DIRECTORY ${GLAD_ROOT}/include/ DESTINATION include/VelixFlow/glad)
install(DIRECTORY ${FREETYPE_ROOT}/include/ DESTINATION include/VelixFlow/freetype)
install(DIRECTORY ${FMOD_ROOT}/include/ DESTINATION include/VelixFlow/fmod)




# === Install CMake Config Package ===
install(EXPORT VelixFlowTargets
        FILE VelixFlowTargets.cmake
        NAMESPACE Velix::
        DESTINATION lib/cmake/VelixFlow
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/VelixFlowConfigVersion.cmake"
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/VelixFlowConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/VelixFlowConfig.cmake"
        INSTALL_DESTINATION lib/cmake/VelixFlow
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/VelixFlowConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/VelixFlowConfigVersion.cmake"
        DESTINATION lib/cmake/VelixFlow
)
