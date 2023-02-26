#include <stddef.h>
#include "src/2D/shapes.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

using namespace Shapes;
using namespace Graphics;

int run();

// refactor methods
SDL_Window* createMainWindow(const char* windowTitle);
void mainLoop(SDL_Window* window);
int sdlDie(const char* exitMessage);

// define shaders
const std::string vertexShaderCode = GraphicsUtilities::read_shader_file("./src/shaders/basic/vertex_shader.vert");
GLchar* vertexShaderSource = (GLchar *) vertexShaderCode.c_str();

const std::string fragmentShaderCode = GraphicsUtilities::read_shader_file("./src/shaders/basic/fragment_shader.frag");
GLchar* fragmentShaderSource = (GLchar *) fragmentShaderCode.c_str();

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
  // initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Failed to initialize SDL: %s\n", SDL_GetError());
    return -1;
  }
  // SDL_GL_LoadLibrary(NULL); // default OpenGL

  // Request OpenGL version to 4.1 before rendering our window
  // SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  // Also request a depth buffer
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  SDL_Window* window = createMainWindow("Algo Sim");
  if (window == NULL)
  {
    return sdlDie("Failed to create SDL window");
  }

  SDL_GLContext mainContext = SDL_GL_CreateContext(window);
  if (mainContext == NULL) {
    return sdlDie("Failed to create OpenGL Context");
  }

  // verify GLAD loader
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    return sdlDie("Failed to initialize GLAD loader");
  }

  mainLoop(window);

  // terminate
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

/**
 * @brief Runs the main loop for the graphics simulator
 *
 * @param window SDL_Window currently rendering the graphics
 */
void mainLoop(SDL_Window* window) {
  SDL_Event e;
  bool quit = false;
  while (quit == false) {
    SDL_GL_SwapWindow(window);
    while (SDL_PollEvent(&e)){
      if( e.type == SDL_QUIT ) { quit = true; }
      else {
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
      }
    }
  }
}

/**
 * @brief Configure OpenGL versions and platform specific information for rendering a window
 * Main window rendered for graphics generation and testing
 *
 * @returns SDL_Window
 */
SDL_Window* createMainWindow(const char* windowTitle) {
  // Create window
  SDL_Window* window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
  return window;
}

/**
 * @brief Kill SDL
 *
 * @param exitMessage Message to log upon exiting program
 * @return int Exit status
 */
int sdlDie(const char* exitMessage) {
  printf("%s : %s\n", exitMessage, SDL_GetError());
  SDL_Quit();
  return -1;
}
