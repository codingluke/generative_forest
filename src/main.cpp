// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"
#include "Frustum.h"

using namespace glm;

// Achtung, die OpenGL-Tutorials nutzen glfw 2.7, glfw kommt mit einem veränderten API schon in der Version 3

// Befindet sich bei den OpenGL-Tutorials unter "common"
#include "shader.hpp"

// Wuerfel und Kugel
#include "objects.hpp"

// Ab Uebung5 werden objloader.hpp und cpp benoetigt
#include "objloader.hpp"

// Ab Uebung7 werden texture.hpp und cpp benoetigt
#include "texture.hpp"

#include "Camera.h"

#include "PerlinNoise.h"

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

Camera *camera;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
			case GLFW_KEY_D:
			camera->key_right = true;
    		//y_rot_robot += 5.f;
			break;

			case GLFW_KEY_A:
			camera->key_left = true;
            //y_rot_robot -= 5.f;
			break;

			case GLFW_KEY_W:
			camera->key_up = true;
    		//x_rot_robot += 5.f;
			break;

			case GLFW_KEY_S:
			camera->key_down = true;
            //x_rot_robot -= 5.f;
			break;
			default:
			break;
		}
	}
	else
	{
		switch(key)
		{
			case GLFW_KEY_D:
			camera->key_right = false;
            //y_rot_robot += 5.f;
			break;

			case GLFW_KEY_A:
			camera->key_left = false;
            //y_rot_robot -= 5.f;
			break;

			case GLFW_KEY_W:
			camera->key_up = false;
            //x_rot_robot += 5.f;
			break;

			case GLFW_KEY_S:
			camera->key_down = false;
            //x_rot_robot -= 5.f;
			break;
			default:
			break;
		}
	}
}

void handleMouseMove(GLFWwindow *window, double mouse_x, double mouse_y)
{
	camera->handleMouseMove(window, mouse_x, mouse_y);
}




// Diese Drei Matrizen global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden koennen
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
GLuint programID;


void sendMVP()
{
	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model;
	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform, konstant fuer alle Eckpunkte
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
}


