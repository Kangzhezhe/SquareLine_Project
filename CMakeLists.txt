cmake_minimum_required(VERSION 3.10)

project(SquareLine_Project C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set( CMAKE_C_FLAGS "-Wall -Wextra -Wno-unused -g" )
set( CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused -g" )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_package(SDL2 REQUIRED SDL2)
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}/../
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/lvgl
    ${PROJECT_SOURCE_DIR}/ui
)

# FILE(GLOB_RECURSE LVGL_Sources CONFIGURE_DEPENDS lvgl/*.c)
# FILE(GLOB_RECURSE UI_Sources CONFIGURE_DEPENDS ui/*.c ui/*.cpp)
# add_executable(${PROJECT_NAME} src/main.c ${LVGL_Sources} ${UI_Sources} src/my_ui.c)
# # string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
# target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

# -----------------------------mqtt---------------------
# set(CMAKE_C_STANDARD 99)
# include_directories(${PROJECT_SOURCE_DIR}/lib/cubeai_lib/include)
# include_directories(${PROJECT_SOURCE_DIR}/model)
# file(GLOB MODEL_SOURCES "model/*.c")
# file(GLOB LIB_SOURCES "./lib/cubeai_lib/static/*.a")

# add_executable(mqtt src/mqtt.cpp src/main.cpp src/utils.cpp ${MODEL_SOURCES})
# target_link_libraries(mqtt PRIVATE ${LIB_SOURCES} m) # Link Math library (libm)
# target_include_directories(mqtt PRIVATE "C:/Program Files/MySQL/MySQL Server 8.0/include")
# target_link_directories(mqtt PRIVATE "C:/Program Files/MySQL/MySQL Server 8.0/lib")
# target_link_directories(mqtt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
# target_link_libraries(mqtt PRIVATE mysql wsock32 ws2_32)
# target_link_libraries(mqtt PRIVATE tensorflowlite_c.dll.if)
# target_include_directories(mqtt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
# set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv)
# FIND_PACKAGE(OpenCV REQUIRED)
# TARGET_LINK_LIBRARIES(mqtt PRIVATE ${OpenCV_LIBS})
# target_include_directories(mqtt PRIVATE ${OpenCV_INCLUDE_DIRS})
# set(BOOST_ROOT "C:/msys64/mingw64")
# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_NO_SYSTEM_PATHS ON) 
# find_package(Boost REQUIRED COMPONENTS filesystem)
# include_directories(${Boost_INCLUDE_DIRS})
# target_link_directories(mqtt PRIVATE ${Boost_LIBRARY_DIRS})
# target_link_libraries(mqtt PRIVATE ${Boost_LIBRARIES})



#-----------------------------test---------------------
# add_executable(test src/test.cpp)
# target_link_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
# target_link_libraries(test PRIVATE tensorflow tensorflow-lite)
# target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
# set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv)
# FIND_PACKAGE(OpenCV REQUIRED)
# TARGET_LINK_LIBRARIES(test PRIVATE ${OpenCV_LIBS})
# target_include_directories(test PRIVATE ${OpenCV_INCLUDE_DIRS})

# # set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}:C:/msys64/mingw64/lib/cmake)
# set(BOOST_ROOT "C:/msys64/mingw64")
# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_NO_SYSTEM_PATHS ON) 
# find_package(Boost REQUIRED COMPONENTS filesystem)
# include_directories(${Boost_INCLUDE_DIRS})
# target_link_directories(test PRIVATE ${Boost_LIBRARY_DIRS})
# target_link_libraries(test PRIVATE ${Boost_LIBRARIES})
# 输出 Boost 的相关信息
# message("Boost version: ${Boost_VERSION}")
# message("Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
# message("Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
# message("Boost_LIBRARIES: ${Boost_LIBRARIES}")

# ----------------------------------------TestTFLite-------------------------
# add_executable(TestTFLite src/TestTFLite.cpp)
# target_link_directories(TestTFLite PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
# target_link_libraries(TestTFLite PRIVATE tensorflowlite_c.dll.if)
# target_include_directories(TestTFLite PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)

# --------------------------------facenet----------------------------------
# add_executable(facenet src/facenet.cpp)
# target_link_directories(facenet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
# target_link_libraries(facenet PRIVATE tensorflowlite_c.dll.if)
# target_include_directories(facenet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
# set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv)
# FIND_PACKAGE(OpenCV REQUIRED)
# TARGET_LINK_LIBRARIES(facenet PRIVATE ${OpenCV_LIBS})
# target_include_directories(facenet PRIVATE ${OpenCV_INCLUDE_DIRS})

# ------------------------------main--------------------------------
# set(CMAKE_C_STANDARD 99)
# include_directories(${PROJECT_SOURCE_DIR}/lib/cubeai_lib/include)
# include_directories(${PROJECT_SOURCE_DIR}/model)
# file(GLOB MODEL_SOURCES "model/*.c")
# file(GLOB LIB_SOURCES "./lib/cubeai_lib/static/*.a")
# add_executable(main src/main.cpp src/utils.cpp ${MODEL_SOURCES})
# target_link_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
# target_link_libraries(main PRIVATE tensorflowlite_c.dll.if)
# target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
# set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv)
# FIND_PACKAGE(OpenCV REQUIRED)
# TARGET_LINK_LIBRARIES(main PRIVATE ${OpenCV_LIBS})
# target_include_directories(main PRIVATE ${OpenCV_INCLUDE_DIRS})
# set(BOOST_ROOT "C:/msys64/mingw64")
# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_NO_SYSTEM_PATHS ON) 
# find_package(Boost REQUIRED COMPONENTS filesystem)
# include_directories(${Boost_INCLUDE_DIRS})
# target_link_directories(main PRIVATE ${Boost_LIBRARY_DIRS})
# target_link_libraries(main PRIVATE ${Boost_LIBRARIES}  ${LIB_SOURCES} m)



