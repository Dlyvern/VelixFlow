cmake_minimum_required(VERSION 3.16)
project(VelixVK)

set(INCLUDE_DIR include)
set(SRC_DIR src)

set(VOLK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/external/volk)

file(GLOB_RECURSE PROJECT_HEADERS ${INCLUDE_DIR}/*.h ${INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES ${SRC_DIR}/*.cpp)

find_package(Vulkan REQUIRED)

message(STATUS "Found Vulkan Include Dir: ${Vulkan_INCLUDE_DIR}")
message(STATUS "Found Vulkan Libs: ${Vulkan_LIBRARIES}")

add_library(volk STATIC ${VOLK_ROOT}/src/volk.c)
target_include_directories(volk
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/volk/include>
        $<INSTALL_INTERFACE:include/VelixFlow/volk>
)

set_target_properties(volk PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(${PROJECT_NAME} SHARED ${PROJECT_HEADERS} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE volk)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> 
        $<BUILD_INTERFACE:${VOLK_ROOT}/include/>
)

set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${Vulkan_INCLUDE_DIR}
    ${GLFW_ROOT}/include/
    ${GLM_ROOT}/include/
)

if(VELIX_PLATFORM_LINUX)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        pthread
        VelixFlow
        ${Vulkan_LIBRARIES}
    )
elseif(VELIX_PLATFORM_WINDOWS)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${Vulkan_LIBRARIES}
        VelixFlow
    )

elseif(VELIX_PLATFORM_MACOS)
    target_link_libraries(${PROJECT_NAME} PUBLIC 
        ${Vulkan_LIBRARIES}
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