#include "igvInterface.h"
#include "src/AdvancedOBJLoader.h"
#include <iostream>
#include <cmath>
#include <vector>

// ... (reszta bez zmian do displayFunc)
igvInterface* igvInterface::_instance = nullptr;

// Static callbacks
void menu_callback(int option);
void shading_menu_callback(int option);
void interaction_menu_callback(int option);
void animation_menu_callback(int option);
void floor_material_menu_callback(int option);
void texture_menu_callback(int option);
void texture_filter_menu_callback(int option);
void light_toggle_menu_callback(int option);
void light_select_menu_callback(int option);

// Mouse and selection state
static int last_mouse_y;
static int selected_dof_by_mouse = -1;
static bool selection_requested = false;
static int selection_x, selection_y;

igvInterface& igvInterface::getInstance() {
    if (!_instance) _instance = new igvInterface;
    return *_instance;
}

igvInterface::igvInterface() {
    // NON-OpenGL initializations
    camera = new Camera();
    triangleMesh = new cgvTriangleMesh();
    articulatedModel = new ArticulatedModel();
    floor = new Quad(20, 20);

    if (!AdvancedOBJLoader::load("objFiles/cow.obj", *triangleMesh)) {
        std::cerr << "Failed to load cow mesh." << std::endl;
    }
    triangleMesh->translate(-5, 0, 0);
    articulatedModel->translate(5, 0, 0);

    selectedObject = nullptr;
    currentObject = 0;
    cameraMode = false;
    flatShading = false;
    articulatedInteractionKeyboard = true;
    animateCamera = false;
    animateModel = false;
    texturingEnabled = true;
    currentTexture = 0;
    selectedLight = 1;
    globalAmbientOn = true;
}

void igvInterface::initScene() {
    // OpenGL-dependent initializations
    // A. Floor and Materials
    floorMaterials.emplace_back(cgvColor(0.1f, 0.1f, 0.1f), cgvColor(0.7f, 0.7f, 0.7f), cgvColor(0.1f, 0.1f, 0.1f), 10.0f); // Matte
    floorMaterials.emplace_back(cgvColor(0.1f, 0.1f, 0.1f), cgvColor(0.5f, 0.5f, 0.5f), cgvColor(0.9f, 0.9f, 0.9f), 100.0f); // Plastic
    floorMaterials.emplace_back(cgvColor(0.2f, 0.2f, 0.2f), cgvColor(0.8f, 0.8f, 0.8f), cgvColor(1.0f, 1.0f, 1.0f), 128.0f); // Metallic
    currentFloorMaterial = 0;
    floor->set_material(&floorMaterials[currentFloorMaterial]);

    // B. Texturing
    try {
        textures.emplace_back("textures/grid.png");
        textures.emplace_back("textures/rock.png");
        textures.emplace_back("textures/grass.png");
    } catch (const std::runtime_error& e) {
        std::cerr << "Error loading textures: " << e.what() << std::endl;
    }
    if (!textures.empty()) {
        floor->set_texture(&textures[currentTexture]);
    }


    // C. Lighting
    GLfloat ambient_light[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);

    lights[1] = cgvLightSource(GL_LIGHT1, cgvPoint3D(0, 5, 5), cgvColor(0.1f, 0.1f, 0.1f), cgvColor(0.8f, 0.8f, 0.8f), cgvColor(1, 1, 1), 1, 0.05f, 0);
    lights[2] = cgvLightSource(GL_LIGHT2, cgvPoint3D(-1, -1, -1, 0.0f), cgvColor(0, 0, 0), cgvColor(0.5f, 0.5f, 0.5f), cgvColor(0.5f, 0.5f, 0.5f), 0, 0, 0);
    lights[3] = cgvLightSource(GL_LIGHT3, cgvPoint3D(0, 8, 0), cgvColor(0, 0, 0), cgvColor(0.9f, 0.9f, 0.9f), cgvColor(1, 1, 1), 1, 0.05f, 0, cgvPoint3D(0, -1, 0, 0.0f), 25.0f, 5.0f);

    lights[1].turnon();
    lights[2].turnon();
    lights[3].turnon();
}


igvInterface::~igvInterface() {
    delete camera;
    delete triangleMesh;
    delete articulatedModel;
    delete floor;
}

void igvInterface::configure_environment(int argc, char** argv, int _window_width, int _window_height, int _pos_X, int _pos_Y, std::string _title) {
    window_width = _window_width;
    window_height = _window_height;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_window_width, _window_height);
    glutInitWindowPosition(_pos_X, _pos_Y);
    glutCreateWindow(_title.c_str());

    initScene();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glDisable(GL_LIGHT0);
    glDisable(GL_COLOR_MATERIAL);

    create_menus();
}

