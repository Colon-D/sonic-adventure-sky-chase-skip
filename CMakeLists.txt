cmake_minimum_required(VERSION 3.14)

project("SASkyChaseSkip")

add_library("${PROJECT_NAME}" SHARED
    "src/main.cpp"
)
target_include_directories("${PROJECT_NAME}" PUBLIC "include")
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_17)

add_subdirectory(extern)
target_link_libraries("${PROJECT_NAME}" PUBLIC
    "mod_loader_common"
    "libmodutils"
)

# get Sonic Adventure DX directory
set("SONIC_ADVENTURE_DX_DIRECTORY" "" CACHE PATH "")
# convert to forward slashes
cmake_path(SET "SONIC_ADVENTURE_DX_DIRECTORY" "${SONIC_ADVENTURE_DX_DIRECTORY}")
# if directory exists
IF(EXISTS "${SONIC_ADVENTURE_DX_DIRECTORY}")
    # set mod directory
    set(
        "MOD_DIRECTORY"
        "${SONIC_ADVENTURE_DX_DIRECTORY}/mods/Sky Chase Skip"
    )
    # set runtime output directory to mod directory
    set_target_properties("${PROJECT_NAME}" PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${MOD_DIRECTORY}"
    )
    # copy files from "resources/SADXModManager_mod" to mod directory
    configure_file(
        "${CMAKE_SOURCE_DIR}/resources/SADXModManager_mod/mod.ini"
        "${MOD_DIRECTORY}/mod.ini"
        COPYONLY
    )
    configure_file(
        "${CMAKE_SOURCE_DIR}/resources/SADXModManager_mod/configschema.xml"
        "${MOD_DIRECTORY}/configschema.xml"
        COPYONLY
    )
ELSE()
    message(WARNING "SONIC_ADVENTURE_DX_DIRECTORY not found")
ENDIF()
