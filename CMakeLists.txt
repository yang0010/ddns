cmake_minimum_required(VERSION 3.6.0)
project(Main)
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)
file(GLOB SRCS
	"${PROJECT_SOURCE_DIR}/include/*"
	"${PROJECT_SOURCE_DIR}/src/*"
)
#link_libraries(./arm64_lib)
#link_directories(${catkin_LIB_DIRS} arm64_lib)
add_executable(ddns ${SRCS})
target_link_libraries(ddns alibabacloud-sdk-core alibabacloud-sdk-alidns curl jsoncpp)