#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include <iostream>


Scene::Scene() :
    activeCameraIndex(0),
    activeModelIndex(0),
    activeLightIndex(0),
    nextLightId(1)
{
    model_color = glm::vec3(0.0f, 0.0f, 0.0f);
    cameras.push_back(std::make_shared<Camera>());
    AddLight();
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
    models.push_back(mesh_model);
}

int Scene::GetModelCount() const
{
    return models.size();
}

MeshModel& Scene::GetModel(int index) const
{
    return *models[index];
}

MeshModel& Scene::GetActiveModel() const
{
    
    return *models[activeModelIndex];
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
    cameras.push_back(camera);
}

int Scene::GetCameraCount() const
{
    return cameras.size();
}

Camera& Scene::GetCamera(int index)
{
    return *cameras[index];
}

Camera& Scene::GetActiveCamera() const
{
    return *cameras[activeCameraIndex];
}

void Scene::SetActiveCameraIndex(int index)
{
    activeCameraIndex = index;
}

int Scene::GetActiveCameraIndex() const
{
    return activeCameraIndex;
}

void Scene::SetActiveModelIndex(int index)
{
    activeModelIndex = index;
}

int Scene::GetActiveModelIndex() const
{
    return activeModelIndex;
}

void Scene::RemoveModel(int i)
{
    if (i > models.size() || i < 0){
        std::cout << "index model is not leagel" << std::endl;
        return;
    }
    models.erase(models.begin() + i);
    activeModelIndex = 0;
}

void Scene::RemoveCamera(int i){
    if (i > cameras.size() || i < 0){
        std::cout << "index camera is not leagel" << std::endl;
        return;
    }
    cameras.erase(cameras.begin() + i);
    activeCameraIndex = 0;
}

void Scene::AddLight()
{
    lights.emplace_back(std::make_shared<Light>(nextLightId++, glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Scene::AddLight(glm::vec3 position)
{
    lights.emplace_back(std::make_shared<Light>(nextLightId++, position));
}

int Scene::GetLightCount() const
{
    return lights.size();
}

Light& Scene::GetLightAtIndex(int index) const
{
    return *lights[index];
}

Light& Scene::GetActiveLight() const
{
    return *lights[activeLightIndex];
}

void Scene::SetActiveLightIndex(size_t index)
{
    activeLightIndex = index;
}

size_t Scene::GetActiveLightIndex() const
{
    return activeLightIndex;
}

void Scene::RemoveLight(size_t index)
{
    if (index < 0 || index > lights.size())
        return;

    lights.erase(lights.begin() + index);
    activeLightIndex = 0;
}

void Scene::ClearAllLights()
{
    lights.clear();
    activeLightIndex = 0;
    nextLightId = 1;
}

void Scene::setActiveShader(const int i)
{
    activeShader = shaders[i];
}
