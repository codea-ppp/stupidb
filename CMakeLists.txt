cmake_minimum_required(VERSION 3.5.1)

project(stupid)

add_library(${PROJECT_NAME} SHARED
	${PROJECT_SOURCE_DIR}/src/stupidb.cpp
	)

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}
	)

target_link_libraries(${PROJECT_NAME} PRIVATE
	mariadb
	)
