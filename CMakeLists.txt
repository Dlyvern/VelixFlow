cmake_minimum_required(VERSION 3.16)
project(VelixFlow)

if(WIN32)
    set(VELIX_PLATFORM "windows")
    message(STATUS "Building for Windows platform")
elseif(UNIX)
    set(VELIX_PLATFORM "linux")
    message(STATUS "Building for Linux platform")
elseif(APPLE)
    set(VELIX_PLATFORM "macos")
    message(STATUS "Building for macOS platform")
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

if(MSVC)
    add_compile_options(/Zc:preprocessor)
    add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Paths ===
set(INCLUDE_DIR include/VelixFlow)
set(SRC_DIR src)
set(LIBRARIES_DIR libraries)

set(GLM_ROOT ${CMAKE_CURRENT_LIST_DIR}/external/glm)
set(PHYSX_ROOT ${CMAKE_CURRENT_LIST_DIR}/external/physx)
set(GLFW_ROOT ${CMAKE_CURRENT_LIST_DIR}/external/glfw)
set(ASSIMP_ROOT ${CMAKE_CURRENT_LIST_DIR}/external/assimp)
set(FREETYPE_ROOT ${CMAKE_CURRENT_LIST_DIR}/external/freetype)
set(LIBZ_ROOT ${CMAKE_CURRENT_LIST_DIR}/external/libz)
set(FMOD_ROOT ${CMAKE_CURRENT_LIST_DIR}/external/fmod)

file(GLOB_RECURSE PROJECT_HEADERS ${INCLUDE_DIR}/*.h ${INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES ${SRC_DIR}/*.cpp)

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

    if(${name} STREQUAL "assimp")
        set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
    endif()

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

find_package(Threads REQUIRED)

if(WIN32)
    add_definitions(-DNOMINMAX)
endif()

# === Create Static Library ===
add_library(${PROJECT_NAME} STATIC ${PROJECT_HEADERS} ${PROJECT_SOURCES})
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

include(cmake/FindPhysX.cmake)

# === Public Includes ===
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARIES_DIR}>

        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include>

        $<BUILD_INTERFACE:${GLFW_ROOT}/include/>
        $<INSTALL_INTERFACE:include/VelixFlow/glfw>

        $<BUILD_INTERFACE:${FREETYPE_ROOT}/include/>
        $<INSTALL_INTERFACE:include/VelixFlow/freetype>

        $<BUILD_INTERFACE:${FMOD_ROOT}/include/>
        $<INSTALL_INTERFACE:include/VelixFlow/fmod>

        $<BUILD_INTERFACE:${ASSIMP_ROOT}/include/>
        $<INSTALL_INTERFACE:include/VelixFlow/assimp>

        $<BUILD_INTERFACE:${GLM_ROOT}/include/>
        $<INSTALL_INTERFACE:include/VelixFlow/glm>
)

# === Link Libraries ===
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
#            ${LIBZ_ROOT}/lib/windows/zlib.lib
            ${GLFW_ROOT}/lib/windows/glfw3.lib
            ${FMOD_ROOT}/lib/windows/fmod_vc.lib
            assimp
            ${FREETYPE_ROOT}/lib/windows/freetype.lib
            Threads::Threads
    )
elseif(APPLE)
    find_package(ZLIB REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(glfw3 REQUIRED)

    target_link_libraries(${PROJECT_NAME} PUBLIC
            # ${GLFW_ROOT}/lib/macos/libglfw3.a
            # ${FMOD_ROOT}/lib/linux/libfmod.dylib Change to .dylib
            glfw
            ${FREETYPE_LIBRARIES}
            assimp
            ZLIB::ZLIB
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
            Threads::Threads
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC
            ${GLFW_ROOT}/lib/linux/libglfw3.a
            ${FMOD_ROOT}/lib/linux/libfmod.so.14.8
            ${FREETYPE_ROOT}/lib/linux/libfreetype.a
            ${LIBZ_ROOT}/lib/linux/libz.a
            assimp
            Threads::Threads
            pthread
            dl
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

option(VELIX_USE_VULKAN "Use Vulkan rendering backend" ON)
option(VELIX_USE_OPENGL "Use OpenGL rendering backend" ON)

if(VELIX_USE_VULKAN)
    message("Using Vulkan")
    add_subdirectory(backends/VelixVK)
endif()
    if(VELIX_USE_OPENGL)
    message("Using OpenGL")
    add_subdirectory(backends/VelixGL)
else()
    message("No rendering backend selected")
endif()
    
install(TARGETS ${PROJECT_NAME}
        EXPORT VelixFlowTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

if(UNIX)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_RPATH "$ORIGIN/../lib"
        INSTALL_RPATH "$ORIGIN/lib"
    )
endif()


install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY ${GLM_ROOT}/include/ DESTINATION include/VelixFlow/glm)
install(DIRECTORY ${GLFW_ROOT}/include/ DESTINATION include/VelixFlow/glfw)
install(DIRECTORY ${ASSIMP_ROOT}/include/ DESTINATION include/VelixFlow/assimp)
install(DIRECTORY ${FREETYPE_ROOT}/include/ DESTINATION include/VelixFlow/freetype)
install(DIRECTORY ${FMOD_ROOT}/include/ DESTINATION include/VelixFlow/fmod)

if(UNIX)
    install(FILES ${FMOD_ROOT}/lib/linux/libfmod.so.14.8 DESTINATION lib)
    install(CODE "execute_process(COMMAND ln -sf libfmod.so.14.8 \${CMAKE_INSTALL_PREFIX}/lib/libfmod.so.14)")
endif()

#TODO fix it
# if(APPLE)
#     install(FILES ${FMOD_ROOT}/lib/macos/libfmod.dylib DESTINATION lib)
# endif()

if(WIN32)
    install(FILES ${FMOD_ROOT}/lib/windows/fmod.dll DESTINATION lib)
endif()

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