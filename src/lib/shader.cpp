#include "shader.hpp"
#include <iostream>
#include "filesys.hpp"

#define YUNIK_GTC_INFOLOG_SIZE 512

namespace YUNIK_GTC {
    /*****************************************************************/
    /*                             Shader                            */
    /*****************************************************************/

    Shader::Shader (const char* shaderSrcPath, const GLenum shaderType) {
        const char* shaderSrc = (const char*)getFileContents(shaderSrcPath);
        shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderSrc, NULL);
        glCompileShader(shader);

        // Compile check
        int success;
        char infoLog[YUNIK_GTC_INFOLOG_SIZE];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, YUNIK_GTC_INFOLOG_SIZE, NULL, infoLog);
            std::cerr << "Error: Shader '" << shaderSrcPath << "' compilation failed\n" << infoLog << std::endl;
        }
    }

    Shader::~Shader (void) {
        glDeleteShader(shader);
    }

    GLuint Shader::getShader (void) {
        return shader;
    }

    /*****************************************************************/
    /*                            Program                            */
    /*****************************************************************/

    ShaderProgram::ShaderProgram (const char* shaderSrcPath_vertex, const char* shaderSrcPath_fragment) {
        Shader* vertexShader = new Shader(shaderSrcPath_vertex, GL_VERTEX_SHADER);
        Shader* fragmentShader = new Shader(shaderSrcPath_fragment, GL_FRAGMENT_SHADER);

        program = glCreateProgram();
        glAttachShader(program, vertexShader->getShader());
        glAttachShader(program, fragmentShader->getShader());
        glLinkProgram(program);

        // Compile check
        int success;
        char infoLog[YUNIK_GTC_INFOLOG_SIZE];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, YUNIK_GTC_INFOLOG_SIZE, NULL, infoLog);
            std::cerr << "Error: Shader program link failed\n" << infoLog << std::endl;
        }

        delete vertexShader;
        delete fragmentShader;
    }

    ShaderProgram::~ShaderProgram (void) {
        glDeleteProgram(program);
    }

    GLuint ShaderProgram::getProgram (void) {
        return program;
    }

    void ShaderProgram::use (void) {
        glUseProgram(program);
    }

    void ShaderProgram::setInt (const std::string& name, int value) {
        glUniform1i(glGetUniformLocation(program, name.c_str()), value);
    }

    void ShaderProgram::setFloat (const std::string& name, float value) {
        glUniform1f(glGetUniformLocation(program, name.c_str()), value);
    }
}
