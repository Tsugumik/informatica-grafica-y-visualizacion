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

igvInterface::igvInterface() {
    cube = new Cube();
    sphere = new Sphere();
    cone = new Cone();
    selectedObject = nullptr;
    currentObject = 0;
    transformationMode = true;

    camera = new Camera();
    cameraMode = false;
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

    // ADD LIGHTING:
    glEnable(GL_LIGHTING); // Enable lighting
    glEnable(GL_LIGHT0); // Enable light source 0

    // Set light position
    GLfloat light_position[] = {2.0f, 2.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Set light colors
    GLfloat white_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient_light[] = {0.3f, 0.3f, 0.3f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

    // Enable color material (so glColor3f works with lighting)
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
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

    igvInterface *instance = &getInstance();

    switch (key) {
        case 27: exit(1);

        case 'c':
        case 'C':
            instance->cameraMode = !instance->cameraMode;
            std::cout << "Camera mode: " << (instance->cameraMode ? "ON" : "OFF") << std::endl;
            break;

        case 'y':
            if (instance->cameraMode) instance->camera->yaw(5.0f);
            else if (instance->selectedObject) instance->selectedObject->rotate(0.0f, 15.0f, 0.0f);
            break;
        case 'Y':
            if (instance->cameraMode) instance->camera->yaw(-5.0f);
            else if (instance->selectedObject) instance->selectedObject->rotate(0.0f, -15.0f, 0.0f);
            break;

        // Zoom
        case '+':
            instance->camera->zoom(-1.0f);  // Zoom in
            break;
        case '-':
            instance->camera->zoom(1.0f);   // Zoom out
            break;

        // Clipping planes
        case 'f':
            instance->camera->moveNearPlane(-0.1f);
            break;
        case 'F':
            instance->camera->moveNearPlane(0.1f);
            break;
        case 'b':
            instance->camera->moveFarPlane(-1.0f);
            break;
        case 'B':
            instance->camera->moveFarPlane(1.0f);
            break;

            // Projection toggle
        case 'p':
        case 'P':
            instance->camera->toggleProjection();
            break;

        // Object selection
        case '1':
            instance->selectObject(1);
            break;
        case '2':
            instance->selectObject(2);
            break;
        case '3':
            instance->selectObject(3);
            break;

        // Transformations - only if object is selected
        case 'x':
            if (instance->selectedObject) instance->selectedObject->rotate(15.0f, 0.0f, 0.0f);
            break;
        case 'X':
            if (instance->selectedObject) instance->selectedObject->rotate(-15.0f, 0.0f, 0.0f);
            break;
        case 'z':
            if (instance->selectedObject) instance->selectedObject->rotate(0.0f, 0.0f, 15.0f);
            break;
        case 'Z':
            if (instance->selectedObject) instance->selectedObject->rotate(0.0f, 0.0f, -15.0f);
            break;
        case 's':
            if (instance->selectedObject) instance->selectedObject->scale(1.1f, 1.1f, 1.1f);
            break;
        case 'S':
            if (instance->selectedObject) instance->selectedObject->scale(0.9f, 0.9f, 0.9f);
            break;
        case 'm':
        case 'M':
            instance->transformationMode = !instance->transformationMode;

            instance->cube->setRSTMode(instance->transformationMode);
            instance->sphere->setRSTMode(instance->transformationMode);
            instance->cone->setRSTMode(instance->transformationMode);

            if (!instance->transformationMode) {
                instance->cube->clearTransformationHistory();
                instance->sphere->clearTransformationHistory();
                instance->cone->clearTransformationHistory();
            }

            std::cout << "Transformation mode: " << (instance->transformationMode ? "RST" : "Sequential") << std::endl;

            break;
        case 'r':
        case 'R':
            if (instance->selectedObject) {
                instance->selectedObject->resetTransformations();
                instance->selectedObject->clearTransformationHistory();
                std::cout << "Object transformations reset" << std::endl;
            }
            break;
    }
    glutPostRedisplay(); // refreshes the contents of the view window
}

void igvInterface::specialKeyboardFunc(int key, int x, int y) {
    igvInterface* instance = &getInstance();

    if (instance->cameraMode) {
        // Camera controls
        switch (key) {
            case GLUT_KEY_LEFT:
                instance->camera->orbit(-5.0f, 0.0f);
                break;
            case GLUT_KEY_RIGHT:
                instance->camera->orbit(5.0f, 0.0f);
                break;
            case GLUT_KEY_UP:
                instance->camera->orbit(0.0f, 5.0f);
                break;
            case GLUT_KEY_DOWN:
                instance->camera->orbit(0.0f, -5.0f);
                break;
        }
    } else {
        // Object transformations
        switch (key) {
            case GLUT_KEY_LEFT:
                if (instance->selectedObject) instance->selectedObject->translate(-0.1f, 0.0f, 0.0f);
                break;
            case GLUT_KEY_RIGHT:
                if (instance->selectedObject) instance->selectedObject->translate(0.1f, 0.0f, 0.0f);
                break;
            case GLUT_KEY_UP:
                if (instance->selectedObject) instance->selectedObject->translate(0.0f, 0.0f, 0.1f);
                break;
            case GLUT_KEY_DOWN:
                if (instance->selectedObject) instance->selectedObject->translate(0.0f, 0.0f, -0.1f);
                break;
        }
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
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    _instance->set_window_width(w);
    _instance->set_window_height(h);

    // Use camera for projection
    _instance->camera->setAspectRatio((float)w / (float)h);
    _instance->camera->applyProjection();
    _instance->camera->applyView();
}


/**
 * Method for displaying the scene
 */
void igvInterface::displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply camera transformations
    igvInterface* instance = &getInstance();
    instance->camera->applyProjection();
    instance->camera->applyView();

    glPushMatrix();

    // Draw axes
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-8.0f, 0.0f, 0.0f); glVertex3f(8.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -8.0f, 0.0f); glVertex3f(0.0f, 8.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -8.0f); glVertex3f(0.0f, 0.0f, 8.0f);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);

    // Draw only selected object
    if (instance->selectedObject) {
        instance->selectedObject->draw();
    }

    glPopMatrix();
    glutSwapBuffers();
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

void igvInterface::selectObject(int objectNum) {
    // Deselect all
    cube->setSelected(false);
    sphere->setSelected(false);
    cone->setSelected(false);

    // Select requested object
    switch (objectNum) {
        case 1:
            selectedObject = cube;
            cube->setSelected(true);
            cube->setRSTMode(transformationMode);
            currentObject = 1;
            std::cout << "Selected cube" << std::endl;
            break;
        case 2:
            selectedObject = sphere;
            sphere->setSelected(true);
            sphere->setRSTMode(transformationMode);
            currentObject = 2;
            std::cout << "Selected sphere" << std::endl;
            break;
        case 3:
            selectedObject = cone;
            cone->setSelected(true);
            cone->setRSTMode(transformationMode);
            currentObject = 3;
            std::cout << "Selected cone" << std::endl;
            break;
    }
}
