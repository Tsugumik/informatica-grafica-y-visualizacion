#pragma once

#include "cgvPoint.h"

/**
 * Labels to define the types of cameras
 */
typedef enum {
	CGV_PARALLEL,
	CGV_PERSPECTIVE,
    CGV_FRUSTRUM
} cameraType;



/**
 * cgvCamera contains the basic functionality to create and manipulate cameras and projections
 */
class cgvCamera {

public:
    // attributes

    cameraType type; // parallel or perspective

    // viewport: parallel and frustum projection parameters
    GLdouble xwmin, xwmax, ywmin, ywmax;

    // viewport: perspective projection parameters
    GLdouble angle, aspect;

    // distances of near and far planes
    GLdouble znear, zfar;

    // viewpoint
    cgvPoint3D P0;

    // view reference point
    cgvPoint3D r;

    // vector up
    cgvPoint3D V;

    // Methods

public:
    // Default constructors and destructor
    cgvCamera();
    ~cgvCamera();

    // Other constructors
    cgvCamera(cameraType _type, cgvPoint3D _P0, cgvPoint3D _r, cgvPoint3D _V);

    // Methods
    // Defines the camera position
    void set(cgvPoint3D _P0, cgvPoint3D _r, cgvPoint3D _V);

    // defines a parallel or frustum type camera
    void set(cameraType _type, cgvPoint3D _P0, cgvPoint3D _r, cgvPoint3D _V,
             double _xwmin, double _xwmax, double _ywmin, double _ywmax, double _znear, double _zfar);

    // defines a perspective camera
    void set(cameraType _type, cgvPoint3D _P0, cgvPoint3D _r, cgvPoint3D _V,
             double _angle, double _aspect, double _znear, double _zfar);

    void apply(void); // applies the vision transform and the projection transform to the objects in the scene
    // associated with the camera parameters
    void zoom(double factor); // zooms in on the camera
};

