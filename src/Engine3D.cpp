//
// Created by part4 on 29.01.2022.
//


#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include "glm/glm/gtc/type_ptr.hpp"
#include "../headers/Shader.h"
#include "../headers/Texture.h"
#include "../headers/Vector2i.h"
#include "../headers/Constants.h"
#include "../headers/stb_image.h"
#include <random>
#include "Vector2.cpp"
#include "Sphere.cpp"
using namespace glm;
using namespace std;
using namespace consts;

static bool usePerspectiveProjection = true;
static Vector2i ScreenSize = Vector2i(1920,1080);
static vec3 cameraPos   = vec3(0.0f, 0.0f,  3.0f);
static vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
static vec3 cameraUp    = vec3(0.0f, 1.0f,  0.0f);
static double rawTime = 0;
static double currentTime = 0;
static Vector2<float> lastMousePos;
static bool firstMouseMove = true;
static float myYaw = -90.0f;
static float myPitch = 0.0f;

class Engine3D {
    ///Rendering and flags
    GLFWwindow *window;
    bool useKeyboard;
    bool useMouse;
    int lockedFps;
    int displayForm;

    ///view
    mat4 view = mat4(1.f);
    GLint viewUniformLoc{};
    GLint bgViewUniformLoc{};

    mat4 projection = mat4(1.f);
    GLint projectionUniformLoc{};
    GLint bgProjectionUniformLoc{};

    mat4 model = mat4(1.f);
    GLint modelUniformLoc{};
    GLint bgModelUniformLoc{};

    ///update of objects
    mat4 transform = mat4(1.f);
    GLint transformUniformLoc{};

