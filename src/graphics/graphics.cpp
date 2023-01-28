#include "graphics.hpp"

namespace Graphics {
  /**
   * @brief Create a Shader Program object containing both a vertex and a fragment shader
   * Sets private shaderProgram variable upon initialization
   *
   * @param vertexShader Vertex shader associated w/ object
   * @param fragmentShader Fragment shader associated w/ object
  */
  Shaders::Shaders(unsigned int vertexShader, unsigned int fragmentShader) {
    // create a new shader object via vertex/fragment shader components
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int shaderProgramSuccess;
    char shaderProgramInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgramSuccess);
    if (!shaderProgramSuccess) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, shaderProgramInfoLog);
      std::cout << "ERROR::SHADER_PROGRAM::COMPILATION_FAILED\n" << shaderProgramInfoLog << std::endl;
    }

    this->shaderProgram = shaderProgram;
  }

  unsigned int Shaders::getShaderProgram() { return this->shaderProgram; }
  void Shaders::setShaderProgram(unsigned int shaderProgram) {
    this->shaderProgram = shaderProgram;
  }
}
