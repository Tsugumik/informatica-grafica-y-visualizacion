#include <cstdlib>
#include <stdio.h>
#include "iostream"
#include "cgvInterface.h"

 cgvInterface interface; // Callbacks must be static and this object is required to access from

// Constructor methods -----------------------------------

cgvInterface::cgvInterface() :pos(1), windowChange(false) {}

cgvInterface::~cgvInterface() {}

// Public methods ----------------------------------------

void cgvInterface::create_world(void) {
    // crear c·maras
    p0 = cgvPoint3D(3.0, 2.0, 4);
    r = cgvPoint3D(0, 0, 0);
    V = cgvPoint3D(0, 1.0, 0);

    interface.camera.set(CGV_PARALLEL, p0, r, V,
                         -1 * 3, 1 * 3, -1 * 3, 1 * 3, 1, 200);

    //perspective parameters
    interface.camera.angle = 60.0;
    interface.camera.aspect = 1.0;
}

void cgvInterface::configure_environment(int argc, char** argv,
                                         int _window_width, int _window_height,
                                         int _pos_X, int _pos_Y,
                                         std::string _title) {
    // initialize interface variables
    window_width = _window_width;
    window_height = _window_height;

    // initialization of the display window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_window_width, _window_height);
    glutInitWindowPosition(_pos_X, _pos_Y);
    glutCreateWindow(_title.c_str());

    glEnable(GL_DEPTH_TEST); // Enables z-buffering of surfaces
    glClearColor(1.0, 1.0, 1.0, 0.0); // Sets the window background color

    glEnable(GL_LIGHTING); // Enables scene lighting
    glEnable(GL_NORMALIZE); // Normalizes the normal vectors for lighting calculations

    create_world(); // Creates the world to be displayed in the window
}

void cgvInterface::start_display_loop() {
    glutMainLoop(); // start the OpenGL display loop
}

void cgvInterface::set_glutKeyboardFunc(unsigned char key, int x, int y) {

    /* IMPORTANT: When implementing this method, you must appropriately change the state of the application objects, but do not make direct calls to OpenGL functions */

    switch (key) {
        case 'p': // change the projection type from parallel to perspective and vice versa
            if (interface.camera.type == CGV_PARALLEL) { // Perspective mode
                    interface.camera.set(CGV_PERSPECTIVE,
                    interface.camera.P0,
                    interface.camera.r,
                    interface.camera.V,
                    interface.camera.angle,
                    interface.camera.aspect,
                    interface.camera.znear,
                    interface.camera.zfar
                );
            }
            else {
                interface.camera.set(CGV_PARALLEL,
                                    interface.camera.P0,
                                    interface.camera.r,
                                    interface.camera.V,
                                    interface.camera.xwmin,
                                    interface.camera.xwmax,
                                    interface.camera.ywmin,
                                    interface.camera.ywmax,
                                    interface.camera.znear,
                                    interface.camera.zfar
                );
            }
            interface.camera.apply();
            break;
        case 'P': // Change the projection type from parallel to perspective and vice versa
            if (interface.camera.type == CGV_PARALLEL) {
                //Perspective mode
                interface.camera.set(CGV_PERSPECTIVE,
                                    interface.camera.P0,
                                    interface.camera.r,
                                    interface.camera.V,
                                    interface.camera.angle,
                                    interface.camera.aspect,
                                    interface.camera.znear,
                                    interface.camera.zfar
                );
            }
            else {
                interface.camera.set(CGV_PARALLEL,
                                    interface.camera.P0,
                                    interface.camera.r,
                                    interface.camera.V,
                                    interface.camera.xwmin,
                                    interface.camera.xwmax,
                                    interface.camera.ywmin,
                                    interface.camera.ywmax,
                                    interface.camera.znear,
                                    interface.camera.zfar
                );
            }
            interface.camera.apply();
            break;
        case 'v': // Change the camera position to display plan, profile, elevation, or perspective views
            interface.update_camera_view(++interface.pos % 4);
            break;
        case 'V': // Change the camera position to display plan, profile, elevation, or perspective views
            interface.update_camera_view(++interface.pos % 4);
            break;
        case '+': // zoom in
            interface.camera.zoom(0.95);
            interface.camera.apply();
            break;
        case '-': // zoom out
            interface.camera.zoom(1.05);
            interface.camera.apply();
            break;
        case 'n': // increase the distance of the near plane
            interface.camera.znear += 0.2;
            interface.camera.apply();
            break;
        case 'N': // decrease the distance of the near plane
            interface.camera.znear -= 0.2;
            interface.camera.apply();
            break;
        case '4': // split the window into four views
            interface.windowChange = !interface.windowChange;
            interface.update_camera_view(0);
            break;
        case 'e': // activate/deactivate the display of the axes
            interface.scene.set_ejes(interface.scene.get_ejes() ? false : true);
            break;
        case 27: // escape key to EXIT
            exit(1);
    }
    glutPostRedisplay(); // refreshes the contents of the viewport and redraws the scene
}

void cgvInterface::set_glutReshapeFunc(int w, int h) {
    // Size the viewport to the new window width and height
    // Save the new viewport values
    interface.set_window_width(w);
    interface.set_window_height(h);

    // Set the camera and projection parameters
    interface.camera.apply();
}

void cgvInterface::set_glutDisplayFunc(){ // clear the window and the z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the viewport
    if (!interface.windowChange) {
        glViewport(0, 0, interface.get_window_width(), interface.get_window_height());
        // display the scene
        interface.scene.display();
    }
    else {
        glViewport(0, interface.get_window_height() / 2, interface.get_window_width() / 2, interface.get_window_height() / 2);
        interface.update_camera_view(0);
        interface.scene.display();
        glViewport(interface.get_window_width() / 2, interface.get_window_height() / 2, interface.get_window_width() / 2,interface.get_window_height() / 2);
        interface.update_camera_view(1);
        interface.scene.display();
        glViewport(0, 0, interface.get_window_width() / 2, interface.get_window_height() / 2);
        interface.update_camera_view(2);
        interface.scene.display();
        glViewport(interface.get_window_width() / 2, 0, interface.get_window_width() / 2, interface.get_window_height() / 2);
        interface.update_camera_view(3);
        interface.scene.display();
    }
    // refresh the window
    glutSwapBuffers(); // used instead of glFlush() to avoid flickering
}

void cgvInterface::initialize_callbacks()  {
    glutKeyboardFunc(set_glutKeyboardFunc);
    glutReshapeFunc(set_glutReshapeFunc);
    glutDisplayFunc(set_glutDisplayFunc);
}

void cgvInterface::update_camera_view(int pos) {
    switch (pos + 1)
    {
        case 1:
            interface.camera.set(p0, r, V); //Basic
            break;
        case 2:
            interface.camera.set(cgvPoint3D(0, 5, 0), cgvPoint3D(0, 0, 0), cgvPoint3D(1, 0, 0)); //Floor
            break;
        case 3:
            interface.camera.set(cgvPoint3D(5, 0, 0), cgvPoint3D(0, 0, 0), cgvPoint3D(0, 1, 0)); //Front view
            break;
        case 4:
            interface.camera.set(cgvPoint3D(0, 0, 5), cgvPoint3D(0, 0, 0), cgvPoint3D(0, 1, 0)); //Profile
            break;
    }

    interface.camera.apply();
}