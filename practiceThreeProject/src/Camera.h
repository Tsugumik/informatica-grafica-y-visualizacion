#ifndef PR1_CAMERA_H
#define PR1_CAMERA_H

class Camera {
private:
    float orbitRadius;
    float orbitAngleY;      // Horizontal orbit angle
    float orbitAngleX;      // Vertical orbit angle (pitch)
    float yawAngle;         // Camera's own Y-axis rotation

    // Projection parameters
    float fov;              // Field of view
    float nearPlane;
    float farPlane;
    float aspectRatio;
    bool perspectiveMode;

    int viewportWidth, viewportHeight;
public:
    Camera();

    // Camera movement
    void orbit(float deltaX, float deltaY);
    void pitch(float delta);
    void yaw(float delta);
    void zoom(float delta);

    // Clipping planes
    void moveNearPlane(float delta);
    void moveFarPlane(float delta);

    // Projection
    void toggleProjection();
    void setAspectRatio(float ratio);

    // Apply camera transformations
    void applyProjection();
    void applyView();

    // Getters
    bool isPerspective() const { return perspectiveMode; }
};


#endif //PR1_CAMERA_H