cmake_minimum_required(VERSION 3.25)
project(pr1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(${PROJECT_NAME}
        igvInterface.cpp
        igvInterface.h
        pr1.cpp
        src/Object3D.cpp
        src/Object3D.h
        src/Camera.cpp
        src/Camera.h
        src/cgvTriangleMesh.cpp
        src/cgvTriangleMesh.h
        ArticulatedModel.cpp
        ArticulatedModel.h
        src/AdvancedOBJLoader.cpp
        src/AdvancedOBJLoader.h
        src/cgvPoint3D.cpp
        src/cgvPoint3D.h
        )

if (APPLE)
    target_link_libraries(pr1 PRIVATE "-framework OpenGL" "-framework GLUT")
    target_compile_definitions(pr1 PRIVATE GL_SILENCE_DEPRECATION)
else ()
    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)
    target_link_libraries(pr1 PRIVATE ${OPENGL_LIBRARIES} GLUT::GLUT)
endif ()
