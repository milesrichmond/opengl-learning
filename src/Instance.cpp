#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <swivel_chair/graphics/shader.h>
#include <swivel_chair/graphics/textRenderer.h>
#include <swivel_chair/graphics/camera.h>
#include <swivel_chair/fileManager.h>
#include <swivel_chair/model/model.h>
#include <swivel_chair/phys/physEngine.h>

#include <swivel_chair/options.h>
#include <swivel_chair/state.h>

#include <iostream>
#include <vector>

int SCR_WIDTH = 1920, SCR_HEIGHT = 1080;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

PhysEngine engn = PhysEngine();

// Sure, why not
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(static_cast<float>(yoffset));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Reload Scene
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        engn.clearObjects();
        engn.loadObjectsFromJSON(Stock::scenes[Options::targetScene]);
        State::passedTime = 0.0f;
    }
    
    // Pausing + gameSpeed reset
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (mods == GLFW_MOD_CONTROL) {
            Options::gameSpeed = 1.0f;
        } else {
            Options::isPaused = !Options::isPaused;
        }   
    }

    // Game Speed
    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        Options::gameSpeed += Options::Controls::zoomSpeed * State::deltaTime;
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        if (Options::gameSpeed < Options::Controls::zoomSpeed * State::deltaTime) {
            Options::gameSpeed = 0.0f;
        } else {
            Options::gameSpeed -= Options::Controls::zoomSpeed * State::deltaTime;
        }
    }
}

// Used in combination with the key_callback method to provide a smooth input for WASD controls
// while still supporting the single key presses from the callback
void keyboard_input_by_frame(GLFWwindow* window) {
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
    // Once again, appeasing the almighty c++ compiler
    Instance() {};

    void init(const std::string optionsPath) {
        windowInit();
        loadFromJSON(optionsPath);

        shaderProgram = Shader(FileSystem::getPath("src/simple.vs").c_str(), FileSystem::getPath("src/simple.fs").c_str());
        textRenderer = TextRenderer(FileSystem::getPath("src/text.vs").c_str(), FileSystem::getPath("src/text.fs").c_str(), SCR_WIDTH, SCR_HEIGHT);

        engn.loadObjectsFromJSON(Stock::scenes[Options::targetScene]);

        isInit = true;
    }

    void start() {
        if (!isInit) {
            std::cout << "Instance is not initialized, cannot start" << std::endl;
            return;
        }

        while (!glfwWindowShouldClose(window)) {
            // per-frame time logic
            // --------------------
            float timeOfCurrentFrame = static_cast<float>(glfwGetTime());
            State::deltaTime = timeOfCurrentFrame - State::timeOfLastFrame;
            State::timeOfLastFrame = timeOfCurrentFrame;
            State::avgFrameTime = (State::avgFrameTime + State::deltaTime) / 2;
            if (!Options::isPaused) {
                State::passedTime += Options::gameSpeed * State::deltaTime;
            }
            
            if(fmod(timeOfCurrentFrame, 0.25) < 0.01) {
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
            glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
            glm::mat4 view = camera.getViewMatrix();
            shaderProgram.setMat4("projection", projection);
            shaderProgram.setMat4("view", view);

            // render the loaded model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
            shaderProgram.setMat4("model", model);

            textRenderer.RenderText(
                floatPrecision(displayedFrameTime * 1000, 3) + " ms", 
                5.0f, 10.0f, 
                Options::Font::scale, 
                glm::vec3(0.8f, 0.8f, 0.8f), 
                SCR_HEIGHT, SCR_WIDTH
            );
            if (Options::isPaused) {
                textRenderer.RenderText(
                    "Game Speed: " + floatPrecision(Options::gameSpeed, 3) + "x (paused)",
                    5.0f, 50.0f, 
                    Options::Font::scale, glm::vec3(0.8f, 0.8f, 0.8f), 
                    SCR_HEIGHT, SCR_WIDTH
                );
            } else {
                textRenderer.RenderText(
                    "Game Speed: " + floatPrecision(Options::gameSpeed, 3) + "x",
                    5.0f, 50.0f, 
                    Options::Font::scale, glm::vec3(0.8f, 0.8f, 0.8f), 
                    SCR_HEIGHT, SCR_WIDTH
                );
            }
            textRenderer.RenderText(
                floatPrecision(State::passedTime, 3) + " s", 
                5.0f, 100.0f, 
                Options::Font::scale, 
                glm::vec3(0.8f, 0.8f, 0.8f), 
                SCR_HEIGHT, SCR_WIDTH
            );

            if (Options::isPaused) {
                engn.draw(shaderProgram, 0);
            } else {
                engn.draw(shaderProgram, State::deltaTime * Options::gameSpeed);
            }
            

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void destroy() {
        if (!isInit) {
            std::cout << "Instance is not initialized, cannot destroy" << std::endl;
            return;
        }
        glfwTerminate();
    }

private:
    const static unsigned int DEFAULT_SCR_WIDTH = 1920;
    const static unsigned int DEFAULT_SCR_HEIGHT = 1080;

    bool isInit = false;
    float displayedFrameTime = 0.0f;

    GLFWwindow* window;
    Shader shaderProgram = Shader();

    

    TextRenderer textRenderer = TextRenderer();

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
        window = glfwCreateWindow(DEFAULT_SCR_WIDTH, DEFAULT_SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        // NO LONGER TRUE, HAD TO MOVE TO MACOS | I'm pretty sure none of these get used with my window manager (i3), so I commented them out
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Turns out this one is needed
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
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

        Options::optionsPath = optionsPath;
        Options::targetScene = data["targetScene"];

        Options::gameSpeed = data["gameSpeed"];
        Options::isPaused = data["isPaused"];
        Options::gravityModifier = data["gravityModifier"];
        Options::restitution = data["restitution"];

        Options::Controls::zoomSpeed = data["controls"]["zoomSpeed"];

        Options::Font::file = data["font"]["file"];
        Options::Font::scale = data["font"]["scale"];

        file.close();
    }
};