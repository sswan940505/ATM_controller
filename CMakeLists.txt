cmake_minimum_required(VERSION 2.8.3)
project(ATM_controller)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    rospy
    std_msgs
    )

find_package(Boost REQUIRED)

catkin_package(
    #  INCLUDE_DIRS include
    #  LIBRARIES beginner_tutorials
    #  CATKIN_DEPENDS roscpp rospy std_msgs
    #  DEPENDS system_lib
)

include_directories(
    ${catkin_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)


FILE(GLOB atmtest_source
    "node/ATMTester/ATMcontroller/*.cpp"
    "node/ATMTester/ATMcontroller/*.h")

add_executable(atmTest node/ATMTester/ATMtester.cpp ${atmtest_source})
target_link_libraries(atmTest ${catkin_LIBRARIES})

