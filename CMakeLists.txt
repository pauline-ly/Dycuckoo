cmake_minimum_required(VERSION 2.8)
project(Dyhash)
find_package(CUDA QUIET REQUIRED)

set(CUDA_NVCC_FLAGS -gencode arch=compute_61,code=sm_61;-O3;--ptxas-options=-v)

include_directories(/usr/local/cuda/samples/common/inc)

set(CMAKE_CXX_STANDARD 11)

set(files
        src/dy_hash.h include/gputimer.h
        src/dyhash/cuckoo.cu src/dyhash/cuckoo.h
        src/dyTest/api.cpp src/dyTest/api.h
        src/dyTest/Main.cpp
        include/mt19937ar.cpp include/mt19937ar.h
        include/random_numbers.cpp include/random_numbers.h
        include/cnmem.cpp include/cnmem.h)
cuda_add_executable(linear ${files})
target_link_libraries(linear pthread)
target_link_libraries(linear gmock)
target_link_libraries(linear gtest)









