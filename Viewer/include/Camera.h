#pragma once
#include <glm/glm.hpp>
#include <string>
#include "glm/gtc/matrix_transform.hpp"
#include "MatrixTransform.h"



class Camera : public MatrixTransform
{
public:
    Camera();
    virtual ~Camera();
    
    void LookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
    void CreateProjection();
    
    const glm::mat4& GetProjectionTransformation() const;
    const glm::mat4& GetViewTransformation() const;
    const glm::mat4 GetCameraTransformation();
    const glm::mat4& GetInvViewTransformation() const;
    void SetOrthoParameters(float width, float height);
    bool GetProjectionFlag();
    void SetProjectionFlag(bool flag);
    
    
    std::vector<float> GetOrthoParameters();
    std::vector<float> GetPerspectiveParameters();
    float GetOrthoParameters(int index);
    
    glm::vec3 getEye();
    glm::vec3 getAt();
    glm::vec3 getUp();
    
    glm::vec3 GetCameraPosition();
    void SetDollyZoom(const float i);
    const float GetDollyZoom();
    
    void setUp(float up_);
    void setDown(float down_);
    void setLeft(float left_);
    void setRight(float right_);
    void setNear(float near_);
    void setFar(float far_);
    
    void setAnglePers(float angle);
    void setAspectPers(float aspect);
    void setNearPers(float near);
    void setFarPers(float far);
    
    void setEyeX(float eyeX_);
    void setEyeY(float eyeY_);
    void setEyeZ(float eyeZ_);
    void setAtX(float atX_);
    void setAtY(float atY_);
    void setAtZ(float atZ_);
    void setUpX(float upX_);
    void setUpY(float upY_);
    void setUpZ(float upZ_);
    
    
    void UpdatePosition(glm::mat4 m);
    
private:
    glm::mat4 view_transformation = glm::mat4(1.0f);
    glm::mat4 projection_transformation = glm::mat4(1.0f);
    glm::mat4 lookAtMatrix = glm::mat4(1.0f);
    glm::vec3 eye = glm::vec3(0.0f, 0.0f, 1000.0f);
    glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    float dollyZoom = 0.0f;
    
    std::vector<float> ortho_parameters = {-640.0f, 640.0f, -360.0f, 360.0f ,1.0f, 2000.0f};
    std::vector<float> perspective_parameters = {45.0f, 1280.0f/720.0f, 1.0f, 2000.0f};
    
    bool projectionFlag = true; //true = orthographic, false = perspective
    
};
