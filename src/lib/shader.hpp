#pragma once

#include <GL/glew.h>

namespace YUNIK_GTC {
    class Shader {
    private:
        GLuint shader;
    public:
        Shader (const GLchar* shaderSrc, const GLenum shaderType);
        ~Shader (void);
        GLuint getShader (void);
    };

    class ShaderProgram {
    private:
        GLuint program;
    public:
        ShaderProgram (const GLchar *shaderSrc_vertex, const GLchar *shaderSrc_fragment);
        ~ShaderProgram (void);
        GLuint getProgram (void);
    };
}
