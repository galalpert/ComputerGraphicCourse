#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
    const glm::vec3& GetVertices(int index) const;
    int GetVerticesCount() const;
	const std::string& GetModelName() const;
    glm::mat4 GetTansformationMatrix();
    //void ScaleAndTranslteModel();
    
    float GetLocalTranslate() const;
    float GetScale() const;
    float GetLocalRotate() const;
    
    float GetLocalX() const;
    float GetLocalY() const;
    float GetLocalZ() const;
    
    float GetLocalAngleX() const;
    float GetLocalAngleY() const;
    float GetLocalAngleZ() const;
    
    float GetWorldTranslate() const;
    float GetWorldRotate() const;
    
    float GetWorldX() const;
    float GetWorldY() const;
    float GetWorldZ() const;
    
    float GetWorldAngleX() const;
    float GetWorldAngleY() const;
    float GetWorldAngleZ() const;
    
    
    void SetLocalTranslate(float translate_);
    void SetScale(float scale_);
    void SetLocalRotate(float rotate_);
    
    void SetLocalX(float x);
    void SetLocalY(float y);
    void SetLocalZ(float z);
    
    void SetLocalAngleX(float angleX);
    void SetLocalAngleY(float angleY);
    void SetLocalAngleZ(float angleZ);
    
    void SetWorldTranslate(float translate_);
    void SetWorldRotate(float roatate_);
    
    void SetWorldX(float x);
    void SetWorldY(float y);
    void SetWorldZ(float z);
    
    void SetWorldAngleX(float angleX);
    void SetWorldAngleY(float angleY);
    void SetWorldAngleZ(float angleZ);
    
    
    glm::mat4 ScaleModel(float scale);
    glm::mat4 TranslateModel(float xt, float yt, float zt);
    glm::mat4 RotateXModel(float angleX);
    glm::mat4 RotateYModel(float angleY);
    glm::mat4 RotateZModel(float angleZ);
    glm::mat4 local_transformation(glm::mat4 scalingMatrix, glm::mat4 translateMatrix, glm::mat4 rotate_x_Matrix, glm::mat4 rotate_y_Matrix, glm::mat4 rotate_z_Matrix);
    glm::mat4 world_transformation(glm::mat4 scalingMatrix, glm::mat4 translateMatrix, glm::mat4 rotate_x_Matrix, glm::mat4 rotate_y_Matrix, glm::mat4 rotate_z_Matrix);
    glm::mat4 final_transformation(glm::mat4 worldMatrix, glm::mat4 localMatrix);

private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
    float translate= 0.0f;
    float scale = 0.0f;
    float rotate = 0.0f;
    float x_trans = 0.0f;
    float y_trans = 0.0f;
    float z_trans = 0.0f;
    float angleX = 0.0f;
    float angleY = 0.0f;
    float angleZ = 0.0f;
    float translate_world = 0.0f;
    float rotate_world = 0.0f;
    float xt_world = 0.0f;
    float yt_world = 0.0f;
    float zt_world = 0.0f;
    float angleX_world = 0.0f;
    float angleY_world = 0.0f;
    float angleZ_world = 0.0f;
};
