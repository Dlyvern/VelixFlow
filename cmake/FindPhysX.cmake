option(VELIX_USE_PHYSX "Enable NVIDIA PhysX support" ON)

if(APPLE)
    message(WARNING "PhysX is not supported on macOS. Disabling PhysX.")
    set(VELIX_USE_PHYSX OFF)
    return()
endif()

if(VELIX_USE_PHYSX)
    set(PHYSX_BUILD_TYPE "checked" CACHE STRING "The build type of PhysX")

    set_property(CACHE PHYSX_BUILD_TYPE PROPERTY STRINGS debug checked profile release)

    set(PHYSX_BUILD_TYPE "checked" CACHE STRING "The build type of PhysX")
    set_property(CACHE PHYSX_BUILD_TYPE PROPERTY STRINGS debug checked profile release)

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
        return()
    endif()


    target_compile_definitions(${PROJECT_NAME} PUBLIC
        PX_PHYSX_CHARACTER_STATIC_LIB
        PX_PHYSX_STATIC_LIB
        VELIX_USE_PHYSX=$<BOOL:${VELIX_USE_PHYSX}>
    )

    target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
            $<BUILD_INTERFACE:${PHYSX_ROOT}/include/>
            $<INSTALL_INTERFACE:include/VelixFlow/PhysX>
    )


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

    install(DIRECTORY ${PHYSX_ROOT}/include/ DESTINATION include/VelixFlow/PhysX)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC VELIX_USE_PHYSX=0)
    message(STATUS "PhysX support disabled")
endif()