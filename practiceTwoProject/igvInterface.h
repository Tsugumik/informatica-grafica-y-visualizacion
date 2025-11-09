#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string>
#include "src/Camera.h"
#include "src/cgvTriangleMesh.h"
#include "ArticulatedModel.h"

class igvInterface {
private:
    cgvTriangleMesh* triangleMesh;
    ArticulatedModel* articulatedModel;

    Object3D* selectedObject;
    int currentObject;

    Camera* camera;
    bool cameraMode;

    bool flatShading;
    bool articulatedInteractionKeyboard;
    bool animateCamera;
    bool animateModel;

    int window_width = 0;
    int window_height = 0;

    static igvInterface* _instance;

    void process_selection(); // Corrected declaration

public:
    static igvInterface& getInstance();
    igvInterface();
    ~igvInterface();

    static void keyboardFunc(unsigned char key, int x, int y);
    static void specialKeyboardFunc(int key, int x, int y);
    static void reshapeFunc(int w, int h);
    static void displayFunc();
    static void mouseFunc(int button, int state, int x, int y);
    static void motionFunc(int x, int y);
    static void idleFunc();

    void configure_environment(int argc, char** argv, int _window_width, int _window_height, int _pos_X, int _pos_Y, std::string _title);
    void initialize_callbacks();
    void start_display_loop();
    void create_menus();

    void selectObject(int objectNum);

    // Public setters for menu callbacks
    void setShading(bool flat);
    void setInteraction(bool keyboard);
    void toggleAnimateModel();
    void toggleAnimateCamera();

    int get_window_width();
    int get_window_height();
    void set_window_width(int _window_width);
    void set_window_height(int _window_height);
};

#endif