void igvInterface::start_display_loop() {
    glutMainLoop();
}

void igvInterface::keyboardFunc(unsigned char key, int x, int y) {
    igvInterface* i = &getInstance();
    float move_speed = 0.5f;
    if (i->selectedLight > 0 && i->selectedLight < 4) {
        switch (key) {
            case 'j': i->lights[i->selectedLight].move(-move_speed, 0, 0); std::cout << "Light moved." << std::endl; break;
            case 'l': i->lights[i->selectedLight].move(move_speed, 0, 0); std::cout << "Light moved." << std::endl; break;
            case 'i': i->lights[i->selectedLight].move(0, move_speed, 0); std::cout << "Light moved." << std::endl; break;
            case 'k': i->lights[i->selectedLight].move(0, -move_speed, 0); std::cout << "Light moved." << std::endl; break;
            case 'u': i->lights[i->selectedLight].move(0, 0, move_speed); std::cout << "Light moved." << std::endl; break;
            case 'o': i->lights[i->selectedLight].move(0, 0, -move_speed); std::cout << "Light moved." << std::endl; break;
        }
    }

    switch (key) {
        case 27: exit(0);
        case 'c': case 'C': i->cameraMode = !i->cameraMode; break;
        case 'p': case 'P': i->camera->toggleProjection(); break;
        case '=': case '+': i->camera->zoom(-1.0f); break;
        case '_': case '-': i->camera->zoom(1.0f); break;
        case '1': i->selectObject(1); break;
        case '2': i->selectObject(2); break;
        case 'X': if(i->selectedObject) i->selectedObject->rotate(15.0f, 0.0f, 0.0f); break;
        case 'x': if(i->selectedObject) i->selectedObject->rotate(-15.0f, 0.0f, 0.0f); break;
        case 'Y': if(i->selectedObject) i->selectedObject->rotate(0.0f, 15.0f, 0.0f); break;
        case 'y': if(i->selectedObject) i->selectedObject->rotate(0.0f, -15.0f, 0.0f); break;
        case 'Z': if(i->selectedObject) i->selectedObject->rotate(0.0f, 0.0f, 15.0f); break;
        case 'z': if(i->selectedObject) i->selectedObject->rotate(0.0f, 0.0f, -15.0f); break;
        case 'S': if(i->selectedObject) i->selectedObject->scale(1.1f, 1.1f, 1.1f); break;
        case 's': if(i->selectedObject) i->selectedObject->scale(0.9f, 0.9f, 0.9f); break;
        case '4': if (i->currentObject == 2) i->articulatedModel->prev_dof(); break;
        case '5': if (i->currentObject == 2) i->articulatedModel->next_dof(); break;
        case 'a': case 'A': i->toggleAnimateModel(); break;
        case 'g': case 'G': i->toggleAnimateCamera(); break;
    }
    glutPostRedisplay();
}

void igvInterface::specialKeyboardFunc(int key, int x, int y) {
    igvInterface* i = &getInstance();
    if (i->cameraMode) {
        switch (key) {
            case GLUT_KEY_LEFT: i->camera->orbit(-5.0f, 0.0f); break;
            case GLUT_KEY_RIGHT: i->camera->orbit(5.0f, 0.0f); break;
            case GLUT_KEY_UP: i->camera->orbit(0.0f, 5.0f); break;
            case GLUT_KEY_DOWN: i->camera->orbit(0.0f, -5.0f); break;
        }
    } else if (i->currentObject == 2 && i->articulatedInteractionKeyboard) {
        switch (key) {
            case GLUT_KEY_UP: i->articulatedModel->increase_dof(); break;
            case GLUT_KEY_DOWN: i->articulatedModel->decrease_dof(); break;
        }
    } else if (i->selectedObject) {
        switch (key) {
            case GLUT_KEY_LEFT: i->selectedObject->translate(-0.5f, 0.0f, 0.0f); break;
            case GLUT_KEY_RIGHT: i->selectedObject->translate(0.5f, 0.0f, 0.0f); break;
            case GLUT_KEY_UP: i->selectedObject->translate(0.0f, 0.5f, 0.0f); break;
            case GLUT_KEY_DOWN: i->selectedObject->translate(0.0f, -0.5f, 0.0f); break;
        }
    }
    glutPostRedisplay();
}

void igvInterface::reshapeFunc(int w, int h) {
    glViewport(0, 0, w, h);
    _instance->set_window_width(w);
    _instance->set_window_height(h);
    _instance->camera->setAspectRatio((float)w / h);
}

