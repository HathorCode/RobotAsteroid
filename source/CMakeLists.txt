cmake_minimum_required(VERSION 3.0)

project(RobotAsteroid)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("third party")
include_directories("third party/freetype/include/")
include_directories("third party/openal/include/")
include_directories("third party/ogg/include/")
include_directories("third party/vorbis/include/")

link_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories("third party/freetype/lib")
link_directories("third party/openal/libs/Win64")
link_directories("third party/ogg")
link_directories("third party/vorbis")

# add_custom_command(TARGET RobotAsteroid PRE_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E copy_directory
#                    ${CMAKE_SOURCE_DIR}/../assets/build ${CMAKE_BUILD_DIRECTORY}/assets)

file(GLOB_RECURSE allCodeFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp" "*.cpp")

if(WIN32)
add_executable(RobotAsteroid WIN32 ${allCodeFiles})
elseif(UNIX)
add_executable(RobotAsteroid ${allCodeFiles})
endif()

target_link_libraries(RobotAsteroid freetype255ST)
target_link_libraries(RobotAsteroid OpenAL32)
target_link_libraries(RobotAsteroid ogg)
target_link_libraries(RobotAsteroid vorbis)

target_compile_definitions(RobotAsteroid
  PRIVATE tcPlatGL)
if(WIN32)
target_compile_definitions(RobotAsteroid
	PRIVATE tcPlatWin)
target_link_libraries(RobotAsteroid OpenGL32)
elseif(UNIX)
if(NOT APPLE)
target_compile_definitions(RobotAsteroid
	PRIVATE tcPlatLinux)
target_link_libraries(RobotAsteroid gl)
endif(NOT APPLE)
endif()

#TODO: add other compilers warnings flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

foreach(src ${allCodeFiles})
	if(src STREQUAL "Global.hpp")
		source_group("\\\\" FILES ${src})
	else()
		get_filename_component(dir ${src} DIRECTORY)
		string(REPLACE "/" "\\" fixedDir ${dir})
		MESSAGE(STATUS ${fixedDir})
		MESSAGE(STATUS ${src})
		source_group(${fixedDir} FILES ${src})
	endif()
endforeach()
