#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include "TestBed.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#define _USE_MATH_DEFINES 
#include <cmath>         

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
unsigned int loadImageToTexture(const char* filePath);

void drawProgressBar(unsigned int shaderProgram, float startX, float startY, float width, float height, int numDivisions, int filledDivisions) {
    float divisionWidth = width / numDivisions;
    bool useTexture = false;

    for (int i = 0; i < numDivisions; ++i) {
        float x = startX + i * divisionWidth;
        float vertices[] = {
            x, startY, x + divisionWidth, startY, x + divisionWidth, startY + height, x, startY + height
        };
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        float r, g, b;
        if (i < filledDivisions) {
            r = 0.0f;
            g = 0.0f;
            b = 1.0f;
        }
        else {
            r = 1.0f;
            g = 0.0f;
            b = 0.0f; 
        }

        int colorLoc = glGetUniformLocation(shaderProgram, "uColor");
        int useTextureLoc = glGetUniformLocation(shaderProgram, "useTexture");
        glUniform3f(colorLoc, r, g, b);
        glUniform1i(useTextureLoc, (int)useTexture);

        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }
}


void drawCompassNeedle(unsigned int shaderProgram, float centerX, float centerY, float radius, int numSegments, float aspectRatio) {
    float twoPi = 2.0f * M_PI;
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    std::vector<float> circleVertices;

    // Crtanje kruga
    for (int i = 0; i <= numSegments; i++) {
        float theta = twoPi * float(i) / float(numSegments);
        float x = radius * cosf(theta) / aspectRatio;
        float y = radius * sinf(theta);
        circleVertices.push_back(centerX + x);
        circleVertices.push_back(centerY + y);
    }

    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    

    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f); // Bela boja za krug
    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 1);

    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    float time = glfwGetTime();
    float angle = sin(time) * (M_PI / 6); // Osciluje ±30 stepeni

    float dx = radius * cosf(angle + (M_PI / 2)) / aspectRatio;
    float dy = radius * sinf(angle + (M_PI / 2));

    std::vector<float> lineVertices = {
        centerX - dx, centerY - dy, // Gornji kraj linije
        centerX, centerY,          // Centar kruga
        centerX + dx, centerY + dy // Donji kraj linije
    };


    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(float), lineVertices.data(), GL_STATIC_DRAW);
    glLineWidth(3.0f); // Podesite debljinu po želji
    // Crvena polovina
    glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f); // Crvena boja
    glDrawArrays(GL_LINES, 0, 2);

    // Plava polovina
    glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f); // Plava boja
    glDrawArrays(GL_LINES, 1, 2);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}



void drawSemiCircle(unsigned int shaderProgram, float centerX, float centerY, float radius, int numSegments, float aspectRatio, float speed) {
    float twoPi = 2.0f * M_PI;
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float time = glfwGetTime(); // Brzina translacije
    float translation = fmod(speed/10, 2.0f * aspectRatio) - aspectRatio; //ovde umesto time ubaciti speed da zavisi od brzine

    std::vector<float> circleVertices;
    circleVertices.push_back(centerX + translation); // Transliran centar X
    circleVertices.push_back(centerY); // Centar Y

    for (int i = 0; i <= numSegments / 2; i++) {
        float theta = twoPi * float(i) / float(numSegments);
        float x = radius * cosf(theta) / aspectRatio + translation; // Translacija
        float y = radius * sinf(theta);
        circleVertices.push_back(centerX + x);
        circleVertices.push_back(centerY + y);
    }

    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform3f(colorLoc, 0.48f, 0.65f, 0.70f);

    int useTextureLoc = glGetUniformLocation(shaderProgram, "useTexture");
    glUniform1i(useTextureLoc, 0);

    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments / 2 + 2);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}


