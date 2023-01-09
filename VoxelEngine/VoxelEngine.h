#pragma once

#include "stb_image.h"
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Chunk.h"
#include "Light.h"

#include <vector>

class VoxelEngine {
public:
	VoxelEngine();
	void render();
private:
	Window window;
	Shader mainShader;
	Camera camera;
	bool cameraDirty = true;
	float fps{ 0.f };

	int frameCount{ 0 };
	float previousSeconds{ 0.f };
	float currentFrameTime{ 0.f };
	float deltaTime{ 0.f };
	float lastFrameTime{ 0.f };

	bool firstMouse{ true };
	float lastX;
	float lastY;

	glm::mat4 projection = glm::mat4(1.f);
	glm::mat4 view = glm::mat4(1.f);
	std::vector<Light> lights;
	GLuint uboLights;
	GLuint texture;

	void initShaders();
	void initLights();
	void initTextures();
	
	void processInput(GLFWwindow* win);
	void mouseCallback(double xpos, double ypos);
	static void mouseMoveCallback(GLFWwindow* w, double xPos, double ypos);
	void updateFPSCounter();
};
