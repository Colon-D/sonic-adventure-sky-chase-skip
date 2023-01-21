cmake_minimum_required(VERSION 3.14)
include(FetchContent)

FetchContent_Declare(
  "mod_loader_common"
  GIT_REPOSITORY "https://github.com/sonicretro/mod-loader-common.git"
  GIT_TAG        "424fab22f4b90e6a92a636af5c6f7f0172da3e7c"
)
FetchContent_MakeAvailable("mod_loader_common")

add_library("mod_loader_common" STATIC
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon/CodeParser.cpp"
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon/FileSystem.cpp"
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon/IniFile.cpp"
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon/stdafx.cpp"
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon/TextConv.cpp"
)
target_include_directories("mod_loader_common" PUBLIC
    "${mod_loader_common_SOURCE_DIR}/ModLoaderCommon"
)
