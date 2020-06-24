
#include "modelloader.h"

using namespace std;
using namespace glm;
using namespace Assimp;

ModelLoader::ModelLoader() {}

void ModelLoader::loadModel(string path)
{
    scene = import.ReadFile(path, aiProcess_Triangulate); //assimp loads the file

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) //if something gone wrong
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl; //error
        return;
    }

    directory = path.substr(0, path.find_last_of('/')); //get only the directory from the whole path to the file

    processNode(scene->mRootNode); //fill the nodes vector
    processNodeAnim(); //fill the nodesAnim vector
    processBone(); //get bones
    processMeshes(scene->mRootNode); //get meshes

    skeleton = new Skeleton(bones);
}

void ModelLoader::getModelData(Skeleton*& skeleton, vector < SMesh* >& meshes)
{
    skeleton = this->skeleton;
    meshes = this->meshes;
}

void ModelLoader::processNode(aiNode* node)
{
    nodes.push_back(node);

    for (size_t j = 0; j < node->mNumChildren; j++) //recursive loop through each child to fill the vector
    {
        processNode(node->mChildren[j]);
    }
}

void ModelLoader::processNodeAnim()
{
    if (scene->mNumAnimations == 0) //if there are no animations 
    {
        return;
    }

    //cout << scene->mNumAnimations << endl;

    for (size_t i = 0; i < scene->mAnimations[0]->mNumChannels; i++) //loop through the animation to get each bone animation data
    {
        nodesAnim.push_back(scene->mAnimations[0]->mChannels[i]);
    }
}

void ModelLoader::processBone()
{
    for (size_t i = 0; i < scene->mNumMeshes; i++) //loop through each mesh of the model
    {
        for (size_t j = 0; j < scene->mMeshes[i]->mNumBones; j++) //loop through each bone which is connected to that mesh
        {
            string boneName = scene->mMeshes[i]->mBones[j]->mName.data; //get bone`s name
            mat4 boneOffset = aiMatrix4x4ToGlm(scene->mMeshes[i]->mBones[j]->mOffsetMatrix); //get bone`s offset matrix. This is the matrix which transforms from the mesh space to the bone space

            Bone* bone = new Bone(bones.size(), boneName, boneOffset); //make new bone from the data we got (index, name, offset)

            bone->node = findAiNode(boneName); //each bone has its node with same name
            bone->nodeAnim = findAiNodeAnim(boneName); //same with the animation node

            if (!bone->nodeAnim) //if there is no animations for the bode. This often happens with root bones
            {
                cout << "ERROR::NO ANIMATIONS FOUND FOR " << boneName << endl;
            }

            bones.push_back(bone); //push back the bone
        }
    }

    for (size_t i = 0; i < bones.size(); i++) //here we are looking for the parent bone for our bones
    {
        string parentName = bones[i]->node->mParent->mName.data; //get the bone parent name. For each aiNode assimp keeps the parent pointer, as the bone has the same name as it`s aiNode we can do like that

        Bone* parentBone = findBone(parentName); //find the parent bone by it`s name

        bones[i]->parentBone = parentBone; //set the parent bone for the bone

        if (!parentBone) //if there is no parent bone
        {
            cout << "NO PARENT BONE FOR " << bones[i]->name << endl;
        }
    }

    //done
}

void ModelLoader::processMeshes(aiNode* node)
{
    //we start with the root node
    for (size_t i = 0; i < node->mNumMeshes; i++) //loop through each mesh this node has
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; //get the mesh from it`s index
        meshes.push_back(processMesh(mesh)); //here we call another function to extract the data and then push_back the complete mesh
    }

    for (size_t j = 0; j < node->mNumChildren; j++) //recursive loop through the each node
    {
        processMeshes(node->mChildren[j]);
    }
}

