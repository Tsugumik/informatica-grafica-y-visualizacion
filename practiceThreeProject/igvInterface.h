#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string>
#include <vector>
#include "src/Camera.h"
#include "src/cgvTriangleMesh.h"
#include "ArticulatedModel.h"
#include "src/Quad.h"
#include "src/cgvLightSource.h"
#include "src/cgvMaterial.h"
#include "src/cgvTexture.h"

class igvInterface {
private:
    cgvTriangleMesh* triangleMesh;
    ArticulatedModel* articulatedModel;
    Quad* floor;

    Object3D* selectedObject;
    int currentObject;

    Camera* camera;
    bool cameraMode;

    bool flatShading;
    bool articulatedInteractionKeyboard;
    bool animateCamera;
    bool animateModel;

    // Practice 3 additions
    std::vector<cgvMaterial> floorMaterials;
    int currentFloorMaterial;

    std::vector<cgvTexture> textures;
    int currentTexture;
    bool texturingEnabled;

    cgvLightSource lights[4];
    int selectedLight;
    bool globalAmbientOn;

    int window_width = 0;
    int window_height = 0;

    static igvInterface* _instance;

    void process_selection();
    void initScene(); // New private method

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
    void setFloorMaterial(int materialIndex);
    void toggleTexturing();
    void setTexture(int textureIndex);
    void setTextureFilter(int filterMode);
    void toggleLight(int lightIndex);
    void selectLight(int lightIndex);


    int get_window_width();
    int get_window_height();
    void set_window_width(int _window_width);
    void set_window_height(int _window_height);
};

#endif
