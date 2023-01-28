#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../vectors.hpp"

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

      /**
       * @brief Draws an array of points to the provided canvas, to be called in the programs main loop
       *
       * @param shaderProgram Takes into account both a vertex and fragment shader
       * @param vertices Vertices to draw
       * @param n Number of vertices
       */
      static void drawPoints(int shaderProgram, Vector2D *vertices, int n) {
        // Vertex Buffer Object
        unsigned int VBO;
        glGenBuffers(1, &VBO);

        // Vertex Array Object
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);

        // bind VAO and VBO to the vertex buffer
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(Vector2D), vertices, GL_STATIC_DRAW);

        // define stride, offset, etc for vertex rendering
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // optional configuration for OpenGL context for wireframe mode
        glPointSize(10);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); // default is GL_FILL - shows rectangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // n = 4?
        glDrawArrays(GL_POINTS, 0, n);
      }
  };
}

#endif