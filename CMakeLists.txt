cmake_minimum_required(VERSION 3.5.1)

project(stupid)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(COMMON_LIB mariadb jsoncpp pthread)

add_library(${PROJECT_NAME} SHARED
	src/stupidb.cpp
	src/stupidbimpl.cpp
	src/zloghub.cpp
	src/dbargs.cpp
	)

option(zlog_enable "enable zlog log out" ON)
option(debug "compile as debug mode)" ON)

if(zlog_enable)
	target_compile_definitions(${PROJECT_NAME} PUBLIC 
		ZLOG_ENABLE
	)

	set(COMMON_LIB ${COMMON_LIB} zlog)

endif()

if(debug)
	add_definitions(-Wall -Wextra -g3 -O0)
else()
	add_definitions(-Wall -Wextra -O3)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}
	)

target_link_libraries(${PROJECT_NAME} PRIVATE ${COMMON_LIB}
	)

add_executable(${PROJECT_NAME}_test1
	test/test1.cpp
	)

target_include_directories(${PROJECT_NAME}_test1 PRIVATE 
	${PROJECT_SOURCE_DIR}
	)

target_link_libraries(${PROJECT_NAME}_test1 PRIVATE ${COMMON_LIB} ${PROJECT_NAME}
	)