void igvInterface::process_selection() {
    glDisable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->applyProjection();
    camera->applyView();
    articulatedModel->render_for_selection();
    unsigned char pixel[3];
    glReadPixels(selection_x, window_height - selection_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    if (pixel[0] > 0 && pixel[0] <= 3) {
        selected_dof_by_mouse = pixel[0] - 1;
        articulatedModel->set_dof(selected_dof_by_mouse);
    } else {
        selected_dof_by_mouse = -1;
    }
    glEnable(GL_LIGHTING);
    selection_requested = false;
}

void igvInterface::displayFunc() {
    igvInterface* i = &getInstance();

    if (selection_requested) {
        i->process_selection();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    i->camera->applyProjection();
    i->camera->applyView(); // Apply view matrix ONCE for the whole scene

    for (int j = 1; j < 4; ++j) {
        i->lights[j].apply();
    }

    glShadeModel(i->flatShading ? GL_FLAT : GL_SMOOTH);

    // Draw axes (without lighting)
    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(-10, 0, 0); glVertex3f(10, 0, 0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f(0, -10, 0); glVertex3f(0, 10, 0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f(0, 0, -10); glVertex3f(0, 0, 10);
    glEnd();
    glEnable(GL_LIGHTING);

    // Draw scene objects
    i->triangleMesh->draw();
    i->articulatedModel->draw();
    i->floor->draw();

    glutSwapBuffers();
}

void igvInterface::idleFunc() {
    static float last_time = 0;
    float current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    if (last_time == 0) last_time = current_time;
    float delta_time = current_time - last_time;
    last_time = current_time;

    igvInterface* i = &getInstance();
    if (i->animateModel) i->articulatedModel->update(current_time);
    if (i->animateCamera) i->camera->orbit(delta_time * 10.0f, 0);

    glutPostRedisplay();
}

void igvInterface::mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!getInstance().articulatedInteractionKeyboard) {
            selection_requested = true;
            selection_x = x;
            selection_y = y;
            last_mouse_y = y;
            glutPostRedisplay();
        }
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        selected_dof_by_mouse = -1;
    }
}

void igvInterface::motionFunc(int x, int y) {
    if (selected_dof_by_mouse != -1) {
        float dy = y - last_mouse_y;
        if (dy > 0) getInstance().articulatedModel->decrease_dof();
        if (dy < 0) getInstance().articulatedModel->increase_dof();
        last_mouse_y = y;
        glutPostRedisplay();
    }
}

void igvInterface::create_menus() {
    int main_menu = glutCreateMenu(menu_callback);
    glutAddMenuEntry("Select Cow", 1);
    glutAddMenuEntry("Select Robot", 2);

    int shading_menu = glutCreateMenu(shading_menu_callback);
    glutAddMenuEntry("Flat", 1);
    glutAddMenuEntry("Smooth", 2);

    int interaction_menu = glutCreateMenu(interaction_menu_callback);
    glutAddMenuEntry("Keyboard", 1);
    glutAddMenuEntry("Mouse (Picking)", 2);

    int animation_menu = glutCreateMenu(animation_menu_callback);
    glutAddMenuEntry("Toggle Model Animation", 1);
    glutAddMenuEntry("Toggle Camera Animation", 2);

    int material_menu = glutCreateMenu(floor_material_menu_callback);
    glutAddMenuEntry("Matte", 0);
    glutAddMenuEntry("Plastic", 1);
    glutAddMenuEntry("Metallic", 2);

    int texture_menu = glutCreateMenu(texture_menu_callback);
    glutAddMenuEntry("Toggle Texturing", 0);
    glutAddMenuEntry("Grid", 1);
    glutAddMenuEntry("Rock", 2);
    glutAddMenuEntry("Grass", 3);

    int filter_menu = glutCreateMenu(texture_filter_menu_callback);
    glutAddMenuEntry("Nearest/Nearest", 0);
    glutAddMenuEntry("Linear/Linear", 1);
    glutAddMenuEntry("Nearest/Linear", 2);
    glutAddMenuEntry("Linear/Nearest", 3);

    int light_toggle_menu = glutCreateMenu(light_toggle_menu_callback);
    glutAddMenuEntry("Toggle Global Ambient", 0);
    glutAddMenuEntry("Toggle Point Light", 1);
    glutAddMenuEntry("Toggle Directional Light", 2);
    glutAddMenuEntry("Toggle Spotlight", 3);

    int light_select_menu = glutCreateMenu(light_select_menu_callback);
    glutAddMenuEntry("Control Point Light", 1);
    glutAddMenuEntry("Control Spotlight", 3);

    glutSetMenu(main_menu);
    glutAddSubMenu("Shading", shading_menu);
    glutAddSubMenu("Interaction Mode", interaction_menu);
    glutAddSubMenu("Animation", animation_menu);
    glutAddSubMenu("Floor Material", material_menu);
    glutAddSubMenu("Texture", texture_menu);
    glutAddSubMenu("Texture Filter", filter_menu);
    glutAddSubMenu("Toggle Lights", light_toggle_menu);
    glutAddSubMenu("Control Light", light_select_menu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void igvInterface::initialize_callbacks() {
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialKeyboardFunc);
    glutReshapeFunc(reshapeFunc);
    glutDisplayFunc(displayFunc);
    glutIdleFunc(idleFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);
}

void igvInterface::selectObject(int objectNum) {
    if (objectNum == 1) selectedObject = triangleMesh;
    else if (objectNum == 2) selectedObject = articulatedModel;
    currentObject = objectNum;
}

void igvInterface::setShading(bool flat) { flatShading = flat; }
void igvInterface::setInteraction(bool keyboard) { articulatedInteractionKeyboard = keyboard; }
void igvInterface::toggleAnimateModel() { animateModel = !animateModel; }
void igvInterface::toggleAnimateCamera() { animateCamera = !animateCamera; }

void igvInterface::setFloorMaterial(int materialIndex) {
    if (materialIndex >= 0 && materialIndex < floorMaterials.size()) {
        currentFloorMaterial = materialIndex;
        floor->set_material(&floorMaterials[currentFloorMaterial]);
    }
}

void igvInterface::toggleTexturing() {
    texturingEnabled = !texturingEnabled;
    if (texturingEnabled && !textures.empty()) {
        floor->set_texture(&textures[currentTexture]);
        std::cout << "Texturing enabled." << std::endl;
    } else {
        floor->set_texture(nullptr);
        std::cout << "Texturing disabled." << std::endl;
    }
}

void igvInterface::setTexture(int textureIndex) {
    if (textureIndex > 0 && textureIndex <= textures.size()) {
        currentTexture = textureIndex - 1;
        if (texturingEnabled) {
            floor->set_texture(&textures[currentTexture]);
            std::cout << "Texture set to index " << currentTexture << std::endl;
        }
    }
}

void igvInterface::setTextureFilter(int filterMode) {
    if (!texturingEnabled || textures.empty() || currentTexture >= textures.size()) return;

    glBindTexture(GL_TEXTURE_2D, textures[currentTexture].getTextureId());
    switch (filterMode) {
        case 0:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case 1:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case 2:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case 3:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
    }
}

void igvInterface::toggleLight(int lightIndex) {
    if (lightIndex == 0) {
        globalAmbientOn = !globalAmbientOn;
        if (globalAmbientOn) {
            GLfloat ambient_on[] = { 0.2f, 0.2f, 0.2f, 1.0f };
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_on);
        } else {
            GLfloat ambient_off[] = { 0.0f, 0.0f, 0.0f, 1.0f };
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_off);
        }
    } else if (lightIndex > 0 && lightIndex < 4) {
        if (lights[lightIndex].is_on()) lights[lightIndex].shutdown();
        else lights[lightIndex].turnon();
    }
}

