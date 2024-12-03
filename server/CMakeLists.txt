project(kp_server)

set(SERVER_SOURCES main.cpp Utils.h Controller.h)

set(SERVER_EXEC_NAME ${PROJECT_NAME})

find_package(spdlog CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
#find_package(jwt-cpp CONFIG REQUIRED)
find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")

add_executable(${SERVER_EXEC_NAME} ${SERVER_SOURCES} ${COMMON_SOURCES})

include($ENV{SUNGEAR_SOURCES_ROOT}/cmake/SungearEngineInclude.cmake)

target_include_directories(${PROJECT_NAME} PRIVATE ../common Sources ${SungearEngine_INCLUDE_DIRS} ${JWT_CPP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SungearEngine_LIBS} Drogon::Drogon PostgreSQL::PostgreSQL OpenSSL::SSL OpenSSL::Crypto)