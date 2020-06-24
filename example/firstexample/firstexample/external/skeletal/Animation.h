#pragma once

#include <iostream>
#include <string>
#include <algorithm>

//define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class Animation
{
public:
    string name; //name of the animation

    float startTime; //start time of the animation
    float endTime; //end time of the animation
    float speed; //speed of the animation
    int priority; //priority of the animation
    bool loop; //is this animation looped

    Animation();
    Animation(string name, vec2 times, float speed = 0.25, int priority = 10, bool loop = false);

    void setName(string name);
    void setTime(vec2 frames);
    void setSpeed(float speed);
    void setPriority(int priority);
    void setLoop(bool loop);

    ~Animation();
};
