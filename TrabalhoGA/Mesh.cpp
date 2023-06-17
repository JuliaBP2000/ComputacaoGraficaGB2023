#include "Mesh.h"

void Mesh::initialize(GLuint VAO, int nVertices, Shader* shader, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis)
{
	this->VAO = VAO;
	this->nVertices = nVertices;
	this->shader = shader;
	this->position = position;
	this->originalPosition = position;
	this->scale = scale;
	this->angle = angle;
	this->axis = axis;
}

void Mesh::update()
{
	glm::mat4 model = glm::mat4(1.0f); // Matriz identidade
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(angle), axis);
	model = glm::scale(model, scale);
	shader->setMat4("model", glm::value_ptr(model));
}

void Mesh::rotate(float angle, const glm::vec3& axis)
{
	this->angle = angle;
	this->axis = axis;
	update();
}

void Mesh::translate(const glm::vec3& position)
{
	this->position = position;
	update();
}

void Mesh::scaleUp()
{
	scale += glm::vec3(0.05f);
	update();
}

void Mesh::scaleDown()
{
	scale -= glm::vec3(0.05f);
	update();
}

void Mesh::move(const glm::vec3& offset)
{
	position += offset;
	update();
}

void Mesh::reset()
{
	position = originalPosition;
	scale = glm::vec3(1.0f);
	angle = 0.0f;
	update();
}

void Mesh::draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, nVertices);
	glBindVertexArray(0);
}

void Mesh::deleteVAO()
{
	glDeleteVertexArrays(1, &VAO);
}