    ///drawing data and objects
    vec3 objectsPos[10] = {
            vec3( 0.0f,  0.0f,  0.0f),
            vec3( 2.0f,  5.0f, -15.0f),
            vec3(-1.5f, -2.2f, -2.5f),
            vec3(-3.8f, -2.0f, -12.3f),
            vec3( 2.4f, -0.4f, -3.5f),
            vec3(-1.7f,  3.0f, -7.5f),
            vec3( 1.3f, -2.0f, -2.5f),
            vec3( 1.5f,  2.0f, -2.5f),
            vec3( 1.5f,  0.2f, -1.5f),
            vec3(-1.3f,  1.0f, -1.5f)
    };
    Shader myColoredShader;
    Shader bgShader;
    Texture myColoredTexture;
    Texture bgTexture;
    unsigned int VAO{};
    unsigned int bgVAO{};
    unsigned int VBO{};
    unsigned int bgVBO{};
    unsigned int EBO{};
    unsigned int bgEBO{};
    ///Custom objects
    Sphere earth;

public:
    Engine3D(int screenResolution, const char *title, int lockedFps, int displayForm, bool useKeyboard,
                       bool useMouse, bool useDoubleBuffering, Colors screenClearColor) {
        this->displayForm = displayForm;
        this->lockedFps = lockedFps;
        this->useKeyboard = useKeyboard;
        this->useMouse = useMouse;

        if (!glfwInit()) {
            cout << "Failed to initialize GLFW" << endl;
            exit(-1);
        }

        GLFWmonitor *monitor = nullptr;
        if (displayForm == displayForm::FULLSCREEN) {
            monitor = glfwGetPrimaryMonitor();
        } else {
            glfwWindowHint(GLFW_DECORATED, true);
            glfwWindowHint(GLFW_RESIZABLE, true);
        }

        auto ScreenRes = getScreenResolution(screenResolution);
        ScreenSize = ScreenRes;
        lastMousePos = Vector2<float>(ScreenSize.getX() / 2,ScreenSize.getY() / 2);

        window = glfwCreateWindow(ScreenRes.getX(), ScreenRes.getY(), title, monitor, nullptr);
        if (!window) {
            glfwTerminate();
            exit(-1);
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            cout << "Failed to initialize GLAD" << endl;
            exit(-1);
        }

        glad_glEnable(GL_DEPTH_TEST);

        glad_glViewport(0, 0, ScreenRes.getX(), ScreenRes.getY());

        glfwWindowHint(GLFW_DOUBLEBUFFER, useDoubleBuffering);
        glfwWindowHint(GLFW_REFRESH_RATE, lockedFps);

        glfwSetErrorCallback(error_callback);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        glad_glClearColor(screenClearColor.getR() / 255.0f, screenClearColor.getG() / 255.0f,
                          screenClearColor.getB() / 255.0f, screenClearColor.getA() / 255.0f);

        if (useKeyboard)
            glfwSetKeyCallback(window, key_callback);

        if (useMouse) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, cursor_position_callback);
            glfwSetMouseButtonCallback(window, mouse_button_callback);
        }

        glfwSwapInterval(1);
    }

    ~Engine3D() {
        glad_glDeleteVertexArrays(1, &VAO);
        glad_glDeleteVertexArrays(1, &bgVAO);
        glad_glDeleteBuffers(1, &VBO);
        glad_glDeleteBuffers(1, &bgVBO);
        glad_glDeleteBuffers(1, &EBO);
        glad_glDeleteBuffers(1, &bgEBO);

        if (window)
            glfwDestroyWindow(window);

        glfwTerminate();
        exit(0);
    }

    static void display_error_message(int code, const char *description) {
        cout << "Error nr.:" << code << " has occured:\n" << description << endl;
    }

    static void error_callback(int code, const char *description) {
        display_error_message(code, description);
    }

    static Vector2i getScreenResolution(int screenResolutionType) {
        if (screenResolutionType == 0) {
            return {640, 480};
        } else if (screenResolutionType == 1) {
            return {1280, 720};
        } else if (screenResolutionType == 2) {
            return {1920, 1080};
        } else return {1920, 1080};
    }

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        ScreenSize = Vector2i(width, height);
        glad_glViewport(0, 0, width, height);
    }

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwTerminate();
            exit(0);
        }

        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            usePerspectiveProjection = !usePerspectiveProjection;
        }

        float cameraSpeed = 20.f * rawTime;
        if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_REPEAT)
            cameraSpeed = 500.f * rawTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;

    }

    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
        Vector2<float> currentMousePos = Vector2<float>(xpos,ypos);

        if(firstMouseMove){
            lastMousePos.setX(currentMousePos.getX());
            lastMousePos.setY(currentMousePos.getY());
            firstMouseMove = false;
        }

        float sensitivity = 0.1f;

        Vector2<float> offset = Vector2<float> (
                currentMousePos.getX() - lastMousePos.getX(),
                lastMousePos.getY() - currentMousePos.getY()
                );
        lastMousePos.setX(currentMousePos.getX());
        lastMousePos.setY(currentMousePos.getY());

        offset = Vector2<float> ( (offset.getX() * sensitivity),(offset.getY() * sensitivity));

        myYaw += offset.getX();
        myPitch += offset.getY();

        if(myPitch > 89.0f)
            myPitch = 89.0f;
        if(myPitch < -89.0f)
            myPitch = -89.0f;

        vec3 direction;
        direction.x = cos(radians(myYaw)) * cos(radians(myPitch));
        direction.y = sin(radians(myPitch));
        direction.z = sin(radians(myYaw)) * cos(radians(myPitch));
        cameraFront = normalize(direction);

    }

    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {

    }

    int run() {
        prepare();
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderAndUpdate(glfwGetTime());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        return 0;
    }

    void renderAndUpdate(double newTime){
        rawTime = newTime - currentTime;
        currentTime = newTime;

        ///activate textures
        glad_glActiveTexture(GL_TEXTURE0);
        glad_glBindTexture(GL_TEXTURE_2D, myColoredTexture.getTexture());
        glad_glActiveTexture(GL_TEXTURE1);
        glad_glBindTexture(GL_TEXTURE_2D, bgTexture.getTexture());

        ///5-Test rendering
        myColoredShader.use();

        glad_glBindVertexArray(VAO);

        myColoredShader.setInt("ourTexture", 0);
        for(auto & objectsPo : objectsPos){
            update(objectsPo);
            glad_glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }

        bgShader.use();
        glad_glBindVertexArray(bgVAO);
        bgShader.setInt("ourTexture",1);

        bgUpdate();
        glad_glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        earth.updateAndDraw(usePerspectiveProjection,ScreenSize,cameraPos,cameraFront,cameraUp);

        glad_glBindVertexArray(0);

        //renderSphere

    }

    void bgUpdate(){
        if (usePerspectiveProjection)
            projection = perspective(radians(90.f), (float) ScreenSize.getX() / (float) ScreenSize.getY(), 0.1f,10000.f);
        else
            projection = ortho(0.f, (float) ScreenSize.getX(), 0.f, (float) ScreenSize.getY(), 0.1f, 10000.f);

        glad_glUniformMatrix4fv(bgProjectionUniformLoc, 1, GL_FALSE, value_ptr(projection));

        model = mat4(1.f);
        glad_glUniformMatrix4fv(bgModelUniformLoc, 1, GL_FALSE, value_ptr(model));

        view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glad_glUniformMatrix4fv(bgViewUniformLoc, 1, GL_FALSE, value_ptr(view));
    }

    double update(vec3 objectPos) {
        random_device rd;
        random_device rd1;
        random_device rd2;
        random_device rd3;

        mt19937 gen(rd());
        mt19937 gen1(rd1());
        mt19937 gen2(rd2());
        mt19937 gen3(rd3());


        uniform_int_distribution<> dist(5,20);
        uniform_int_distribution<> dist1(-5,-5);
        uniform_int_distribution<> dist2(-12,-12);
        uniform_int_distribution<> dist3(-10,-10);

        auto degree = (float) dist(gen);
        auto xStr = (float) dist1(gen1);
        auto yStr = (float) dist2(gen2);
        auto zStr = (float) dist3(gen3);

        transform = rotate(transform, radians((float) rawTime / 2 * degree), vec3(xStr, yStr, zStr));
        glad_glUniformMatrix4fv(transformUniformLoc, 1, GL_FALSE, value_ptr(transform));

        if (usePerspectiveProjection)
            projection = perspective(radians(90.f), (float) ScreenSize.getX() / (float) ScreenSize.getY(), 0.1f,10000.f);
        else
            projection = ortho(0.f, (float) ScreenSize.getX(), 0.f, (float) ScreenSize.getY(), 0.1f, 10000.f);

        glad_glUniformMatrix4fv(projectionUniformLoc, 1, GL_FALSE, value_ptr(projection));

        model = mat4(1.f);
        model = translate(model,objectPos);
        glad_glUniformMatrix4fv(modelUniformLoc, 1, GL_FALSE, value_ptr(model));

        view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glad_glUniformMatrix4fv(viewUniformLoc, 1, GL_FALSE, value_ptr(view));

        //Make updates here

        return currentTime;
    }

    void prepare() {

        view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        myColoredShader = Shader(R"(..\src\shadersSources\firstSquare\squareVertex.shader)",
                                 R"(..\src\shadersSources\firstSquare\squareFrag.shader)");

        bgShader = Shader(R"(..\src\shadersSources\backgroundShaders\bgVertex.shader)",
                                 R"(..\src\shadersSources\backgroundShaders\bgFrag.shader)");

        myColoredTexture = Texture("../assets/texture.png");
        bgTexture = Texture("../assets/stars.jpg");

        stbi_set_flip_vertically_on_load(false);

        transformUniformLoc = glad_glGetUniformLocation(myColoredShader.ID, "transform");
        projectionUniformLoc = glad_glGetUniformLocation(myColoredShader.ID, "projection");
        modelUniformLoc = glad_glGetUniformLocation(myColoredShader.ID, "model");
        viewUniformLoc = glad_glGetUniformLocation(myColoredShader.ID, "view");

        bgProjectionUniformLoc = glad_glGetUniformLocation(bgShader.ID, "projection");
        bgModelUniformLoc = glad_glGetUniformLocation(bgShader.ID, "model");
        bgViewUniformLoc = glad_glGetUniformLocation(bgShader.ID, "view");

        myColoredShader.use();
        myColoredShader.setInt("ourTexture", 0);

        float firstSquare[] = {
                //position                                      //colors                                      //texture coords
                0.5f, 0.5f, 0.5f,                 0.0f, 0.0f, 1.f,                  1.0f, 1.0f, //top right                    - 0

                0.5f, -0.5f, 0.5f,                0.0f, 1.f, 0.0f,              1.0f, 0.0f, //bottom right               - 1

                -0.5f, -0.5f, 0.5f,             1.0f, 0.0f, 0.0f,               0.0f, 0.0f, //bottom left                - 2

                -0.5f, 0.5f, 0.5f,              1.0f, 0.0f, 1.f,                0.0f, 1.0f, //top left                   - 3


                0.4f, 0.6f, -1.f,               0.0f, 0.0f, 1.f,                0.0f, 0.0f, //hidden - top right         - 4

                0.4f, -0.4f, -1.f,              0.0f, 1.f, 0.0f,                0.0f, 1.0f, //hidden - bottom right     - 5

                -0.6f, -0.4f, -1.f,             1.0f, 0.0f, 0.0f,               1.0f, 1.0f, //hidden - bottom left       - 6

                -0.6f, 0.6f, -1.f,              1.0f, 0.0f, 1.f,                1.0f, 0.0f, //hidden - top left          - 7
        };

        float bgSquare[]= {
                2500.f,2500.f,2500.f,       1.0f,1.0f,
                2500.f,-2500.f,2500.f,       1.0f,0.0f,
                -2500.f,-2500.f,2500.f,       0.0f,0.0f,
                -2500.f,2500.f,2500.f,       0.0f,1.0f,

                2500.f,2500.f,-2500.f,       0.0f,0.0f,
                2500.f,-2500.f,-2500.f,       0.0f,1.0f,
                -2500.f,-2500.f,-2500.f,       1.0f,1.0f,
                -2500.f,2500.f,-2500.f,       1.0f,0.0f,
        };

        unsigned int indices[] = {
                //prawo
                0, 4, 1,
                1, 5, 4,

                //lewa
                2, 3, 6,
                3, 6, 7,

                //gora
                0, 3, 4,
                3, 4, 7,

                //dol
                1, 2, 6,
                1, 5, 6,

                //przednia sciana
                1, 2, 3,
                0, 1, 3,

                //tylna sciana
                5, 6, 7,
                4, 5, 7,


        };

        ///0-Creation of VAO,VBO,EBO
        glad_glGenVertexArrays(1, &VAO);
        glad_glGenVertexArrays(1, &bgVAO);
        glad_glGenBuffers(1, &VBO);
        glad_glGenBuffers(1, &bgVBO);
        glad_glGenBuffers(1, &EBO);
        glad_glGenBuffers(1, &bgEBO);

        ///1-Binding of them
        glad_glBindVertexArray(VAO);

        glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glad_glBufferData(GL_ARRAY_BUFFER, sizeof(firstSquare), firstSquare, GL_STATIC_DRAW);

        glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        ///2-Setting vertex linking for openGL (call once in prepare)
        glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (0 * sizeof(float)));
        glad_glEnableVertexAttribArray(0);

        /// color attribute
        glad_glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glad_glEnableVertexAttribArray(1);

        /// texture attribute
        glad_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glad_glEnableVertexAttribArray(2);

        ///bg vao's etc
        glad_glBindVertexArray(bgVAO);

        glad_glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
        glad_glBufferData(GL_ARRAY_BUFFER, sizeof(bgSquare), bgSquare, GL_STATIC_DRAW);

        glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
        glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


        glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (0 * sizeof(float)));
        glad_glEnableVertexAttribArray(0);

        glad_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glad_glEnableVertexAttribArray(1);

        glad_glBindVertexArray(0);

        earth = Sphere(18,36,500.f,
                       "../assets/earth_night.jpg","../assets/earth_daymap.jpg","../assets/earth_clouds.jpg",
                       R"(..\src\shadersSources\sphereShaders\vertex.shader)",R"(..\src\shadersSources\sphereShaders\frag.shader)"
                       ,vec3(0.f,0.f,400.f));

    }
};
