#include "VoxelEngine.h"

VoxelEngine::VoxelEngine() : camera(glm::vec3{16, 16, 20}) {
	window = Window("VoxelEngine", 800, 600);
	window.setColor({ 0,0,0,1 });
	initShaders();
	initLights();
	initTextures();
	//callbacks
	glfwSetWindowUserPointer(window.getParentWindow(), this);
	glfwSetCursorPosCallback(window.getParentWindow(), mouseMoveCallback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void VoxelEngine::render() {
	Chunk chunk;
	
	GLuint blockIdx = glGetUniformBlockIndex(mainShader.ID, "LightBlock");
	glUniformBlockBinding(mainShader.ID, blockIdx, 0);

	glCreateBuffers(1, &uboLights);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * lights.size(), &lights.front(), GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, blockIdx, uboLights);

	glBindTexture(GL_TEXTURE_2D, texture);

	while (window.isOpened()) {
		currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		updateFPSCounter();
		processInput(window.getParentWindow());

		window.clearBuffers();
		
		mainShader.use();
		if (cameraDirty) {
			auto viewMatrix = camera.getViewMatrix();
			mainShader.setMat4("view", viewMatrix);
			mainShader.setMat3("viewInvTrans", glm::mat3(glm::transpose(glm::inverse(viewMatrix))));
			mainShader.setVec3("EyeDirection", -camera.getFrontVec());
			cameraDirty = false;
		}
		projection = glm::perspective(45.f, window.aspect(), 0.1f, 100.f);
		mainShader.setMat4("projection", projection);
		chunk.render();

		window.swapBuffers();
		window.poll();
	}
	window.terminate();
}

void VoxelEngine::initShaders() {
	mainShader = Shader((std::string(SRC_DIR) + "/Shaders/vshader.glsl").c_str(), (std::string(SRC_DIR) + "/Shaders/fshader.glsl").c_str());
}

void VoxelEngine::initLights() {
	lights.push_back(Light{ .color = glm::vec3(1,1,1),
							.isEnabled = true,
							.ambient = glm::vec3(0.1, 0, 0),
							.isLocal = true,
							.position = glm::vec3(16, 36, 0),
							.isSpot = false,
							.halfVector = {},
							.spotCosCutoff = 0.8,
							.coneDirection = glm::vec3(0,0,-1),
							.spotExponent = 2,
							.constantAttenuation = 0.1f,
							.linearAttenuation = 0.1f,
							.quadraticAttenuation = 0.1f
	});
}

void VoxelEngine::initTextures() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load((std::string(SRC_DIR) + "/Textures/wood.jpg").c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void VoxelEngine::processInput(GLFWwindow* win) {
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(win, true);
	}

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
		camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
		cameraDirty = true;
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
		camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
		cameraDirty = true;
	}
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
		camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
		cameraDirty = true;
	}
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
		camera.processKeyboard(CameraMovement::LEFT, deltaTime);
		cameraDirty = true;
	}
}


void VoxelEngine::mouseCallback(double xpos, double ypos) {
	float x = (float)xpos;
	float y = (float)ypos;

	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xoffset = x - lastX;
	float yoffset = lastY - y;	
	lastX = x;
	lastY = y;

	camera.processMouseMovement(xoffset, yoffset, deltaTime);
	cameraDirty = true;
}

void VoxelEngine::mouseMoveCallback(GLFWwindow* w, double xpos, double ypos) {
	VoxelEngine* obj = static_cast<VoxelEngine*>(glfwGetWindowUserPointer(w));
	obj->mouseCallback(xpos, ypos);
}

void VoxelEngine::updateFPSCounter() {
	float elapsedSeconds = currentFrameTime - previousSeconds;
	if (elapsedSeconds > 0.25) {
		previousSeconds = currentFrameTime;
		std::ostringstream title;
		fps = (double)frameCount / elapsedSeconds;
		title << fps << " FPS";
		glfwSetWindowTitle(window.getParentWindow(), title.str().c_str());
		frameCount = 0;
	}
	frameCount++;
}
