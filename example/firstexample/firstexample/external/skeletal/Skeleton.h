#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;
using namespace glm;
using namespace Assimp;

#define MAX_BONES_AMOUNT 100

class Skeleton
{
private:
    vector < Bone* > bones; //here we keep our bones
    vector < mat4 > bonesMatrices; //final transformation matrices that go to the shader

    float time; //little timer

    Animation* activeAnimation; //little Animation class to keep the animation data

    void renderBonesMatrices(ShaderLoader* shader); //update and send bones` matrices to the shader

public:
    Skeleton(vector < Bone* >& bones);

    void playAnimation(Animation* anim, bool reset = true); //play desired animation
    void stopAnimation(); //stop playing

    void update(ShaderLoader* shader); //here we update the animation and call updateSkeletonMatrices

    ~Skeleton();
};
