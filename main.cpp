// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <Windows.h>    
#include <gl/GL.h>    
#include <gl/GLU.h>

#include "glm/GLM/glm.hpp"

#include <iostream>
#include <ctime>

#include "vec/vec.h"
#include "defines.h"

#include "texture.h"
#include "model.h"
#include "player.h"

#include <GLFW/glfw3.h>

#define WIN_TITLE "Model"
#define SCENE_OBJ_FILE "F:\\ConsoleApplication1\\Debug\\__model.obj"

#define SC_WIDTH  1000
#define SC_HEIGHT 800

#define MAX_FPS 80

static float FPS = 0;

void fps_repr() {
	std::cout << "FPS: " << FPS << std::endl;
}

void __key_callback(GLFWwindow* window, KEY key, int scancode, int action, int mods);
void __mouse_passive_motion_callback(GLFWwindow*, double, double);

class MainLoop {
private:
	GLFWwindow* window;
	const char* win_title;
	unsigned int update_delay = 1000 / MAX_FPS;
	unsigned int sc_width, sc_height, sc_width_2, sc_height_2;
	Player* player;
	Model* scene;
	Camera* camera;

public:
	MainLoop(unsigned int sc_width, unsigned int sc_height, const char* win_title, const char* scene_model_path) {
		this->sc_width = sc_width;
		this->sc_height = sc_height;
		this->sc_width_2 = sc_height / 2;
		this->sc_height_2 = sc_height / 2;
		this->win_title = win_title;

		this->window = nullptr;
		this->scene = new Model(scene_model_path);
		scene->scale(1.0f / 130.0f);
		this->player = new Player(vec3f(0, 0.0f, 3.0f));
	}

	~MainLoop() {
		delete player;
		delete scene;
	}

	void init() {
		if(!glfwInit()) exit(EXIT_FAILURE);	
		window = glfwCreateWindow(sc_width, sc_height, win_title, NULL, NULL);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, __mouse_passive_motion_callback);
		glfwSetKeyCallback(window, __key_callback);
		glfwSetErrorCallback(this->error_callback);

		if (!window) {
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);
	}

	void launch() {
		glfwGetFramebufferSize(window, (int*)&sc_width, (int*)&sc_height);
		glViewport(0, 0, sc_width, sc_height);

		camera = new Camera(player->position());
		camera->bind_to(player);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(camera->fov_y, (float)sc_width / (float)sc_height, 0.01f, 100.0f);

		glShadeModel(GL_FLAT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		while (!glfwWindowShouldClose(window))
		{
			this->display();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}

	void display() {		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		player->update();
		camera->update();
		//scene->draw();

		// World Center lines 
		//glBindTexture(GL_TEXTURE_2D, 0);

		glBegin(GL_LINES);
		glColor3f(1, 0, 0);

		glVertex3f(-2.0f, 0, 0);
		glVertex3f(2.0f, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, -2.0f);
		glVertex3f(0, 0, 2.0f);

		glColor3f(0, 0, 1);
		glVertex3f(0, -2.0f, 0);
		glVertex3f(0, 2.0f, 0);

		//glEnd();

		//glBegin(GL_TRIANGLES);
		//glColor3f(1, 0, 0);

		//glVertex3f(-2.0f, 0, 0);
		//glVertex3f(2.0f, 0, 0);
		//glVertex3f(0.0f, 2, 0);
		//
		glColor3f(0, 0, 0);
		glEnd();

		glBegin(GL_LINES);
		glColor3f(1, 0, 1);
		glEnd();


		scene->draw();
	}

	void key_process(KEY key, int action) {
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_E) fps_repr();

			player->keydown_update(key);
		} else if(action == GLFW_RELEASE) {
			player->keyup_update(key);
		}
	}

	void keydown_process(KEY key) {
		player->keydown_update(key);
	}

	void keyup_process(KEY key) {
		player->keyup_update(key);
	}

	void mouse_passive_motion(double x, double y) {
		double dx = sc_width_2 - x;
		double dy = sc_height_2 - y;

		player->mouse_position_update(dx, dy);
		glfwSetCursorPos(window, sc_width_2, sc_height_2);
	}

	static void error_callback(int error, const char* description) {
		time_t c_time;
		time(&c_time);

		char timestamp[80];
		ctime_s(timestamp, 80, &c_time);

		std::cerr << timestamp << "\tError: " << description << std::endl;
	}
};

MainLoop* cInstance;

void __key_callback(GLFWwindow* window, KEY key, int scancode, int action, int mods) { cInstance->key_process(key, action); }
void __mouse_passive_motion_callback(GLFWwindow* window, double x, double y) { cInstance->mouse_passive_motion(x, y); }

int main(int argc, char** argv) {
	atexit(&fps_repr);

	MainLoop mloop = MainLoop(
		SC_WIDTH,
		SC_HEIGHT,
		WIN_TITLE,
		SCENE_OBJ_FILE
	);
	cInstance = &mloop;

	mloop.init();
	mloop.launch();

	return 0;
}