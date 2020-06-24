// firstexample.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define WIDTH 640
#define HEIGHT 480

#include <iostream>
#include "skeletal/GameObject.h"

int main()
{
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Example", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // Set up settings
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glfwWindowHint(GLFW_SAMPLES, 4);
    // Load Shader
    ShaderLoader* anim = new ShaderLoader();
    anim->loadShaders("anim.vert", "anim.frag");

    GameObject* object = new GameObject(); //create model 
    object->createGraphicsObject("wort/wort.fbx"); //get data from file
    object->playAnimation(new Animation("animexample", vec2(0, 190), 0.30, 10, true));
    glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 cameraFront = glm::vec3(0.f, 0.f, 1.f);
    glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);

    glm::mat4 trans = glm::mat4(1.f);
    trans = glm::translate(trans, glm::vec3(0.f, -3.f, 15.f));
    trans = glm::scale(trans, glm::vec3(0.05f));
    trans = glm::rotate(trans, glm::radians(180.f), glm::vec3(0.f, -1.f, 0.f));

    glm::mat4 view = glm::mat4(1.0f);

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glm::mat4 projection = glm::mat4(1.0f);

    projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 10000.0f);

    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        anim->use();
        anim->setMat4("view", view);
        anim->setMat4("projection", projection);
        anim->setMat4("model", trans);
        object->render(anim);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        glfwSwapBuffers(window);
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, 1);
        }
    }
    glfwTerminate();
}
