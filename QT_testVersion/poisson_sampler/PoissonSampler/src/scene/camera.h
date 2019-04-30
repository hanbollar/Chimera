#pragma once

#include <globals.h>

#include <raytracing/ray.h>
#include <openGL/drawable.h>

#include "samplers/sampler.h"
#include "warpfunctions.h"

//A perspective projection camera
//Receives its eye position and reference point from the scene file
class Camera {
public:
    Camera();
    Camera(unsigned int w, unsigned int h);
    Camera(unsigned int w, unsigned int h, const Vector3f &e, const Vector3f &r, const Vector3f &worldUp);
    Camera(const Camera &c);

    void CopyAttributes(const Camera &c);

    float fovy;
    unsigned int width, height;  // Screen dimensions
    float near_clip;  // Near clip plane distance
    float far_clip;  // Far clip plane distance

    //Computed attributes
    float aspect;

    Point3f  eye,      //The position of the camera in world space
             ref;      //The point in world space towards which the camera is pointing
    Vector3f look,     //The normalized vector from eye to ref. Is also known as the camera's "forward" vector.
             up,       //The normalized vector pointing upwards IN CAMERA SPACE. This vector is perpendicular to LOOK and RIGHT.
             right,    //The normalized vector pointing rightwards IN CAMERA SPACE. It is perpendicular to UP and LOOK.
             world_up, //The normalized vector pointing upwards IN WORLD SPACE. This is primarily used for computing the camera's initial UP vector.
             V,        //Represents the vertical component of the plane of the viewing frustum that passes through the camera's reference point. Used in Camera::Raycast.
             H;        //Represents the horizontal component of the plane of the viewing frustum that passes through the camera's reference point. Used in Camera::Raycast.

    float lensRadius = -1.0f; //-HB: if lensRad > 0 then using lensCamera otherwise using normal camera
    float focalDistance = 30.0f; //-HB
    Sampler* samp = new Sampler(100, 0); //-HB

    Matrix4x4 GetViewProj() const;

    void RecomputeAttributes();

    void RotateAboutUp(float deg);
    void RotateAboutRight(float deg);

    void TranslateAlongLook(float amt);
    void TranslateAlongRight(float amt);
    void TranslateAlongUp(float amt);
};
