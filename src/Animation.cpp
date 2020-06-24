#include "Animation.h"

using namespace std;
using namespace glm;

Animation::Animation()
{
    name = "unknown";

    startTime = 0;
    endTime = 0;
    speed = 0.25; //default is 0.25 ~ 24 frames per second
    priority = 10; //default priority is 10
    loop = false;
}

Animation::Animation(string name, vec2 times, float speed, int priority, bool loop)
{
    this->name = name;

    startTime = times.x; //starting frame
    endTime = times.y; //ending frame

    this->speed = speed;

    this->priority = priority;

    this->loop = loop;
}

void Animation::setName(string name)
{
    this->name = name;
}

void Animation::setTime(vec2 frames)
{
    startTime = frames.x;
    endTime = frames.y;
}

void Animation::setSpeed(float speed)
{
    this->speed = speed;
}

void Animation::setPriority(int priority)
{
    this->priority = priority;
}

void Animation::setLoop(bool loop)
{
    this->loop = loop;
}

Animation::~Animation() {}
