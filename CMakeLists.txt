#cmake minimum version request 
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#project name
PROJECT(MiniCalc)

#head file path
INCLUDE_DIRECTORIES(include)
#source directory
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

#set environment variable
SET(SRC_LIST ${DIR_SRCS})
#SET(PROJECT_BINARY_DIR ./bin)
#SET(PROJECT_SOURCE_DIR ./src)

#c++ 11 support
SET(CMAKE_CXX_FLAGS "-std=c++11")

#add executable file
ADD_EXECUTABLE(MiniCalc ${SRC_LIST})