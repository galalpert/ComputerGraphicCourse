#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <iostream>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"

/**
 * Fields
 */


double zoomFactor = 1;
bool zoomChanged = false;

bool flat_flag = false;
bool phong_flag = true;
bool gouraud_flag = false;

bool cylindrical_flag = false;
bool plane_flag = true;
bool spherical_flag = false;

bool none_flag = true;
bool toon_flag = false;
bool normal_flag = false;
bool environment_flag = false;
bool show_demo_window = false;
bool show_another_window = false;

bool ortho_flag = true;
bool persp_flag = false;


glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
static float windowWidth = 1280, windowHeight = 720;

float Aspect;
bool flag;
float localScale, worldScale, localScaleCamera, worldScaleCamera, dolly_zoom;
glm::vec3 local_translate, world_translate, local_rotate, world_rotate, local_translateCamera, world_translateCamera, local_rotateCamera, world_rotateCamera;
glm::vec3 eye, at, up;
glm::vec4 model_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
std::vector<float> orthographic_param, perspective_param;



/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int windowWidth, int windowHeight, const char* windowName);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);
void initializeCamera(Scene& scene);
void initializeModel(MeshModel& model);
//void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);


/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    // TODO: Handle mouse scroll here
}

void initializeCamera(Scene& scene)
{
    localScaleCamera = scene.GetActiveCamera().getScaleLocal();
    local_translateCamera = scene.GetActiveCamera().getTranslateLocal();
    local_rotateCamera = scene.GetActiveCamera().getRotateLocal();
    
    worldScaleCamera = scene.GetActiveCamera().getScaleWorld();
    world_translateCamera = scene.GetActiveCamera().getTranslateWorld();
    world_rotateCamera = scene.GetActiveCamera().getRotateWorld();
    
    orthographic_param = scene.GetActiveCamera().GetOrthoParameters();
    perspective_param = scene.GetActiveCamera().GetPerspectiveParameters();
    
    eye = scene.GetActiveCamera().getEye();
    at = scene.GetActiveCamera().getAt();
    up = scene.GetActiveCamera().getUp();
    
    dolly_zoom = scene.GetActiveCamera().GetDollyZoom();
}

void initializeModel(MeshModel& model)
{
    localScale = model.getScaleLocal();
    local_translate = model.getTranslateLocal();
    local_rotate = model.getRotateLocal();
    
    world_translate = model.getTranslateWorld();
    worldScale = model.getScaleWorld();
    world_rotate = model.getRotateWorld();
    
    model_color = glm::vec4 (model.getColor(), 1.0f);
}


int main(int argc, char **argv)
{
    GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
    if (!window)return 1;
    

    ImGuiIO& io = SetupDearImgui(window);
//
//    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
//    glEnable(GL_DEPTH_TEST);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    int frameBufferWidth, frameBufferHeight;
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);


    Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
    Scene scene = Scene();
    scene.GetActiveCamera().SetOrthoParameters((float)frameBufferWidth, (float)frameBufferHeight);
    windowWidth = frameBufferWidth;
    windowHeight = frameBufferHeight;
        
    glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        // Poll and process events
        glfwPollEvents();
        
        // Imgui stuff
        StartFrame();
        DrawImguiMenus(io, scene);
        
        // Render scene
        RenderFrame(window, scene, renderer, io);
    }
        Cleanup(window);
        return 0;
    }

    static void GlfwErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }


GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
  {
      glfwSetErrorCallback(GlfwErrorCallback);
      if (!glfwInit())
          return NULL;
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      
      #if __APPLE__
          glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      #endif
      
      GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
      glfwMakeContextCurrent(window);
      glfwSwapInterval(1); // Enable vsync
                           // very importent!! initialization of glad
                           // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

      gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
      return window;
  }

    ImGuiIO& SetupDearImgui(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();
        // Setup style
        ImGui::StyleColorsDark();
//        glfwSetScrollCallback(window, glfw_OnMouseScroll);

        return io;
    }

    void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
    {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    zoomFactor = glm::pow(1.1, -yoffset);
    zoomChanged = true;
    }

    void StartFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
    {
        ImGui::Render();
        int frameBufferWidth, frameBufferHeight;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
        if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
               {
                   // TODO: Set new aspect ratio
                   windowWidth = frameBufferWidth;
                   windowHeight = frameBufferHeight;
               
                   //update window size in render
                   renderer.SetViewportWidth(windowWidth);
                   renderer.SetViewportHeight(windowHeight);
//                   renderer.ColorBufferUpdate();
                               
                   //update window size in all cameras
                   for (int i = 0; i < scene.GetCameraCount(); i++){
                       scene.GetCamera(i).SetOrthoParameters(windowWidth, windowHeight);
                   }

               }
        
        if (!io.WantCaptureKeyboard) {
            // TODO: Handle keyboard events here
            if(scene.GetModelCount()>0){
                //translate
                // using arrows
                if (io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]])
                {      // Up - upper arrow
                    scene.GetActiveModel().setLocalTranslate('y', scene.GetActiveModel().getTranslateLocal()[1] + 0.1);
                }
                if (io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]])
                {    // Down - down arrow
                    scene.GetActiveModel().setLocalTranslate('y', scene.GetActiveModel().getTranslateLocal()[1] - 0.1);
                }
                if (io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]])
                {   // Right - right arrow
                    scene.GetActiveModel().setLocalTranslate('x', scene.GetActiveModel().getTranslateLocal()[0] + 0.1);
                }
                if (io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]])
                {    // Left - left arrow
                    scene.GetActiveModel().setLocalTranslate('x', scene.GetActiveModel().getTranslateLocal()[0] - 0.1);
                }
                
                if (io.KeysDown[87] || io.KeysDown[119])
                {
                    scene.GetActiveModel().setRotateLocal('y', scene.GetActiveModel().getRotateLocal()[1] + 1);
                }
                if (io.KeysDown[83] || io.KeysDown[115])
                {
                    scene.GetActiveModel().setRotateLocal('y', scene.GetActiveModel().getRotateLocal()[1] - 1);
                }
                if (io.KeysDown[68] || io.KeysDown[100])
                {
                    scene.GetActiveModel().setRotateLocal('x', scene.GetActiveModel().getRotateLocal()[0] + 1);
                }
                if (io.KeysDown[65] || io.KeysDown[97])
                {
                    scene.GetActiveModel().setRotateLocal('x', scene.GetActiveModel().getRotateLocal()[0] - 1);
                }
            }
        }

        if (!io.WantCaptureMouse)
        // TODO: Handle mouse events here
        {
            if(scene.GetModelCount()>0)
            {
                if (io.MouseDown[0])
                {
                    scene.GetActiveModel().setScale(scene.GetActiveModel().getScaleLocal()+0.01);
                }
                
                if (io.MouseDown[1])
                {
                    if(scene.GetActiveModel().getScaleLocal() != 0)
                    {
                        scene.GetActiveModel().setScale(scene.GetActiveModel().getScaleLocal()-0.01);
                    }
                }
            }
        }
        
        // Clear the screen and depth buffer
        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.Render(scene);
        
        // Imgui stuff
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Swap front and back buffers
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    void Cleanup(GLFWwindow* window)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void DrawImguiMenus(ImGuiIO& io, Scene& scene)
    {
        /**
         * MeshViewer menu
         */
//        ImGui::Begin("MeshViewer Menu");
        
        // Menu Bar
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open", "CTRL+O"))
                {
                    nfdchar_t* outPath = NULL;
                    nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
                    if (result == NFD_OKAY)
                    {
                        scene.AddModel(Utils::LoadMeshModel(outPath));
                        free(outPath);
                    }
                    else if (result == NFD_CANCEL)
                    {
                    }
                    else
                    {
                    }

                }
         
                //added quick access to objects
             
                /*if (ImGui::MenuItem("Banana"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/banana.obj"));
                if (ImGui::MenuItem("Beethoven"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/beethoven.obj"));
                if (ImGui::MenuItem("Bishop"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/bishop.obj"));
                if (ImGui::MenuItem("Blob"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/blob.obj"));
                if (ImGui::MenuItem("Bunny"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/bunny.obj"));
                if (ImGui::MenuItem("Camera"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/camera.obj"));
                if (ImGui::MenuItem("Chain"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/chain.obj"));
                if (ImGui::MenuItem("Cow"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/cow.obj"));
                if (ImGui::MenuItem("Demo"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/demo.obj"));
                if (ImGui::MenuItem("Dolphin"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/dolphin.obj"));
                if (ImGui::MenuItem("Feline"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/feline.obj"));
                if (ImGui::MenuItem("Pawn"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/pawn.obj"));
                if (ImGui::MenuItem("Teapot"))
                    scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/teapot.obj"));*/

                if (ImGui::MenuItem("Banana"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/banana.obj"));
                if (ImGui::MenuItem("Beethoven"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/beethoven.obj"));
                if (ImGui::MenuItem("Bishop"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/bishop.obj"));
                if (ImGui::MenuItem("Blob"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/blob.obj"));
                if (ImGui::MenuItem("Bunny"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/bunny.obj"));
                if (ImGui::MenuItem("Camera"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/camera.obj"));
                if (ImGui::MenuItem("Chain"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/chain.obj"));
                if (ImGui::MenuItem("Cow"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/cow.obj"));
                if (ImGui::MenuItem("Demo"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/demo.obj"));
                if (ImGui::MenuItem("Dolphin"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/dolphin.obj"));
                if (ImGui::MenuItem("Feline"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/feline.obj"));
                if (ImGui::MenuItem("Pawn"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/pawn.obj"));
                if (ImGui::MenuItem("Teapot"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/teapot.obj"));
                if (ImGui::MenuItem("Horse"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/horse.obj"));
                if (ImGui::MenuItem("Nefertiti"))
                    scene.AddModel(Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/nefertiti.obj"));

                ImGui::EndMenu();
            }
            
            
            for (int i = 0; i < scene.GetModelCount(); i++)
            {
                MeshModel& model = scene.GetModel(i);
                initializeModel(model);

                ImGui::Begin((model.GetModelName()).c_str());
                
            
                // local controller
                ImGui::Text("Local Transformations");
                ImGui::SliderFloat("Scale ",&localScale, 1.0f, 10.0f);
                model.setScale(localScale);
                flag = flag || ImGui::IsItemActivated();

                ImGui::SliderFloat("X Translate", &local_translate[0], -windowWidth / 2.0f, windowWidth / 2.0f);
                model.setLocalTranslate('x', local_translate[0]);
                flag = flag || ImGui::IsItemActivated();


                ImGui::SliderFloat("Y Translate", &local_translate[1], -windowHeight / 2.0f, windowHeight / 2.0f);
                model.setLocalTranslate('y', local_translate[1]);
                flag = flag || ImGui::IsItemActivated();


                ImGui::SliderFloat("Z Translate", &local_translate[2], -1000.0f, 1000.0f);
                model.setLocalTranslate('z', local_translate[2]);
                flag = flag || ImGui::IsItemActivated();


                ImGui::SliderFloat("X axis Rotate", &local_rotate[0], 0, 360);
                model.setRotateLocal('x', local_rotate[0]);
                flag = flag || ImGui::IsItemActivated();

                ImGui::SliderFloat("Y axis Rotate", &local_rotate[1], 0.0f, 360.0f);
                model.setRotateLocal('y', local_rotate[1]);
                flag = flag || ImGui::IsItemActivated();

                ImGui::SliderFloat("Z axis Rotate", &local_rotate[2], 0.0f, 360.0f);
                model.setRotateLocal('z', local_rotate[2]);
                flag = flag || ImGui::IsItemActivated();

                ImGui::Separator();
                //world controller
                ImGui::Text("World Transformation");
                ImGui::SliderFloat("Scale world",&worldScale, 0.0f, 100.0f);
                model.setScaleWorld(worldScale);
                flag = flag || ImGui::IsItemActivated();

                ImGui::SliderFloat("X Translate World", &world_translate[0], -windowWidth / 2.0f, windowWidth / 2.0f);

                model.setWorldTranslte('x', world_translate[0]);
                flag = flag || ImGui::IsItemActivated();

                ImGui::SliderFloat("Y Translate World", &world_translate[1], -windowHeight / 2.0f, windowHeight / 2.0f);
                model.setWorldTranslte('y', world_translate[1]);
                flag = flag || ImGui::IsItemActivated();

                ImGui::SliderFloat("Z Translate World", &world_translate[2], 0.0f, 1000.0f);
                model.setWorldTranslte('z', world_translate[2]);
                flag = flag || ImGui::IsItemActivated();

                ImGui::SliderFloat("X axis Rotate World", &world_rotate[0], -360.0f, 360.0f);
                model.setRotateWorld('x', world_rotate[0]);
                flag = flag || ImGui::IsItemActivated();

                ImGui::SliderFloat("Y axis Rotate World", &world_rotate[1], -360.0f, 360.0f);
                model.setRotateWorld('y', world_rotate[1]);
                flag = flag || ImGui::IsItemActivated();

                ImGui::SliderFloat("Z axis Rotate World", &world_rotate[2], -360.0f, 360.0f);
                model.setRotateWorld('z', world_rotate[2]);
                flag = flag || ImGui::IsItemActivated();

                if (ImGui::CollapsingHeader("Materials"))
                {
                    Material& current_material = model.material;
                    ImGui::ColorEdit3("Ambient", (float*)&current_material.ambient);
                    ImGui::ColorEdit3("Diffuse", (float*)&current_material.diffuse);
                    ImGui::ColorEdit3("Specular", (float*)&current_material.specular);
                    ImGui::SliderFloat("Shininess", (float*)&current_material.shininess, 0, 5);
                }

                ImGui::Separator();

                ImGui::Text("Shader");
                ImGui::Checkbox("flat shading", &flat_flag); 
                ImGui::SameLine();
                if (flat_flag) {
                    gouraud_flag = false;
                    phong_flag = false;
                    scene.setActiveShader(0);
                }
                ImGui::Checkbox("Gouraud shading", &gouraud_flag); 
                if (gouraud_flag) {
                    flat_flag = false;
                    phong_flag = false;
                    scene.setActiveShader(1);
                }

                ImGui::Checkbox("phong", &phong_flag);
                if (phong_flag) {
                    flat_flag = false;
                    gouraud_flag = false;
                    scene.setActiveShader(2);
                }

                ImGui::Separator();
                ImGui::Checkbox("Plane", &plane_flag);
                if (plane_flag) {
                    cylindrical_flag = false;
                    spherical_flag = false;
                    scene.GetActiveModel().canonicalProjection = 0;
                }
                ImGui::Checkbox("Cylindrical", &cylindrical_flag);
                if (cylindrical_flag) {
                    plane_flag = false;
                    spherical_flag = false;
                    scene.GetActiveModel().canonicalProjection = 1;

                }
                ImGui::Checkbox("Spherical", &spherical_flag);
                if (spherical_flag) {
                    cylindrical_flag = false;
                    plane_flag = false;
                    scene.GetActiveModel().canonicalProjection = 2;
                }

                ImGui::Separator();
                ImGui::Checkbox("None", &none_flag); 
                if (none_flag) {
                    toon_flag = false;
                    normal_flag = false;
                    environment_flag = false;
                    scene.GetActiveModel().mapTex = 0;
                }

                ImGui::Checkbox("Toon", &toon_flag);
                if (toon_flag) {
                    none_flag = false;
                    normal_flag = false;
                    environment_flag = false;
                    scene.GetActiveModel().mapTex = 1;
                }

                ImGui::Checkbox("Normal", &normal_flag); 
                if (normal_flag) {
                    none_flag = false;
                    toon_flag = false;
                    environment_flag = false;
                    scene.GetActiveModel().mapTex = 2;
                }
                
           ImGui::Checkbox("Environment", &environment_flag); 
           if (environment_flag) {
               none_flag = false;
               normal_flag = false;
               toon_flag = false;
               scene.GetActiveModel().mapTex = 3;
           }
                
                
                ImGui::Separator();
               
                if(flag)
                {
                    scene.SetActiveModelIndex(i);
                }
                
                ImGui::End();
            }
            
            ImGui::EndMainMenuBar();
        }

//        // Controls
//        ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
//        // TODO: Add more controls as needed
//
//        ImGui::End();

        /**
         * Imgui demo - you can remove it once you are familiar with imgui
         */
        
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
         
            static float f = 0.0f;
            static int counter = 0;
            static int indexCamera = scene.GetActiveCameraIndex();
            
            
            initializeCamera(scene);
            ImGui::Begin("Camera");
            // update render's view port according to screen size
            if (ImGui::Button("Add Camera")){
                scene.AddCamera(std::make_shared<Camera>());
                int i = scene.GetCameraCount()-1;
                scene.GetCamera(i).SetOrthoParameters(windowWidth, windowHeight);
            }
            
            ImGui::Text("Active Camera Number: %d ", indexCamera);
            ImGui::SameLine();
            if ((ImGui::Button("<-")))
            {
                if (indexCamera >= 1){
                    scene.SetActiveCameraIndex(--indexCamera);
                    initializeCamera(scene);
                    if(scene.GetActiveCamera().GetProjectionFlag()){
                        ortho_flag = true;
                        persp_flag = false;
                    }
                    else{
                        ortho_flag = false;
                        persp_flag = true;
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("->"))
            {
                if (indexCamera < scene.GetCameraCount()-1){
                    scene.SetActiveCameraIndex(++indexCamera);
                    initializeCamera(scene);
                    
                    if(scene.GetActiveCamera().GetProjectionFlag())
                    {
                        ortho_flag = true;
                        persp_flag = false;
                    }
                    else
                    {
                        ortho_flag = false;
                        persp_flag = true;
                    }
                }
            }
            
            ImGui::Separator();
            ImGui::Checkbox("Orthographic", &ortho_flag); // Buttons return true when clicked (most widgets return
            ImGui::SameLine();
            if (ortho_flag){
                persp_flag = false;
                scene.GetActiveCamera().SetProjectionFlag(true);
            }
            else persp_flag = true;
            ImGui::Checkbox("Perspective", &persp_flag); // Buttons return true when clicked (most widgets return
            if (persp_flag){
                ortho_flag = false;
                scene.GetActiveCamera().SetProjectionFlag(false);
            }
            else ortho_flag = true;
            if (ImGui::CollapsingHeader("Projections"))
            {
                if (!ortho_flag)
                {
                    float dolly = dolly_zoom;
                    ImGui::Text("Perspective Projection");
                    ImGui::SliderFloat("fovy", &perspective_param[0], -360.0f, 360.0f);
                    scene.GetActiveCamera().setAnglePers(perspective_param[0]);
                    ImGui::SliderFloat("aspect", &perspective_param[1], 0.0f, 10.0f);
                    scene.GetActiveCamera().setAspectPers(perspective_param[1]);
                    ImGui::SliderFloat("near", &perspective_param[2], -1000.0f, 1000.0f);
                    scene.GetActiveCamera().setNearPers(perspective_param[2]);
                    ImGui::SliderFloat("far", &perspective_param[3], -1000.0f, 1000.0f);
                    scene.GetActiveCamera().setFarPers(perspective_param[3]);
                    ImGui::InputFloat("Dolly Zoom", &dolly_zoom, 1.0f, 1.0f, "%.2f");
                    if(dolly != dolly_zoom)
                    {
                        scene.GetActiveCamera().SetDollyZoom(dolly_zoom);
                    }
                }
                else
                {
                    ImGui::Text("Orthographic Projection");
                    ImGui::SliderFloat("left", &orthographic_param[0], -1000.0f, 1000.0f);
                    scene.GetActiveCamera().setUp(orthographic_param[0]);
                    ImGui::SliderFloat("right", &orthographic_param[1], -1000.0f, 1000.0f);
                    scene.GetActiveCamera().setDown(orthographic_param[1]);
                    ImGui::SliderFloat("down", &orthographic_param[2], -1000.0f, 1000.0f);
                    scene.GetActiveCamera().setLeft(orthographic_param[2]);
                    ImGui::SliderFloat("up", &orthographic_param[3], -1000.0f, 1000.0f);
                    scene.GetActiveCamera().setRight(orthographic_param[3]);
                    ImGui::SliderFloat("near", &orthographic_param[4], -1000.0f, 1000.0f);
                    scene.GetActiveCamera().setNear(orthographic_param[4]);
                    ImGui::SliderFloat("far", &orthographic_param[5], -1000.0f, 1000.0f);
                    scene.GetActiveCamera().setFar(orthographic_param[5]);
                }
            }

            if (ImGui::CollapsingHeader("Look AT")){
                if (ImGui::CollapsingHeader("Eye")){
                    float curreyez = eye[2];
                    ImGui::InputFloat("eye x", &eye[0], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().setEyeX(eye[0]);
                    ImGui::InputFloat("eye y", &eye[1], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().setEyeY(eye[1]);
                    ImGui::InputFloat("eye z", &eye[2], 10.0f, 10.0f, "%.2f");
                    if(curreyez != eye[2])
                        scene.GetActiveCamera().setEyeZ(eye[2]);
                }
                if (ImGui::CollapsingHeader("At")){
                    ImGui::InputFloat("at x", &at[0], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().setAtX(at[0]);
                    ImGui::InputFloat("at y", &at[1], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().setAtY(at[1]);
                    ImGui::InputFloat("at z", &at[2], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().setAtZ(at[2]);
                }
                if (ImGui::CollapsingHeader("Up")){
                    ImGui::InputFloat("up x", &up[0], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().setUpX(up[0]);
                    ImGui::InputFloat("up y", &up[1], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().setUpY(up[1]);
                    ImGui::InputFloat("up z", &up[2], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().setUpZ(up[2]);
                }
            }
               
            if (ImGui::CollapsingHeader("Camera Transformations"))
            {
                if (ImGui::CollapsingHeader("Camera Local Transformations"))
                {
                    ImGui::SliderFloat("Camera Scale local",& localScaleCamera, 1.0f, 100.0f);
                    scene.GetActiveCamera().setScale(localScaleCamera);
                    
                    ImGui::SliderFloat("Camera X Translate", &local_translateCamera[0], -windowWidth / 2.0f, windowWidth / 2.0f);
                    scene.GetActiveCamera().setLocalTranslate('x', local_translateCamera[0]);
                    
                    ImGui::SliderFloat("Camera Y Translate", &local_translateCamera[1], -windowHeight / 2.0f, windowHeight / 2.0f);
                    scene.GetActiveCamera().setLocalTranslate('y', local_translateCamera[1]);

                    ImGui::SliderFloat("Camera Z Translate", &local_translateCamera[2], -1000.0f, 1000.0f);
                    scene.GetActiveCamera().setLocalTranslate('z', local_translateCamera[2]);

                    ImGui::SliderFloat("Camera X axis Rotate", &local_rotateCamera[0], -360.0f, 360.0f);
                    scene.GetActiveCamera().setRotateLocal('x', local_rotateCamera[0]);

                    ImGui::SliderFloat("Camera Y axis Rotate", &local_rotateCamera[1], -360.0f, 360.0f);
                    scene.GetActiveCamera().setRotateLocal('y', local_rotateCamera[1]);

                    ImGui::SliderFloat("Camera Z axis Rotate", &local_rotateCamera[2], -360.0f, 360.0f);
                    scene.GetActiveCamera().setRotateLocal('z', local_rotateCamera[2]);
                }
                
                if (ImGui::CollapsingHeader("Camera World Transformations")){
             
                    ImGui::SliderFloat("Camera Scale World",&worldScaleCamera, 1.0f, 100.0f);
                    scene.GetActiveCamera().setScaleWorld(worldScaleCamera);
                   
                    ImGui::SliderFloat("Camera X Translate World", &world_translateCamera[0], -windowWidth / 2.0f, windowWidth / 2.0f);
                    scene.GetActiveCamera().setWorldTranslte('x', world_translateCamera[0]);

                    ImGui::SliderFloat("Camera Y Translate World", &world_translateCamera[1], -windowHeight / 2.0f, windowHeight / 2.0f);
                    scene.GetActiveCamera().setWorldTranslte('y', world_translateCamera[1]);

                    ImGui::SliderFloat("Camera Z Translate World", &world_translateCamera[2], -1000.0f, 1000.0f);
                    scene.GetActiveCamera().setWorldTranslte('z', world_translateCamera[2]);
                    
                    ImGui::SliderFloat("Camera X axis Rotate World", &world_rotateCamera[0], -360.0f, 360.0f);
                    scene.GetActiveCamera().setRotateWorld('x', world_rotateCamera[0]);

                    ImGui::SliderFloat("Camera Y axis Rotate World", &world_rotateCamera[1], -360.0f, 360.0f);
                    scene.GetActiveCamera().setRotateWorld('y', world_rotateCamera[1]);

                    ImGui::SliderFloat("Camera Z axis Rotate World", &world_rotateCamera[2], -360.0f, 360.0f);
                    scene.GetActiveCamera().setRotateWorld('z', world_rotateCamera[2]);
                }
            }
       
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }


        ImGui::Begin("Lights");
        if (scene.HasLights()) {
            ImGui::Text("Current Lights:");
            ImGui::SameLine();
        }
        else {
            ImGui::Text("Add Light");
        }
        if (ImGui::Button("Add Light"))
            scene.AddLight();
        
        size_t lights_count = scene.GetLightCount();
        for (int i = 0; i < lights_count; i++)
        {
            auto& light = scene.GetLightAtIndex(i);
            ImGui::PushID(i);
            if (ImGui::CollapsingHeader(scene.GetLightAtIndex(i).GetName().c_str()))
            {
                ImGui::ColorEdit3("Ambient light", (float*)&light.ambient);
                ImGui::ColorEdit3("Diffuse light", (float*)&light.diffuse);
                ImGui::ColorEdit3("Specular light", (float*)&light.specular);
                
                auto& pos = light.GetPosition();
                ImGui::SliderFloat("light X Position", (float*)&pos.x,-2560, 2560);
                ImGui::SliderFloat("light Y Position", (float*)&pos.y, -1440, 1440);
                ImGui::SliderFloat("light Z Position", (float*)&pos.z, -1000, 1000);
            }
            ImGui::PopID();
        }
        ImGui::End();
    }











