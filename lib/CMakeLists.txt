add_library(EncodeLib STATIC 
    Encode.cpp 
    Node.cpp 
    Decode.cpp)
target_include_directories(EncodeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})