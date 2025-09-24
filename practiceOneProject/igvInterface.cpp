#include <cstdlib>

#include "igvInterface.h"


// Application of the Singleton pattern
igvInterface* igvInterface::_instance = nullptr;

// Public methods ----------------------------------------

/**
 * Method for accessing the single object of the class, in application of the Singleton design pattern.
 * @return A reference to the single object of the class.
 */
igvInterface& igvInterface::getInstance()
{  if ( !_instance )
   {  _instance = new igvInterface;
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
                                         int _pos_Y, std::string _title)
{
    // initialization of interface attributes
    window_width = _window_width;
    window_height = _window_height;

    // initialization of the display window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_window_width,_window_height);
    glutInitWindowPosition(_pos_X,_pos_Y);
    glutCreateWindow(_title.c_str());

    glEnable(GL_DEPTH_TEST); // activates Z-buffer face culling
    glClearColor(1.0,1.0,1.0,0.0); // sets the window background color
}

/**
 * Method to display the scene and wait for events on the interface
 */
void igvInterface::start_display_loop()
{
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
void igvInterface::keyboardFunc(unsigned char key, int x, int y)
{  switch (key)
   {
    case 27: // Escape key to EXIT
         exit(1);
         break;
   }
   glutPostRedisplay(); // refreshes the contents of the view window
}

/**
 * Method that defines the camera and viewport. It is called automatically
 * when the window size is changed.
 * @param w New window width
 * @param h New window height
 * @pre All parameters are assumed to have valid values
 */
void igvInterface::reshapeFunc(int w, int h)
{
    // resize the viewport to the new window width and height
   glViewport(0,0,(GLsizei) w,(GLsizei) h);

   // we save new values from the display window
   _instance->set_window_width( w );
   _instance->set_window_height( h );

   // sets the type of projection to use
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glOrtho(-1,1,-1,1,-1,200);

   // the vision camera is defined
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(1.5,1.0,2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

/**
 * Method for displaying the scene
 */
void igvInterface::displayFunc()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the window and the Z-buffer
    glPushMatrix(); // saves the modeling matrix

    // TODO: Section A: paint the axes

    // TODO: Section B: display primitive cube in wireframe mode

    // TODO: Section C
    // TODO: Section D: upper face with triangles


    glPopMatrix(); // restores the modeling matrix
    glutSwapBuffers(); // used instead of glFlush() to prevent flickering
}

/**
 * Method to initialize callbacks
 */
void igvInterface::initialize_callbacks()
{  glutKeyboardFunc(keyboardFunc);
   glutReshapeFunc(reshapeFunc);
   glutDisplayFunc(displayFunc);
}

/**
 * Method to query the width of the display window
 * @return The value stored as the width of the display window
 */
int igvInterface::get_window_width()
{  return window_width;
}

/**
 * Method to query the height of the display window
 * @return The value stored as the height of the display window
 */
int igvInterface::get_window_height()
{  return window_height;
}

/**
 * Method to change the width of the display window
 * @param _window_width New value for the width of the display window
 * @pre It is assumed that the parameter has a valid value
 * @post The window width stored in the application changes to the new value
 */
void igvInterface::set_window_width(int _window_width)
{  window_width = _window_width;
}

/**
 * Method to change the height of the display window
 * @param _window_height New value for the height of the display window
 * @pre It is assumed that the parameter has a valid value
 * @post The window height stored in the application changes to the new value
 */
void igvInterface::set_window_height(int _window_height)
{
    window_height = _window_height;
}
