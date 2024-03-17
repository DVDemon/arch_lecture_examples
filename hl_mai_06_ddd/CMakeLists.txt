cmake_minimum_required(VERSION 3.2)

project(ddd_examples C CXX)

foreach(i RANGE 5)
 add_executable(example_${i} example_${i}.cpp)
 set_target_properties(example_${i} PROPERTIES LINKER_LANGUAGE CXX)
 set_target_properties(example_${i} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)   
endforeach(i)
