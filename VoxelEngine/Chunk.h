#pragma once

#include "Block.h"
#include "MeshLoader.h"

#include <memory>
#include <iostream>

constexpr float BLOCK_WIDTH = 1.f;

class Chunk {
public:
	Chunk(){
		MeshLoader loader(std::string(SRC_DIR) + "/Models/cube.obj");
		vertices = loader.getData();

		blocks = std::make_unique<std::unique_ptr<std::unique_ptr<Block[]>[]>[]>(CHUNK_SIZE);
		for (uint8_t i = 0; i < CHUNK_SIZE; i++) {
			blocks[i] = std::make_unique<std::unique_ptr<Block[]>[]>(CHUNK_SIZE);
			for (uint8_t j = 0; j < CHUNK_SIZE; j++) {
				blocks[i][j] = std::make_unique<Block[]>(CHUNK_SIZE);
			}
		}
		initBuffers();
		setupSphere();
		createMesh();
	}
	
	~Chunk(){}
	
	void render(){
		glBindVertexArray(vao);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, INSTANCE_COUNT);
		glBindVertexArray(0);
	}	
private:
	std::unique_ptr<std::unique_ptr<std::unique_ptr<Block[]>[]>[]> blocks;
	static const int CHUNK_SIZE = 32;
	GLuint vao, vbo;
	GLuint modelMatrixBuffer;
	GLuint modelInvTransBuffer;
	int INSTANCE_COUNT = 0;
	
	//Cube info
	std::vector<VertexData> vertices;

	void initBuffers() {	
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glCreateBuffers(1, &vbo);
		glNamedBufferData(vbo, vertices.size() * sizeof(VertexData), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(glm::vec3)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(2 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(2);

		//model matrix buffer
		glGenBuffers(1, &modelMatrixBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, modelMatrixBuffer);
		glNamedBufferData(modelMatrixBuffer, sizeof(glm::mat4) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, 0, GL_DYNAMIC_DRAW);

		int modelMatrixLoc = 3;
		for (int i = 0; i < 4; i++) {
			glVertexAttribPointer(modelMatrixLoc + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
			glEnableVertexAttribArray(modelMatrixLoc + i);
			glVertexAttribDivisor(modelMatrixLoc + i, 1);
		}

		//normal matrix buffer
		glGenBuffers(1, &modelInvTransBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, modelInvTransBuffer);
		glNamedBufferData(modelInvTransBuffer, sizeof(glm::mat3) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, 0, GL_DYNAMIC_DRAW);

		int normalMatrixLoc = 7;
		for (int i = 0; i < 3; i++) {
			glVertexAttribPointer(normalMatrixLoc + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)(sizeof(glm::vec3) * i));
			glEnableVertexAttribArray(normalMatrixLoc + i);
			glVertexAttribDivisor(normalMatrixLoc + i, 1);
		}
	}

	void createMesh() {
		glBindBuffer(GL_ARRAY_BUFFER, modelMatrixBuffer);
		glm::mat4* matrices = (glm::mat4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		glBindBuffer(GL_ARRAY_BUFFER, modelInvTransBuffer);
		glm::mat3* matricesInvTrans = (glm::mat3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		INSTANCE_COUNT = 0;
		for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
			for (uint8_t y = 0; y < CHUNK_SIZE; y++) {
				for (uint8_t z = 0; z < CHUNK_SIZE; z++) {
					if (!blocks[x][y][z].isActive()) { continue; }
					glm::mat4 tmp = glm::translate(glm::vec3{ x * BLOCK_WIDTH, y * BLOCK_WIDTH, -z * BLOCK_WIDTH });
					matrices[INSTANCE_COUNT] = tmp;
					matricesInvTrans[INSTANCE_COUNT] = glm::mat3(glm::transpose(glm::inverse(tmp)));
					INSTANCE_COUNT++;
				}
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, modelMatrixBuffer);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, modelInvTransBuffer);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void setupSphere() {
		for (uint8_t x = 0; x < CHUNK_SIZE; x++) {
			for (uint8_t y = 0; y < CHUNK_SIZE; y++) {
				for (uint8_t z = 0; z < CHUNK_SIZE; z++) {
					if (sqrt((float)(x - CHUNK_SIZE / 2) * (x - CHUNK_SIZE / 2) + (y - CHUNK_SIZE / 2) * (y - CHUNK_SIZE / 2) + (z - CHUNK_SIZE / 2) * (z - CHUNK_SIZE / 2)) <= CHUNK_SIZE / 2) {
						blocks[x][y][z].setActive(true);
					}
				}
			}
		}
	}
};

