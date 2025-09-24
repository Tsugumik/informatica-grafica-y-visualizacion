cmake_minimum_required(VERSION 3.25)
project(pr2b)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(${PROJECT_NAME}
        src/cgvCamera.cpp
        src/cgvCamera.h
        src/cgvScene3D.cpp
        src/cgvScene3D.h
        src/cgvInterface.cpp
        src/cgvInterface.h
        src/cgvPoint.cpp
        src/cgvPoint.h
        src/pr2b.cpp)

if (LINUX)
    find_path(OPENGL_REGISTRY_INCLUDE_DIRS "GL/glcorearb.h")
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_REGISTRY_INCLUDE_DIRS})

    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

    find_package(GLUT REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE GLUT::GLUT)
endif ()

if (WIN32)
    find_package(opengl_system)
    target_link_libraries(${PROJECT_NAME} opengl::opengl)

    find_package(opengl-registry)
    target_link_libraries(${PROJECT_NAME} opengl-registry::opengl-registry)

    find_package(FreeGLUT)
    target_link_libraries(${PROJECT_NAME} FreeGLUT::freeglut_static)
endif ()


if(APPLE)
    # Enlazar frameworks de macOS
    target_link_libraries(pr2b PRIVATE "-framework OpenGL" "-framework GLUT")
    # Silencia warnings por deprecación (opcional)
    target_compile_definitions(pr2b PRIVATE GL_SILENCE_DEPRECATION)
else()
    target_link_libraries(pr2b PRIVATE OpenGL::GL)
endif()