#include "MatrixTransform.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"


MatrixTransform::MatrixTransform(){}

const float MatrixTransform::getScaleLocal()
{
    return scaleLocal;
}

const float MatrixTransform::getScaleWorld()
{
    return scaleWorld;
}

void MatrixTransform::setScale(float scale)
{
    scaleLocal = scale;
}

void MatrixTransform::setScaleWorld(float scale)
{
    scaleWorld = scale;
}

const glm::vec3& MatrixTransform::getTranslateLocal()
{
    return translateLocal;
}

const glm::vec3& MatrixTransform::getTranslateWorld()
{
    return translateWorld;
}

const glm::vec3& MatrixTransform::getRotateLocal()
{
    return rotate;
}

const glm::vec3& MatrixTransform::getRotateWorld()
{
    return rotateWorld;
}


void MatrixTransform::setLocalTranslate(const char c, const float d)
{
    switch (c) {
    case 'x':
            this->translateLocal[0] = d;
        break;
    case 'y':
            this->translateLocal[1] = d;
        break;
    case 'z':
            this->translateLocal[2] = d;
        break;
    }
}


void MatrixTransform::setWorldTranslte(const char c, const float d)
{
    switch (c) {
    case 'x':
            this->translateWorld[0] = d;
        break;
    case 'y':
            this->translateWorld[1] = d;
        break;
    case 'z':
            this->translateWorld[2] = d;
        break;
    }
}

void MatrixTransform::setRotateLocal(const char c, const float d)
{
    switch (c) {
    case 'x':
            this->rotate[0] = d;
        break;
    case 'y':
            this->rotate[1] = d;
        break;
    case 'z':
            this->rotate[2] = d;
        break;
    }
}

void MatrixTransform::setRotateWorld(const char c, const float d)
{
    switch (c) {
    case 'x':
            this->rotateWorld[0] = d;
        break;
    case 'y':
            this->rotateWorld[1] = d;
        break;
    case 'z':
            this->rotateWorld[2] = d;
        break;
    }
}


glm::mat4 MatrixTransform::createRotationMatrix(char c,float d)
{
    switch (c) {
    case 'x':
            return glm::rotate(glm::mat4(1.0f),glm::radians(d),glm::vec3(1.0f, 0.0f, 0.0f));
        break;
    case 'y':
            return glm::rotate(glm::mat4(1.0f),glm::radians(d),glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case 'z':
            return glm::rotate(glm::mat4(1.0f),glm::radians(d),glm::vec3(0.0f, 0.0f, 1.0f));
        break;
    
    }
}


void MatrixTransform::createTranslationLocal()
{
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scaleLocal, scaleLocal, scaleLocal));
    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), getTranslateLocal());
    
    glm::mat4 rotation_x = createRotationMatrix('x', rotate[0]);
    glm::mat4 rotation_y = createRotationMatrix('y', rotate[1]);
    glm::mat4 rotation_z = createRotationMatrix('z', rotate[2]);
    rotateLocal = rotation_z * rotation_y * rotation_x;
    
    //update local matrix
    local = transMat * scaleMat * rotateLocal;
}


void MatrixTransform::createTranslationWorld(const char c){
    //scale
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scaleWorld, scaleWorld, scaleWorld));
    
    //translate
    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), getTranslateWorld());
    
    //rotateion
    glm::mat4 rotation_x = createRotationMatrix('x', rotateWorld[0]);
    glm::mat4 rotation_y = createRotationMatrix('y', rotateWorld[1]);
    glm::mat4 rotation_z = createRotationMatrix('z', rotateWorld[2]);
    rotateWorldMat = rotation_z * rotation_y * rotation_x;
    switch (c) {
    case 'a':
            //create transormed matrix
            world = transMat * scaleMat * rotateWorldMat;
    case 'x':
            //rotate y and z axes
            modelAxisMatrix = transMat * rotateWorldMat/rotation_x;
        break;
    case 'y':
            //rotate x and z axis
            modelAxisMatrix = transMat *  rotateWorldMat/rotation_y;
        break;
    case 'z':
            //rotate x and y axis
            modelAxisMatrix = transMat *  rotateWorldMat/rotation_z;
        break;
    case 'o':
            //move 0.0
            modelAxisMatrix = transMat ;
        break;
    }
}

glm::mat4 MatrixTransform::getTransformationMatrix()
{
    createTranslationWorld('a');
    createTranslationLocal();
    return world * local;
}

glm::mat4 MatrixTransform::getLocalTransformation()
{
    createTranslationLocal();
    return local;
}

glm::mat4 MatrixTransform::getModelAxis(const char c)
{
    createTranslationWorld(c);
    return modelAxisMatrix;
}


const glm::mat4& MatrixTransform::getLocalRotationMatrix()
{
    return this->rotateLocal;
}

const glm::mat4& MatrixTransform::getRotationWorldMatrix()
{
    return this->rotateWorldMat;
}
