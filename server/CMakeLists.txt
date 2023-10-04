file(GLOB_RECURSE header_list 
    "include/**/*.hpp"
    "include/*.hpp")

file(GLOB_RECURSE source_list 
    "src/**/*.cpp"
    "src/*.cpp")

message(STATUS "${source_list}")

add_executable(${PROJECT_NAME}-server ${header_list} ${source_list})

target_include_directories(${PROJECT_NAME}-server PRIVATE include)

target_link_libraries(${PROJECT_NAME}-server LINK_PUBLIC ${PROJECT_NAME}::Shared)

install(TARGETS ${PROJECT_NAME}-server)
