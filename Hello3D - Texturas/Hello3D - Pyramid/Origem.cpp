/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico/Computação Gráfica - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 20/04/2023
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include "Object.h"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 v_color;
};

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

vector<string> getTextureConfig();
vector<string> getObjConfig();
void getCameraConfig();
int objectId = 0;
vector<string> objectsFiles;

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;

bool rotateX = false, rotateY = false, rotateZ = false, translate = false, redraw=false;

glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 3.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
float cameraSpeed = 0.05;

bool firstMouse = true;
float lastX = 0.0, lastY = 0.0;
float yaw = -90.0, pitch = 0.0;


// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho GB CG", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
	glfwSetCursorPosCallback(window, mouse_callback);

	//Desabilita o desenho do cursor do mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	Shader shader("phong.vs", "phong.fs");
	
	glUseProgram(shader.ID);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");
	//
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	//Definindo a matriz de view (posição e orientação da câmera)
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	GLint viewLoc = glGetUniformLocation(shader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

	//Definindo a matriz de projeção perpectiva
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	////Definindo as propriedades do material 
	//shader.setFloat("ka", 0.4);
	//shader.setFloat("kd", 0.5);
	//shader.setFloat("ks", 0.5);
	//shader.setFloat("q", 10);

	//Definindo as propriedades da fonte de luz
	shader.setVec3("lightPos", -2.0f, 10.0f, 3.0f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	objectsFiles = getObjConfig();
	vector<string> textureFiles = getTextureConfig();

	
	Object obj;
	obj.initialize(objectsFiles[objectId],textureFiles[objectId], &shader);
	
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader.ID, "colorBuffer"), 0);


	glEnable(GL_DEPTH_TEST);

	getCameraConfig();

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();
		if (redraw) {
			obj.renew();
			obj.initialize(objectsFiles[objectId], textureFiles[objectId], &shader);
			redraw = false;
		}
		// Limpa o buffer de cor
		glClearColor(.4f, .0f, .9f, .8f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();

		model = glm::mat4(1);

		if (translate) {
			model = glm::translate(model, glm::vec3(0.0, 0.0, cos(angle) * 10.0));
		}
		else if (rotateX)
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

		//Alterando a matriz de view (posição e orientação da câmera)
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

		//Enviando a posição da camera para o shader
		shader.setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);

		obj.update();
		obj.draw();
		
		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	//glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		translate = true;
		rotateX = false;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		translate = false;
		rotateX = false;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		translate = false;
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		translate = false;
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		translate = false;
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}

	if (key == GLFW_KEY_W)
	{
		cameraPos += cameraSpeed * cameraFront;
	}

	if (key == GLFW_KEY_S)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}

	if (key == GLFW_KEY_A)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (key == GLFW_KEY_D)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (key == GLFW_KEY_O) {
		if (objectsFiles.size() - 1 >= objectId) {
			objectId++;
			redraw = true;
		}
		if (objectsFiles.size() - 1 < objectId) {
			objectId = 0;
			redraw = true;
		}
	}


}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	// cout << xpos << " " << ypos << endl;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float offsetx = xpos - lastX;
	float offsety = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	offsetx *= cameraSpeed;
	offsety *= cameraSpeed;

	pitch += offsety;
	yaw += offsetx;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);


}


void getCameraConfig() {
	vector<string> objectsFiles;

	ifstream inputFileStream("scene.txt");
	const int MAX_CHARACTERES_LINE = 50;

	if (inputFileStream.is_open())
	{
		char line[MAX_CHARACTERES_LINE];
		string strLine;
		int nLine = 0;

		while (nLine < 3)
		{
			inputFileStream.getline(line, MAX_CHARACTERES_LINE);
			strLine = line;
			string word;
			istringstream sline(line);
			sline >> word;

			string camParam = word.substr(0, 6);

			if (camParam == "camPos")
			{
				float camPosX, camPosY, camPosZ;
				sline >> camPosX;
				sline >> camPosY;
				sline >> camPosZ;

				cameraPos.x = camPosX;
				cameraPos.y = camPosY;
				cameraPos.z = camPosZ;
			}

			if (camParam == "camFrn")
			{
				float camFrnX, camFrnY, camFrnZ;
				sline >> camFrnX;
				sline >> camFrnY;
				sline >> camFrnZ;

				cameraFront.x = camFrnX;
				cameraFront.y = camFrnY;
				cameraFront.z = camFrnZ;
			}

			if (camParam == "camUpv")
			{
				float camUpvX, camUpvY, camUpvZ;
				sline >> camUpvX;
				sline >> camUpvY;
				sline >> camUpvZ;

				cameraUp.x = camUpvX;
				cameraUp.y = camUpvY;
				cameraUp.z = camUpvZ;
			}
			nLine++;
		}
		inputFileStream.close();
	}
	else cout << "Erro ao ler o arquivo config" << endl;
}

vector<string> getObjConfig()
{
	vector<string> objectsFiles;

	ifstream inputFileStream("scene.txt");
	if (inputFileStream.is_open()) {
		string line;
		while (getline(inputFileStream, line)) {
			stringstream ss(line);
			string prefix;
			ss >> prefix;

			if (prefix == "object") {
				string textureFile;
				ss >> textureFile;
				objectsFiles.push_back(textureFile);
			}
		}
		inputFileStream.close();
	}

	return objectsFiles;
};

vector<string> getTextureConfig() {
	vector<string> textureFiles;

	ifstream inputFileStream("scene.txt");
	if (inputFileStream.is_open()) {
		string line;
		while (getline(inputFileStream, line)) {
			stringstream ss(line);
			string prefix;
			ss >> prefix;

			if (prefix == "texture") {
				string textureFile;
				ss >> textureFile;
				textureFiles.push_back(textureFile);
			}
		}
		inputFileStream.close();
	}

	return textureFiles;
}