void drawBlueRectangle(unsigned int shaderProgram, float windowHeight) {
    float rectHeight = 2.0f / 6.0f; // 1/6 visine prozora
    float upperY = 0.0f;
    float lowerY = upperY - rectHeight;
    float vertices[] = {
        // X      Y
        -1.0f,  upperY,       // Gornji levi ugao
         1.0f,  upperY,       // Gornji desni ugao
         1.0f,  lowerY,       // Donji desni ugao
        -1.0f,  lowerY        // Donji levi ugao
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Postavljanje boje na plavu
    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    //glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f); // RGB za plavu boju
    glUniform3f(colorLoc, 0.48f, 0.65f, 0.70f);

    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}





int main(void) {
    Ship ship = getShip();
    if (!glfwInit()) {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

 
    const char wTitle[] = "Brod";

    GLFWwindow* window;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor(); // Get the primary monitor
    const GLFWvidmode* mode = glfwGetVideoMode(monitor); // Get the video mode of the monitor

    // Use the video mode width and height for fullscreen
    window = glfwCreateWindow(mode->width, mode->height, wTitle, monitor, NULL);
    if (window == NULL) {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
    unsigned int texture = loadImageToTexture("Pictures/projekat1.jpg");

    float vertices[] = {
        // X      Y       S    T
        -1.0f, -1.0f,   0.0f, 1.0f, // Bottom Left
         1.0f, -1.0f,   1.0f, 1.0f, // Bottom Right
         1.0f, -0.33f,  1.0f, 0.0f, // Top Right
        -1.0f, -0.33f,  0.0f, 0.0f  // Top Left
    };



    unsigned int indices[] = {
        0, 1, 2, // First Triangle
        2, 3, 0  // Second Triangle
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    startSimulation(&ship);
    double progressSpeed = 0;
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        //std::cout << "ship.fuel: " << ship.getFuelAmount() << std::endl;
        //std::cout << "Ship.maxfuel: " << ship.getMaxFuelAmount() << std::endl;
        std::cout << "Ship.maxfuel: " << ship.getMaxSpeed() << std::endl;
        std::cout << "Ship: " << ship.getSpeed() << std::endl;

        int windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);


        glClearColor(0.0f, 0.0f, 0.5f, 1.0f); // Tamnija plava boja



        glClear(GL_COLOR_BUFFER_BIT);

        // Crtanje slike
        glUseProgram(unifiedShader);
        glUniform1i(glGetUniformLocation(unifiedShader, "useTexture"), 1); 
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Crtanje progres bara

         //= ship.getSpeed(); // Primer: 5 od 10 podeoka je popunjeno
        int progressFuel = (int)((float)ship.getFuelAmount() / (float)ship.getMaxFuelAmount() * 10);
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
            progressSpeed += 0.5;
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            progressSpeed = 0;
        }
        if (progressFuel < 0) {
            progressFuel = 0;
        }
        else if (progressFuel > 10) {
            progressFuel = 10;
        }
        drawProgressBar(unifiedShader, -0.85f, -0.9f, 0.2f, 0.05f, 10, progressFuel);
        drawProgressBar(unifiedShader, -0.45f, -0.55f, 0.9f, 0.07f, 1000, progressSpeed);

        float circleRadius = 0.1f; // Radius kruga
        float circleX = 1.02f - circleRadius - 0.05f; // X koordinata
        float circleY = -1.0f + circleRadius + 0.05f; // Y koordinata
        int numSegments = 100; // Broj segmenata
        // U glavnoj funkciji
        drawCompassNeedle(unifiedShader, circleX, circleY, circleRadius, numSegments, aspectRatio);


        float semiCircleRadius = 0.5f; // Radius of the semi-circle
        int semiCircleSegments = 100; // Number of segments for smoothness
        float semiCircleY = -0.9f + semiCircleRadius + 0.1f; // Adjust Y-position here

        drawSemiCircle(unifiedShader, 0.0f, semiCircleY, semiCircleRadius, semiCircleSegments, aspectRatio,ship.getSpeed());

        drawBlueRectangle(unifiedShader, static_cast<float>(windowHeight));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    endSimulation(&ship);

    glDeleteTextures(1, &texture);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(unifiedShader);

    glfwTerminate();
    return 0;
}




unsigned int compileShader(GLenum type, const char* source) {
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open()) {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
    std::string temp = ss.str();
    const char* sourceCode = temp.c_str();

    unsigned int shader = glCreateShader(type);
    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}

unsigned int createShader(const char* vsSource, const char* fsSource) {
    unsigned int program = glCreateProgram();
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}

unsigned int loadImageToTexture(const char* filePath) {
    int TextureWidth, TextureHeight, TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData) {
        std::cout << "Slika uspesno ucitana: " << filePath << std::endl;
        std::cout << "Dimenzije: " << TextureWidth << " x " << TextureHeight << std::endl;
        std::cout << "Broj kanala: " << TextureChannels << std::endl;

        GLint InternalFormat = GL_RGB;
        if (TextureChannels == 4) {
            InternalFormat = GL_RGBA;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(ImageData);
        return Texture;
    }
    else {
        std::cerr << "Greska: Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        return 0;
    }
}

