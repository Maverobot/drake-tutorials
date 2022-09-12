# find python libraries
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
find_package(PythonLibs 3.0 REQUIRED)
include_directories(${Python3_INCLUDE_DIRS} ${NumPy_INCLUDE_DIRS})


# populate matplotlib repository
include(FetchContent)
FetchContent_Declare(
  matplotlib
  GIT_REPOSITORY https://github.com/Cryoris/matplotlib-cpp.git
  GIT_TAG        1121c8a1328324923f36ab0df9537e2849f41a87
)
FetchContent_GetProperties(matplotlib)
if(NOT matplotlib_POPULATED)
  FetchContent_Populate(matplotlib)
endif()
include_directories(SYSTEM ${matplotlib_SOURCE_DIR})
link_libraries(${Python3_LIBRARIES} Python3::NumPy)
