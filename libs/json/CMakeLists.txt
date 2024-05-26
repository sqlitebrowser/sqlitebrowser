cmake_minimum_required(VERSION 3.15)

add_library(nlohmann_json INTERFACE)
add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)

set(NLOHMANN_JSON_INCLUDE_BUILD_DIR ./single_include)

target_include_directories(nlohmann_json INTERFACE ./single_include)
