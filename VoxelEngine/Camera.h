#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float FOV = 45.f;

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
public:
	Camera(const glm::vec3& pos, const glm::vec3& upVec = glm::vec3(0, 1, 0), float yawAngle = YAW, float pitchAngle = PITCH) : movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV) {
		position = pos;
		front = glm::vec3(0, 0, -1);
		worldUp = upVec;
		yaw = yawAngle;
		pitch = pitchAngle;
		updateCameraVectors();
	}

	void processKeyboard(CameraMovement direction, float deltaTime) {
		float velocity = movementSpeed * deltaTime;
		switch (direction)
		{
		case CameraMovement::FORWARD:
			position += front * velocity;
			break;
		case CameraMovement::BACKWARD:
			position -= front * velocity;
			break;
		case CameraMovement::LEFT:
			position -= right * velocity;
			break;
		case CameraMovement::RIGHT:
			position += right * velocity;
			break;
		default:
			break;
		}
	}

	void processMouseMovement(float xoffset, float yoffset, float deltaTime) {
		yaw += xoffset * mouseSensitivity * (deltaTime * 1000.f);
		pitch += yoffset * mouseSensitivity * (deltaTime * 1000.f);
		
		if (yaw >= 360) {
			yaw = yaw - 360;
		}
		if (yaw <= -360) {
			yaw = yaw + 360;
		}
		if (pitch > 89.f) {
			pitch = 89.f;
		}
		if (pitch < -89.f) {
			pitch = -89.f;
		}
		updateCameraVectors();
	}
	
	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}

	glm::vec3 getFrontVec() {
		return front;
	}

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float fov;

	void updateCameraVectors() {
		glm::vec3 tempFront;
		tempFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		tempFront.y = sin(glm::radians(pitch));
		tempFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(tempFront);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
};
