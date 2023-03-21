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
#include "MeshModel.h"

/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
bool flag;
float scale;
float xt;
float yt;
float zt;
float angleX;
float angleY;
float angleZ;

float xt_world;
float yt_world;
float zt_world;
float angleX_world;
float angleY_world;
float angleZ_world;

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

void PrintVertices(std::shared_ptr<MeshModel> model)
{
    for (int i = 0; i < model->GetVerticesCount(); i++)
    {
        std::cout << "v" << i << ": [";
        for(int j = 0; j < 3; j++)
        {
            std::cout << model->GetVertices(i)[j];
			if (j != 2) {
				std::cout << ", ";
			}
			
        }
        std::cout << "]" <<std::endl;
    }
}

void PrintFaces(std::shared_ptr<MeshModel> model)
{
    //print faces
    for(int i = 0; i < model->GetFacesCount(); i++)
    {
        std::cout << "f" << i << ": " << std::endl;

        for(int j = 0; j < 3; j++)
        {
            std::cout << "v" << j << ":" << model->GetFace(i).GetVertexIndex(j)  << "  ";
			std::cout << "vt" << j << ":" << model->GetFace(i).GetTextureIndex(j) << "  ";
			std::cout << "vn" << j << ":" << model->GetFace(i).GetNormalIndex(j) << std::endl;
		
        }
        std::cout << std::endl;
    }
}

