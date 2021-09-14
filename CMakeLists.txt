cmake_minimum_required(VERSION 3.5.1)

project(stupid)

add_library(${PROJECT_NAME} SHARED
	${PROJECT_SOURCE_DIR}/src/stupidb.cpp
	${PROJECT_SOURCE_DIR}/src/dbargs.cpp
	)

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}
	)

target_link_libraries(${PROJECT_NAME} PRIVATE
	mariadb
	)

add_executable(${PROJECT_NAME}_test 
	${PROJECT_SOURCE_DIR}/test/connect.cpp
	${PROJECT_SOURCE_DIR}/src/stupidb.cpp
	)

target_include_directories(${PROJECT_NAME}_test PRIVATE 
	${PROJECT_SOURCE_DIR}
	)

target_link_libraries(${PROJECT_NAME}_test PRIVATE
	mariadb
	)
