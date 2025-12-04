#include "igvInterface.h"
#include "src/AdvancedOBJLoader.h"
#include <iostream>
#include <cmath>

igvInterface* igvInterface::_instance = nullptr;

// Static callbacks
void menu_callback(int option);
void shading_menu_callback(int option);
void interaction_menu_callback(int option);
void animation_menu_callback(int option);

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
    camera = new Camera();
    triangleMesh = new cgvTriangleMesh();
    articulatedModel = new ArticulatedModel();

    if (!AdvancedOBJLoader::load("objFiles/cow.obj", *triangleMesh)) {
        std::cerr << "Failed to load cow mesh." << std::endl;
    }
    triangleMesh->set_specular_reflectivity(0.1f);
    triangleMesh->set_shininess(10.0f);
    triangleMesh->translate(-5, 0, 0);

    articulatedModel->translate(5, 0, 0);

    selectedObject = nullptr;
    currentObject = 0;
    cameraMode = false;
    flatShading = false;
    articulatedInteractionKeyboard = true;
    animateCamera = false;
    animateModel = false;
}

igvInterface::~igvInterface() {
    delete camera;
    delete triangleMesh;
    delete articulatedModel;
}

void igvInterface::configure_environment(int argc, char** argv, int _window_width, int _window_height, int _pos_X, int _pos_Y, std::string _title) {
    window_width = _window_width;
    window_height = _window_height;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_window_width, _window_height);
    glutInitWindowPosition(_pos_X, _pos_Y);
    glutCreateWindow(_title.c_str());

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = { 10.0f, 10.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    GLfloat white_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    GLfloat ambient_light[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    create_menus();
}

void igvInterface::start_display_loop() {
    glutMainLoop();
}

void igvInterface::keyboardFunc(unsigned char key, int x, int y) {
    igvInterface* i = &getInstance();
    switch (key) {
        case 27: exit(0);
        case 'c': case 'C': i->cameraMode = !i->cameraMode; break;
        case 'p': case 'P': i->camera->toggleProjection(); break;
        case '=': //support keys with/without shift too
        case '+': i->camera->zoom(-1.0f); break;
        case '_':
        case '-': i->camera->zoom(1.0f); break;
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
    // This is now called from displayFunc
    glDisable(GL_LIGHTING);
    glDisable(GL_DITHER);

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
    glEnable(GL_DITHER);
    selection_requested = false; // Reset flag
}

void igvInterface::displayFunc() {
    igvInterface* i = &getInstance();

    if (selection_requested) {
        i->process_selection();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    i->camera->applyProjection();
    i->camera->applyView();

    glShadeModel(i->flatShading ? GL_FLAT : GL_SMOOTH);

    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(-10, 0, 0); glVertex3f(10, 0, 0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f(0, -10, 0); glVertex3f(0, 10, 0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f(0, 0, -10); glVertex3f(0, 0, 10);
    glEnd();
    glEnable(GL_LIGHTING);

    i->triangleMesh->draw();
    i->articulatedModel->draw();

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
    igvInterface* i = &getInstance();
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!i->articulatedInteractionKeyboard) {
            selection_requested = true;
            selection_x = x;
            selection_y = y;
            last_mouse_y = y;
            glutPostRedisplay(); // Request a redraw to process selection
        }
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        selected_dof_by_mouse = -1;
    }
}

void igvInterface::motionFunc(int x, int y) {
    igvInterface* i = &getInstance();
    if (selected_dof_by_mouse != -1) {
        float dy = y - last_mouse_y;
        if (dy > 0) i->articulatedModel->decrease_dof();
        if (dy < 0) i->articulatedModel->increase_dof();
        last_mouse_y = y;
        glutPostRedisplay();
    }
}

void igvInterface::create_menus() {
    int shading_menu = glutCreateMenu(shading_menu_callback);
    glutAddMenuEntry("Flat", 1);
    glutAddMenuEntry("Smooth", 2);

    int interaction_menu = glutCreateMenu(interaction_menu_callback);
    glutAddMenuEntry("Keyboard", 1);
    glutAddMenuEntry("Mouse (Picking)", 2);

    int animation_menu = glutCreateMenu(animation_menu_callback);
    glutAddMenuEntry("Toggle Model Animation", 1);
    glutAddMenuEntry("Toggle Camera Animation", 2);

    glutCreateMenu(menu_callback);
    glutAddMenuEntry("Select Cow", 1);
    glutAddMenuEntry("Select Robot", 2);
    glutAddSubMenu("Shading", shading_menu);
    glutAddSubMenu("Interaction Mode", interaction_menu);
    glutAddSubMenu("Animation", animation_menu);
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
    if (objectNum == 1) {
        selectedObject = triangleMesh;
        currentObject = 1;
    } else if (objectNum == 2) {
        selectedObject = articulatedModel;
        currentObject = 2;
    }
}

void igvInterface::setShading(bool flat) { flatShading = flat; }
void igvInterface::setInteraction(bool keyboard) { articulatedInteractionKeyboard = keyboard; }
void igvInterface::toggleAnimateModel() { animateModel = !animateModel; }
void igvInterface::toggleAnimateCamera() { animateCamera = !animateCamera; }

void menu_callback(int option) {
    igvInterface::getInstance().selectObject(option);
    glutPostRedisplay();
}

void shading_menu_callback(int option) {
    igvInterface::getInstance().setShading(option == 1);
    glutPostRedisplay();
}

void interaction_menu_callback(int option) {
    igvInterface::getInstance().setInteraction(option == 1);
}

void animation_menu_callback(int option) {
    if (option == 1) igvInterface::getInstance().toggleAnimateModel();
    if (option == 2) igvInterface::getInstance().toggleAnimateCamera();
}

int igvInterface::get_window_width() { return window_width; }
int igvInterface::get_window_height() { return window_height; }
void igvInterface::set_window_width(int w) { window_width = w; }
void igvInterface::set_window_height(int h) { window_height = h; }