int main(int argc, char **argv)
{
   /* std::shared_ptr<MeshModel> model;
    model = Utils::LoadMeshModel("C:/Users/galal/OneDrive/מסמכים/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/banana.obj");*/

    //print the mesh model data
    /*
    PrintVertices(model); //print vertices
    PrintFaces(model); //print faces
    */
  //  model->ScaleAndTranslteModel();
    
	
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
   /* scene.AddModel(model);*/
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
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
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
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
	}

    if (!io.WantCaptureKeyboard)
    {
        // TODO: Handle keyboard events here
        if (scene.GetModelCount() > 0) {

            //Translate - move the model using arrow keys
            if (io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]]) //Up arrow key moves the model up
            {
                scene.GetActiveModel().SetLocalY(scene.GetActiveModel().GetLocalY() + 1);
            }
            
            if (io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]])  //Down arrow key moves the model down
            {
                    scene.GetActiveModel().SetLocalY(scene.GetActiveModel().GetLocalY() - 1);
            }
            
            if (io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]]) //Right arrow key moves the model to the right
            {
                scene.GetActiveModel().SetLocalX(scene.GetActiveModel().GetLocalX() + 1);
            }
         
            if (io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]]) //Left arrow key moves the model to the left
            {
                    scene.GetActiveModel().SetLocalX(scene.GetActiveModel().GetLocalX() - 1);
            }


            //Scaling by the ENTER and BECKSAPCE keys
            if (io.KeysDown[io.KeyMap[ImGuiKey_Enter]]) //In order to magnify the model use ENTER key
            {
                scene.GetActiveModel().SetScale(scene.GetActiveModel().GetScale() + 1);
            }
            if (io.KeysDown[io.KeyMap[ImGuiKey_Backspace]]) //In order to diminish the model use the BACKSPACE key
            {
                
                if (scene.GetActiveModel().GetScale() != 0) //keep the scale positive
                {
                    scene.GetActiveModel().SetScale(scene.GetActiveModel().GetScale() - 1);
                }
            }

            
            //Rotate the mesh model using the W-A-S-D keys
            //y axis
            if (io.KeysDown[68]) //d - increase y angle
            {
                scene.GetActiveModel().SetLocalAngleY(scene.GetActiveModel().GetLocalAngleY() + 3);
            }
            if (io.KeysDown[65]) //a - decrease y angle
            {
                scene.GetActiveModel().SetLocalAngleY(scene.GetActiveModel().GetLocalAngleY() - 3);
            }

            //x axis
            if (io.KeysDown[87]) //w - increase x angle
            {
                scene.GetActiveModel().SetLocalAngleX(scene.GetActiveModel().GetLocalAngleX() + 3);
            }
            if (io.KeysDown[83]) //s - increase x angle
            {
                scene.GetActiveModel().SetLocalAngleX(scene.GetActiveModel().GetLocalAngleX() - 3);
            }
        }   
  }

    if (!io.WantCaptureMouse)
    {
        // TODO: Handle mouse events here
        if (!io.WantCaptureMouse)
            // TODO: Handle mouse events here
        {
            if (scene.GetModelCount() > 0)
            {
                // Left mouse button - move to the left
                if (io.MouseDown[0])
                {
                    scene.GetActiveModel().SetScale(scene.GetActiveModel().GetScale() + 1);
                }
                // Right mouse button - move to the right
                if (io.MouseDown[1])
                {
                    scene.GetActiveModel().SetScale(scene.GetActiveModel().GetScale() - 1);
                }
            }
        }
    }

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
	ImGui::Begin("MeshViewer Menu");
	
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
     
            if (ImGui::MenuItem("Banana"))
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
                scene.AddModel(Utils::LoadMeshModel("/Users/daniel_glazman/Documents/GitHub/computer-graphics-2023-daniel_glazman-gal_alpert/Data/teapot.obj"));

            
            
            ImGui::EndMenu();
        }
        
        for (int i = 0; i < scene.GetModelCount(); i++) {
            MeshModel& model = scene.GetModel(i);
            
            scale = model.GetScale();
            xt = model.GetLocalX();
            yt = model.GetLocalY();
            zt = model.GetLocalZ();
            angleX = model.GetLocalAngleX();
            angleY = model.GetLocalAngleY();
            angleZ = model.GetLocalAngleZ();
            
            xt_world = model.GetWorldX();
            yt_world = model.GetWorldY();
            zt_world = model.GetWorldZ();
            angleX_world = model.GetWorldAngleX();
            angleY_world = model.GetWorldAngleY();
            angleZ_world = model.GetWorldAngleZ();
            
            
            ImGui::Begin((model.GetModelName()).c_str());
            
            flag = ImGui::IsItemActivated();
            ImGui::Separator();
            
            ImGui::SliderFloat("Scale", &scale, 0.0f, 100.0f);
            model.SetScale(scale);
            flag = flag || ImGui::IsItemActivated();
            
            ImGui::Text("Local Transformation");
            
            ImGui::SliderFloat("X Translate ", &xt, 0.0f, 1000.0f);
            model.SetLocalX(xt);
            flag = flag || ImGui::IsItemActivated();
            ImGui::SliderFloat("Y Translate", &yt, 0.0f, 1000.0f);
            model.SetLocalY(yt);
            flag = flag || ImGui::IsItemActivated();
            ImGui::SliderFloat("Z Translate", &zt, 0.0f, 1000.0f);
            model.SetLocalZ(zt);
            flag = flag || ImGui::IsItemActivated();
            
            ImGui::SliderFloat("X axis Rotate", &angleX, -360.0f, 360.0f);
            model.SetLocalAngleX(angleX);
            flag = flag || ImGui::IsItemActivated();
            ImGui::SliderFloat("Y axis Rotate", &angleY, -360.0f, 360.0f);
            model.SetLocalAngleY(angleY);
            flag = flag || ImGui::IsItemActivated();
            ImGui::SliderFloat("Z axis Rotate", &angleZ, -360.0f, 360.0f);
            model.SetLocalAngleZ(angleZ);
            flag = flag || ImGui::IsItemActivated();
            
            
            ImGui::Text("World Transformation");
            
            ImGui::SliderFloat("X Translate World", &xt_world, 0.0f, 1000.0f);
            model.SetWorldX(xt_world);
            flag = flag || ImGui::IsItemActivated();
            ImGui::SliderFloat("Y Translate World", &yt_world, 0.0f, 1000.0f);
            model.SetWorldY(yt_world);
            flag = flag || ImGui::IsItemActivated();
            ImGui::SliderFloat("Z Translate World", &zt_world, 0.0f, 1000.0f);
            model.SetWorldZ(zt_world);
            flag = flag || ImGui::IsItemActivated();
            
            ImGui::SliderFloat("X axis Rotate World", &angleX_world, -360.0f, 360.0f);
            model.SetWorldAngleX(angleX_world);
            flag = flag || ImGui::IsItemActivated();
            ImGui::SliderFloat("Y axis Rotate World", &angleY_world, -360.0f, 360.0f);
            model.SetWorldAngleY(angleY_world);
            flag = flag || ImGui::IsItemActivated();
            ImGui::SliderFloat("Z axis Rotate World", &angleZ_world, -360.0f, 360.0f);
            model.SetWorldAngleZ(angleZ_world);
            flag = flag || ImGui::IsItemActivated();
            
            ImGui::Separator();
            
            if (flag)
            {
                scene.SetActiveModelIndex(i);
            }
            
            ImGui::End();
        }
        // TODO: Add more menubar items (if you want to)
        ImGui::EndMainMenuBar();
    }
    
    ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	ImGui::End();

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

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
        
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}
