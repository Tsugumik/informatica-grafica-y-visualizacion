#ifndef __CGVINTERFACE
#define __CGVINTERFACE

#if defined(__APPLE__) && defined(__MACH__)

#include <GLUT/glut.h>

#include <OpenGL/gl.h>

#include <OpenGL/glu.h>

#else
#include <GL/glut.h>
#endif

#include "cgvScene3D.h"
#include "cgvCamera.h"

using namespace std;

class cgvInterface {
protected:
    // Attributes
    int window_width; // initial width of the display window
    int window_height; // initial height of the display window
    int pos;
    bool windowChange;

    cgvScene3D scene; // scene displayed in the window defined by igvInterface
    cgvCamera camera; // camera used to display the scene

    // Panoramic view values
    cgvPoint3D p0, r, V;

public:
    // Default constructors and destructor
    cgvInterface();
    ~cgvInterface();

    // Static methods
    // event callbacks
    static void set_glutKeyboardFunc(unsigned char key, int x, int y); //method for handling keyboard events
    static void set_glutReshapeFunc(int w, int h); // method that defines the vision camera and the viewport
    // called automatically when the window is resized
    static void set_glutDisplayFunc(); // method for visualizing the scene


    // Methods
    // Creates the world displayed in the window
    void create_world(void);

    // initializes all parameters to create a display window
    void configure_environment(int argc, char** argv, // main parameters
                               int _window_width, int _window_height, // width and height of the display window
                               int _pos_X, int _pos_Y, // initial position of the display window
                                std::string _title); // title of the display window

    void initialize_callbacks(); // initializes all callbacks

    void start_display_loop(); // display the scene and wait for events on the interface

    // get_ and set_ methods for accessing attributes
    int get_window_width() { return window_width; };
    int get_window_height() { return window_height; };

    void set_window_width(int _window_width) { window_width = _window_width; };
    void set_window_height(int _window_height) { window_height = _window_height; };

    void update_camera_view(int pos);
};

#endif
