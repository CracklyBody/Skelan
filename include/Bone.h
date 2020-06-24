#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;
using namespace glm;
using namespace Assimp;

class Bone
{
public:
    string name; //the name of the bone
    int id; //bones id in the model

    aiNode* node; //the node this bone is attached to
    aiNodeAnim* nodeAnim; //this bone animation node

    Bone* parentBone; //parent bone
    mat4 offset; //offset matrix

    //keyframe data. This data is calculated in a particular period of time
    vec3 pos;
    quat rot;
    vec3 scal;


    Bone(int id, string name, mat4& offset);

    mat4 getParentTransforms(); //calculates the whole transformation matrix starting with the root bone 

    vec3 calcInterpolatedPosition(float time); //calculates interpolated position between two keyframes
    quat calcInterpolatedRotation(float time); //calculates interpolated rotation between two keyframes

    int findPosition(float time); //finds the index of the keyframe with the value less then time
    int findRotation(float time); //same as above

    void updateKeyframeTransform(float time); //updates this bone transformation matrix

    ~Bone();
};
