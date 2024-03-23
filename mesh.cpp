#include "mesh.h"

Face::Face(size_t vertices_count, unsigned int* ptexture_id) {
	this->vertices_count = vertices_count;
	this->ptexture_id = ptexture_id;
}

Face::~Face() {

}

void Face::push_vertex(glm::vec3* vertex) {
	vertices.push_back(vertex);
}

void Face::push_tex_vertex(glm::vec2* t_vertex) {
	tex_vertices.push_back(t_vertex);
}

void Face::draw() {
	assert(ptexture_id != nullptr);
	glBindTexture(GL_TEXTURE_2D, *ptexture_id);

	glBegin(GL_TRIANGLES);

	for (size_t i = 0; i < vertices_count; i++) {
		glm::vec3* vertex = vertices.at(i);
		glm::vec2* tex_vertex = tex_vertices.at(i);

		glTexCoord2f(tex_vertex->x, tex_vertex->y);
		glVertex3f(vertex->x, vertex->y, vertex->z);
	}

	glEnd();
}


Mesh::Mesh() {
	this->pmax_coord = nullptr;
}

Mesh::~Mesh() {
	for (Face* f : faces) delete f;
	for (glm::vec3* v : vertices) delete v;
	for (glm::vec2* tv : tex_vertices) delete tv;
	for (Texture* tex : textures) delete tex;
}

void Mesh::scale(float _value) {
	if (pmax_coord == nullptr) {
		float _max[1];
		*_max = 0;

		for (glm::vec3* vertex : vertices) {

		}
	}
}

void Mesh::push_texture(Texture* texture) {
	textures.push_back(texture);
}

void Mesh::push_face(Face* face) {
	faces.push_back(face);
}

void Mesh::push_vertex(glm::vec3* vertex) {
	vertices.push_back(vertex);
}

void Mesh::push_tex_vertex(glm::vec2* tex_vertex) {
	tex_vertices.push_back(tex_vertex);
}

void Mesh::draw() {
	for (Face* face : faces) face->draw();
}