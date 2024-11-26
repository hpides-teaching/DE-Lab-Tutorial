cmake_minimum_required(VERSION 3.10)

project(DE-Lab-Base)

set(CMAKE_CXX_STANDARD 20)


message(STATUS "ARCH: ${CMAKE_SYSTEM_PROCESSOR}")
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    add_definitions(-DX86_64)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    add_definitions(-DAARCH64)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Set the architecture flag based on processor type
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # No direct -march equivalent for MSVC, use appropriate architecture-specific options if necessary
    # Add any MSVC specific flags here if required
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH}) # To allow CMake to locate our Find*.cmake files


set(DEFAULT_LIB_DIRS $ENV{HOME}/local /opt/local /usr/local /usr)

include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY "https://github.com/nlohmann/json/"
  GIT_TAG "v3.11.3"
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY "https://github.com/jarro2783/cxxopts/"
  GIT_TAG "v3.2.1"
)
FetchContent_MakeAvailable(cxxopts)

target_include_directories(cxxopts INTERFACE ${cxxopts_SOURCE_DIR}/include)

FetchContent_Declare(
  perf-cpp-external
  GIT_REPOSITORY "https://github.com/jmuehlig/perf-cpp"
  GIT_TAG "v0.8.3"
)
FetchContent_MakeAvailable(perf-cpp-external)

set(ENABLE_CLANG_TIDY OFF CACHE BOOL "Run clang-tidy")
if (ENABLE_CLANG_TIDY)
  include_directories(${nlohmann_json_SOURCE_DIR}/single_include) # tidy-clang otherwise wont be able to find corresponding headers.
  set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -format-style='file';
    -config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy;
    -header-filter=${CMAKE_CURRENT_SOURCE_DIR}/src/;
  )
endif()

target_compile_options(perf-cpp PRIVATE -w) # GCC/Clang: -w disables all warnings for perf-cpp target

add_subdirectory(src)