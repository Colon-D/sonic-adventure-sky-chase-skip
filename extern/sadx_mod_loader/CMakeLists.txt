cmake_minimum_required(VERSION 3.14)
include(FetchContent)

FetchContent_Declare(
  "sadx_mod_loader"
  GIT_REPOSITORY "https://github.com/X-Hax/sadx-mod-loader.git"
  GIT_TAG        "60e8ebe16c00175e9bd481c0991c0e645d186fc2"
)
FetchContent_MakeAvailable("sadx_mod_loader")

add_library("libmodutils" STATIC
    "${sadx_mod_loader_SOURCE_DIR}/libmodutils/stdafx.cpp"
    "${sadx_mod_loader_SOURCE_DIR}/libmodutils/LandTableInfo.cpp"
    "${sadx_mod_loader_SOURCE_DIR}/libmodutils/ModelInfo.cpp"
    "${sadx_mod_loader_SOURCE_DIR}/libmodutils/AnimationFile.cpp"
    "${sadx_mod_loader_SOURCE_DIR}/libmodutils/Trampoline.cpp"
    "${sadx_mod_loader_SOURCE_DIR}/libmodutils/GameObject.cpp"
    "${sadx_mod_loader_SOURCE_DIR}/libmodutils/ArchiveX.cpp"
    "${sadx_mod_loader_SOURCE_DIR}/libmodutils/GoalRing.cpp"
    "${sadx_mod_loader_SOURCE_DIR}/libmodutils/GoalRingModels.cpp"
)
target_include_directories("libmodutils" PUBLIC
    "${sadx_mod_loader_SOURCE_DIR}/SADXModLoader/include"
    "${sadx_mod_loader_SOURCE_DIR}/libmodutils"
)
