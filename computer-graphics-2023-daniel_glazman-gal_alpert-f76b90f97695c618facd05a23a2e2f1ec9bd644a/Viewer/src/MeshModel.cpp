#include "MeshModel.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
    model_name(model_name)
{

}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const glm::vec3& MeshModel::GetVertices(int index) const
{
    return vertices[index];
}

int MeshModel::GetVerticesCount() const
{
    return vertices.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}


float MeshModel::GetLocalTranslate() const
{
    return translate;
}

float MeshModel::GetScale() const
{
    return scale;
}

float MeshModel::GetLocalRotate() const
{
    return rotate;
}

float MeshModel::GetLocalX() const
{
    return x_trans;
}

float MeshModel::GetLocalY() const
{
    return y_trans;
}

float MeshModel::GetLocalZ() const
{
    return z_trans;
}

float MeshModel::GetLocalAngleX() const
{
    return angleX;
}

float MeshModel::GetLocalAngleY() const
{
    return angleY;
}

float MeshModel::GetLocalAngleZ() const
{
    return angleZ;
}

float MeshModel::GetWorldTranslate() const
{
    return translate_world;
}


float MeshModel::GetWorldRotate() const
{
    return rotate_world;
}

float MeshModel::GetWorldX() const
{
    return xt_world;
}

float MeshModel::GetWorldY() const
{
    return yt_world;
}

float MeshModel::GetWorldZ() const
{
    return zt_world;
}

float MeshModel::GetWorldAngleX() const
{
    return angleX_world;
}

float MeshModel::GetWorldAngleY() const
{
    return angleY_world;
}

float MeshModel::GetWorldAngleZ() const
{
    return angleZ_world;
}

void MeshModel::SetLocalTranslate(float translate_)
{
    this->translate = translate_;
}
void MeshModel::SetScale(float scale_)
{
    this->scale = scale_;
}
void MeshModel::SetLocalRotate(float rotate_)
{
    this->rotate = rotate_;
}

void MeshModel::SetLocalX(float x)
{
    this->x_trans = x;
}
void MeshModel::SetLocalY(float y)
{
    this->y_trans = y;
}
void MeshModel::SetLocalZ(float z)
{
    this->z_trans = z;
}

void MeshModel::SetLocalAngleX(float angleX)
{
    this->angleX = angleX;
}
void MeshModel::SetLocalAngleY(float angleY)
{
    this->angleY = angleY;
}
void MeshModel::SetLocalAngleZ(float angleZ)
{
    this->angleZ = angleZ;
}

void MeshModel::SetWorldTranslate(float translate_)
{
    this->translate_world = translate_;
}

void MeshModel::SetWorldRotate(float rotate_)
{
    this->rotate_world = rotate_;
}

void MeshModel::SetWorldX(float x)
{
    this->xt_world = x;
}
void MeshModel::SetWorldY(float y)
{
    this->yt_world = y;
}
void MeshModel::SetWorldZ(float z)
{
    this->zt_world = z;
}

void MeshModel::SetWorldAngleX(float angleX)
{
    this->angleX_world = angleX;
}
void MeshModel::SetWorldAngleY(float angleY)
{
    this->angleY_world = angleY;
}
void MeshModel::SetWorldAngleZ(float angleZ)
{
    this->angleZ_world = angleZ;
}

glm::mat4 MeshModel::GetTansformationMatrix()
{
    glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1200, 400, 200));
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(275, 275, 275));
    return translateMatrix * scalingMatrix;
}

//void MeshModel::ScaleAndTranslteModel()
//{
//    glm::mat4 scaleAndTranslateMatrix = GetTansformationMatrix();
//    
//    for (int i = 0; i < vertices.size(); i++)
//    {
//        glm::vec4 vertexVector(vertices[i], 1.0f);
//        glm::vec4 transformVec = scaleAndTranslateMatrix * vertexVector;
//        glm::vec3 finalVecteor(transformVec);
//        vertices[i] = finalVecteor;
//    }
//}

glm::mat4 MeshModel::ScaleModel(float scale)
{
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
    
    return scalingMatrix;
}

glm::mat4 MeshModel::TranslateModel(float x, float y, float z)
{
    glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));

    return translateMatrix;
}

glm::mat4 MeshModel::RotateXModel(float angleX)
{
    glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
 
    return rotateMatrix;
}

glm::mat4 MeshModel::RotateYModel(float angleY)
{
    glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));

    return rotateMatrix;
}

glm::mat4 MeshModel::RotateZModel(float angleZ)
{
    glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));

    return rotateMatrix;
}

glm::mat4 MeshModel::local_transformation(glm::mat4 scalingMatrix, glm::mat4 translateMatrix, glm::mat4 rotate_x_Matrix, glm::mat4 rotate_y_Matrix, glm::mat4 rotate_z_Matrix)
{
    glm::mat4 localMatrix = translateMatrix * scalingMatrix * (rotate_x_Matrix * rotate_y_Matrix * rotate_z_Matrix);
    return localMatrix;
}

glm::mat4 MeshModel::world_transformation(glm::mat4 scalingMatrix, glm::mat4 translateMatrix, glm::mat4 rotate_x_Matrix, glm::mat4 rotate_y_Matrix, glm::mat4 rotate_z_Matrix)
{
    glm::mat4 worldMatrix = translateMatrix * scalingMatrix * (rotate_x_Matrix * rotate_y_Matrix * rotate_z_Matrix);
    return worldMatrix;
}

glm::mat4 MeshModel::final_transformation(glm::mat4 worldMatrix, glm::mat4 localMatrix) {

    glm::mat4 finalTrans = worldMatrix * localMatrix;
    return finalTrans;
}


