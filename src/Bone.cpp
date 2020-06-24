#include "animation.h"
#include "convert.h"
#include "loadshader.h"
#include "mesh.h"
#include "bone.h"

using namespace std;
using namespace glm;
using namespace Assimp;

Bone::Bone(int id, string name, mat4& offset)
{
    this->id = id; //set id
    this->name = name; //set name
    this->offset = offset; //set offset matrix

    //null pointers
    parentBone = 0;
    node = 0;
    nodeAnim = 0;
}

mat4 Bone::getParentTransforms()
{
    Bone* b = parentBone; //this bone`s parent

    vector < mat4 > matrices; //vector to remember matrices

    while (b) //loop through all parents to push_back all transformation matrices
    {
        mat4 tmp = aiMatrix4x4ToGlm(b->node->mTransformation); //get transformation matrix
        matrices.push_back(tmp); //remember it

        b = b->parentBone; //next parent
    }

    mat4 res = mat4(1.0f); //all parent transformation matrix

    for (int i = matrices.size() - 1; i >= 0; i--) //loop backward! The last element of the array is the root bone 
    {
        res *= matrices[i]; //here we calculate the combined transformation matrix
    }

    return res;
}

vec3 Bone::calcInterpolatedPosition(float time)
{
    vec3 out; //result

    if (nodeAnim->mNumPositionKeys == 1) //if amount of keyframes == 1 we dont have to iterpolate anything
    {
        aiVector3D help = nodeAnim->mPositionKeys[0].mValue; //get its value/position

        out = vec3(help.x, help.y, help.z);

        return out;
    }

    //if there is more than one keyframe
    int positionIndex = findPosition(time); //find index of the keyframe
    int nextPositionIndex = positionIndex + 1; //the next index

    float deltaTime = (float)(nodeAnim->mPositionKeys[nextPositionIndex].mTime - nodeAnim->mPositionKeys[positionIndex].mTime); //calculate time difference between keyframes 
    float factor = (time - (float)nodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime; //calculate how far we are between keyframes in the interval of [0;1]

    aiVector3D begin = nodeAnim->mPositionKeys[positionIndex].mValue; //get the first keyframe value
    aiVector3D end = nodeAnim->mPositionKeys[nextPositionIndex].mValue; //get the second one

    vec3 p1(begin.x, begin.y, begin.z);
    vec3 p2(end.x, end.y, end.z);

    out = mix(p1, p2, factor); //using glm function to mix the values dependent on factor

    //cout << out.x << ' ' << out.y << ' ' << out.z << endl << endl;

    return out;
}

quat Bone::calcInterpolatedRotation(float time)
{
    quat out; //result

    if (nodeAnim->mNumRotationKeys == 1) //if amount of keyframes == 1 we dont have to iterpolate anything
    {
        aiQuaternion help = nodeAnim->mRotationKeys[0].mValue; //get it`s value/rotation quaternion

        out = quat(help.w, help.x, help.y, help.z);
        return out;
    }

    //if there is more than one keyframe
    int rotationIndex = findRotation(time); //find index of the keyframe
    int nextRotationIndex = rotationIndex + 1; //the next index

    float deltaTime = (float)(nodeAnim->mRotationKeys[nextRotationIndex].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime); //calculate time difference between keyframes
    float factor = (time - (float)nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;  //calculate how far we are between keyframes in the interval of [0;1]

    aiQuaternion begin = nodeAnim->mRotationKeys[rotationIndex].mValue;
    aiQuaternion end = nodeAnim->mRotationKeys[nextRotationIndex].mValue;

    quat r1(begin.w, begin.x, begin.y, begin.z);
    quat r2(end.w, end.x, end.y, end.z);

    //up to this point everything is the same as for position

    out = slerp(r1, r2, factor); //here we use another glm function to mix the quternion values dependen on factor

    //cout << out.x << ' ' << out.y << ' ' << out.z << endl << endl;

    return out;
}

int Bone::findPosition(float time)
{
    for (size_t i = 0; i < nodeAnim->mNumPositionKeys - 1; i++) //loop through each position frame
    {
        if (time < nodeAnim->mPositionKeys[i + 1].mTime) //if the given time is less than in a keyframe
        {
            return i;
        }
    }

    return -1;
}

int Bone::findRotation(float time)
{
    for (size_t i = 0; i < nodeAnim->mNumRotationKeys - 1; i++) //loop through each rotation keyframe
    {
        if (time < nodeAnim->mRotationKeys[i + 1].mTime) //if the given time is less then in a keyframe
        {
            return i;
        }
    }

    return -1;
}

void Bone::updateKeyframeTransform(float time)
{
    if (!nodeAnim) //if there is no animations for this bone
    {
        return;
    }

    scal = vec3(1.0); //we don`t have scaling animations
    pos = calcInterpolatedPosition(time); //calculate interpolated position
    rot = calcInterpolatedRotation(time); //calculate interpolated rotation

    mat4 res(1.0);

    //here we construct the transformation matrix in STR order
    res *= scale(scal);
    res *= translate(pos);
    res *= mat4_cast(rot);

    node->mTransformation = glmToAiMatrix4x4(res); //mTransformation is public variable so we can store there our new transformation matrix
}

Bone::~Bone() {}
