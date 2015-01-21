// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/type_ptr.hpp>
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

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

Camera *camera;

float x_rot = 0.0f;
float y_rot = 0.0f;
float z_rot = 0.0f;

float x_rot_robot = 0.0f;
float y_rot_robot = 0.0f;
float z_rot_robot = 0.0f;

float z_rot_robot_hand = 0.0f;

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

void drawCS()
{
	glm::mat4 Save = Model;
	Model = glm::translate(Model, glm::vec3(0.0, 0.0, 0.0));
	Model = glm::scale(Model, glm::vec3(2.0, 0.01, 0.01));
	sendMVP();
	drawCube();

	Model = Save;
	Model = glm::translate(Model, glm::vec3(0.0, 0.0, 0.0));
	Model = glm::scale(Model, glm::vec3(0.01, 2.0, 0.01));
	sendMVP();
	drawCube();

	Model = Save;
	Model = glm::translate(Model, glm::vec3(0.0, 0.0, 0.0));
	Model = glm::scale(Model, glm::vec3(0.01, 0.01, 2.0));
	sendMVP();
	drawCube();
}

void drawGround(float groundLevel)
{
    GLfloat extent      = 600.0f; // How far on the Z-Axis and X-Axis the ground extends
    GLfloat stepSize    = 20.0f;  // The size of the separation between points

    // Set colour to white
    glColor3ub(255, 255, 255);

    // Draw our ground grid
    glBegin(GL_LINES);
    for (GLint loop = -extent; loop < extent; loop += stepSize)
    {
        // Draw lines along Z-Axis
        glVertex3f(loop, groundLevel,  extent);
        glVertex3f(loop, groundLevel, -extent);

        // Draw lines across X-Axis
        glVertex3f(-extent, groundLevel, loop);
        glVertex3f( extent, groundLevel, loop);
    }
    glEnd();
}

