#include "gameobject.h"

GameObject::GameObject()
{
    modelLoader = new ModelLoader();

    localTransform = mat4(1.0);
}

void GameObject::createGraphicsObject(string path)
{
    modelLoader->loadModel(path); //load the model from the file

    modelLoader->getModelData(skeleton, meshes); //get the loaded data and store it in this class
}

void GameObject::playAnimation(Animation* anim, bool reset)
{
    skeleton->playAnimation(anim, reset); //play animation
}

void GameObject::stopAnimation()
{
    skeleton->stopAnimation(); //stop animation
}

void GameObject::applyLocalRotation(float angle, vec3 axis)
{
    vec3 sc;
    quat rot;
    vec3 tran;
    vec3 skew;
    vec4 perspective;

    decompose(localTransform, sc, rot, tran, skew, perspective);

    localTransform = mat4(1.0);
    localTransform *= translate(tran);
    localTransform *= scale(sc);
    localTransform *= rotate(radians(angle), axis) * mat4_cast(conjugate(rot));
}

void GameObject::applyLocalPosition(vec3 translation)
{
    vec3 sc;
    quat rot;
    vec3 tran;
    vec3 skew;
    vec4 perspective;

    decompose(localTransform, sc, rot, tran, skew, perspective);

    localTransform = mat4(1.0);
    localTransform *= translate(translation) * translate(tran);
    localTransform *= scale(sc);
    localTransform *= mat4_cast(conjugate(rot));
}

void GameObject::render(ShaderLoader* shader)
{
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "localTransform"), 1, GL_FALSE, value_ptr(localTransform));

    skeleton->update(shader); //rendering the skeleton part

    for (int i = 0; i < meshes.size(); i++) //loop through the meshes
    {
        meshes[i]->draw(shader); //rendering the mesh part
    }
}

GameObject::~GameObject()
{
    delete modelLoader;

    for (int i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }

    delete skeleton;
}
