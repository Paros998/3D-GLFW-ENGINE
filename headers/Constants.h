//
// Created by part4 on 15.01.2022.
//

#ifndef GRAFIKA_CONSTANTS_H
#define GRAFIKA_CONSTANTS_H
#include "Colors.h"
#include "glad/glad.h"

namespace consts {
    enum displayForm {
        WINDOWED = 0,
        FULLSCREEN = 1
    };

    enum lockedFPS {
        FPS_60 = 60,
        FPS_75 = 75,
        FPS_90 = 90,
        FPS_120 = 120,
        FPS_144 = 144
    };

    enum screenResolution{
        RES_640x480 = 0,
        RES_1280x720 = 1,
        RES_1920x1080 = 2
    };

    class Colours{
    public:
        static Colors WHITE;
        static Colors BLACK;
        static Colors RED;
    };

    static const GLchar *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "layout (location = 1) in vec3 aColor;\n"

                                     "out vec3 vertexColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "      gl_Position = vec4(aPos,1.0);\n"
                                     "      vertexColor = aColor;\n"
                                     "}\0";

    static const GLchar *fragmentShaderSource = "#version 330 core\n"
                                              "out vec4 FragColor;\n"
                                              "in vec3 vertexColor;\n"
                                              "void main()\n"
                                              "{\n"
                                              "     FragColor = vec4(vertexColor,1.0);\n"
                                              "}\0";
}


#endif //GRAFIKA_CONSTANTS_H
