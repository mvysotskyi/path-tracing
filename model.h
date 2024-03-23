#pragma once

#include <vector>
#include <string>
#include <cstdio>
#include <sstream>
#include <unordered_map>

#include "defines.h"
#include "mesh.h"
#include "texture.h"

struct TriangularFace {
	vec3f *v1, *v2, *v3;

	TriangularFace();
	void draw() const;
};

class Model {
private:
	std::unordered_map<std::string, std::string> textures_resources;
	unsigned int* c_texture_id = nullptr;

	std::vector<vec3f> vertices;
	std::vector<TriangularFace> faces;

private:
	void process_vertex(const std::string& vertex_str);
	//void process_texture_vertex(const char* raw);
	void process_face(const std::string& face_str);
	//void process_texture(const char* raw);
	//void process_textures_lib(const char* file_name);

public:
	Model();
	Model(const std::string& resource_file_path);
	~Model();

	void scale(float value);
	void draw();
};