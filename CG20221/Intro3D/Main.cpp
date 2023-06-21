#include <iostream>
#include <string>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "SceneManager.h"
#include "Camera.h"

using namespace std;

const GLuint WIDTH = 800, HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

bool rotateX = false, rotateY = false, rotateZ = false;

bool firstMouse = true;
float lastX = WIDTH / 2.0, lastY = HEIGHT / 2.0;
float yaw = -90.0, pitch = 0.0;

GLFWwindow* window;
SceneManager* sceneManager = new SceneManager;
Camera* camera = new Camera;

int main()
{
	window = sceneManager->initialize();

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	camera->initialize();

	Shader* shader = new Shader();

	glUseProgram(shader->ID);

	//pega o objeto e os atributos da camera que existem no nosso arquivo de entrada
	vector<string> objectsFiles = sceneManager->getObjConfig();

	GLuint VAO;
	for (int i = 0; i < objectsFiles.size(); i++) {
		VAO = sceneManager->setupMesh(objectsFiles[i]);
	}

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shader->ID, "model");
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));
	shader->setMat4("model", glm::value_ptr(model));

	glm::mat4 view = glm::mat4(1);
	view = glm::lookAt(camera->position, glm::vec3(0.0f, 0.0f, 0.0f), camera->upDirectionVector);
	shader->setMat4("view", glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1);
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	shader->setMat4("projection", glm::value_ptr(projection));

	

	shader->setVec3("lightPos", 0.0, 5.0, 2.0);
	shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader->setVec3("objectColor", 0.196, 0.450, 0.658);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->ID, "colorBuffer"), 0);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.08f, .08f, .08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);


		float angle = (GLfloat)glfwGetTime();

		model = glm::mat4(1);

		//model = glm::translate(model, glm::vec3(0.0, 0.0, cos(angle) * 10.0));
		if (rotateX)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));

		}
		else if (rotateY)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else if (rotateZ)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		}

		glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

		glm::mat4 view = glm::mat4(1);
		view = glm::lookAt(camera->position, camera->position + camera->front, camera->upDirectionVector);
		shader->setMat4("view", glm::value_ptr(view));

		glBindTexture(GL_TEXTURE_2D, );
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sceneManager->getMeshSize());
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera->front = glm::normalize(front);

}

void processInput(GLFWwindow* window)
{
	glfwPollEvents();

	if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (glfwGetKey(window, GLFW_KEY_Z == GLFW_PRESS))
	{
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->position += camera->speed * camera->front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->position -= camera->speed * camera->front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->position -= glm::normalize(glm::cross(camera->front, camera->upDirectionVector)) * camera->speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->position += glm::normalize(glm::cross(camera->front, camera->upDirectionVector)) * camera->speed;
	}
}

