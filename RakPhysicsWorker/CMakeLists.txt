cmake_minimum_required(VERSION 2.6)
project(rakphysicsworker)
set(CMAKE_CXX_FLAGS " -D_ELPP_THREAD_SAFE=1 -D_ELPP_DEBUG_ERRORS=1 -D_ELPP_STACKTRACE_ON_CRASH=1 -D_ELPP_FORCE_USE_STD_THREAD=1 -D_ELPP_LOGGING_FLAGS_FROM_ARG=1 -I/Deps/bullet/ -std=gnu++11 -lpthread -lrt")

include_directories ("Deps/RakNet/Source/")
include_directories ("Deps/bullet/src/")

file(GLOB files "*.cpp" "*.c" "*.a" "*.cxx")

add_executable(rakphysicsworker ${files})
target_link_libraries(rakphysicsworker ${CMAKE_SOURCE_DIR}/libraknet.a)
target_link_libraries(rakphysicsworker ${CMAKE_SOURCE_DIR}/libBulletDynamics.a)
target_link_libraries(rakphysicsworker ${CMAKE_SOURCE_DIR}/libBulletCollision.a)
target_link_libraries(rakphysicsworker ${CMAKE_SOURCE_DIR}/libLinearMath.a)

install(TARGETS rakphysicsworker RUNTIME DESTINATION bin)
