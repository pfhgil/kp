project(kp_server)

set(SERVER_SOURCES main.cpp)

set(SERVER_EXEC_NAME ${PROJECT_NAME})

find_package(spdlog CONFIG REQUIRED)

add_executable(${SERVER_EXEC_NAME} ${SERVER_SOURCES} ${COMMON_SOURCES})

include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/SungearEngineInclude.cmake)

target_include_directories(${PROJECT_NAME} PRIVATE ../common Sources ${SungearEngine_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SungearEngine_LIBS} Drogon::Drogon PostgreSQL::PostgreSQL)