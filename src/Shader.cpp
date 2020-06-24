#include "Shader.h"

GLuint create_shader_program_from_strings(const char* vertex_shader_str, const char* fragment_shader_str)
{
	assert(vertex_shader_str && fragment_shader_str);

	GLuint shader_program = glCreateProgram();
	GLuint vertex_shader_handle = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_handle = glCreateShader(GL_FRAGMENT_SHADER);

	// VERTEX SHADER COMPILE
	glShaderSource(vertex_shader_handle, 1, &vertex_shader_str, NULL);
	glCompileShader(vertex_shader_handle);
	int lparams = -1;
	glGetShaderiv(vertex_shader_handle, GL_COMPILE_STATUS, &lparams);
	if (lparams != GL_TRUE)
	{
		fprintf(stderr, "ERROR: vertex shader index %u did not compile\n", vertex_shader_handle);
		const int max_lenght = 2048;
		int actual_lenght = 0;
		char slog[2048];
		glGetShaderInfoLog(vertex_shader_handle, max_lenght, &actual_lenght, slog);
		fprintf(stderr, "shader info log for GL index %u:\n%s\n", vertex_shader_handle, slog);

		glDeleteShader(vertex_shader_handle);
		glDeleteShader(fragment_shader_handle);
		glDeleteProgram(shader_program);
		return 0;
	}

	// FRAGMENT SHADER COMPILE
	glShaderSource(fragment_shader_handle, 1, &fragment_shader_str, NULL);
	glCompileShader(fragment_shader_handle);
	lparams = -1;
	glGetShaderiv(fragment_shader_handle, GL_COMPILE_STATUS, &lparams);
	if (lparams != GL_TRUE)
	{
		fprintf(stderr, "ERROR: fragment shader index %u did not compile\n", fragment_shader_handle);
		const int max_lenght = 2048;
		int actual_lenght = 0;
		char slog[2048];
		glGetShaderInfoLog(fragment_shader_handle, max_lenght, &actual_lenght, slog);
		fprintf(stderr, "shader info log for GL index %u:\n%s\n", fragment_shader_handle, slog);

		glDeleteShader(vertex_shader_handle);
		glDeleteShader(fragment_shader_handle);
		glDeleteProgram(shader_program);
		return 0;
	}

	glAttachShader(shader_program, fragment_shader_handle);
	glAttachShader(shader_program, vertex_shader_handle);

	glLinkProgram(shader_program);
	glDeleteShader(vertex_shader_handle);
	glDeleteShader(fragment_shader_handle);
	lparams = -1;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &lparams);

	if (lparams != GL_TRUE)
	{
		fprintf(stderr, "ERROR: could not link shader program GL index %u\n",
			shader_program);

		const int max_length = 2048;
		int actual_length = 0;
		char plog[2048];
		glGetProgramInfoLog(shader_program, max_length, &actual_length, plog);
		fprintf(stderr, "program info log for GL index %u:\n%s", shader_program, plog);

		glDeleteProgram(shader_program);
		return 0;
	}
	return shader_program;
}

GLuint create_shader_program_from_files(const char* vertex_shader_filename, const char* fragment_shader_filename)
{

	assert(vertex_shader_filename && fragment_shader_filename);

	printf("loading shader from files '%s' and '%s'\n", vertex_shader_filename, fragment_shader_filename);

	char vs_shader_str[MAX_SHADER_SZ];
	char fs_shader_str[MAX_SHADER_SZ];
	vs_shader_str[0] = fs_shader_str[0] = '\0';
	// READ VERTEX SHADER INTO A BUFFER
	FILE* fp = fopen(vertex_shader_filename, "r");
	if (!fp)
	{
		fprintf(stderr, "ERROR: could not open vertex shader file '%s'\n", vertex_shader_filename);
		return 0;
	}
	size_t count = fread(vs_shader_str, 1, MAX_SHADER_SZ - 1, fp);
	assert(count < MAX_SHADER_SZ - 1);
	vs_shader_str[count] = '\0';
	fclose(fp);

	// READ VERTEX SHADER INTO A BUFFER
	fp = fopen(fragment_shader_filename, "r");
	if (!fp)
	{
		fprintf(stderr, "ERROR: could not open fragment shader file '%s'\n", vertex_shader_filename);
		return 0;
	}
	count = fread(fs_shader_str, 1, MAX_SHADER_SZ - 1, fp);
	assert(count < MAX_SHADER_SZ - 1);
	fs_shader_str[count] = '\0';
	fclose(fp);

	return create_shader_program_from_strings(vs_shader_str, fs_shader_str);
}


Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	this->Program = create_shader_program_from_files(vertexPath, fragmentPath);
}

void Shader::Use()
{
	glUseProgram(this->Program);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->Program, name.c_str()), value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->Program, name.c_str()), value);

}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->Program, name.c_str()), value);

}
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(this->Program, name.c_str()),1, &value[0]);

}
void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(this->Program, name.c_str()), x, y);

}
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(this->Program, name.c_str()), 1, &value[0]);

}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(this->Program, name.c_str()), x, y,z);

}
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(this->Program, name.c_str()), 1,&value[0]);

}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(this->Program, name.c_str()), x, y,z,w);
}
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(this->Program, name.c_str()),1,GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(this->Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);

}
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);

}