file(GLOB_RECURSE header_list 
    "include/**/*.hpp"
    "include/*.hpp")

file(GLOB_RECURSE source_list 
    "src/**/*.cpp"
    "src/*.cpp")

add_executable(${PROJECT_NAME}-client ${header_list} ${source_list})

target_include_directories(${PROJECT_NAME}-client PRIVATE include)

target_link_libraries(${PROJECT_NAME}-client LINK_PUBLIC ${PROJECT_NAME}::Shared)

install(TARGETS ${PROJECT_NAME}-client)
