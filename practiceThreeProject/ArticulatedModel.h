#ifndef __ARTICULATED_MODEL_H
#define __ARTICULATED_MODEL_H

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "src/Object3D.h"
#include "src/cgvPoint3D.h"

class ArticulatedModel : public Object3D {
public:
    ArticulatedModel();
    ~ArticulatedModel() = default;

    void draw() override;
    void render_for_selection();

    void next_dof();
    void prev_dof();
    void increase_dof();
    void decrease_dof();
    void set_dof(int dof_id);
    void update(float delta_time);

private:
    float dof[3];
    int active_dof;
    float dof_min[3];
    float dof_max[3];

    // Methods to draw primitives
    void draw_base();
    void draw_arm1();
    void draw_arm2();
    void draw_head();
};

#endif
