#include <cstdlib>

#include "igvInterface.h"

#include <iostream>


// Application of the Singleton pattern
igvInterface *igvInterface::_instance = nullptr;

// Public methods ----------------------------------------

/**
 * Method for accessing the single object of the class, in application of the Singleton design pattern.
 * @return A reference to the single object of the class.
 */
igvInterface &igvInterface::getInstance() {
    if (!_instance) {
        _instance = new igvInterface;
    }

    return *_instance;
}

/**
 * Initializes all parameters to create a display window.
 * @param argc Number of command line parameters when running the application.
 *             application.
 * @param argv Command line parameters when running the application.
 * @param _window_width Initial width of the display window.
 * @param _window_height Initial height of the display window.
 * @param _pos_X X coordinate of the initial position of the display window.
 *               display window
 * @param _pos_Y Y coordinate of the initial position of the
 *               display window
 * @param _title Title of the display window
 * @pre It is assumed that all parameters have valid values
 * @post Changes the height and width of the window stored in the object
 */
void igvInterface::configure_environment(int argc, char **argv, int _window_width, int _window_height, int _pos_X,
                                         int _pos_Y, std::string _title) {
    // initialization of interface attributes
    window_width = _window_width;
    window_height = _window_height;

    // initialization of the display window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_window_width, _window_height);
    glutInitWindowPosition(_pos_X, _pos_Y);
    glutCreateWindow(_title.c_str());

    glEnable(GL_DEPTH_TEST); // activates Z-buffer face culling
    glClearColor(1.0, 1.0, 1.0, 0.0); // sets the window background color
}

/**
 * Method to display the scene and wait for events on the interface
 */
void igvInterface::start_display_loop() {
    glutMainLoop(); // starts the GLUT display loop
}

/**
 * Method for controlling keyboard events
 * @param key Code of the key pressed
 * @param x X coordinate of the mouse cursor position at the time of the
 *          keyboard event
 * @param y Y coordinate of the mouse cursor position at the time of the
 *          keyboard event
 * @pre It is assumed that all parameters have valid values
 * @post The attribute that indicates whether the axes should be drawn or not can
 *       change
 */
void igvInterface::keyboardFunc(unsigned char key, int x, int y) {
    std::cout << "Key pressed: " << key << std::endl;
    switch (key) {
        case 27: // Escape key to EXIT
            exit(1);

        // Object selection
        case '1':
            std::cout << "Selected Object 1" << std::endl;
            break;
        case '2':
            std::cout << "Selected Object 2" << std::endl;
            break;
        case '3':
            std::cout << "Selected Object 3" << std::endl;
            break;

        // Rotations
        case 'x':
            std::cout << "Rotation X+ (15 degrees)" << std::endl;
            break;
        case 'X':
            std::cout << "Rotation X- (-15 degrees)" << std::endl;
            break;
        case 'y':
            std::cout << "Rotation Y+ (15 degrees)" << std::endl;
            break;
        case 'Y':
            std::cout << "Rotation Y- (-15 degrees)" << std::endl;
            break;
        case 'z':
            std::cout << "Rotation Z+ (15 degrees)" << std::endl;
            break;
        case 'Z':
            std::cout << "Rotation Z- (-15 degrees)" << std::endl;
            break;

        // Scaling
        case 's':
            std::cout << "Scale up (+10%)" << std::endl;
            break;
        case 'S':
            std::cout << "Scale down (-10%)" << std::endl;
            break;

        // Y translation
        case 'u':
            std::cout << "Translation Y+ (+0.1)" << std::endl;
            break;
        case 'U':
            std::cout << "Translation Y- (-0.1)" << std::endl;
            break;

        // Transformation mode toggle
        case 'm':
        case 'M':
            std::cout << "Toggle transformation mode (RST <-> Sequential)" << std::endl;
            break;

        // Camera mode
        case 'c':
        case 'C':
            std::cout << "Toggle camera mode" << std::endl;
            break;

        // Camera clipping planes
        case 'f':
            std::cout << "Move front clipping plane forward" << std::endl;
            break;
        case 'F':
            std::cout << "Move front clipping plane backward" << std::endl;
            break;
        case 'b':
            std::cout << "Move back clipping plane forward" << std::endl;
            break;
        case 'B':
            std::cout << "Move back clipping plane backward" << std::endl;
            break;

        // Zoom
        case '+':
            std::cout << "Zoom in" << std::endl;
            break;
        case '-':
            std::cout << "Zoom out" << std::endl;
            break;

        // Projection toggle
        case 'p':
        case 'P':
            std::cout << "Toggle projection (Perspective <-> Parallel)" << std::endl;
            break;
    }
    glutPostRedisplay(); // refreshes the contents of the view window
}

void igvInterface::specialKeyboardFunc(int key, int x, int y) {
    std::cout << "Special key pressed: " << key << std::endl;
    switch(key)
    {
        case GLUT_KEY_LEFT:
            std::cout << "Translation X- (-0.1)" << std::endl;
            break;
        case GLUT_KEY_RIGHT:
            std::cout << "Translation X+ (+0.1)" << std::endl;
            break;
        case GLUT_KEY_UP:
            std::cout << "Translation Z+ (+0.1)" << std::endl;
            break;
        case GLUT_KEY_DOWN:
            std::cout << "Translation Z- (-0.1)" << std::endl;
            break;
    }
    glutPostRedisplay();
}

/**
 * Method that defines the camera and viewport. It is called automatically
 * when the window size is changed.
 * @param w New window width
 * @param h New window height
 * @pre All parameters are assumed to have valid values
 */
void igvInterface::reshapeFunc(int w, int h) {
    // resize the viewport to the new window width and height
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    // we save new values from the display window
    _instance->set_window_width(w);
    _instance->set_window_height(h);

    // sets the type of projection to use
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1, 1, -1, 1, -1, 200);

    // the vision camera is defined
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.5, 1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

/**
 * Method for displaying the scene
 */
void igvInterface::displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the window and the Z-buffer
    glPushMatrix(); // saves the modeling matrix

    // TODO: Section A: paint the axes

    glBegin(GL_LINES);

    // X AXIS (RED)
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    // Y AXIS (GREEN)
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);


    // Z AXIS (BLUE)
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();


    // TODO: Section B: display primitive cube in wireframe mode

    glColor3f(0.0, 0.0, 0.0);
    glutWireCube(0.5);

    // TODO: Section C
    // TODO: Section D: upper face with triangles


    glPopMatrix(); // restores the modeling matrix
    glutSwapBuffers(); // used instead of glFlush() to prevent flickering
}

/**
 * Method to initialize callbacks
 */
void igvInterface::initialize_callbacks() {
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialKeyboardFunc);
    glutReshapeFunc(reshapeFunc);
    glutDisplayFunc(displayFunc);
}

/**
 * Method to query the width of the display window
 * @return The value stored as the width of the display window
 */
int igvInterface::get_window_width() {
    return window_width;
}

/**
 * Method to query the height of the display window
 * @return The value stored as the height of the display window
 */
int igvInterface::get_window_height() {
    return window_height;
}

/**
 * Method to change the width of the display window
 * @param _window_width New value for the width of the display window
 * @pre It is assumed that the parameter has a valid value
 * @post The window width stored in the application changes to the new value
 */
void igvInterface::set_window_width(int _window_width) {
    window_width = _window_width;
}

/**
 * Method to change the height of the display window
 * @param _window_height New value for the height of the display window
 * @pre It is assumed that the parameter has a valid value
 * @post The window height stored in the application changes to the new value
 */
void igvInterface::set_window_height(int _window_height) {
    window_height = _window_height;
}