SMesh* ModelLoader::processMesh(aiMesh* mesh)
{
    vector < smesh::Vertex > vertices; //meshes vertices (position, normal, texture coords, bone ids, bone weights)
    vector < unsigned int > indices; //vertex indices for EBO
    vector < smesh::Texture > textures; //textures (id, type, path)

    for (size_t i = 0; i < mesh->mNumVertices; i++) //loop through each vertex in the mesh
    {
        smesh::Vertex vertex;
        vec3 helpVec3;

        helpVec3.x = mesh->mVertices[i].x; //positions
        helpVec3.y = mesh->mVertices[i].y;
        helpVec3.z = mesh->mVertices[i].z;

        vertex.position = helpVec3; //set position


        helpVec3.x = mesh->mNormals[i].x; //normals
        helpVec3.y = mesh->mNormals[i].y;
        helpVec3.z = mesh->mNormals[i].z;

        vertex.normal = helpVec3; //set normal


        if (mesh->mTextureCoords[0]) //if there is a texture
        {
            vec2 helpVec2;

            helpVec2.x = mesh->mTextureCoords[0][i].x; //textures coords
            helpVec2.y = mesh->mTextureCoords[0][i].y;

            vertex.texCoords = helpVec2; //set texture coords
        }
        else
        {
            vertex.texCoords = vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    //we have loaded the vertex data, its time for the bone data

    for (size_t i = 0; i < mesh->mNumBones; i++) //loop through each bone that mesh has
    {
        aiBone* bone = mesh->mBones[i];

        for (size_t j = 0; j < bone->mNumWeights; j++) //loop through the each bone`s vertex it carries in that mesh
        {
            aiVertexWeight vertexWeight = bone->mWeights[j]; //get the id and the weight of the carried vertex

            int startVertexID = vertexWeight.mVertexId; //get the carried vertex id

            for (int k = 0; k < BONES_AMOUNT; k++) //BONES_AMOUNT is a constant that is for the maximum amount of the bones per vertex
            {
                if (vertices[startVertexID].weights[k] == 0.0) //if we have an empty space for one more bone in the vertex
                {
                    vertices[startVertexID].boneIDs[k] = findBoneId(bone->mName.data); //set bone index to the vertex it carries

                    vertices[startVertexID].weights[k] = vertexWeight.mWeight; //set bone weight/strength to the vertex it carries

                    break; //only one place for the single bone
                }

                if (k == BONES_AMOUNT - 1) //if we have more that enough bones
                {
                    //cout << "ERROR::LOADING MORE THAN " << BONES_AMOUNT << " BONES\n"; //this could take a lot of time
                    break;
                }
            }
        }
    }

    //we have loaded everything for the vertices

    for (size_t i = 0; i < mesh->mNumFaces; i++) //loop through the mesh`s planes/faces
    {
        aiFace face = mesh->mFaces[i];

        //if face.mNumIndices == 3 we are loading triangles
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    //we have loaded indices, loading materials

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; //get the material by it`s index

    vector < smesh::Texture > diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse"); //calling another function to load diffuse textures
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end()); //instead of the loop we can insert vector like that

    vector < smesh::Texture > specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular"); //calling another function to load specular textures. We dont have lights in this app, but if you want to add some...
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    return new SMesh(vertices, indices, textures);
}

vector < smesh::Texture > ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector < smesh::Texture > textures; //vector for the particular texture type

    for (size_t i = 0; i < mat->GetTextureCount(type); i++) //loop through each typed texture
    {
        aiString helpStr;

        mat->GetTexture(type, i, &helpStr); //get texture`s name

        bool skip = false;

        for (size_t j = 0; j < textures_loaded.size(); j++) //loop through already loaded textures
        {
            if (strcmp(textures_loaded[j].path.data(), helpStr.C_Str()) == 0) //if we have already loaded it
            {
                textures.push_back(textures_loaded[j]); //use the loaded one instead of loading the new one

                skip = true;
                break;
            }
        }

        if (!skip) //if the texture is new
        {
            smesh::Texture texture;

            texture.id = textureFromFile(helpStr.C_Str()); //calling another function to load texture from file

            texture.type = typeName; //set the texture`s type

            cout << "smesh::Texture type: " << typeName << endl;

            texture.path = helpStr.C_Str(); //set the texture`s path

            textures.push_back(texture); //take the texture
            textures_loaded.push_back(texture); //remember the texture
        }
    }

    return textures;
}

unsigned int ModelLoader::textureFromFile(const char* path)
{
    string filename = string(path); //convert to string
    filename = directory + '/' + filename; //concatenate strings to get the whole path 

    unsigned int textureID;
    glGenTextures(1, &textureID); //gen texture, opengl function
    glBindTexture(GL_TEXTURE_2D, textureID); //bind the texture

    int W, H, comp; //width and height
    unsigned char* image = stbi_load(filename.c_str(), &W, &H, &comp, 4);
        //SOIL_load_image(filename.c_str(), &W, &H, 0, SOIL_LOAD_RGBA); //using SOIL to load the RGBA image

    if (image) //if the image is fine
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); //fill the texture with image data
        glGenerateMipmap(GL_TEXTURE_2D); //generate mipmaps 


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(image);
        //SOIL_free_image_data(image); //free the memory
    }
    else
    {
        cout << "Fail to load texture at path: " << path << endl;
        stbi_image_free(image);
        //SOIL_free_image_data(image);
    }

    return textureID;
}

Bone* ModelLoader::findBone(string name)
{
    for (size_t i = 0; i < bones.size(); i++) //just loop through the array of bones
    {
        if (bones[i]->name == name) //if name mathes the name
        {
            return bones[i]; //return bone
        }
    }

    return 0;
}

int ModelLoader::findBoneId(string name)
{
    for (size_t i = 0; i < bones.size(); i++) //just loop through the array of bones
    {
        if (bones[i]->name == name) //if name mathes the name
        {
            return bones[i]->id; //return id
        }
    }

    return -1;
}

aiNode* ModelLoader::findAiNode(string name)
{
    for (size_t i = 0; i < nodes.size(); i++) //loop through the array of nodes
    {
        if (nodes[i]->mName.data == name) //if node name mathes the name
        {
            return nodes[i]; //return node
        }
    }

    return 0;
}

aiNodeAnim* ModelLoader::findAiNodeAnim(string name)
{
    for (size_t i = 0; i < nodesAnim.size(); i++) //loop through the array of animation nodes
    {
        if (nodesAnim[i]->mNodeName.data == name) //if animation node name matches the name 
        {
            return nodesAnim[i]; //return animation node
        }
    }

    return 0;
}

ModelLoader::~ModelLoader() {}
