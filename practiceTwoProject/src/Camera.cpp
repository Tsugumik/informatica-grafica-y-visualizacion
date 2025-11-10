#include "Camera.h"
#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

Camera::Camera() {
    orbitRadius = 25.0f; // Increased from 5.0f
    orbitAngleY = 45.0f;
    orbitAngleX = 30.0f;
    yawAngle = 0.0f;

    fov = 45.0f;
    nearPlane = 0.1f;
    farPlane = 100.0f;
    aspectRatio = 1.0f;
    perspectiveMode = true;

    viewportWidth = 800;
    viewportHeight = 600;
}

void Camera::orbit(float deltaX, float deltaY) {
    orbitAngleY += deltaX;
    orbitAngleX += deltaY;

    if (orbitAngleX > 89.0f) orbitAngleX = 89.0f;
    if (orbitAngleX < -89.0f) orbitAngleX = -89.0f;
}

void Camera::pitch(float delta) {
    orbitAngleX += delta;
    if (orbitAngleX > 89.0f) orbitAngleX = 89.0f;
    if (orbitAngleX < -89.0f) orbitAngleX = -89.0f;
}

void Camera::yaw(float delta) {
    yawAngle += delta;
}

void Camera::zoom(float delta) {
    orbitRadius += delta;
    if (orbitRadius < 1.0f) orbitRadius = 1.0f;
}

void Camera::moveNearPlane(float delta) {
    nearPlane += delta;
    if (nearPlane < 0.01f) nearPlane = 0.01f;
    if (nearPlane >= farPlane) nearPlane = farPlane - 0.1f;
}

void Camera::moveFarPlane(float delta) {
    farPlane += delta;
    if (farPlane <= nearPlane) farPlane = nearPlane + 0.1f;
}

void Camera::toggleProjection() {
    perspectiveMode = !perspectiveMode;
}

void Camera::setAspectRatio(float ratio) {
    aspectRatio = ratio;
}

void Camera::applyProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (perspectiveMode) {
        gluPerspective(fov, aspectRatio, nearPlane, farPlane);
    } else {
        float size = orbitRadius * 0.1f; // Adjust ortho size based on radius
        glOrtho(-size * aspectRatio, size * aspectRatio, -size, size, nearPlane, farPlane);
    }
}

void Camera::applyView() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float radY = orbitAngleY * M_PI / 180.0f;
    float radX = orbitAngleX * M_PI / 180.0f;

    float x = orbitRadius * cos(radX) * sin(radY);
    float y = orbitRadius * sin(radX);
    float z = orbitRadius * cos(radX) * cos(radY);

    gluLookAt(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(yawAngle, 0.0f, 1.0f, 0.0f);
}
