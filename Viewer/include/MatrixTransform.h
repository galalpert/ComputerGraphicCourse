#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MatrixTransform
{
public:
    MatrixTransform();
    const float getScaleLocal();
    const glm::vec3& getTranslateLocal();
    const glm::vec3& getRotateLocal();
    const glm::vec3& getTranslateWorld();
    const glm::vec3& getRotateWorld();
    const float getScaleWorld();

    glm::mat4 createRotationMatrix(char coordinates, float degree);
    void createTranslationLocal();
    void createTranslationWorld(const char c);
    glm::mat4 getTransformationMatrix();
    glm::mat4 getLocalTransformation();
    const glm::mat4& getLocalRotationMatrix();
    const glm::mat4& getRotationWorldMatrix();
    glm::mat4 getModelAxis(const char c);

    void setScale(float scale);
    void setScaleWorld( float scale);
    void setLocalTranslate(const char c, const float d);
    void setWorldTranslte(const char c, const float d);
    void setRotateWorld(const char c, const float d);
    void setRotateLocal(const char c, const float d);

private:
    //local
    glm::mat4 local = glm::mat4(1.0f);
    glm::vec3 translateLocal = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 rotate = glm::vec3(0.f, 0.f, 0.f);
    glm::mat4 rotateLocal = glm::mat4(1.0f);
    float scaleLocal = 1.f;

    //world
    glm::mat4 world = glm::mat4(1.0f);
    glm::vec3 translateWorld = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 rotateWorld = glm::vec3(0.f, 0.f, 0.f);
    glm::mat4 rotateWorldMat = glm::mat4(1.0f);
    float scaleWorld = 1.0f;
    
    glm::mat4 modelAxisMatrix;
};

