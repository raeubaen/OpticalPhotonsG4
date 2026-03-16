#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.27)
project(CrilinSimOptical)

#----------------------------------------------------------------------------
# Find Geant4 package with UI and Vis drivers
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Find ROOT package
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})

# Setup include directories for this project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIRS}
                    ${ROOT_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
set(SOURCES
    src/main.cc
    src/PrimaryGeneratorMessenger.cc
    src/PrimaryGeneratorAction.cc
    src/DetectorConstruction.cc
    src/EnergyTimeSD.cc
    src/EventAction.cc
)

set(HEADERS
    include/PrimaryGeneratorAction.hh
    include/PrimaryGeneratorMessenger.hh
    include/DetectorConstruction.hh
    include/CONFIGURATION.hh
    include/Geometry.hh
    include/EnergyTimeSD.hh
    include/EventAction.hh
)

#----------------------------------------------------------------------------
# Add the executable and link to Geant4 and ROOT
add_executable(CrilinSimOptical ${SOURCES} ${HEADERS})
target_link_libraries(CrilinSimOptical ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy macros to the build directory (optional)
set(CrilinSimOptical_SCRIPTS
    macro/init_vis.mac
    macro/run.mac
)

foreach(_script ${CrilinSimOptical_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
  )
endforeach()

#----------------------------------------------------------------------------
# Install the executable
install(TARGETS CrilinSimOptical DESTINATION bin)

#----------------------------------------------------------------------------
# ROOT settings
include(${ROOT_USE_FILE})
