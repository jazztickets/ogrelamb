cmake_minimum_required(VERSION 2.8)

# project name
project(ogrelamb)
subdirs(tools)

# add extra find modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# set up ogre cmake module path based on OS
if(MSVC)
	add_definitions("/D_CRT_SECURE_NO_WARNINGS /wd4251 /wd4275" )

	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

	set(Boost_USE_STATIC_LIBS TRUE)
	set(BOOST_ROOT "$ENV{OGRE_HOME}/boost/")
	set(OGRE_BOOST_COMPONENTS thread date_time)

	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)
elseif(UNIX)
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -H")

	# set default build type
	if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE Release)
	endif()

	if(EXISTS "/usr/local/lib/OGRE/cmake")
		set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	elseif(EXISTS "/usr/lib/OGRE/cmake")
		set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	else()
		message(SEND_ERROR "Failed to find module path.")
	endif()
	set(OGRE_BOOST_COMPONENTS thread date_time system)

	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
endif()

# find ogre
find_package(OGRE REQUIRED)
if(NOT OGRE_FOUND)
	message(SEND_ERROR "Could not find OGRE!")
endif()

# find ois
find_package(OIS REQUIRED)
if(NOT OIS_FOUND)
	message(SEND_ERROR "Could not find OIS!")
endif()

# find boost
add_definitions("-DBOOST_ALL_NO_LIB")
find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} REQUIRED)

# bullet
set(BULLET_ROOT "$ENV{BULLET_ROOT}")
find_package(Bullet REQUIRED)

# add include paths
include_directories(${Boost_INCLUDE_DIR})
include_directories(${OGRE_INCLUDE_DIRS})
include_directories(${OIS_INCLUDE_DIRS})
include_directories(${BULLET_INCLUDE_DIRS})
include_directories(${OGRE_Terrain_INCLUDE_DIRS})
include_directories(${OGRE_Paging_INCLUDE_DIRS})
include_directories("src")
#include_directories(${LUA_INCLUDE_DIR})

# add source files
file(GLOB MAIN_FILES src/*.cpp src/*.h)

source_group(main FILES ${MAIN_FILES})

# add binary
add_executable(${CMAKE_PROJECT_NAME} ${MAIN_FILES})

# link libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${OGRE_LIBRARIES} ${OGRE_Terrain_LIBRARIES} ${OGRE_Paging_LIBRARIES} ${OIS_LIBRARIES} ${Boost_LIBRARIES} ${BULLET_LIBRARIES})

# copy ogre DLLs to working directory
if(WIN32)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/debug/OgreMain_d.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/debug/OgreTerrain_d.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/debug/OgrePaging_d.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/debug/OIS_d.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/debug/RenderSystem_GL_d.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/debug/Plugin_CgProgramManager_d.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/release/OgreMain.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/release/OgreTerrain.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/release/OgrePaging.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/release/OIS.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/release/RenderSystem_GL.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/release/Plugin_CgProgramManager.dll" ${PROJECT_SOURCE_DIR}/working)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{OGRE_HOME}/bin/release/cg.dll" ${PROJECT_SOURCE_DIR}/working)
endif()
