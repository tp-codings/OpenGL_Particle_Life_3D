#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader/shader.h>
#include <Camera/camera.h>
#include <ModelLoader/model.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#include <imgui/imconfig.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Life3D_Particles.h"

class Life3D_Engine
{
public:
	Life3D_Engine();
	void run();

private:
	//Window
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	int windowPosX;
	int windowPosY;

	GLFWwindow* window;

	//Font
	FT_Library ft;
	FT_Face face;

	int fontSize;

	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	std::map<char, Character> Characters;

	//Timing
	float deltaTime;
	float lastFrame;
	float lastTime;
	float TIME_STEP;
	float currentFrame;
	float FPS;

	int frameCount;

	//Camera
	Camera camera;

	float lastMouseX;
	float lastMouseY;
	bool firstMouse;

	//Shader
	Shader particleShader;
	Shader screenShader;
	Shader cubeShader;
	Shader textShader;

	//Matrizen
	glm::mat4 textProjection;
	glm::mat4 projection;
	glm::mat4 view;

	float attraction[5][5];

	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::vec3> colorData;

	//Particles
	std::vector<Life3D_Particles*> red;
	std::vector<Life3D_Particles*> yellow;
	std::vector<Life3D_Particles*> green;
	std::vector<Life3D_Particles*> blue;
	std::vector<Life3D_Particles*> white;
	std::vector<std::vector<Life3D_Particles*>> particles;

	Model* lightSource;
	glm::vec3 dirLightDirection;
	glm::vec3 dirLightPos;
	float angleHor;
	float angleVer;

	//World objects
	Model* sphere;
	Model* cube;

	//Settings
	int amount;
	int postProcessingChoice;
	int shaderChoice;

	float timeFactor;
	float distanceMax;
	float cubeSize;
	float scale;
	float cameraSpeed;
	float friction;
	float frictionHalfLife;

	bool viewMode;
	bool start;
	bool showBorder;

	ImVec4 dirLightColor;

	//Framebuffer
	unsigned int screenVAO;
	unsigned int screenVBO;
	unsigned int framebuffer;
	unsigned int texColorBuffer;
	unsigned int rbo;

	unsigned int textVAO;
	unsigned int textVBO;

	unsigned int instanceVBO;
	unsigned int colorVBO;

	//Vertices
	float* cubemapVertices;
	float* quadVertices;
	float* cubeVertices;

	//Inits------------------------------------------------------------------------------

	int initWindow();

	void initCamera();
	void initVertices();
	void initBuffer();
	void initShader();
	void initTextures();
	void initFont();
	void initVariables();

	//Input------------------------------------------------------------------------------

	void input();
	void processInput(GLFWwindow* window, Shader reflectionShader);

	//Callbacks------------------------------------------------------------------------------

	void framebuffer_size_callback(int width, int height);
	static void framebuffer_size_callback_wrapper(GLFWwindow* window, int width, int height);

	void mouse_callback(double xpos, double ypos);
	static void mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos);

	void scroll_callback(double xoffset, double yoffset);
	static void scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset);

	//Helper------------------------------------------------------------------------------

	unsigned int loadTexture(char const* path);
	std::vector<Life3D_Particles*> create(int number, glm::vec3 color);
	int random(int range, int start);
	float force(float x, float a);

	//Updates------------------------------------------------------------------------------

	void update();
	void updateDeltaTime();
	void updateCamera();
	void updateInteraction(std::vector<Life3D_Particles*> particle1, std::vector<Life3D_Particles*> particle2, float attraction);

	//Rendering------------------------------------------------------------------------------

	void Draw();
	void DrawScene();
	void DrawSettings();
	void DrawScreen();
	void DrawCube();
	void DrawText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color);
};

