#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LoadShader.h"
#define BONES_AMOUNT 6
namespace smesh {
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;

		float boneIDs[BONES_AMOUNT] = { 0.0f };
		float weights[BONES_AMOUNT] = { 0.0f };
	};

	struct Texture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};
}
class SMesh
{
private:
	GLuint VAO;
	GLuint VBO, EBO;

	std::vector<smesh::Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<smesh::Texture> textures;

	void setupSMesh();
public:
	SMesh(std::vector<smesh::Vertex>& v, std::vector<GLuint>& i, std::vector<smesh::Texture>& t);
	void draw(ShaderLoader* ShaderLoader);
	~SMesh();
};