#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Graphics {
  class Shaders {
    public:
      Shaders(unsigned int vertexShader, unsigned int fragementShader);
      unsigned int getShaderProgram();
      void setShaderProgram(unsigned int shaderProgram);
    private:
      unsigned int shaderProgram;
  };

  class GraphicsUtilities {
    public:
      /**
       * @brief Reads the contents of a shader file
       *
       * @param shader_file Path to file containing shader
       * @return std::string Contents of shader file
       */
      static std::string read_shader_file(const char *shader_file) {
        // no feedback is provided for stream errors / exceptions.
        std::ifstream file(shader_file);
        if (!file) return std::string ();

        file.ignore(std::numeric_limits<std::streamsize>::max());
        auto size = file.gcount();

        if (size > 0x10000) // 64KiB sanity check for shaders:
            return std::string ();

        file.clear();
        file.seekg(0, std::ios_base::beg);

        std::stringstream sstr;
        sstr << file.rdbuf();
        sstr << "\0";
        file.close();

        // std::cout << sstr.str();

        std::string shaderCode = sstr.str().c_str();
        return shaderCode;
      }

      /**
       * @brief Create a Shader object
       *
       * @param shaderSource GLSL code for shader to be rendered
       * @param shader OpenGL shader created with macro type specified
       */
      static void createShader(GLchar* shaderSource, unsigned int shader) {
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);

        // check for shader compilation errors:
        int  shaderSuccess;
        char vertexInfoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderSuccess);
        if (!shaderSuccess)
        {
          glGetShaderInfoLog(shader, 512, NULL, vertexInfoLog);
          std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << vertexInfoLog << std::endl;
        }
      }
  };
}

#endif