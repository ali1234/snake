# Basic parameters; check that these match your project / environment
cmake_minimum_required(VERSION 3.9)

# Wrapper for the Pico SDK, which is only included if PICO_BOARD is set
if (PICO_BOARD)
    include(${32BLIT_DIR}/32blit-pico/sdk_import.cmake)
endif()

project(snake)

set(PROJECT_SOURCE snake.cpp snake.hpp)

set(PROJECT_DISTRIBS LICENSE README.md)

if(MSVC)
  add_compile_options("/W4" "/wd4244" "/wd4324" "/wd4458" "/wd4100")
else()
  add_compile_options("-Wall" "-Wextra" "-Wdouble-promotion" "-Wno-unused-parameter")
endif()

find_package (32BLIT CONFIG REQUIRED PATHS ../32blit-sdk)

blit_executable (${PROJECT_NAME} ${PROJECT_SOURCE})
blit_metadata (${PROJECT_NAME} metadata.yml)
blit_assets_yaml (${PROJECT_NAME} assets.yml)
target_compile_definitions(${PROJECT_NAME} PRIVATE ALLOW_HIRES=0)
add_custom_target (flash DEPENDS ${PROJECT_NAME}.flash)

# setup release packages
install (FILES ${PROJECT_DISTRIBS} DESTINATION .)
set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set (CPACK_GENERATOR "ZIP" "TGZ")
include (CPack)
