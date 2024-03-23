#pragma once

#include <vector>
#include "glm/GLM/glm.hpp"

#include "texture.h"

class Face {
private:
	size_t vertices_count;
	unsigned int* ptexture_id;

	std::vector<glm::vec3*> vertices;
	std::vector<glm::vec2*> tex_vertices;

public:
	Face(size_t vertices_count, unsigned int* ptexture_id);
	~Face();

	void push_vertex(glm::vec3* vertex);
	void push_tex_vertex(glm::vec2* t_vertex);

	void draw();
};

class Mesh {
private:
	std::vector<Texture*> textures;
	std::vector<Face*> faces;
	float* pmax_coord;

public:
	std::vector<glm::vec3*> vertices;
	std::vector<glm::vec2*> tex_vertices;

public:
	Mesh();
	~Mesh();

	void scale(float value);
	void push_texture(Texture* texture);
	void push_face(Face* face);
	void push_vertex(glm::vec3* vertex);
	void push_tex_vertex(glm::vec2* tex_vertex);

	void draw();
};