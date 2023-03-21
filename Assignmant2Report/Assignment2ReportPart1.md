# Assignment 2 Part a 

## 1. Bounding Rectangles Of Mesh Model Triangles:
In Order to draw the bounding rectangles of each triangle, we calculated the minimum and maximum of x and y coordinates from the three points of the triangle. Afterwards we using the drawLine function we draw the lines of the rectangle - for each triangle of the model.
In addition, we found the minimum and maximum z coordinate of the triangle, in order to color the rectangle lines according to it's depth.

![900d3a11-b425-442d-bfdf-1384625df3be](https://user-images.githubusercontent.com/103389828/211146689-768ce97c-5311-49ed-b9e4-6fab50fee703.jpg)

![150b3248-c79c-4639-b6be-38185f11a6f1](https://user-images.githubusercontent.com/103389828/211146698-96f71542-92d1-4b33-b4c8-b09aa7df3421.jpg)


```cpp

void Renderer::DrawBoundingRectangleForTriangle(MeshModel& model, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    float min_x, max_x, min_y, max_y, min_z, max_z;
    float depthColorRange = 1;

    max_x = std::max(p1.x, p2.x);
    max_x = std::max(max_x, p3.x);
    min_x = std::min(p1.x, p2.x);
    min_x = std::min(min_x, p3.x);
    
    max_y = std::max(p1.y, p2.y);
    max_y = std::max(max_y, p3.y);
    min_y = std::min(p1.y, p2.y);
    min_y = std::min(min_y, p3.y);
    
    max_z = std::max(p1.z, p2.z);
    max_z = std::max(max_z, p3.z);
    min_z = std::min(p1.z, p2.z);
    min_z = std::min(min_z, p3.z);
    
    float z = (p1.z + p2.z + p3.z) / 3;
    depthColorRange = ((z - min_z) / (max_z - min_z));
    glm::vec3 rectColor(0, 1-depthColorRange, depthColorRange);
    
    glm::vec3 p1_rect(min_x, min_y, 1.0f);
    glm::vec3 p2_rect(max_x, min_y, 1.0f);
    glm::vec3 p3_rect(min_x, max_y, 1.0f);
    glm::vec3 p4_rect(max_x, max_y, 1.0f);

    DrawLine(p1_rect, p2_rect, rectColor);
    DrawLine(p1_rect, p3_rect, rectColor);
    DrawLine(p2_rect, p4_rect, rectColor);
    DrawLine(p3_rect, p4_rect, rectColor);
}

```

## 3. Edge Walking Algorithm:
We chose to implement the Edge Walking algorithm to fill the triangles.

![758bbbec-7e92-42a1-b695-d2ea8cd31e98](https://user-images.githubusercontent.com/103389828/211146838-f1fb3bcc-c4c9-43f4-b7f3-755b825e8dbe.jpg)

```cpp
void Renderer::edgeWalkingAlgo(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 trianglesColor)
{
    std::vector<glm::vec2> points = { p1,p2,p3 };

    // Sort the points
    std::sort(points.begin(), points.end(), pointsCompareByY);

    float z = (p1.z + p2.z + p3.z) / 3;

    float y1 = points[0][1];
    float y2 = points[1][1];
    float y3 = points[2][1];

    //calc the slopes 
    float slope1 = (points[0][1] - points[1][1]) / (points[0][0] - points[1][0]);
    float slope2 = (points[1][1] - points[2][1]) / (points[1][0] - points[2][0]);
    float slope3 = (points[2][1] - points[0][1]) / (points[2][0] - points[0][0]);

    //calc the free element in the equation
    float n1 = points[0][1] - (slope1 * points[0][0]);
    float n2 = points[1][1] - (slope2 * points[1][0]);
    float n3 = points[2][1] - (slope3 * points[2][0]);

    int leftX, rightX;

    // draw hightest point to middle point
    for (float y = y2; y < y3; y++)
    {
        if ((points[1][0] - points[2][0]) == 0)
        {
            leftX = points[1][0];
        }    
        else
        {
            leftX = (y - n2) / slope2;
        }
        
        if ((points[2][0] - points[0][0]) == 0)
        {
            rightX = points[0][0];
        }    
        else
        {
            rightX = (y - n3) / slope3;
        }
        
        glm::vec2 point1(leftX,(int)y);
        glm::vec2 point2(rightX,(int)y);
        DrawLine(point1, point2, trianglesColor, z);
    }

    // draw middle to lowest point
    for (float y = y1; y < y2; y++)
    {
        if ((points[0][0] - points[1][0]) == 0)
        {
            leftX = points[1][0];
        }    
        else
        {
            leftX = (y - n1) / slope1;
        }    
            
        if ((points[2][0] - points[0][0]) == 0)
        {
            rightX = points[0][0];
        }    
        else
        {
            rightX = (y - n3) / slope3;
        }    
            
        glm::vec2 point1(leftX,(int)y);
        glm::vec2 point2(rightX,(int)y);
        DrawLine(point1, point2, trianglesColor, z);
    }
}

```

## 4. Z-Buffer Algorithm:
We implemented the Z-Buffer algorithm. We saved in the buffer the z coordinate (depth) of each pixel and colored them according to the depth. 
Comparison between edge walking and Z-Buffer coloring:

![6151e81a-1441-40c6-a1fd-30b539b0f933](https://user-images.githubusercontent.com/103389828/211147208-1c6d9458-b3e0-4b51-a020-90d871031240.jpg)

![ccecee9c-7831-4a8f-8aa9-f2909146c8d8](https://user-images.githubusercontent.com/103389828/211147215-d64932c1-b9fd-4f59-9439-c57a4a30be77.jpg)

![2445c06d-2aaf-4cf6-adc1-17aaee1f98b5](https://user-images.githubusercontent.com/103389828/211147227-69128141-85be-42b2-8735-5d67d7f967b6.jpg)

![f5667ef4-cab7-4d68-b1e4-e28ab4c8066f](https://user-images.githubusercontent.com/103389828/211147231-e42b73a5-e882-448c-9167-3c32796d1552.jpg)