void igvInterface::selectLight(int lightIndex) {
    if (lightIndex == 1 || lightIndex == 3) {
        selectedLight = lightIndex;
        std::cout << "Controlling Light " << lightIndex << ". Use I,J,K,L,U,O to move." << std::endl;
    }
}

void menu_callback(int option) { igvInterface::getInstance().selectObject(option); glutPostRedisplay(); }
void shading_menu_callback(int option) { igvInterface::getInstance().setShading(option == 1); glutPostRedisplay(); }
void interaction_menu_callback(int option) { igvInterface::getInstance().setInteraction(option == 1); }
void animation_menu_callback(int option) { if (option == 1) igvInterface::getInstance().toggleAnimateModel(); else igvInterface::getInstance().toggleAnimateCamera(); }
void floor_material_menu_callback(int option) { igvInterface::getInstance().setFloorMaterial(option); glutPostRedisplay(); }
void texture_menu_callback(int option) { if (option == 0) igvInterface::getInstance().toggleTexturing(); else igvInterface::getInstance().setTexture(option); glutPostRedisplay(); }
void texture_filter_menu_callback(int option) { igvInterface::getInstance().setTextureFilter(option); glutPostRedisplay(); }
void light_toggle_menu_callback(int option) { igvInterface::getInstance().toggleLight(option); glutPostRedisplay(); }
void light_select_menu_callback(int option) { igvInterface::getInstance().selectLight(option); }

int igvInterface::get_window_width() { return window_width; }
int igvInterface::get_window_height() { return window_height; }
void igvInterface::set_window_width(int w) { window_width = w; }
void igvInterface::set_window_height(int h) { window_height = h; }
