#include "Object.h"

using namespace std;

class Cube : public Object
{
public:
	Cube()
	{
	}

	int setupGeometry()
	{
		GLfloat vertices[] = {
			-0.5f,-0.5f,-0.5f, 1.0, 1.0, 0.0,
			-0.5f,-0.5f, 0.5f, 0.0, 1.0, 1.0,
			-0.5f, 0.5f, 0.5f, 1.0, 0.0, 1.0,

			0.5f, 0.5f,-0.5f,  1.0, 1.0, 0.0,
			-0.5f,-0.5f,-0.5f, 1.0, 0.0, 1.0,
			-0.5f, 0.5f,-0.5f, 0.0, 1.0, 1.0,

			 0.5f,-0.5f, 0.5f, 1.0, 1.0, 0.0,
			-0.5f,-0.5f,-0.5f, 0.0, 1.0, 1.0,
			 0.5f,-0.5f,-0.5f, 1.0, 0.0, 1.0,

			 0.5f, 0.5f,-0.5f, 1.0, 1.0, 0.0,
			 0.5f,-0.5f,-0.5f, 0.0, 1.0, 1.0,
			-0.5f,-0.5f,-0.5f, 1.0, 0.0, 1.0,

			-0.5f,-0.5f,-0.5f, 1.0, 1.0, 0.0,
			-0.5f, 0.5f, 0.5f, 1.0, 1.0, 0.0,
			-0.5f, 0.5f,-0.5f, 1.0, 1.0, 0.0,

			0.5f,-0.5f, 0.5f,  1.0, 1.0, 0.0,
			-0.5f,-0.5f, 0.5f, 1.0, 1.0, 0.0,
			-0.5f,-0.5f,-0.5f, 1.0, 1.0, 0.0,

			-0.5f, 0.5f, 0.5f, 1.0, 1.0, 0.0,
			-0.5f,-0.5f, 0.5f, 1.0, 1.0, 0.0,
			0.5f,-0.5f, 0.5f,  1.0, 0.0, 1.0,

			 0.5f, 0.5f, 0.5f, 1.0, 1.0, 0.0,
			 0.5f,-0.5f,-0.5f, 1.0, 1.0, 0.0,
			 0.5f, 0.5f,-0.5f, 1.0, 0.0, 1.0,

			 0.5f,-0.5f,-0.5f, 1.0, 0.0, 1.0,
			 0.5f, 0.5f, 0.5f, 1.0, 0.0, 1.0,
			 0.5f,-0.5f, 0.5f, 0.0, 1.0, 1.0,

			 0.5f, 0.5f, 0.5f, 0.0, 1.0, 1.0,
			 0.5f, 0.5f,-0.5f, 1.0, 0.0, 1.0,
			-0.5f, 0.5f,-0.5f, 0.0, 1.0, 1.0,

			 0.5f, 0.5f, 0.5f, 0.0, 1.0, 1.0,
			-0.5f, 0.5f,-0.5f, 0.0, 1.0, 1.0,
			-0.5f, 0.5f, 0.5f, 0.0, 1.0, 1.0,

			 0.5f, 0.5f, 0.5f, 1.0, 1.0, 0.0,
			-0.5f, 0.5f, 0.5f, 0.0, 1.0, 1.0,
			 0.5f,-0.5f, 0.5f, 0.0, 1.0, 1.0,
		};

		GLuint VBO, VAO;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return VAO;
	}
};