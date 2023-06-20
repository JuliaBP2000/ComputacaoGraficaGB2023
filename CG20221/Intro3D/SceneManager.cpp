#include "SceneManager.h"
#include "Cube.cpp"
#include "Mesh.cpp"
#include "Camera.h"

bool firstCursorMovement ;

glm::mat4 model;

//Camera* camera;

double xCursor, yCursor;
double lastX, lastY;

GLuint VAO;
Shader* shader;
Mesh* mesh;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

GLFWwindow* SceneManager::initialize()
{
	loadGraphics();
	setupWindow();

	return window;
}

void SceneManager::loadGraphics()
{
	glfwInit();

	window = glfwCreateWindow(WIDTH, HEIGHT, "Ola, 3D!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
}

void SceneManager::setupWindow()
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

int SceneManager::setupMesh(string meshFileName)
{
	mesh = new Mesh(meshFileName);

	return mesh->setupGeometry();
}

int SceneManager::getMeshSize() 
{
	return mesh->size;
}

vector<string> SceneManager::getObjConfig()
{
	vector<string> objectsFiles;

	ifstream inputFileStream;
	inputFileStream.open("./Models/scene_config.txt");

	const int MAX_CHARACTERES_LINE = 50;

	if (inputFileStream.is_open())
	{
		char line[MAX_CHARACTERES_LINE];
		string strLine;

		while (!inputFileStream.eof())
		{
			inputFileStream.getline(line, MAX_CHARACTERES_LINE);
			strLine = line;
			string word;
			istringstream sline(line);
			sline >> word;

			string param = word.substr(0, 3);

			if (param == "obj")
			{
				objectsFiles.push_back(word.substr(3, word.size()));
			}
		}
		inputFileStream.close();
	}
	else cout << "Erro ao ler o arquivo config" << endl;

	return objectsFiles;
};
