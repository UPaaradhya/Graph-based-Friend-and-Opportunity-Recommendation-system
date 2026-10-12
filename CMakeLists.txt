cmake_minimum_required(VERSION 3.16)
project(FriendOpportunityNeo4j LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCURL REQUIRED IMPORTED_TARGET libcurl)
add_executable(friend_opportunity
    src/main.cpp
    src/Neo4jDatabase.cpp
    src/UserService.cpp
    src/GraphManager.cpp
    src/RecommendationService.cpp
    src/OpportunityService.cpp
)
target_include_directories(friend_opportunity PRIVATE src)
target_link_libraries(friend_opportunity PRIVATE PkgConfig::LIBCURL)
