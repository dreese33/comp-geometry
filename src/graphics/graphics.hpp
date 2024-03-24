#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <vector>

#include "../vectors.hpp"

using namespace std;

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
      static void drawPoints(int shaderProgram, vector<shared_ptr<Vector2D>> vertices, int n) {
        // Vertex Buffer Object
        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        for (int i = 0; i < n; i++) {
          cout << "test " << vertices[i]->vector[0] << " " << vertices[i]->vector[1] << endl;
        }
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector2D), &vertices[0]->vector, GL_STATIC_DRAW);

        // Vertex Array Object
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // define stride, offset, etc for vertex rendering
        // we are enabling this for 2D at the moment
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector2D), (void*)0);
        glEnableVertexAttribArray(0);

        // optional configuration for OpenGL context for wireframe mode
        glPointSize(10);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); // default is GL_FILL - shows rectangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_POINTS, 0, n);
      }
  };
}

#endif