void drawSeg(float h)
{
	glm::mat4 Save = Model;
	Model = glm::translate(Model, glm::vec3(0.0, h / 2, 0.0));
	Model = glm::scale(Model, glm::vec3(h / 5, h / 2, h / 5));
	sendMVP();
	drawSphere(10,10);
	Model = Save;
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
    glfwSetCursorPos(window, window_width / 2., window_height / 2.);
    camera = new Camera(window, vec3(0.0, 0.0, 0.0), window_width, window_height);
    std::cout << to_string(camera->getPosition()) << std::endl;
	// Auf Keyboard-Events reagieren
	glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, handleMouseMove);
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// UEBUNG 5 //
	std::vector<unsigned short> indices; // Eckpunkte des Objektes
	std::vector<glm::vec3> vertices; // Eckpunkte des Objektes
	std::vector<glm::vec2> uvs; // Texturen koordinaten, an welchem Eckpunkt ist welches pixel der Textur.
	std::vector<glm::vec3> normals; // Beleuchtung der Eckpunkte, winkel des Lichtes

	//bool res = loadOBJ("Content/ladybird.obj", vertices, uvs, normals);
	bool res = loadAssImp("Content/Hlow.DAE", indices, vertices, uvs, normals);
	//bool res = loadOBJ("Content/teapot.obj", vertices, uvs, normals);

	// Jedes Objekt eigenem VAO (VertexArrayObject) zuordnen, damit mehrere Objekte moeglich sind
	// VAOs sind Container fuer mehrere Buffer, die zusammen gesetzt werden sollen.
	GLuint VertexArrayIDTeapot;
	glGenVertexArrays(1, &VertexArrayIDTeapot);
	glBindVertexArray(VertexArrayIDTeapot);

	// Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer); // Kennung erhalten
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
	// Buffer zugreifbar für die Shader machen
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
	// Mapping zum Vertex Shader
	glEnableVertexAttribArray(0); // siehe layout im vertex shader: location = 0
	glVertexAttribPointer(0,  // location = 0
				  3,  // Datenformat vec3: 3 floats fuer xyz
				  GL_FLOAT,
				  GL_FALSE, // Fixedpoint data normalisieren ?
				  0, // Eckpunkte direkt hintereinander gespeichert
				  (void*) 0); // abweichender Datenanfang ?
	// END UEBUNG 5 //

	GLuint normalbuffer; // Hier alles analog für Normalen in location == 2
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2); // siehe layout im vertex shader
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint uvbuffer; // Hier alles analog für Texturkoordinaten in location == 1 (2 floats u und v!)
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // siehe layout im vertex shader
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Load the texture
	GLuint Texture = loadBMP_custom("Content/mandrill.bmp");

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("Content/StandardShading.vertexshader", "Content/StandardShading.fragmentshader");
	// lade color sharer für farbige darstellung
	//programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

	// Shader auch benutzen !
	glUseProgram(programID);

	// Lichtquelle positionierung
	glm::vec4 lightPos = glm::vec4(4,4,-4,0);
	glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);

	glm::mat4 lightTransformation(1.0);

  float currentTime = 0.0;
  float lastTime = 0.0;

  // Forest matrix
  float forest[16] = {
    1, 0, 3, 2,
    0, 1, 3, 1,
    1, 0, 4, 3,
    2, 4, 2, 0
  };
  glm::mat4 theForest = glm::make_mat4(forest);

	// Eventloop
	glm::mat4 Save = Model;
	while (!glfwWindowShouldClose(window))
	{
		// Calculate the processing time for the last frame
        currentTime = glfwGetTime();
        camera->deltaTime = currentTime - lastTime;
        camera->moveOnPlaneXY(currentTime - lastTime);
        lastTime = currentTime;
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		// Transformationsmatrix!
		Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

		// Camera matrix
		// Transformationsmatrix!
        glm::vec3 cam_position = camera->getPosition();
        glm::vec3 cam_direction = camera->getDirection();
        glm::vec3 cam_up = camera->getUp();
		View = glm::lookAt(cam_position, // Camera is at (0,0,-5), in World Space
						   cam_position + cam_direction,  // and looks at the origin
						   cam_up); // Head is up (set to 0,-1,0 to look upside-down)

		// Model matrix : an identity matrix (model will be at the origin)
		// Transfromationsmatrix!
		Model = glm::mat4(1.0f);
    Save = Model;

		//Model = glm::rotate(Model, x_rot, glm::vec3(1, 0, 0));
		//Model = glm::rotate(Model, y_rot, glm::vec3(0, 1, 0));
		//Model = glm::rotate(Model, z_rot, glm::vec3(0, 0, 1));

		sendMVP();
    drawCS();

		// Zeichne ein Segment
    Model = Save;
	  glm::mat4 Save2 = Model;

    // Zeichne Wald
    float scale = 0.0f;
    Model = glm::scale(Model, glm::vec3(1.0 / 100.0, 1.0 / 100.0, 1.0 / 100.0));
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        if (theForest[row][col] > 0) {
          Save2 = Model;
          Model = glm::rotate(Model, -90.0f, glm::vec3(1, 0, 0));
          scale = theForest[row][col] / 2.0f;
          Model = glm::scale(Model, glm::vec3(scale, scale, scale));
          sendMVP();
          glBindVertexArray(VertexArrayIDTeapot);
          glDrawArrays(GL_TRIANGLES, 0, vertices.size());
          Model = Save2;
        }
		    Model = glm::translate(Model, glm::vec3(0.0, 0.0, 60.0));
      }
		  Model = glm::translate(Model, glm::vec3(60.0, 0.0, -240.0));
    }

    lightTransformation = glm::translate(Model, glm::vec3(0.0, 0.3, 0.0));
    lightPos = lightTransformation * glm::vec4(0,0,0,1);
    glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);

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

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteTextures(1, &Texture);

	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

    delete camera;
	return 0;
}

