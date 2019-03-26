  cmake_minimum_required(VERSION 2.8.12.2)

  SET( CMAKE_CXX_FLAGS "-std=c++11 -O3")

  project(Docking_manager)

  # Find includes in corresponding build directories
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  # Instruct CMake to run moc automatically when needed.
  set(CMAKE_AUTOMOC ON)

#  SET(QT_USE_QTNETWORK true)

  # Find the QtWidgets library
  find_package(Qt5Widgets)

  find_package(Qt5Network)


  ###########################add pcl
  add_compile_options(-fext-numeric-literals)
  find_package(PCL REQUIRED)
  include_directories(${PCL_INCLUDE_DIRS})
  link_directories(${PCL_LIBRARY_DIRS})
  add_definitions(${PCL_DEFINITIONS})

  ############################add openGL
  find_package( OpenGL REQUIRED )
  list( APPEND SUMMARY_DEBUG_LIBS ${OPENGL_gl_LIBRARY} )

#  add_subdirectory(HesaiLidarSDK)      #local sdk
   add_subdirectory(Pandar40P)

   include_directories(
           Pandar40P/include
           Pandar40P/src/Pandar40P/include)


#  INCLUDE(${QT_USE_FILE})
  INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR} ${QT_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR})


  # Tell CMake to create the helloworld executable
  add_executable(Docking_manager  main.cpp mainwindow.h mainwindow.cpp openglwindow.h openglwindow.cpp mythread.h mythread.cpp add_model.h add_model.cpp add_detetion.h add_detetion.cpp qdoubleslider.h qdoubleslider.cpp add_detetion.ui add_model.ui mainwindow.ui)

  # Use the Widgets module from Qt 5.
  target_link_libraries(Docking_manager Qt5::Widgets)
  target_link_libraries(Docking_manager ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY} ${PCL_LIBRARIES} ${QT_QTNETWORK_LIBRARY})
#  target_link_libraries(Docking_manager hesaiLidarSDK -lpthread)    #local sdk
  target_link_libraries(Docking_manager Pandar40PSDK -lpthread)
  target_link_libraries(Docking_manager Qt5::Network)


  find_package(Qt5Core)

  get_target_property(QtCore_location Qt5::Core LOCATION)

#  find_package(Qt5Core)

  set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")

  # set up a mapping so that the Release configuration for the Qt imported target is
  # used in the COVERAGE CMake configuration.
  set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
