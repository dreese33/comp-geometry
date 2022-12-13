#include <stddef.h>
#include "src/graphics/graphics.hpp"
using namespace Graphics;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void drawPoints(int VAO, int shaderProgram, int n);
int run();

// refactor methods
GLFWwindow* createMainWindow();
void mainLoop(GLFWwindow* window, unsigned int VAO, unsigned int shaderProgram, int n);

// define shaders
const std::string vertexShaderCode = GraphicsUtilities::read_shader_file("./src/shaders/basic/vertex_shader.vert");
GLchar* vertexShaderSource = (GLchar *) vertexShaderCode.c_str();

const std::string fragmentShaderCode = GraphicsUtilities::read_shader_file("./src/shaders/basic/fragment_shader.frag");
GLchar* fragmentShaderSource = (GLchar *) fragmentShaderCode.c_str();

const std::string fragmentShaderCode1 = GraphicsUtilities::read_shader_file("./src/shaders/basic/fragment_shader_other.frag");
GLchar* fragmentShaderSource1 = (GLchar *) fragmentShaderCode.c_str();

// program entry point
int main(void)
{
  return run();
}

int run() {
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
  GraphicsUtilities::createShader(vertexShaderSource, vertexShader);

  // create fragment shader
  unsigned int fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
  GraphicsUtilities::createShader(fragmentShaderSource, fragmentShader);

  // create shader program
  Shaders shaderProgram = Shaders(vertexShader, fragmentShader);

  // optional configuration for OpenGL context for wireframe mode
  glPointSize(10);
  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); // default is GL_FILL - shows rectangle

  // cleanup and delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  mainLoop(window, VAO, shaderProgram.getShaderProgram(), n);
  glfwTerminate();

  return 0;
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
