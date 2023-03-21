#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "Light.h"
using namespace std;

class Scene {
public:
    Scene();
    
    //model functions
    void AddModel(const shared_ptr<MeshModel>& mesh_model);
    int GetModelCount() const;
    MeshModel& GetModel(int index) const;
    MeshModel& GetActiveModel() const;
    
    //camera functions
    void AddCamera(const shared_ptr<Camera>& camera);
    int GetCameraCount() const;
    Camera& GetCamera(int index);
    Camera& GetActiveCamera() const;
    void SetActiveCameraIndex(int index);
    int GetActiveCameraIndex() const;
    void SetActiveModelIndex(int index);
    int GetActiveModelIndex() const;

    //light functions
    void AddLight();
    void AddLight(glm::vec3 position);
    int GetLightCount() const;
    Light& GetLightAtIndex(int index) const;
    Light& GetActiveLight() const;
    void SetActiveLightIndex(size_t index);
    size_t GetActiveLightIndex() const;
    bool HasLights() const { return !lights.empty(); }
    void RemoveLight(size_t index);
    void ClearAllLights();
    void setActiveShader(const int i);
    glm::vec3 model_color;


    void RemoveModel(int i);
    void RemoveCamera(int i);
    

    private:
    vector<shared_ptr<MeshModel>> models;
    vector<shared_ptr<Camera>> cameras;
    vector<shared_ptr<Light>> lights;
    int activeCameraIndex;
    int activeModelIndex;
    size_t activeLightIndex;
    int nextLightId;
    string activeShader;
    string shaders[3] = {"FLAT","GOURAUD", "PHONG"};
};
