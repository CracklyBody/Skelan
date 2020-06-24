#include "loadshader.h"
#include "convert.h"
#include "SMesh.h"
#include "animation.h"
#include "bone.h"
#include "skeleton.h"

using namespace std;
using namespace glm;
using namespace Assimp;

Skeleton::Skeleton(vector < Bone* >& bones)
{
    this->bones = bones; //set bones

    time = 0; //time is 0

    activeAnimation = 0;
}

void Skeleton::playAnimation(Animation* anim, bool reset)
{
    if (activeAnimation) //if something is playing
    {
        if (anim->priority >= activeAnimation->priority) //if new animation has greater or equal priority
        {
            activeAnimation = anim; //change the animation
        }
    }
    else
    {
        activeAnimation = anim;
    }

    if (reset)
    {
        time = activeAnimation->startTime; //set timer to the beginning
    }
}

void Skeleton::stopAnimation()
{
    time = 0;
    activeAnimation = 0;
}

void Skeleton::renderBonesMatrices(ShaderLoader* shader)
{
    if (!bones.empty()) //if there are bones in the model
    {
        glUniform1i(glGetUniformLocation(shader->ID, "meshWithBones"), 1); //say the shader that there are some bones in the mesh

        bonesMatrices.clear(); //clear the values

        for (int i = 0; i < MAX_BONES_AMOUNT; i++) //loop through 100 bones
        {
            if (i >= int(bones.size())) //if we have run out of bones
            {
                bonesMatrices.push_back(mat4(1.0)); //there is no transformation matrix, so we leave it as 1.0
            }
            else
            {
                mat4 res = bones[i]->getParentTransforms() * aiMatrix4x4ToGlm(bones[i]->node->mTransformation); //calculate transformation matrix for our bone

                bonesMatrices.push_back(res * bones[i]->offset); //calculate full transformation matrix
            }

            glUniformMatrix4fv(glGetUniformLocation(shader->ID, ("bones[" + to_string(i) + "]").c_str()), 1, GL_FALSE, value_ptr(bonesMatrices[i])); //send the matrix to the shader
        }
    }
    else //if there arent any
    {
        glUniform1i(glGetUniformLocation(shader->ID, "meshWithBones"), 0); //say the shader that there are no bones in the mesh
    }
}

void Skeleton::update(ShaderLoader* shader)
{
    renderBonesMatrices(shader); //send bones` matrices to the shader

    if (!activeAnimation) //if there is no animation
    {
        return; //do nothing
    }

    time += activeAnimation->speed; //update the timer

    if (time < activeAnimation->startTime) //if time is less than starting time of the animation 
    {
        time = activeAnimation->startTime; //set time to the start
    }

    if (time > activeAnimation->endTime) //if the time is grater than the end time
    {
        if (activeAnimation->loop) //if animation is looped
        {
            time = activeAnimation->startTime; //set time to the start
        }
        else
        {
            stopAnimation(); //stop animation
        }
    }

    for (int i = 0; i < bones.size(); i++) //loop through each bone
    {
        bones[i]->updateKeyframeTransform(time); //calculate it`s tranformation matrix
    }
}

Skeleton::~Skeleton()
{
    for (int i = 0; i < bones.size(); i++)
    {
        delete bones[i];
    }

    delete activeAnimation;
}
