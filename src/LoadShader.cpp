#include "LoadShader.h"

ShaderLoader::ShaderLoader() {}

GLuint ShaderLoader::loadShaders(string vertex_path, string fragment_path)
{
    const char* vertex_file_path = vertex_path.c_str();
    const char* fragment_file_path = fragment_path.c_str();

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the smesh::Vertex ShaderLoader code from the file
    string VertexShaderCode;
    ifstream VertexShaderStream(vertex_file_path, ios::in);

    if (VertexShaderStream.is_open())
    {
        stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }
    else
    {
        printf("Impossible to open %s\n", vertex_file_path);
        getchar();
        return 0;
    }

    // Read the Fragment ShaderLoader code from the file
    string FragmentShaderCode;
    ifstream FragmentShaderStream(fragment_file_path, ios::in);

    if (FragmentShaderStream.is_open())
    {
        stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile smesh::Vertex ShaderLoader
    printf("Compiling shader : %s\n", vertex_file_path);

    char const* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check smesh::Vertex ShaderLoader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 0)
    {
        vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }


    // Compile Fragment ShaderLoader
    printf("Compiling shader : %s\n", fragment_file_path);

    char const* FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment ShaderLoader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 0)
    {
        vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }


    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();

    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 0)
    {
        vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }


    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    ID = ProgramID;
    return ProgramID;
}

void ShaderLoader::use()
{
    glUseProgram(ID);
}

ShaderLoader::~ShaderLoader() {}

void ShaderLoader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}
void ShaderLoader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);

}
void ShaderLoader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);

}
void ShaderLoader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);

}
void ShaderLoader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);

}
void ShaderLoader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);

}
void ShaderLoader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);

}
void ShaderLoader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);

}
void ShaderLoader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
}
void ShaderLoader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderLoader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);

}
void ShaderLoader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);

}