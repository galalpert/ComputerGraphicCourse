# Assignment 2 - Part 2

## 1. Ambient Light
In order to illuminate the model in Ambient light we multiplied the ambient color light intensity vector with the ambiet color of the model.
For the first example, we chose white ambient light color and burgundy ambient color of the model. We can see that the final color of the model is 
burgundy color.

![45992b4f-1680-43c1-93ee-96a6e50a99d8](https://user-images.githubusercontent.com/103389828/213721509-e997dc4f-f259-4817-b935-4f356b9c6470.jpg)

For another example we chose green ambient light and burgundy ambient color of the model. This combination causes the model to appear black because the
multiplication gives a zero vector (black).

<img width="1283" alt="Screenshot 2023-01-20 at 16 23 50" src="https://user-images.githubusercontent.com/103389828/213722105-7f009049-6d49-42f4-8587-71df9bed5a28.png">

## 2. Diffuse Component With Flat Shading
I then computed the diffuse component using flat shading.
In the following example we used a white ambient and white diffuse light.

![flatShading](https://user-images.githubusercontent.com/103389828/214249142-8290fa9c-10e8-4b28-951e-5cd8501bed8b.jpg)


## 3. Gouraud Shading 

![GouraudShading](https://user-images.githubusercontent.com/103389828/214249155-518f4b88-c364-4956-aaab-2dfcea37710d.jpg)


## 4. Specular Lighting - Light Normals & Reflection Normals
In the first example we show the light direction normals when the light position is at (0,0,0).

<img width="1282" alt="light_direction_normals" src="https://user-images.githubusercontent.com/103389828/214258332-923b912b-2401-42fd-b14c-7672ae5ea4ff.png">

In the second example we show the light direction normals when the light position is at the upper left corner of the screen.

<img width="1285" alt="light_direction_normals_diffDirection" src="https://user-images.githubusercontent.com/103389828/214258754-980b57c1-71ad-4e6f-bde6-217016f90a4c.png">

Here we show the light reflection normals (in yellow)

<img width="1282" alt="reflection_normals" src="https://user-images.githubusercontent.com/103389828/214258946-4773c8d3-e39c-4774-8267-6350bcdfd8a5.png">


## 5. Specular Light With Phong Shading
We calculated the specular light and added it to the ambient and diffuse light in order to create the Phong shading.

![b496365f-3ff7-448c-b4e0-6702ad6cb0d3](https://user-images.githubusercontent.com/103389828/214341109-8c81870f-f0ec-4e5b-b120-f4fc3421b3be.jpg)


## 6. Multiple Lights And Shadings
We added the option to add multuiple lights to the screen.

![c03652d4-bc8c-41ad-a172-58c1e3076aec](https://user-images.githubusercontent.com/103389828/214341365-3f2ead25-e0e0-4019-9ec9-f19838325da5.jpg)

![bfafdcc4-5c54-43ce-a4d9-cd14623bec93](https://user-images.githubusercontent.com/103389828/214341414-0ce95b89-f281-43bb-a18b-f9ec04098fc6.jpg)

