#include <stddef.h>
#include "src/2D/shapes.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

using namespace Shapes;
using namespace Graphics;

int run();

// refactor methods
SDL_Window* createMainWindow();
void mainLoop(SDL_Window* window);

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

  SDL_Window* window = createMainWindow();
  if (window == NULL)
  {
    std::cout << "Failed to create SDL window" << std::endl;
    SDL_Quit();
    return -1;
  }

  // verify GLAD loader
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
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
SDL_Window* createMainWindow() {
  SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;
  screenSurface = SDL_GetWindowSurface( window );
  //Fill the surface white
  SDL_FillRect( screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF ));
  //Update the surface
  SDL_UpdateWindowSurface( window );
  return window;
}
