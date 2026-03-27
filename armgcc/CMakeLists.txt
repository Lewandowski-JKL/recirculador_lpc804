# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION 2)
SET (MCUXPRESSO_CMAKE_FORMAT_MINOR_VERSION 0)

include(ide_overrides.cmake OPTIONAL)

if(CMAKE_SCRIPT_MODE_FILE)
  message("${MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION}")
  return()
endif()


# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})
SET(APP_SRC_DIR "${ProjDirPath}/../src")
SET(LIB_DIR "${ProjDirPath}/../lib")

# if (DEFINED ENV{SdkRootDirPath})
#    SET(SdkRootDirPath "$ENV{SdkRootDirPath}")
# else()
#    SET(SdkRootDirPath "${ProjDirPath}/../../../../..")
# endif()

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(Recirculador)

set(MCUX_BUILD_TYPES debug release)

set(MCUX_SDK_PROJECT_NAME Recirculador.elf)

# no topo (após project/sets), ou antes do configure
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# if (NOT DEFINED SdkRootDirPath)
#    SET(SdkRootDirPath ${SdkRootDirPath})
# endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

# add_executable(${MCUX_SDK_PROJECT_NAME} 
# "${APP_SRC_DIR}/app/main.c"
# "${APP_SRC_DIR}/app/system.c"
# "${APP_SRC_DIR}/services/calendar/calendar.c"
# "${APP_SRC_DIR}/drivers/flash_memory/flash_memory.c"
# "${APP_SRC_DIR}/drivers/peripheral/adc/adc.c"
# "${APP_SRC_DIR}/stubs/peripheral/adc/adc.c"
# #"${SdkRootDirPath}/core/boards/lpcxpresso804/board.c"
# #"${SdkRootDirPath}/core/boards/lpcxpresso804/board.h"
# #"${SdkRootDirPath}/core/boards/lpcxpresso804/clock_config.c"
# #"${SdkRootDirPath}/core/boards/lpcxpresso804/clock_config.h"
# )



# Cria o executável vazio
add_executable(${MCUX_SDK_PROJECT_NAME})

# Coleta TODOS os .c dentro de src/ recursivamente
file(GLOB_RECURSE APP_C CONFIGURE_DEPENDS
    "${APP_SRC_DIR}/*.c"
)

# Adiciona todas as sources ao executável
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE ${APP_C})



# Adiciona fontes ao target (nome gerado pelo MCUX: ${MCUX_SDK_PROJECT_NAME})
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE ${APP_C})

# Inclui todos os .c do seu src/ (recursivo)
file(GLOB_RECURSE APP_C CONFIGURE_DEPENDS
    "${APP_SRC_DIR}/*.c"
    "${APP_SRC_DIR}/*/*.c"
    "${APP_SRC_DIR}/*/*/*.c"
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${APP_SRC_DIR}
    ${APP_SRC_DIR}/app
    ${APP_SRC_DIR}/drivers
    ${APP_SRC_DIR}/drivers/flash_memory
    ${APP_SRC_DIR}/drivers/gpio
    ${APP_SRC_DIR}/drivers/interrupt
    ${APP_SRC_DIR}/drivers/peripheral
    ${APP_SRC_DIR}/drivers/peripheral/adc
    ${APP_SRC_DIR}/drivers/peripheral/i2c
    ${APP_SRC_DIR}/drivers/pin_mux
    ${APP_SRC_DIR}/services
    ${APP_SRC_DIR}/services/calendar
    ${APP_SRC_DIR}/services/checksum
    ${APP_SRC_DIR}/services/led_control
    ${APP_SRC_DIR}/services/log_lpc804
    ${APP_SRC_DIR}/services/message_manager
    ${APP_SRC_DIR}/services/ntc_thermistor
    ${APP_SRC_DIR}/services/registers_manager
    ${APP_SRC_DIR}/services/task
    ${APP_SRC_DIR}/services/fota
    ${APP_SRC_DIR}/services/rtc
    ${APP_SRC_DIR}/stubs
    ${ProjDirPath}/../include
    ${LIB_DIR}/repo/core/boards/lpcxpresso804
    ${LIB_DIR}/repo/core/CMSIS/core/include
    ${LIB_DIR}/repo/core/CMSIS/core/include
    ${LIB_DIR}/repo/core/devices/LPC804
    ${LIB_DIR}/repo/core/drivers/iap
    ${LIB_DIR}/repo/core/drivers/common
)

# include(${SdkRootDirPath}/core/devices/LPC804/all_lib_device.cmake)
include(${LIB_DIR}/repo/core/devices/LPC804/all_lib_device.cmake)


# Ajuste o caminho se necessário: use o mesmo prefixo visto no log (lib/repo/core/...)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    "${LIB_DIR}/repo/core/drivers/iap/fsl_iap.c"
)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map")