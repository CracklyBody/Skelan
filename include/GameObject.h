#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "loadshader.h"
#include "animation.h"
#include "mesh.h"
#include "bone.h"
#include "skeleton.h"
#include "modelloader.h"
using namespace std;
using namespace glm;

class GameObject
{
private:
    ModelLoader* modelLoader; //class to import models from files

    vector < SMesh* > meshes; //here we keep meshes data
    Skeleton* skeleton; //and a skeleton

    mat4 localTransform;

public:
    GameObject();

    void createGraphicsObject(string path); //method to load the model and create a graphical object

    void playAnimation(Animation* anim, bool reset = true); //start the animation
    void stopAnimation(); //stop the animation

    //here we can adjust the model. Often loaded models are strangely rotated or scaled, we can manualy tweak them here
    void applyLocalRotation(float angle, vec3 axis);
    void applyLocalPosition(vec3 translation);
    //void setLocalScale(vec3 sc);
    //void setLocalPosition(vec3 pos);

    void render(ShaderLoader* shader); //render the model to the shader

    ~GameObject();
};
