#include <stddef.h>
#include "src/2D/shapes.hpp"

using namespace Shapes;
using namespace Graphics;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int run();

// refactor methods
GLFWwindow* createMainWindow();
void mainLoop(GLFWwindow* window);

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

/**
 * @brief Runs the main program, serves as the simulator's
 * entry point
 */
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

  mainLoop(window);
  glfwTerminate();

  return 0;
}

/**
 * @brief Runs the main loop for the graphics simulator
 *
 * @param window GLFWwindow currently rendering the graphics
 */
void mainLoop(GLFWwindow* window) {
  while (!glfwWindowShouldClose(window))
  {
    // prevents crazy flickering (TODO - research color/depth buffer bits)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // create vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GraphicsUtilities::createShader(vertexShaderSource, vertexShader);

    // create fragment shader
    unsigned int fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
    GraphicsUtilities::createShader(fragmentShaderSource, fragmentShader);

    // create shader program
    Shaders shaderProgram = Shaders(vertexShader, fragmentShader);

    // cleanup and delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Polygon *polygon = (Polygon*) ShapeFactory::constructShape(POLYGON, VERTEX_SHAPE);
    Vector2D center = { 0.0f, 0.0f };
    polygon->setNumberOfSides(4);
    polygon->setCenterPt(center);
    polygon->setRadius(0.5f);
    polygon->calculateVertices();

    GraphicsUtilities::drawPoints(
      shaderProgram.getShaderProgram(),
      polygon->getVertices(),
      polygon->getNumberOfSides()
    );

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
