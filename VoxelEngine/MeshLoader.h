#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

struct VertexData {
	glm::vec3 vertices;
	glm::vec3 normals;
	glm::vec2 textures;
};

class MeshLoader {
public:
	MeshLoader(const std::string& path) {
		file.open(path);
		if (!file.is_open()) {
			std::cerr << "File could not be opened!\n";
			exit(-1);
		}
	}
	std::vector<VertexData> getData() {
		std::vector<VertexData> data;

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> textures;
		std::vector<glm::vec3> normals;

		std::string line;
		std::string id;

		while (std::getline(file, line)) {
			std::istringstream lineStream(line);
			lineStream >> id;

			if (id == "v") {
				float x, y, z;
				lineStream >> x >> y >> z;
				vertices.push_back(glm::vec3(x, y, z));
			}
			else if (id == "vt") {
				float x, y;
				lineStream >> x >> y;
				textures.push_back(glm::vec2(x, y));
			}
			else if (id == "vn") {
				float x, y, z;
				lineStream >> x >> y >> z;
				normals.push_back(glm::vec3(x, y, z));
			}
			else if (id == "f") {
				std::string v, t, n;
				for (unsigned int i = 0; i != 3; i++) {
					std::getline(lineStream, v, '/');
					std::getline(lineStream, t, '/');
					std::getline(lineStream, n, ' ');

					data.push_back(VertexData{ vertices[std::stoi(v) - 1], normals[std::stoi(n) - 1], textures[std::stoi(t) - 1] });
				}
			}
		}
		return data;
	}
private:
	std::ifstream file;
};