# ---------------------------cubeai----------------------------
# set(CMAKE_C_STANDARD 99)
# include_directories(${PROJECT_SOURCE_DIR}/lib/cubeai_lib/include)
# include_directories(${PROJECT_SOURCE_DIR}/model)
# file(GLOB MODEL_SOURCES "model/*.c")
# file(GLOB LIB_SOURCES "./lib/cubeai_lib/static/*.a")
# add_executable(cubeai  src/cubeai.cpp ${MODEL_SOURCES})
# target_link_libraries(cubeai PRIVATE ${LIB_SOURCES} m) # Link Math library (libm)
# set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv)
# FIND_PACKAGE(OpenCV REQUIRED)
# TARGET_LINK_LIBRARIES(cubeai PRIVATE ${OpenCV_LIBS})
# target_include_directories(cubeai PRIVATE ${OpenCV_INCLUDE_DIRS})

# ---------------------------utils------------------------------------
# add_executable(utils src/utils.cpp)
# set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv)
# FIND_PACKAGE(OpenCV REQUIRED)
# TARGET_LINK_LIBRARIES(utils PRIVATE ${OpenCV_LIBS})
# target_include_directories(utils PRIVATE ${OpenCV_INCLUDE_DIRS})

# --------------------------qt5---------------------------------


# set(CMAKE_C_COMPILER C:/Qt/Qt5.14.0/Tools/mingw730_64/bin/gcc.exe)
# set(CMAKE_CXX_COMPILER C:/Qt/Qt5.14.0/Tools/mingw730_64/bin/g++.exe)

set(CMAKE_C_STANDARD 99)
include_directories(${PROJECT_SOURCE_DIR}/lib/cubeai_lib/include)
include_directories(${PROJECT_SOURCE_DIR}/model)
file(GLOB MODEL_SOURCES "model/*.c")
file(GLOB LIB_SOURCES "./lib/cubeai_lib/static/*.a")

set(CMAKE_PREFIX_PATH C:/Qt/Qt5.14.0/5.14.0/mingw73_64/lib/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Widgets LinguistTools Sql GUI REQUIRED)
add_executable(untitled
    src/qt5.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    inc/qt5.hpp
    src/mqtt.cpp src/main.cpp src/utils.cpp ${MODEL_SOURCES}
    src/miniprogram.cpp
)
target_link_libraries(untitled PRIVATE Qt5::Widgets Qt5::Sql)
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} )


target_link_libraries(untitled PRIVATE ${LIB_SOURCES} m) # Link Math library (libm)
target_include_directories(untitled PRIVATE "C:/Program Files/MySQL/MySQL Server 8.0/include")
target_link_directories(untitled PRIVATE "C:/Program Files/MySQL/MySQL Server 8.0/lib")
target_link_directories(untitled PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_include_directories(untitled PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(untitled PRIVATE mysql wsock32 ws2_32)
target_link_libraries(untitled PRIVATE tensorflowlite_c.dll.if)
target_include_directories(untitled PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv)
FIND_PACKAGE(OpenCV REQUIRED)
TARGET_LINK_LIBRARIES(untitled PRIVATE ${OpenCV_LIBS})
target_include_directories(untitled PRIVATE ${OpenCV_INCLUDE_DIRS})
set(BOOST_ROOT "C:/msys64/mingw64")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_SYSTEM_PATHS ON) 
find_package(Boost REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})
target_link_directories(untitled PRIVATE ${Boost_LIBRARY_DIRS})
target_link_libraries(untitled PRIVATE ${Boost_LIBRARIES})
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/crow/lib/cmake/Crow")
set(MYSQL_CONCPP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lib/mysql_connector")
set(MYSQL_CONCPP_LIBRARIES
    ${MYSQL_CONCPP_ROOT}/lib64/debug/libmysqlcppconn-9.dll
    ${MYSQL_CONCPP_ROOT}/lib64/debug/libmysqlcppconn8-2.dll
)
target_link_libraries(untitled PRIVATE ${MYSQL_CONCPP_LIBRARIES})
target_include_directories(untitled PRIVATE  ${MYSQL_CONCPP_ROOT}/include/jdbc)
find_package(Crow REQUIRED)
target_link_libraries(untitled PRIVATE Crow::Crow wsock32 ws2_32)



#---------------------------miniprogram----------------
# set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/crow/lib/cmake/Crow")
# set(MYSQL_CONCPP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lib/mysql_connector")
# set(MYSQL_CONCPP_LIBRARIES
#     ${MYSQL_CONCPP_ROOT}/lib64/debug/libmysqlcppconn-9.dll
#     ${MYSQL_CONCPP_ROOT}/lib64/debug/libmysqlcppconn8-2.dll
# )
# add_executable (miniprogram src/miniprogram.cpp)
# target_link_libraries(miniprogram PRIVATE ${MYSQL_CONCPP_LIBRARIES})
# target_include_directories(miniprogram PRIVATE  ${MYSQL_CONCPP_ROOT}/include/jdbc)
# find_package(Crow REQUIRED)
# target_link_libraries(miniprogram PRIVATE Crow::Crow wsock32 ws2_32)
# set(BOOST_ROOT "C:/msys64/mingw64")
# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_NO_SYSTEM_PATHS ON) 
# find_package(Boost REQUIRED COMPONENTS filesystem)
# include_directories(${Boost_INCLUDE_DIRS})
# target_link_directories(miniprogram PRIVATE ${Boost_LIBRARY_DIRS})
# target_link_libraries(miniprogram PRIVATE ${Boost_LIBRARIES})
