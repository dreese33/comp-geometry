#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>

#include <stddef.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void drawPoints(int VAO, int shaderProgram, int n);

// refactor methods
GLFWwindow* createMainWindow();
unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
void createShader(GLchar* shaderSource, unsigned int shader);
void mainLoop(GLFWwindow* window, unsigned int VAO, unsigned int shaderProgram, int n);

// shader file parser
static std::string read_shader_file(const char *shader_file);

// define shaders
const std::string vertexShaderCode = read_shader_file("./shaders/basic/vertex_shader.vert");
GLchar* vertexShaderSource = (GLchar *) vertexShaderCode.c_str();

const std::string fragmentShaderCode = read_shader_file("./shaders/basic/fragment_shader.frag");
GLchar* fragmentShaderSource = (GLchar *) fragmentShaderCode.c_str();

const std::string fragmentShaderCode1 = read_shader_file("./shaders/basic/fragment_shader_other.frag");
GLchar* fragmentShaderSource1 = (GLchar *) fragmentShaderCode.c_str();

// program entry point
int main(void)
{
  // Render main window
  GLFWwindow* window = createMainWindow();
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // execute on current thread
  glfwMakeContextCurrent(window);

  // verify GLAD loader
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  /*****************************************************************/
  /* SHAPE DEFINITION                                              */
  /*****************************************************************/

  // number of vertices
  int n = 4;
  float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
  };

  /*****************************************************************/
  /* SPECIFY HOW OPENGL SHOULD INTERPRET THE VERTEX DATA           */
  /*****************************************************************/

  // Vertex Buffer Object
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  // Vertex Array Object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  // bind VAO and VBO to the vertex buffer
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // define stride, offset, etc for vertex rendering
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // create vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  createShader(vertexShaderSource, vertexShader);

  // create fragment shader
  unsigned int fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
  createShader(fragmentShaderSource, fragmentShader);

  // create shader program
  unsigned int shaderProgram = createShaderProgram(vertexShader, fragmentShader);

  // optional configuration for OpenGL context for wireframe mode
  glPointSize(10);
  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); // default is GL_FILL - shows rectangle

  // cleanup and delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  mainLoop(window, VAO, shaderProgram, n);
  glfwTerminate();

  return 0;
}

/**
 * @brief Create a Shader object
 *
 * @param shaderSource GLSL code for shader to be rendered
 * @param shader OpenGL shader created with macro type specified
 */
void createShader(GLchar* shaderSource, unsigned int shader) {
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
 * @brief Create a Shader Program object containing both a vertex and a fragment shader
 *
 * @param vertexShader Vertex shader associated w/ object
 * @param fragmentShader Fragment shader associated w/ object
 * @return unsigned int
 */
unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
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

  return shaderProgram;
}

/**
 * @brief Runs the main loop for the graphics simulator
 *
 * @param window GLFWwindow currently rendering the graphics
 */
void mainLoop(GLFWwindow* window, unsigned int VAO, unsigned int shaderProgram, int n) {
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // prevents crazy flickering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawPoints(VAO, shaderProgram, n);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

/**
 * @brief Configure OpenGL versions and platform specific information for rendering a window
 * Main window rendered for graphics generation and testing
 *
 * @returns GLFWwindow object
 */
GLFWwindow* createMainWindow() {
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
  GLFWwindow *window = glfwCreateWindow(800, 600, "Graphics Sim", NULL, NULL);
  return window;
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

/**
 * @brief Draws an array of points to the provided canvas, to be called in the programs main loop
 *
 * @param VAO Vertex Array Object
 * @param shaderProgram Corresponding shader program (typically comprised of fragment/vertex shader)
 * @param n Number of vertices
 */
void drawPoints(int VAO, int shaderProgram, int n) {
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawArrays(GL_POINTS, 0, 4);
}

static std::string read_shader_file (const char *shader_file)
{
  // no feedback is provided for stream errors / exceptions.

  std::ifstream file (shader_file);
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
