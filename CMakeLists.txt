cmake_minimum_required(VERSION 3.16.3)
project(course_work_3_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(course_work_3_2 src/main.cpp src/producer/producer.cpp src/producer/producer.h src/storage/storage.cpp src/storage/storage.h src/repair_producer/repair_producer.cpp src/repair_producer/repair_producer.h src/verification/verification.cpp src/verification/verification.h)
