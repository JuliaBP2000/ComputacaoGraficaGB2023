#pragma once

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Common/include/Shader.h"

class Mesh
{
public:
	Mesh() {}
	~Mesh() {}
	void initialize(GLuint VAO, int nVertices, Shader* shader, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis);
	void update();
	void rotate(float angle, const glm::vec3& axis);
	void translate(const glm::vec3& position);
	void scaleUp();
	void scaleDown();
	void move(const glm::vec3& offset);
	void reset();
	void draw();
	void deleteVAO();
	glm::vec3 position;

protected:
	GLuint VAO;
	int nVertices;
	Shader* shader;
	
	glm::vec3 originalPosition;
	glm::vec3 scale;
	float angle;
	glm::vec3 axis;

};

