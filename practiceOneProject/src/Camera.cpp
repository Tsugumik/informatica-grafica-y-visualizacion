//
// Created by Błażej Drozd on 01/10/2025.
//

#include "Camera.h"
#include <iostream>
#include <GLUT/glut.h>

Camera::Camera() {
    orbitRadius = 5.0f;
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
    orbitAngleY += deltaX * 2.0f;  // Horizontal orbit
    orbitAngleX += deltaY * 2.0f;  // Vertical orbit (pitch)

    // Clamp vertical angle to avoid flipping
    if (orbitAngleX > 89.0f) orbitAngleX = 89.0f;
    if (orbitAngleX < -89.0f) orbitAngleX = -89.0f;

    std::cout << "Orbit: H=" << orbitAngleY << ", V=" << orbitAngleX << std::endl;
}

void Camera::pitch(float delta) {
    orbitAngleX += delta * 2.0f;
    if (orbitAngleX > 89.0f) orbitAngleX = 89.0f;
    if (orbitAngleX < -89.0f) orbitAngleX = -89.0f;
    std::cout << "Pitch: " << orbitAngleX << std::endl;
}

void Camera::yaw(float delta) {
    yawAngle += delta * 2.0f;
    std::cout << "Yaw: " << yawAngle << std::endl;
}

void Camera::zoom(float delta) {
    if (perspectiveMode) {
        fov += delta * 2.0f;
        if (fov < 10.0f) fov = 10.0f;
        if (fov > 90.0f) fov = 90.0f;
    } else {
        orbitRadius += delta * 0.5f;
        if (orbitRadius < 1.0f) orbitRadius = 1.0f;
        if (orbitRadius > 20.0f) orbitRadius = 20.0f;
    }
    std::cout << "Zoom - FOV: " << fov << ", Radius: " << orbitRadius << std::endl;
}

/*
void Camera::moveNearPlane(float delta) {
    nearPlane += delta * 0.1f;
    if (nearPlane < 0.01f) nearPlane = 0.01f;
    if (nearPlane >= farPlane) nearPlane = farPlane - 0.1f;
    std::cout << "Near plane: " << nearPlane << std::endl;
}

void Camera::moveFarPlane(float delta) {
    farPlane += delta;
    if (farPlane <= nearPlane) farPlane = nearPlane + 0.1f;
    if (farPlane > 1000.0f) farPlane = 1000.0f;
    std::cout << "Far plane: " << farPlane << std::endl;
}*/

void Camera::moveNearPlane(float delta) {
    nearPlane += delta * 1.0f;
    if (nearPlane < 0.01f) nearPlane = 0.01f;
    if (nearPlane >= farPlane) nearPlane = farPlane - 0.1f;
    std::cout << "Near plane: " << nearPlane << std::endl;
}

void Camera::moveFarPlane(float delta) {
    farPlane += delta * 1.0f;
    if (farPlane <= nearPlane) farPlane = nearPlane + 0.1f;
    if (farPlane > 1000.0f) farPlane = 1000.0f;
    std::cout << "Far plane: " << farPlane << std::endl;
}

void Camera::toggleProjection() {
    perspectiveMode = !perspectiveMode;
    std::cout << "Projection: " << (perspectiveMode ? "Perspective" : "Orthogonal") << std::endl;
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
        float size = orbitRadius;
        glOrtho(-size * aspectRatio, size * aspectRatio,
                -size, size, nearPlane, farPlane);
    }
}

void Camera::applyView() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Calculate camera position from orbit angles
    float radY = orbitAngleY * M_PI / 180.0f;
    float radX = orbitAngleX * M_PI / 180.0f;

    float x = orbitRadius * cos(radX) * cos(radY);
    float y = orbitRadius * sin(radX);
    float z = orbitRadius * cos(radX) * sin(radY);


    glRotatef(yawAngle, 0.0f, 1.0f, 0.0f);

    gluLookAt(x, y, z,           // Camera position
              0.0f, 0.0f, 0.0f,  // Look at origin
              0.0f, 1.0f, 0.0f); // Up vector
}