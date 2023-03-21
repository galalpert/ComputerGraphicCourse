## Assigment 1 part 3

# 1.

We implemented a camera with ortho projection and build GUI menu allowing the user to change the projection

## View volume before changes:
![52d7f73a-c223-49f0-8b5e-cd0ba297fcc3](https://user-images.githubusercontent.com/103389828/208489637-74c16943-36b5-42f1-b7b8-60239730cb8a.jpg)

## View volume after changes:
![923c8640-406a-4642-8cfe-708326cc95f8](https://user-images.githubusercontent.com/103389828/208489651-704a9b87-0b9a-4abf-9a9c-932d84c3c69b.jpg)


### Renderer Code
```cpp
void Renderer::Render(const Scene& scene)
{
    // TODO: Replace this code with real scene rendering code
    int half_width = viewport_width / 2;
    int half_height = viewport_height / 2;
    
    //draw mesh
    if (scene.GetModelCount() != 0)
    {
        glm::vec3 color(1, 0, 0);
        Camera camera = scene.GetActiveCamera();
        glm::vec3 eye (0.0f, 0.0f, 500.0f);
        glm::vec3 at (0.0f, 0.0f, 1.0f);
        glm::vec3 up (0.0f, 1.0f, 0.0f);
        camera.SetCameraLookAt(eye, at, up);
        camera.OrthoProjection(camera.getLeft(), camera.getRight(), camera.getDown(), camera.getUp(), camera.getNear(), camera.getFar());
        
        for (int i = 0; i < scene.GetModelCount(); i++)
        {
            MeshModel& model = scene.GetModel(i);
            

            glm::mat4 local = model.local_transformation(model.ScaleModel(model.GetScale()), model.TranslateModel(model.GetLocalX(), model.GetLocalY(), model.GetLocalZ()), model.RotateXModel(model.GetLocalAngleX()), model.RotateYModel(model.GetLocalAngleY()), model.RotateZModel(model.GetLocalAngleZ()));

            glm::mat4 world = model.world_transformation(model.ScaleModel(model.GetScale()), model.TranslateModel(model.GetWorldX(), model.GetWorldY(), model.GetWorldZ()), model.RotateXModel(model.GetWorldAngleX()), model.RotateYModel(model.GetWorldAngleY()), model.RotateZModel(model.GetWorldAngleZ()));

            glm::mat4 finalTrans = model.final_transformation(world, local);
            finalTrans = camera.GetProjectionTransformation() * camera.GetViewTransformation() * finalTrans;
           

            //draw
            for (int i = 0; i < model.GetFacesCount(); i++)
            {
                glm::vec4 p1_(model.GetVertices(model.GetFace(i).GetVertexIndex(0) - 1), 1.0f);
                glm::vec4 p2_(model.GetVertices(model.GetFace(i).GetVertexIndex(1) - 1), 1.0f);
                glm::vec4 p3_(model.GetVertices(model.GetFace(i).GetVertexIndex(2) - 1), 1.0f);

                p1_ = finalTrans * p1_;
                p2_ = finalTrans * p2_;
                p3_ = finalTrans * p3_;

                glm::vec3 p1(p1_);
                glm::vec3 p2(p2_);
                glm::vec3 p3(p3_);
                
                Normalize(p1);
                Normalize(p2);
                Normalize(p3);

                DrawLine(p1, p2, color);
                DrawLine(p1, p3, color);
                DrawLine(p2, p3, color);
            }
        }
    }
}
```

### Main Code:
```cpp
 //Camera GUI

    
        std::shared_ptr<Camera> cameraPtr(new Camera());
        scene.AddCamera(cameraPtr);

        Camera& camera = scene.GetCamera(0);

        left_ortho = camera.getLeft();
        right_ortho = camera.getRight();
        up_ortho = camera.getUp();
        down_ortho = camera.getDown();
        near_or = camera.getNear();
        far_or = camera.getFar();
        ImGui::Begin("Camera Menu");

        ImGui::Text("Orthographic Projection");
        ImGui::SliderFloat("Up", &up_ortho, -1000.0f, 1000.0f);
        camera.setUp(up_ortho);
        ImGui::SliderFloat("Down", &down_ortho, -1000.0f, 1000.0f);
        camera.setDown(down_ortho);
        ImGui::SliderFloat("Left", &left_ortho, -1000.0f, 1000.0f);
        camera.setLeft(left_ortho);
        ImGui::SliderFloat("Right", &right_ortho, -1000.0f, 1000.0f);
        camera.setRight(right_ortho);
        ImGui::SliderFloat("Near", &near_or, -1000.0f, 1000.0f);
        camera.setNear(near_or);
        ImGui::SliderFloat("Far", &far_or, -1000.0f, 1000.0f);
        camera.setFar(far_or);
        
        ImGui::End();
 ```

# 2.
We created a new GUI for the camera transformation in a similar way to the model transformation, mean as, world and local transformation.
In the renderer, we multiplied the inverse of the local-world matrix of the camera by the projection matrix and the model matrix we founded before.

## Moving in camera frame:
https://user-images.githubusercontent.com/103389828/208649997-b9dc7f87-a02c-423b-a80a-b3f3d6fa1d3a.mp4

## Moving in world frame:
https://user-images.githubusercontent.com/103389828/208650739-1375664e-ad52-471a-a653-186585f64745.mp4

```cpp

            glm::mat4 local = model.local_transformation(model.ScaleModel(model.GetScale()), model.TranslateModel(model.GetLocalX(), model.GetLocalY(), model.GetLocalZ()), model.RotateXModel(model.GetLocalAngleX()), model.RotateYModel(model.GetLocalAngleY()), model.RotateZModel(model.GetLocalAngleZ()));

            glm::mat4 world = model.world_transformation(model.ScaleModel(model.GetScale()), model.TranslateModel(model.GetWorldX(), model.GetWorldY(), model.GetWorldZ()), model.RotateXModel(model.GetWorldAngleX()), model.RotateYModel(model.GetWorldAngleY()), model.RotateZModel(model.GetWorldAngleZ()));

            glm::mat4 finalTrans = model.final_transformation(world, local);

            glm::mat4 localCamera = camera.Cameralocal_transformation(camera.CameraScaleModel(camera.CameraGetScale()), camera.CameraTranslateModel(camera.CameraGetLocalX(), camera.CameraGetLocalY(), camera.CameraGetLocalZ()), camera.CameraRotateXModel(camera.CameraGetLocalAngleX()), camera.CameraRotateYModel(camera.CameraGetLocalAngleY()), camera.CameraRotateZModel(camera.CameraGetLocalAngleZ()));

            glm::mat4 worldCamera = camera.Cameraworld_transformation(camera.CameraScaleModel(camera.CameraGetScale()), camera.CameraTranslateModel(camera.CameraGetWorldX(), camera.CameraGetWorldY(), camera.CameraGetWorldZ()), camera.CameraRotateXModel(camera.CameraGetWorldAngleX()), camera.CameraRotateYModel(camera.CameraGetWorldAngleY()), camera.CameraRotateZModel(camera.CameraGetWorldAngleZ()));

            glm::mat4 finalTransCamera = camera.Camerafinal_transformation(worldCamera, localCamera);

            finalTransCamera = glm::inverse(finalTransCamera);

            finalTrans = camera.GetProjectionTransformation() * finalTransCamera * finalTrans;
```

# 3.
We allowed the user to set the view volume automatically based on the window's size by looking for a change in the window and updating the orthographic projection parameters:

```cpp

if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
{
	window_width = frameBufferWidth;
        window_height = frameBufferHeight;
        renderer.SetViewportWidth(window_width);
        renderer.SetViewportHeight(window_height);
        renderer.UpdateBuffers();

       for (int i = 0; i < scene.GetCameraCount(); i++)
       {
           scene.GetCamera(i).setHeight(window_height);
           scene.GetCamera(i).setWidth(window_width);
           scene.GetCamera(i).setUp(window_height / 2);
           scene.GetCamera(i).setDown(-1 * (window_height / 2));
           scene.GetCamera(i).setLeft(-1 * (window_width / 2));
           scene.GetCamera(i).setRight(window_width / 2);
           scene.GetCamera(i).SetAspectPers(window_width / window_height);
        }
}
	
```

As sainity check we changed the size of the window and made sure the propotion of our modle remain the same:

![1](https://user-images.githubusercontent.com/101366820/208679598-194e0934-7f00-4296-a784-d60bd8c06d5a.png)
 ![2](https://user-images.githubusercontent.com/101366820/208679610-cb7ca306-d5bf-4eb3-affd-cc797d30ee55.png)
![3](https://user-images.githubusercontent.com/101366820/208679621-0c722ab6-95fe-4545-a3f2-5374af37a0f1.png)

    
# 4.
## For each model we draw the x, y and z axis for world frame and modle frame:

https://user-images.githubusercontent.com/103389828/208501146-32c19c3e-8845-425b-b9b1-c73a559bfa88.mp4

    
```cpp
    
       //local axis
            glm::vec4 axis_p1_local (0, 0, 0, 1.0);
            glm::vec4 x_axis_p2_local (10, 0, 0, 1.0);
            glm::vec4 y_axis_p2_local (0, 10, 0, 1.0);
            glm::vec4 z_axis_p2_local (0, 0, 10, 1.0);
            
            axis_p1_local = projectionMatrix * finalTransCamera * world * axis_p1_local;
            x_axis_p2_local = projectionMatrix * finalTransCamera * world * x_axis_p2_local;
            y_axis_p2_local = projectionMatrix * finalTransCamera * world * y_axis_p2_local;
            z_axis_p2_local = projectionMatrix * finalTransCamera * world * z_axis_p2_local;

            glm::vec3 axis_p1_local_(axis_p1_local);
            glm::vec3 x_axis_p2_local_(x_axis_p2_local);
            glm::vec3 y_axis_p2_local_(y_axis_p2_local);
            glm::vec3 z_axis_p2_local_(z_axis_p2_local);

            Normalize(axis_p1_local_);
            Normalize(x_axis_p2_local_);
            Normalize(y_axis_p2_local_);
            Normalize(z_axis_p2_local_);


            DrawLine(axis_p1_local_, x_axis_p2_local_, localAxisColor);
            DrawLine(axis_p1_local_, y_axis_p2_local_, localAxisColor);
            DrawLine(axis_p1_local_, z_axis_p2_local_, localAxisColor);


            //world axis

            glm::vec4 axis_p1(-1, -1, -1, 1.0);
            glm::vec4 x_axis_p2(1, -1, -1, 1.0);
            glm::vec4 y_axis_p2(-1, 1, -1, 1.0);
            glm::vec4 z_axis_p2(-1, -1, 1, 1.0);
           
            glm::mat4 axisWorldMatrix = projectionMatrix * finalTransCamera * model.ScaleModel(model.GetScale()) * model.ScaleModel(model.GetScale());
            axis_p1 = axisWorldMatrix * axis_p1;
            x_axis_p2 = axisWorldMatrix * x_axis_p2;
            y_axis_p2 = axisWorldMatrix * y_axis_p2;
            z_axis_p2 = axisWorldMatrix * z_axis_p2;

            glm::vec3 axis_p1_(axis_p1);
            glm::vec3 x_axis_p2_(x_axis_p2);
            glm::vec3 y_axis_p2_(y_axis_p2);
            glm::vec3 z_axis_p2_(z_axis_p2);

            Normalize(axis_p1_);
            Normalize(x_axis_p2_);
            Normalize(y_axis_p2_);
            Normalize(z_axis_p2_);

            DrawLine(axis_p1_, x_axis_p2_, worldAxisColor);
            DrawLine(axis_p1_, y_axis_p2_, worldAxisColor);
            DrawLine(axis_p1_, z_axis_p2_, worldAxisColor);
	    
```

            
# 5.
## We Show the difference between transforming a model in the world frame and in the model frame:
   We used two different transformation:
   T1 is rotating on the X axis.
   T2 is rotating on the Y axis.
   
  ### T1 in camera frame and T2 in the world frame:
  ![WhatsApp Image 2022-12-19 at 20 56 23](https://user-images.githubusercontent.com/103389828/208499618-10e0e7c7-1b73-498d-8a34-78374117943c.jpeg)
  
  ### T2 in the camera frame and T1 in world frame:
 ![3ccbf79b-8bbc-4209-881d-9ccd2df4dd53](https://user-images.githubusercontent.com/103389828/208650826-9810614c-f3ad-44d4-b729-750c19989964.jpg)

# 6. 
   We implemented and displayed the bounding box of the model in local and world coordinates. 
   When we transform the model in the local frame, we can see that the box is rotating according the the axis of the local frame. when we transform in the world frame,    the box and the object transform according to the world frame. 
   the differences can be seen here:

https://user-images.githubusercontent.com/101366820/208663823-3be2dc44-58ab-468d-a917-6075024afff0.mp4

   
# 7.
  We computed and displayed the face normals and vertex normals.
  As a sanity check for our implementation of face normals, and vertex normals we performed all possible transformaions. 
  As the  normals transformed with the model, we saw that our normals were correct.
   
  ### Vertex Normals:
  ![WhatsApp Image 2022-12-20 at 11 46 52](https://user-images.githubusercontent.com/103389828/208648143-1d84488d-1113-4206-ab47-884842db8c8f.jpeg)

  ![WhatsApp Image 2022-12-20 at 11 48 13](https://user-images.githubusercontent.com/103389828/208648153-8a54b30e-b9ee-415c-abf8-e1af65c1ceec.jpeg)
  
  ### Face Normals:
<img width="1277" alt="Screenshot 2022-12-20 at 13 22 57" src="https://user-images.githubusercontent.com/103389828/208655807-298b8eef-ffc2-439d-b617-62351fe824fa.png">

<img width="1276" alt="Screenshot 2022-12-20 at 13 23 31" src="https://user-images.githubusercontent.com/103389828/208655826-fee7b67b-8c2e-4e2e-9338-c720fb328968.png">


# 8.
 ### Perspective Projection:
  As sanity check we saw that when we chose it, it was possible to translate on the Z axis, which is not possible in the orthographic projection.
  In addition, we added to the camera GUI the option to change the perspective projection parameters.
  The difference between orthographic and perspective projection is showed in the screenshotes below:
  
 ### Orthographic Projection:
 ![0dae1b17-7765-4fcf-9346-b4a733e02303](https://user-images.githubusercontent.com/103389828/208501537-6c397fd8-73ec-4693-b0d0-7ba0a8e1c07d.jpg)

 ### Perspective Projection:
 ![ebfc35a0-a1ce-4d04-a59e-3f0ded6f60d6](https://user-images.githubusercontent.com/103389828/208501707-68e5a003-40b8-4405-b854-b5d77fcf98b6.jpg)
 
 
# 9.
  ### Setting the projection to perspective. 
  We show here the difference between changing the zoom and moving closer or farther from the model:
  
  https://user-images.githubusercontent.com/103389828/208502684-8a2dcbd3-231a-4bf5-a1a2-d019f34e9de1.mp4
  
# 10.
 Setting the camera position to a certian position and orient it towards the origin using LookAt. 
 We show that in the video below:
 
# 11. 
 ### Implementing the dolly zoom:
https://user-images.githubusercontent.com/103389828/208503218-ff4d1163-5a7d-4432-ae0d-14510cac7b46.mp4

# 12.
 ### We show here multiple cameras and models on the sceen:
 
 https://user-images.githubusercontent.com/103389828/208503511-460df590-d0ae-4609-9fb0-3a03d67f8e37.mp4
 
 ![WhatsApp Image 2022-12-19 at 21 25 42](https://user-images.githubusercontent.com/103389828/208504223-a750c40a-7b79-4a81-a6ea-88bcb40e58f8.jpeg)
 
![3fd1ca51-30d5-4a40-b7b5-64c653ca001c](https://user-images.githubusercontent.com/103389828/208504413-5091aa34-3eee-4a50-b70c-da2064a3e801.jpg)


# 13.
 ### Screenshot of the camera GUI:
 ![daaef8e2-fa2c-403f-ae60-d5ae05e9749a](https://user-images.githubusercontent.com/103389828/208503745-ea644058-2a52-4952-b2e2-3544b1736d35.jpg)







           
