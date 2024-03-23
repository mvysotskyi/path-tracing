#include <Windows.h>    
#include <gl/GL.h>    
#include <gl/GLU.h>

#include "model.h"
#include "mesh.h"

TriangularFace::TriangularFace() {
	v1 = nullptr;
	v2 = nullptr;
	v3 = nullptr;
}

void TriangularFace::draw() const {
	glBegin(GL_LINE_LOOP);

	glColor3f(1, 1, 0);
	glVertex3f(v1->x, v1->y, v1->z);
	glVertex3f(v2->x, v2->y, v2->z);
	glVertex3f(v3->x, v3->y, v3->z);
	//glVertex3f(v1->x, v1->y, v1->z);

	glEnd();
}

Model::Model() {}

Model::Model(const std::string& resource_file_path) {
	this->c_texture_id = nullptr;

	std::ifstream resource_file(resource_file_path);

	if (!resource_file.is_open()) {
		std::cerr << "Cannot open " << resource_file_path << std::endl;
		return;
	}

	std::string vertex_mark = "v ";
	std::string tex_vertex_mark = "vt ";
	std::string face_mark = "f ";
	std::string textures_lib_mark = "mtllib ";
	std::string texture_mark = "usemtl ";

	std::string fline;
	while (resource_file.good()) {
		std::getline(resource_file, fline);

		if (fline.substr(0, vertex_mark.size()) == vertex_mark) {
			process_vertex(fline.c_str() + vertex_mark.size());
			continue;
		}

		if (fline.substr(0, face_mark.size()) == face_mark) {
			process_face(fline.c_str() + face_mark.size());
			continue;
		}
	}
}

Model::~Model() {}

void Model::process_vertex(const std::string& vertex_str) {
	vec3f vertex;
	std::stringstream ss(vertex_str);
	
	ss >> vertex.x >> vertex.y >> vertex.z;
	vertices.push_back(vertex);
}

//void Model::process_texture_vertex(const char* raw) {
//	float tex_vertices_buffer[2];
//	_strexcludef(raw, 2, tex_vertices_buffer);
//
//	glm::vec2* tex_vertex = new glm::vec2(tex_vertices_buffer[0], tex_vertices_buffer[1]);
//	m_mesh->push_tex_vertex(tex_vertex);
//}

void Model::process_face(const std::string& face_str) {
	TriangularFace face;
	std::stringstream ss(face_str);

	std::string s1, s2, s3;
	ss >> s1 >> s2 >> s3;

	std::cout << face_str;

	face.v1 = &vertices.at(std::stol(s1) - 1);
	face.v2 = &vertices.at(std::stol(s2) - 1);
	face.v3 = &vertices.at(std::stol(s3) - 1);

	faces.push_back(face);
}

//void Model::process_texture(const char* raw) {
//	Texture* texture;
//
//	try {
//		texture = new Texture(textures_resources.at(raw).c_str());
//	}
//	catch (const std::exception& E) {
//		std::cout << E.what() << std::endl;
//		std::cout << "invalid texture name \"" << raw << "\" may cause this problem\n";
//
//		abort();
//	}
//
//	texture->make_texture();
//	c_texture_id = &texture->texture_id;
//
//	m_mesh->push_texture(texture);
//}

//void Model::process_textures_lib(const char* file_name) {
//	char _folder_buffer[256], file_path[256];
//	_splitpath_s(this->file_path, nullptr, 0, _folder_buffer, 256, nullptr, 0, nullptr, 0);
//	_makepath_s(file_path, 256, nullptr, _folder_buffer, file_name, nullptr);
//
//	std::ifstream i_file(file_path);
//
//	if (!i_file.is_open()) {
//		std::cerr << "Cannot open " << file_path << std::endl;
//		return;
//	}
//
//	const char* none_mark = "None";
//	const char* new_mark = "newmtl ";
//	const char* resource_mark = "map_Kd ";
//
//	std::string _cur_key;
//	std::string _cur_val;
//
//	std::string fline;
//	while (i_file.good()) {
//		std::getline(i_file, fline);
//		const char* cs_fline = fline.c_str();
//
//		if (strstr(cs_fline, new_mark)) {
//			cs_fline += strlen(new_mark);
//			_cur_key = cs_fline;
//
//			if (strcmp(none_mark, cs_fline) == 0) {
//				textures_resources.insert({ _cur_key, "" });
//			}
//		} else if (strstr(cs_fline, resource_mark)) {
//			cs_fline += strlen(resource_mark);
//			_cur_val = cs_fline;
//
//			_cur_val.insert(0, std::string(_folder_buffer));
//
//			textures_resources.insert({ _cur_key, _cur_val });
//		}
//	}
//
//	i_file.close();
//}

void Model::scale(float value) {
	for (vec3f& v : vertices) v *= value;
}

void Model::draw() {
	for (const TriangularFace& face : faces) face.draw();
}