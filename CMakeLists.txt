# (1)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(OMGsim)


# check for x64 or x86
get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
if (${LIB64} STREQUAL "TRUE")
    set(LIBSUFFIX 64)
else()
    set(LIBSUFFIX "")
endif()

# (2)
find_package(Geant4 REQUIRED ui_all vis_all)

if(NOT DEFINED ENV{G4INSTALL})
    MESSAGE(FATAL_ERROR "VARIABLE G4INSTALL WAS NOT FOUND")
    return()
endif()
SET(CMAKE_MODULE_PATH $ENV{G4ROOT}/lib/Geant4-$ENV{G4VERSION}/Modules)

IF (EXISTS $ENV{G4ROOT}/lib/Geant4-$ENV{G4VERSION})
  SET(CMAKE_MODULE_PATH $ENV{G4ROOT}/lib/Geant4-$ENV{G4VERSION}/Modules)
ELSE ()
  SET(CMAKE_MODULE_PATH $ENV{G4ROOT}/lib64/Geant4-$ENV{G4VERSION}/Modules)
ENDIF (EXISTS $ENV{G4ROOT}/lib/Geant4-$ENV{G4VERSION})

find_package(ROOT)


link_directories(${ROOT_LIBRARY_DIR})
link_directories($ENV{LIBCONFIG_ROOT/lib})

# (3)
include(${Geant4_USE_FILE})
#include_directories(*/include)
include_directories(${ROOT_INCLUDE_DIR})


# (4) organize project libraries
file(GLOB sources */src/[a-zA-Z0-9]*.cc)
file(GLOB headers */include/)

foreach(adir IN LISTS sources)
  if (adir MATCHES .*/tests/+src.*)
    list (REMOVE_ITEM sources ${adir})
  endif ()
endforeach ()

foreach(adir IN LISTS headers)
  if (adir MATCHES .*/tests/+include/)
    list (REMOVE_ITEM headers ${adir})
  endif ()
endforeach ()


file(GLOB test_sources tests/src/[a-zA-Z0-9]*.cc)
file(GLOB test_headers tests/include/)

#list(REMOVE_ITEM sources ${test_sources})

#message (${sources})
#message (${test_sources})

file(GLOB PROGRAMS programs/[a-zA-Z1-9]*.cxx)
file(GLOB TESTS tests/[a-zA-Z1-9]*.cxx)
include_directories (${headers})

# (5) set the needed cflags
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -pedantic -Wno-non-virtual-dtor -Wno-long-long -Wwrite-strings -Wpointer-arith -Woverloaded-virtual -Wno-variadic-macros -Wshadow -pipe -std=c++11 -DPROJECT_SOURCE_DIR=\\\"${PROJECT_SOURCE_DIR}\\\"")

#add config++ lib
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L$ENV{LIBCONFIG_ROOT}/lib -lconfig++")

if(USE_FISSION_NEW)
  #SET(LFISSION "-L/${CMAKE_SOURCE_DIR}/FissionLib/fission_v1.9.1/lib -lFission")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/${CMAKE_SOURCE_DIR}/FissionLib/fission_v1.9.1/lib -lFission")
  include_directories(${CMAKE_SOURCE_DIR}/FissionLib/fission_v1.9.1/include)
  add_definitions(-DFISSION_NEW)
endif()


if (DEBUG)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else ()
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif ()

if (FULL_OPTIMIZATION)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -flto")
endif (FULL_OPTIMIZATION)

if (${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "4.8.9")
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=auto")
endif(${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER "4.8.9")


SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__CXX_MIN_VERSION=${CMAKE_CXX_COMPILER_VERSION}")


message (${CMAKE_CXX_FLAGS})


# (6) finish with all the binaries
if(ENABLE_TESTING)
  add_library(omgsim SHARED ${sources} ${test_sources})
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_TESTING=1")
  foreach(aprog IN LISTS TESTS)
    get_filename_component(aprogname ${aprog} NAME_WE)
    add_executable(${aprogname} ${aprog} ${headers} ${test_headers})
    target_link_libraries(${aprogname} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} omgsim)
    #target_link_libraries(${aprogname} ${ROOT_LIBRARIES})
    #target_link_libraries(${aprogname} omgsim)
  endforeach ()
else(ENABLE_TESTING)
  add_library(omgsim SHARED ${sources})
  foreach(aprog IN LISTS PROGRAMS)
    get_filename_component(aprogname ${aprog} NAME_WE)
    add_executable(${aprogname} ${aprog} ${headers})
    target_link_libraries(${aprogname} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} omgsim)
    #target_link_libraries(${aprogname} ${ROOT_LIBRARIES})
    #target_link_libraries(${aprogname} omgsim)
  endforeach ()
endif(ENABLE_TESTING)

file(COPY ${PROJECT_SOURCE_DIR}/scripts DESTINATION ${PROJECT_BINARY_DIR})
