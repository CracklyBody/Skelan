#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL/stb_image_aug.h>
#include <SOIL/SOIL.h>
#include "loadshader.h"
#include "convert.h"
#include "animation.h"
#include "SMesh.h"
#include "bone.h"
#include "skeleton.h"
using namespace std;
using namespace glm;
using namespace Assimp;

class ModelLoader
{
private:
    vector < Bone* > bones; //here we load bones data
    vector < SMesh* > meshes; //here we load meshes data
    vector < smesh::Texture > textures_loaded; //here we keep the loaded textures to optimize resources
    string directory; //this is the directory where the model is kept
    Skeleton* skeleton;

    Importer import; //assimp`s importer
    const aiScene* scene; //here assimp stores all the loaded data

    vector < aiNode* > nodes; //we need this vector to store important information into bones in future. Actually in aiNode the transformation matrix of the particular bone is kept 
    vector < aiNodeAnim* > nodesAnim; //here the transformation animation data of the particular bone is kept

    void processNode(aiNode* node); //this method is needed to fill the nodes vector above 
    void processNodeAnim(); //this method is needed to fill the nodesAnim vector above
    void processBone(); //here we extract bones data from assimp into our format
    void processMeshes(aiNode* node); //extract meshes data
    SMesh* processMesh(aiMesh* mesh); //extract the single mesh data

    vector < smesh::Texture > loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName); //extract textures data
    unsigned int textureFromFile(const char* path); //load textures from file

    Bone* findBone(string name); //find the bone via it`s name
    int findBoneId(string name); //find bone`s index in bones vector via it`s name 
    aiNode* findAiNode(string name); //find aiNode via it`s name
    aiNodeAnim* findAiNodeAnim(string name); //find aiNodeAnim via it`s Name

public:
    ModelLoader();

    void loadModel(string path); //call this to load the model

    void getModelData(Skeleton*& skeleton, vector < SMesh* >& meshes); //this method is used to get the loaded data and then to store it into the gameobject

    ~ModelLoader();
};
