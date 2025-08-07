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
    set(CMAKE_OSX_ARCHITECTURES arm64)
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

set(GLM_ROOT ${CMAKE_SOURCE_DIR}/external/glm)
set(PHYSX_ROOT ${CMAKE_SOURCE_DIR}/external/physx)
set(GLFW_ROOT ${CMAKE_SOURCE_DIR}/external/glfw)
set(ASSIMP_ROOT ${CMAKE_SOURCE_DIR}/external/assimp)
set(FREETYPE_ROOT ${CMAKE_SOURCE_DIR}/external/freetype)
set(LIBZ_ROOT ${CMAKE_SOURCE_DIR}/external/libz)
set(FMOD_ROOT ${CMAKE_SOURCE_DIR}/external/fmod)

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

# === External Dependencies ===
find_package(Threads REQUIRED)

if(WIN32)
    add_definitions(-DNOMINMAX)
endif()

option(VELIX_USE_PHYSX "Enable NVIDIA PhysX support" ON)

if(APPLE)
    message(WARNING "PhysX is not supported on macOS. Disabling PhysX.")
    set(VELIX_USE_PHYSX OFF)
endif()

set(PHYSX_BUILD_TYPE "checked" CACHE STRING "The build type of PhysX")

set_property(CACHE PHYSX_BUILD_TYPE PROPERTY STRINGS debug checked profile release)

if(VELIX_USE_PHYSX)
    set(PHYSX_BUILD_TYPE "checked" CACHE STRING "The build type of PhysX")
    set_property(CACHE PHYSX_BUILD_TYPE PROPERTY STRINGS debug checked profile release)
endif()

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

set(PHYSX_LIB_DIR "${PHYSX_ROOT}/lib/${VELIX_PLATFORM}/${PHYSX_BUILD_TYPE}")
set(PHYSX_LIB_EXPECTED "${PHYSX_LIB_DIR}/libPhysX_static_64.a")

if(WIN32)
    set(PHYSX_LIB_EXPECTED "${PHYSX_LIB_DIR}/PhysXExtensions_static_64.lib")
endif()

if(VELIX_USE_PHYSX AND NOT EXISTS "${PHYSX_LIB_EXPECTED}")
    message(STATUS "Prebuilt PhysX not found. Building it manually...")

    if(WIN32)
        execute_process(
            COMMAND ${CMAKE_SOURCE_DIR}/scripts/build_physx_windows.bat
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE PHYSX_BUILD_RESULT
        )
    elseif(UNIX)
        execute_process(
            COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/build_physx_linux.sh
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE PHYSX_BUILD_RESULT
        )
    endif()

    if(NOT PHYSX_BUILD_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to build PhysX")
    endif()
elseif(VELIX_USE_PHYSX)
    message(STATUS "Using prebuilt PhysX from ${PHYSX_LIB_EXPECTED}")
else()
    message(STATUS "VELIX_USE_PHYSX is OFF — skipping PhysX setup.")
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

target_compile_definitions(${PROJECT_NAME} PUBLIC VELIX_USE_PHYSX=$<BOOL:${VELIX_USE_PHYSX}>)

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

if(VELIX_USE_PHYSX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PX_PHYSX_CHARACTER_STATIC_LIB PX_PHYSX_STATIC_LIB)

    if(WIN32)
        target_link_libraries(${PROJECT_NAME} PUBLIC
            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysX_64.lib
            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXPvdSDK_static_64.lib
            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXVehicle2_static_64.lib
            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXCharacterKinematic_static_64.lib
            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXExtensions_static_64.lib
            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXCooking_64.lib
            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXCommon_64.lib
            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXFoundation_64.lib
            ${PHYSX_ROOT}/lib/windows/${PHYSX_BUILD_TYPE}/PhysXTask_static_64.lib

        )
    elseif(UNIX)
        target_link_libraries(${PROJECT_NAME} PUBLIC
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysX_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXPvdSDK_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXVehicle2_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXCharacterKinematic_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXExtensions_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXCooking_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXCommon_static_64.a
            ${PHYSX_ROOT}/lib/linux/${PHYSX_BUILD_TYPE}/libPhysXFoundation_static_64.a
    )
    endif()
endif()

# === Link Libraries ===
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
#            ${LIBZ_ROOT}/lib/windows/zlib.lib
            ${GLFW_ROOT}/lib/windows/glfw3.lib
            assimp
            ${FREETYPE_ROOT}/lib/windows/freetype.lib
            Threads::Threads
    )
elseif(APPLE)
    find_package(ZLIB REQUIRED)
    find_package(Freetype REQUIRED)
    
    target_link_libraries(${PROJECT_NAME} PUBLIC
            ${GLFW_ROOT}/lib/macos/libglfw3.a
            # ${FMOD_ROOT}/lib/linux/libfmod.dylib Change to .dylib
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
        
install(TARGETS VelixFlow
        EXPORT VelixFlowTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

if(UNIX)
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