glm::vec3 extractWorldCoords(glm::mat4 mat)
{
	glm::vec3 pos;
	pos[0] = mat[3][0];
	pos[1] = mat[3][1];
	pos[2] = mat[3][2];
	return pos;
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Fehler werden auf stderr ausgegeben, s. o.
	glfwSetErrorCallback(error_callback);


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// Open a window and create its OpenGL context
	// glfwWindowHint vorher aufrufen, um erforderliche Resourcen festzulegen
	int window_width = 1024;
	int window_height = 768;
	GLFWwindow* window = glfwCreateWindow(window_width, // Breite
										  window_height,  // Hoehe
										  "CG - Tutorial", // Ueberschrift
										  NULL,  // windowed mode
										  NULL); // shared windoe

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	// Make the window's context current (wird nicht automatisch gemacht)
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	// GLEW ermöglicht Zugriff auf OpenGL-API > 1.1
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, handleMouseMove);
	glfwSetCursorPos(window, window_width / 2., window_height / 2.);
	camera = new Camera(window, glm::vec3(-1.5, 0.07, -1.4), window_width, window_height);
	camera->setDirection(0.0, 0.7);
	

	
    // std::cout << to_string(camera->getPosition()) << std::endl;
	// Auf Keyboard-Events reagieren
	
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Object treeLow("Content/trees9/tree_1_low.3ds", "Content/trees9/Bark___1.bmp");
	Object tree("Content/trees9/tree_1_mid.3ds", "Content/trees9/Bark___0.bmp");
	Object treeHigh("Content/trees9/tree_1.3ds", "Content/trees9/Bark___0.bmp");

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("Content/StandardShading.vertexshader",
		"Content/StandardShading.fragmentshader");
	// Shader auch benutzen !
	glUseProgram(programID);

	// Lichtquelle positionierung
	//glm::vec4 lightPos = glm::vec4(4,4,-4,0);
	//glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);

	glm::mat4 lightTransformation(1.0);

	

  // Make Perlin Noise, might need some tweaking.
	unsigned int seed = floor(double(random() * 250));
	PerlinNoise pn(seed);
	double treeDensity = 0.2;
	double isTree = 0;
  // Forest matrix
	float forest[100][100] = { 0 };
	for (int row = 0; row < 100; row++) {
		for (int col = 0; col < 100; col++) {
			isTree = floor(random() % 2 + treeDensity);
			if (isTree)
			{
				double val = pn.noise((double)row / 100. , (double)col / 100. , 0.5 ) * 100;
				val = val - floor(val);
				val *= 2;
				// std::cout << val << std::endl;
				forest[row][col] = val;
			}
			else
				forest[row][col] = -1;
		}
	}

	// Eventloop
	glm::mat4 Save = Model;
	Frustum frustum;
	float lastTime = 0.0;
	while (!glfwWindowShouldClose(window))
	{
		// Calculate the processing time for the last frame
		float currentTime = glfwGetTime();
		camera->setDeltaTime(currentTime - lastTime);
		camera->moveOnPlaneXY();
		lastTime = currentTime;
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Projection matrix : 45° Field of View, 4:3 ratio,
    //                     display range : 0.1 unit <-> 100 units
		// Transformationsmatrix!
		Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		frustum.setCamInternals(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);

		// Camera matrix
		// Transformationsmatrix!
		glm::vec3 cam_position = camera->getPosition();
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), cam_position.x, cam_position.y, cam_position.z);
		glm::vec3 cam_direction = camera->getDirection();
		glm::vec3 cam_up = camera->getUp();
		glm::vec3 l = cam_position + cam_direction;
		View = glm::lookAt(cam_position, // Camera is at (0,0,-5), in World Space
						   l,  // and looks at the origin
						   cam_up); // Head is up (set to 0,-1,0 to look upside-down)
		frustum.setCamDef(cam_position, l, cam_up);

		// Model matrix : an identity matrix (model will be at the origin)
		// Transfromationsmatrix!
		Model = glm::mat4(1.0f);
		Save = Model;

		// Zeichne ein Segment
		Model = Save;
		glm::mat4 Save2 = Model;
		//tree.render(Model, View, Projection, programID);
		//Model = glm::translate(Model, glm::vec3(0.0, 0.0, 10.0));
    //head.render(Model, View, Projection, programID);

		Model = Save;
		Model = glm::scale(Model, vec3(4.0f, 0.009f, 4.0f));
		Model = glm::translate(Model, vec3(0.7f, 0.0f, 0.7f));
		sendMVP();
		drawCube();
    //glDeleteTextures(1, &texture);
		Model = Save;

		tree.render(Model, View, Projection, programID);

    // Zeichne Wald

		float scale = 0.0f;
		for (int row = 0; row < 100; row++) {
			for (int col = 0; col < 100; col++) {
				if (forest[row][col] > 0) {
					Save2 = Model;
					glm::vec3 position = extractWorldCoords(Model) + glm::vec3(0.0, 0.15, 0.0);
					float dist = camera->getDistanceTo(position);
					Model = glm::rotate(Model, -90.0f, glm::vec3(1, 0, 0));
					scale = forest[row][col];
					Model = glm::scale(Model, glm::vec3(scale, scale, scale));
					if (scale > 0 && frustum.pointInFrustum(position)) {
						if (dist < 1) {
							tree.render(Model, View, Projection, programID);
						} else if (dist < 6) {
							treeLow.render(Model, View, Projection, programID);
						}
					}
					Model = Save2;
				}
				Model = glm::translate(Model, glm::vec3(0.0, 0.0, 0.2));
			}
			Model = glm::translate(Model, glm::vec3(0.2, 0.0, -20.0));
		}


		//lightTransformation = glm::translate(Model, glm::vec3(0.0, 0.3, 0.0));
		//lightPos = lightTransformation * glm::vec4(0,0,0,1);
		//glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);

        // drawGround(-100.0f); // Draw lower ground grid
        // drawGround(100.0f);  // Draw upper ground grid
		// Swap buffers, hintergrundspeicher in vordergrundspeicher laden.
		// Das Bild wird zu erst im hintergrundspeicher vollständig gezeichnet und dann
		// in den Vordergrundspeicher geladen, so wird "flakern" beim rendering vermieden.
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();

        //     	glfwSetCursorPosCallback(window, NULL);
        //		...
        //     	glfwSetCursorPosCallback(window, handleMouseMove);
	}

	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	delete camera;
	return 0;
}

