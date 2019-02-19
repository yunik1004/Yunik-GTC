#pragma once

#include <GL/glew.h>

namespace YUNIK_GTC {
    class Shader {
    private:
        GLuint shader;
    public:
        Shader (const char* shaderSrcPath, const GLenum shaderType);
        ~Shader (void);
        GLuint getShader (void);
    };

    class ShaderProgram {
    private:
        GLuint program;
    public:
        ShaderProgram (const char* shaderSrcPath_vertex, const char* shaderSrcPath_fragment);
        ~ShaderProgram (void);
        GLuint getProgram (void);

        void use (void);
    };
}
