file(GLOB_RECURSE header_list 
    "include/**/*.hpp"
    "include/*.hpp")

file(GLOB_RECURSE source_list 
    "src/**/*.cpp"
    "src/*.cpp")

find_package(asio)
find_package(nlohmann_json)

add_executable(${PROJECT_NAME}-client ${header_list} ${source_list})

target_include_directories(${PROJECT_NAME}-client PRIVATE include)

target_link_libraries(${PROJECT_NAME}-client LINK_PUBLIC ${PROJECT_NAME}::Shared)
target_link_libraries(${PROJECT_NAME}-client PRIVATE asio::asio)
target_link_libraries(${PROJECT_NAME}-client PRIVATE nlohmann_json::nlohmann_json)
install(TARGETS ${PROJECT_NAME}-client)
