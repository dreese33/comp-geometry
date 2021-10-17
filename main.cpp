#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void drawShapeFromTriangles(int VAO, int bufferObject, int shaderProgram, bool element);

// define shaders
const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // RGBA \n"
  "}\0";

const char *fragmentShaderSource1 = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f); // RGBA yellow \n"
  "}\0";

// program entry point
int main(void)
{
  glfwInit();

  // v3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

// macOS specific config
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create glfw window
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // execute on current thread
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // rendering viewport dimensions, can be smaller than window dimensions
  // x, y, width, height
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  /*****************************************************************/
  /* SHAPE DEFINITION                                              */
  /*****************************************************************/
  // 2D triangle in 3D space
  // float vertices[] = {
  //   -0.5f, -0.5f, 0.0f,
  //   0.5f, -0.5f, 0.0f,
  //   0.0f, 0.5f, 0.0f,
  // };

  float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    // -0.5f, 0.5f, 0.0f,
  };

  float vertices1[] = {
    0.5f, 1.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    -0.5f, 1.5f, 0.0f,
  };
  // unsigned int indices[] = {
  //   0, 1, 3,
  //   1, 2, 3,
  // };

  /*****************************************************************/
  /* SPECIFY HOW OPENGL SHOULD INTERPRET THE VERTEX DATA           */
  /*****************************************************************/

  // VBO - vertex buffer object
  // stores large number of vertices in GPU memory
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  unsigned int VBO1;
  glGenBuffers(1, &VBO1);

  // WE MUST BIND VERTEX ARRAY OBJECT (VAO) IN ORDER FOR OPENGL TO COMPILE AND RUN PROPERLY
  // This saves us significant space, and keeps settings bound to the VAO buffer without re-initialization
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  // EBO - element buffer object
  // unsigned int EBO;
  // glGenBuffers(1, &EBO);

  // bind VAO and VBO to the vertex buffer
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // element buffer object bindings
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // define stride, offset, etc for vertex rendering
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  unsigned int VAO1;
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  /*****************************************************************/
  /* CREATE VERTEX SHADER                                          */
  /*****************************************************************/
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // check for shader compilation errors:
  int  vertexSuccess;
  char vertexInfoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
  if (!vertexSuccess)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexInfoLog << std::endl;
  }

  /*****************************************************************/
  /* CREATE FRAGMENT SHADER                                        */
  /*****************************************************************/
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // check compilation errors for fragment shader
  int fragmentSuccess;
  char fragmentInfoLog[512];
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
  if (!fragmentSuccess) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentInfoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentInfoLog << std::endl;
  }

  /*****************************************************************/
  /* CREATE SHADER PROGRAM                                         */
  /*****************************************************************/
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

  // once program is linked, we no longer need the shader objects!!
  // glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // define second shader program
  unsigned int fragmentShader1;
  fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
  glCompileShader(fragmentShader1);

  // check compilation errors for fragment shader
  int fragmentSuccess1;
  char fragmentInfoLog1[512];
  glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &fragmentSuccess1);
  if (!fragmentSuccess) {
    glGetShaderInfoLog(fragmentShader1, 512, NULL, fragmentInfoLog1);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentInfoLog1 << std::endl;
  }

  unsigned int shaderProgram1;
  shaderProgram1 = glCreateProgram();

  glAttachShader(shaderProgram1, vertexShader);
  glAttachShader(shaderProgram1, fragmentShader1);
  glLinkProgram(shaderProgram1);

  int shaderProgramSuccess1;
  char shaderProgramInfoLog1[512];
  glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &shaderProgramSuccess1);
  if (!shaderProgramSuccess1) {
    glGetProgramInfoLog(shaderProgram1, 512, NULL, shaderProgramInfoLog1);
    std::cout << "ERROR::SHADER_PROGRAM::COMPILATION_FAILED\n" << shaderProgramInfoLog1 << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader1);

  // optional configuration for OpenGL context for wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // default is GL_FILL - shows rectangle

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // prevents crazy flickering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 6 vertices in total
    drawShapeFromTriangles(VAO, 0, shaderProgram, false);
    drawShapeFromTriangles(VAO1, 0, shaderProgram1, false);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

// resize viewport on callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// see if escape is pressed
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// draws a shape given a VAO, a VBO/EBO, and a shader program
void drawShapeFromTriangles(int VAO, int bufferObject, int shaderProgram, bool element) {
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  if (element) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 6 vertices specified for now
  } else {
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 vertices specified, multiple draws for our purposes here
  }
}
