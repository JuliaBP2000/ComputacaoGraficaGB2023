#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

using namespace std;

class Camera
{
public:
	Camera() {};
	~Camera() {};

	glm::mat4 view, projection;
	glm::vec3 position, upDirectionVector, front;

	float speed = 0.05f;
	float yaw = -90.0f, pitch = 0.0f;

	enum movement { MOVE_FWD = GLFW_KEY_W, MOVE_BKW = GLFW_KEY_S, MOVE_LEFT = GLFW_KEY_A, MOVE_RIGHT = GLFW_KEY_D };
	enum viewSide { FRONT = GLFW_KEY_1, BACK = GLFW_KEY_2, LEFT = GLFW_KEY_3, RIGHT = GLFW_KEY_4, TOP = GLFW_KEY_5 };

	void initialize()
	{
		vector<float> camConf = getConfig();

		position = glm::vec3(camConf[0], camConf[1], camConf[2]);
		front = glm::vec3(camConf[3], camConf[4], camConf[5]);
		upDirectionVector = glm::vec3(camConf[6], camConf[7], camConf[8]);
	}

	void updateViewMatrix()
	{
		view = glm::mat4(1);

		view = glm::lookAt(position, position + front, upDirectionVector);
	}

	void updatePosition(int direction)
	{
		view = glm::mat4(1);

		switch (direction) 
		{
			case(MOVE_FWD):
				position += speed * front;
				break;
			case(MOVE_BKW):
				position -= speed * front;
				break;
			case(MOVE_RIGHT):
				position += glm::normalize(glm::cross(front, upDirectionVector)) * speed;
				break;
			case(MOVE_LEFT):
				position -= glm::normalize(glm::cross(front, upDirectionVector)) * speed;
				break;
		}
	}

	vector<float> getConfig()
	{
		vector<float> camConfig;

		ifstream inputFileStream;
		inputFileStream.open("./Models/scene_config.txt");

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

					camConfig.push_back(camPosX);
					camConfig.push_back(camPosY);
					camConfig.push_back(camPosZ);
				}

				if (camParam == "camFrn")
				{
					float camFrnX, camFrnY, camFrnZ;
					sline >> camFrnX;
					sline >> camFrnY;
					sline >> camFrnZ;

					camConfig.push_back(camFrnX);
					camConfig.push_back(camFrnY);
					camConfig.push_back(camFrnZ);
				}

				if (camParam == "camUpv")
				{
					float camUpvX, camUpvY, camUpvZ;
					sline >> camUpvX;
					sline >> camUpvY;
					sline >> camUpvZ;

					camConfig.push_back(camUpvX);
					camConfig.push_back(camUpvY);
					camConfig.push_back(camUpvZ);
				}
				nLine++;
			}
			inputFileStream.close();
		}
		else cout << "Erro ao ler o arquivo config" << endl;

		return camConfig;
	};

	void moveForward() {
		position += speed * front;
	}

	void moveBackwards() {
		position -= speed * front;
	}

	void moveToRight() {
		position -= glm::normalize(glm::cross(front, upDirectionVector)) * speed;
	}

	void moveToLeft() {
		position += glm::normalize(glm::cross(front, upDirectionVector)) * speed;
	}

	void updateFront(glm::vec3 vec) {
		front = vec;
	}

	void updateViewDirection(int direction)
	{
		view = glm::mat4(1);

		switch (direction)
		{
			case(FRONT):
				position = glm::vec3(0.0f, 0.0f, 1.0f);
				upDirectionVector = glm::vec3(0.0f, 1.0f, 0.0f);
				break;
			case(BACK):
				position = glm::vec3(0.0f, 0.0f, -1.0f);
				upDirectionVector = glm::vec3(0.0f, 1.0f, 0.0f);
				break;
			case(LEFT):
				position = glm::vec3(-1.0f, 0.0f, 0.0f);
				upDirectionVector = glm::vec3(0.0f, 1.0f, 0.0f);
				break;
			case(RIGHT):
				position = glm::vec3(1.0f, 0.0f, 0.0f);
				upDirectionVector = glm::vec3(0.0f, 1.0f, 0.0f);
				break;
			case(TOP):
				position = glm::vec3(0.0f, 1.0f, 0.0f);
				upDirectionVector = glm::vec3(0.0f, 0.0f, 1.0f);
				break;
		}
	}
};