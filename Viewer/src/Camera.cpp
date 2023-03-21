#include "Camera.h"
#include <iostream>



Camera::Camera() {}
Camera::~Camera() {}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
    return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
    return view_transformation;
}

const glm::mat4x4& Camera::GetInvViewTransformation() const
{
    return lookAtMatrix;
}

const glm::mat4x4 Camera::GetCameraTransformation()
{
    CreateProjection();
    LookAt(eye, at, up);
    glm::mat4 temp = glm::inverse(getTransformationMatrix());
    UpdatePosition(temp);
    return lookAtMatrix * temp;
}


void Camera::LookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
    lookAtMatrix = glm::lookAt(eye, at, up);
    view_transformation = glm::inverse(lookAtMatrix);
}

void Camera::SetProjectionFlag(bool flag)
{
    projectionFlag = flag;
}

bool Camera::GetProjectionFlag()
{
    return projectionFlag;
}


void Camera::SetOrthoParameters(float width, float height)
{
    ortho_parameters[0] = -width/2.f;
    ortho_parameters[1] = width/2.f;
    ortho_parameters[2] = -height/2.f;
    ortho_parameters[3] = height/2.f;
    
    //update aspect ratio
    perspective_parameters[1] = width/height;
}

void Camera::setUp(float up_)
{
    ortho_parameters[0] = up_;
}

void Camera::setDown(float down_)
{
    ortho_parameters[1] = down_;
}

void Camera::setLeft(float left_)
{
    ortho_parameters[2] = left_;
}

void Camera::setRight(float right_)
{
    ortho_parameters[3] = right_;
}

void Camera::setNear(float near_)
{
    ortho_parameters[4] = near_;
}

void Camera::setFar(float far_)
{
    ortho_parameters[5] = far_;
}

void Camera::setAnglePers(float angle_per)
{
    perspective_parameters[0] = angle_per;
}

void Camera::setAspectPers(float aspect)
{
    perspective_parameters[1] = aspect;
}

void Camera::setNearPers(float near)
{
    perspective_parameters[2] = near;
}

void Camera::setFarPers(float far)
{
    perspective_parameters[3] = far;
}

void Camera::setEyeX(float eyeX_) { eye[0] = eyeX_; }
void Camera::setEyeY(float eyeY_) { eye[1] = eyeY_; }
void Camera::setEyeZ(float eyeZ_) { eye[2] = eyeZ_; }
void Camera::setAtX(float atX_) { at[0] = atX_; }
void Camera::setAtY(float atY_) { at[1] = atY_; }
void Camera::setAtZ(float atZ_) { at[2] = atZ_; }
void Camera::setUpX(float upX_) { up[0] = upX_; }
void Camera::setUpY(float upY_) { up[1] = upY_; }
void Camera::setUpZ(float upZ_) { up[2] = upZ_; }

void Camera::CreateProjection()
{
    if (projectionFlag)
    {
        projection_transformation = glm::ortho(ortho_parameters[0], ortho_parameters[1], ortho_parameters[2], ortho_parameters[3], ortho_parameters[4], ortho_parameters[5]);
    }
    else
    {
        projection_transformation = glm::perspective(glm::radians(perspective_parameters[0]), perspective_parameters[1], perspective_parameters[2],perspective_parameters[3]);
    }
}

void Camera::SetDollyZoom(const float i)
{
    this->dollyZoom = i;
    setAnglePers(45.0f + (0.5*i));
    setEyeZ(1000 - (i * 10.0f));
}

glm::vec3 Camera::GetCameraPosition()
{
    return position;
}

void Camera::UpdatePosition(glm::mat4 m)
{
    position = m * glm::vec4(1.0f);
}

glm::vec3 Camera::getEye()
{
    return eye;
}

glm::vec3 Camera::getAt()
{
    return at;
}

glm::vec3 Camera::getUp()
{
    return up;
}

std::vector<float> Camera::GetOrthoParameters()
{
    return ortho_parameters;
}

std::vector<float> Camera::GetPerspectiveParameters()
{
    return perspective_parameters;
}

float Camera::GetOrthoParameters(int index)
{
    return ortho_parameters[index];
}

const float Camera::GetDollyZoom()
{
    return dollyZoom;
}



