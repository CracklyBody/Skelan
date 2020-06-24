#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from)
{
	glm::mat4 to;

	to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1; to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
	to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2; to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
	to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3; to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
	to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4; to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

	return to;
}

static aiMatrix4x4 glmToAiMatrix4x4(glm::mat4& from)
{
	return aiMatrix4x4(from[0][0], from[1][0], from[2][0], from[3][0],
		from[0][1], from[1][1], from[2][1], from[3][1],
		from[0][2], from[1][2], from[2][2], from[3][2],
		from[0][3], from[1][3], from[2][3], from[3][3] );
}