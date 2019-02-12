#include "shader.hpp"
#include <iostream>

namespace YUNIK_GTC {
    /*****************************************************************/
    /*                             Shader                            */
    /*****************************************************************/

    Shader::Shader (const GLchar* shaderSrc, const GLenum shaderType) {
        shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderSrc, NULL);
        glCompileShader(shader);
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

    ShaderProgram::ShaderProgram (const GLchar *shaderSrc_vertex, const GLchar *shaderSrc_fragment) {
        Shader* vertexShader = new Shader(shaderSrc_vertex, GL_VERTEX_SHADER);
        Shader* fragmentShader = new Shader(shaderSrc_fragment, GL_FRAGMENT_SHADER);

        program = glCreateProgram();
        glAttachShader(program, vertexShader->getShader());
        glAttachShader(program, fragmentShader->getShader());
        glLinkProgram(program);

        delete vertexShader;
        delete fragmentShader;
    }

    ShaderProgram::~ShaderProgram (void) {
        glDeleteProgram(program);
    }

    GLuint ShaderProgram::getProgram (void) {
        return program;
    }
}
