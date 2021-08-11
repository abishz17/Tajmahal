cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)

project(Graphics VERSION 1.1)

link_libraries(stdc++fs)

include_directories("headers")

set(files "main.cpp")

set(includes "colors.h" "projection.h" "model.h"
             "transformations.h" "camera.h" "matrix.h"
			 "line.h" "drawTriangle.h" "structure.h"
			  )

#actual execution of project
add_executable(${PROJECT_NAME} ${files} ${includes})

#glfw
target_link_libraries(${PROJECT_NAME} PRIVATE glut GLU GL)

if($CMAKE_SYSTEM_NAME} MATCHES "linux")
	message("On LINUX")
elseif($CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("On Windows")
endif()
