cmake_minimum_required(VERSION 3.5.1)

project(stupid)
set(COMMON_LIB mariadb jsoncpp)

add_library(${PROJECT_NAME} SHARED
	${PROJECT_SOURCE_DIR}/src/stupidb.cpp
	${PROJECT_SOURCE_DIR}/src/stupidbimpl.cpp
	${PROJECT_SOURCE_DIR}/src/dbargs.cpp
	)

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}
	)

add_definitions(-Wall -Wextra -g3 -O0)

target_link_libraries(${PROJECT_NAME} PRIVATE ${COMMON_LIB}
	)

add_executable(${PROJECT_NAME}_test 
	${PROJECT_SOURCE_DIR}/test/connect.cpp
	)

target_include_directories(${PROJECT_NAME}_test PRIVATE 
	${PROJECT_SOURCE_DIR}
	)

target_link_libraries(${PROJECT_NAME}_test PRIVATE ${COMMON_LIB} ${PROJECT_NAME}
	)
