cmake_minimum_required(VERSION 3.14)
project(oop_lab7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Пути к исходным файлам
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Включаем директорию с заголовочными файлами
include_directories(${INCLUDE_DIR})

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
    TLS_VERIFY false
)
FetchContent_MakeAvailable(googletest)

# Список исходных файлов для основного приложения
set(CPP_SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/npc.cpp
    ${SRC_DIR}/druid.cpp
    ${SRC_DIR}/orc.cpp
    ${SRC_DIR}/slaver.cpp
    ${SRC_DIR}/npc_factory.cpp
    ${SRC_DIR}/fight_visitor.cpp
    ${SRC_DIR}/dungeon.cpp
    ${SRC_DIR}/observer.cpp
    ${SRC_DIR}/utils.cpp
)

# Основное приложение
add_executable(lab7 ${CPP_SOURCES})

# Тесты
set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
    ${SRC_DIR}/npc.cpp
    ${SRC_DIR}/druid.cpp
    ${SRC_DIR}/orc.cpp
    ${SRC_DIR}/slaver.cpp
    ${SRC_DIR}/npc_factory.cpp
    ${SRC_DIR}/fight_visitor.cpp
    ${SRC_DIR}/dungeon.cpp
    ${SRC_DIR}/observer.cpp
    ${SRC_DIR}/utils.cpp
)

add_executable(tests ${TEST_SOURCES})

# Подключаем директории с заголовками
target_include_directories(lab7 PRIVATE ${INCLUDE_DIR})
target_include_directories(tests PRIVATE ${INCLUDE_DIR})

# Линковка библиотек
target_link_libraries(tests 
    PRIVATE 
    gtest 
    gtest_main
)

enable_testing()
add_test(NAME oop_lab7_tests COMMAND tests)