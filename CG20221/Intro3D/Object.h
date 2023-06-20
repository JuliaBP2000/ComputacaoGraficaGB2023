#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

class Object
{
public:
	const int TRIANGLES = 12;
	const int NUM_VERTICES = 3;

	bool shouldRotate;

	Object()
	{
	}

	int setupGeometry();
};
