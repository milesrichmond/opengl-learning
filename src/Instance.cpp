#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <nlohmann/json.hpp>

#include <custom/fileManager.hpp>
#include <custom/graphics/camera.hpp>
#include <custom/graphics/shader.hpp>
#include <custom/graphics/textRenderer.hpp>
#include <custom/model/model.hpp>
#include <custom/physics/engine.hpp>
#include <custom/state/gameOptions.hpp>
#include <custom/state/gameState.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int SCR_WIDTH = 1920, SCR_HEIGHT = 1080;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

Phys::Engine engn = Phys::Engine();

// Sure, why not
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  SCR_WIDTH = width;
  SCR_HEIGHT = height;
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.processMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.processMouseScroll(static_cast<float>(yoffset));
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  // Close window
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  // Reload Scene
  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
    engn.clearObjects();
    engn.loadObjectsFromJSON(Stock::scenes[Opt::targetScene]);
    State::timeElapsed = 0.0f;
  }

  // Pausing + gameSpeed reset
  if (key == GLFW_KEY_P && action == GLFW_PRESS) {
    if (mods == GLFW_MOD_CONTROL) {
      Opt::gameSpeed = 1.0f;
    } else {
      Opt::isPaused = !Opt::isPaused;
    }
  }

  // Game Speed
  if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
    Opt::gameSpeed += Opt::Controls::zoomSpeed * State::deltaTime;
  }
  if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
    if (Opt::gameSpeed < Opt::Controls::zoomSpeed * State::deltaTime) {
      Opt::gameSpeed = 0.0f;
    } else {
      Opt::gameSpeed -= Opt::Controls::zoomSpeed * State::deltaTime;
    }
  }
}

// Used in combination with the key_callback method to provide a smooth input
// for WASD controls while still supporting the single key presses from the
// callback
void keyboard_input_by_frame(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.processKeyboard(FORWARD, State::deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.processKeyboard(BACKWARD, State::deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.processKeyboard(LEFT, State::deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.processKeyboard(RIGHT, State::deltaTime);
  }
}

// Part of function contents from a stackoverflow post by AlexD on Mar 22, 2015
// https://stackoverflow.com/a/29200671
std::string floatPrecision(float value, int precision) {
  std::stringstream stream;
  stream.fill(' ');
  stream << std::setw(3) << std::setprecision(precision) << value;
  return stream.str();
}

class Instance {
public:
  // shaderProgram cannot be defined here due to glCurrentContext conflictions
  Instance() {}

  void init(const std::string optionsPath) {
    windowInit();
    loadFromJSON(optionsPath);

    shaderProgram = Shader(FileSystem::getPath("src/simple.vs").c_str(),
                           FileSystem::getPath("src/simple.fs").c_str());

    textRenderer =
        TextRenderer(FileSystem::getPath("src/text.vs").c_str(),
                     FileSystem::getPath("src/text.fs").c_str(),
                     FileSystem::getPath(Opt::Font::fontPath).c_str(),
                     SCR_WIDTH, SCR_HEIGHT);

    engn.loadObjectsFromJSON(Stock::scenes[Opt::targetScene]);
  }

  void start() {
    while (!glfwWindowShouldClose(window)) {
      // per-frame time logic
      // --------------------
      float timeOfCurrentFrame = static_cast<float>(glfwGetTime());
      State::deltaTime = timeOfCurrentFrame - State::timeOfLastFrame;
      State::timeOfLastFrame = timeOfCurrentFrame;
      State::avgFrameTime = (State::avgFrameTime + State::deltaTime) / 2;
      if (!Opt::isPaused) {
        State::timeElapsed += Opt::gameSpeed * State::deltaTime;
      }

      if (fmod(timeOfCurrentFrame, 0.25) < 0.01) {
        displayedFrameTime = State::avgFrameTime;
      }

      keyboard_input_by_frame(window);

      // render
      // ------
      glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // don't forget to enable shader before setting uniforms
      shaderProgram.use();

      // view/projection transformations
      glm::mat4 projection =
          glm::perspective(glm::radians(camera.zoom),
                           (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
      glm::mat4 view = camera.getViewMatrix();
      shaderProgram.setMat4("projection", projection);
      shaderProgram.setMat4("view", view);

      // render the loaded model
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
      shaderProgram.setMat4("model", model);

      textRenderer.RenderText(
          floatPrecision(displayedFrameTime * 1000, 3) + " ms", 5.0f, 10.0f,
          Opt::Font::scale, glm::vec3(0.8f, 0.8f, 0.8f), SCR_HEIGHT, SCR_WIDTH);
      if (Opt::isPaused) {
        textRenderer.RenderText(
            "Game Speed: " + floatPrecision(Opt::gameSpeed, 3) + "x (paused)",
            5.0f, 50.0f, Opt::Font::scale, glm::vec3(0.8f, 0.8f, 0.8f),
            SCR_HEIGHT, SCR_WIDTH);
      } else {
        textRenderer.RenderText(
            "Game Speed: " + floatPrecision(Opt::gameSpeed, 3) + "x", 5.0f,
            50.0f, Opt::Font::scale, glm::vec3(0.8f, 0.8f, 0.8f), SCR_HEIGHT,
            SCR_WIDTH);
      }
      textRenderer.RenderText(
          floatPrecision(State::timeElapsed, 3) + " s", 5.0f, 100.0f,
          Opt::Font::scale, glm::vec3(0.8f, 0.8f, 0.8f), SCR_HEIGHT, SCR_WIDTH);

      if (Opt::isPaused) {
        engn.draw(shaderProgram, 0);
      } else {
        engn.draw(shaderProgram, State::deltaTime * Opt::gameSpeed);
      }

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse
      // moved etc.)
      // -------------------------------------------------------------------------------
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  void destroy() { glfwTerminate(); }

private:
  const static unsigned int DEFAULT_SCR_WIDTH = 1920;
  const static unsigned int DEFAULT_SCR_HEIGHT = 1080;

  float displayedFrameTime = 0.0f;

  GLFWwindow *window;
  Shader shaderProgram = Shader();
  TextRenderer textRenderer;

  void windowInit() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(DEFAULT_SCR_WIDTH, DEFAULT_SCR_HEIGHT,
                              "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return;
    }
    glfwMakeContextCurrent(window);
    // NO LONGER TRUE, HAD TO MOVE TO MACOS | I'm pretty sure none of these get
    // used with my window manager (i3), so I commented them out
    glfwSetFramebufferSizeCallback(
        window, framebuffer_size_callback); // Turns out this one is needed
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void loadFromJSON(std::string const optionsPath) {
    std::ifstream file(optionsPath, std::ifstream::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file: " + optionsPath);
    }
    nlohmann::json data = nlohmann::json::parse(file);

    Opt::optionsPath = optionsPath;
    Opt::targetScene = data["targetScene"];

    Opt::gameSpeed = data["gameSpeed"];
    Opt::isPaused = data["isPaused"];
    Opt::Parameters::gravityCoef = data["gravityModifier"];
    Opt::Parameters::restitutionCoef = data["restitution"];

    Opt::Controls::zoomSpeed = data["controls"]["zoomSpeed"];

    Opt::Font::fontPath = data["font"]["file"];
    Opt::Font::scale = data["font"]["scale"];

    file.close();
  }
};
