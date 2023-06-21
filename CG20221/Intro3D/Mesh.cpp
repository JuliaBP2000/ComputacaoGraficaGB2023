#include "Object.h"
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "stb_image.h"

using namespace std;

class Mesh : public Object
{
public:
	int size;
	/*vector <glm::vec2> textureCoordinates;
	vector<GLuint> textures;*/
	

	Mesh(string objPath/*, string texturePath*/)
	{
		filePath = MESHES_PATH + objPath;

		parseFromFile(filePath);
		//textures = loadTexture(MESHES_PATH + texturePath);
		
		size = verticeAttribute.size() * 3;
	}

	int setupGeometry()
	{
		createGeometryBuffer();

		GLuint VBO, VAO;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, geometryBuffer.size() * sizeof(GLfloat), geometryBuffer.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);



		return VAO;
	};

private:
	vector <glm::vec3> vertices, normals;
	vector <glm::vec3> verticeAttribute, normalAttribute, textureAttribute;

	vector <float> geometryBuffer;

	const string MESHES_PATH = "./Models/";
	const string VERTICES = "v";
	const string NORMALS = "vn";
	const string TEXTURE_COORDS = "vt";
	const string FACES = "f";

	const string DELIMITER = "/";

	string filePath;

	void parseFromFile(string path)
	{
		ifstream inputFileStream;
		inputFileStream.open(filePath);

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


				if (word == VERTICES || word == NORMALS || word == TEXTURE_COORDS || word == FACES)
				{
					glm::vec3 attribute;
					if (word == VERTICES)
					{
						sline >> attribute.x >> attribute.y >> attribute.z;
						vertices.push_back(attribute);
					}
					if (word == NORMALS)
					{
						sline >> attribute.x >> attribute.y >> attribute.z;
						normals.push_back(attribute);
					}
					if (word == TEXTURE_COORDS)
					{
						glm::vec2 attribute;
						sline >> attribute.x >> attribute.y;
						//textureCoordinates.push_back(attribute);
					}
					if (word == FACES)
					{
						string i1, i2, i3;
						glm::vec3 iv, ivn, ivt;
						sline >> i1 >> i2 >> i3;
						vector <string> tokens;

						tokens = tokenize(i1);
						iv[0] = atof(tokens[0].c_str());
						ivt[0] = atof(tokens[1].c_str());
						ivn[0] = atof(tokens[2].c_str());

						tokens = tokenize(i2);
						iv[1] = atof(tokens[0].c_str());
						ivt[1] = atof(tokens[1].c_str());
						ivn[1] = atof(tokens[2].c_str());

						tokens = tokenize(i3);
						iv[2] = atof(tokens[0].c_str());
						ivt[2] = atof(tokens[1].c_str());
						ivn[2] = atof(tokens[2].c_str());

						verticeAttribute.push_back(iv);
						normalAttribute.push_back(ivn);
						//textureAttribute.push_back(ivt);
					}
				}
			}
			inputFileStream.close();
		}
		else cout << "Erro ao ler o arquivo" << endl;
	};

	vector <string> tokenize(string buffer) {
		vector <string> tokens;
		string strbuffer(buffer);
		string token = "";
		int pos = 0;

		do {
			pos = strbuffer.find(DELIMITER);
			if (pos > 0) {
				token = strbuffer.substr(0, pos);
				strbuffer = strbuffer.substr(pos + DELIMITER.size());
				tokens.push_back(token);
			}
			else tokens.push_back(strbuffer);
		} while (pos > 0);

		return tokens;
	};

	void createGeometryBuffer()
	{
		for (int i = 0; i < verticeAttribute.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				float positionX = vertices[(int)verticeAttribute[i][j] - 1].x;
				float positionY = vertices[(int)verticeAttribute[i][j] - 1].y;
				float positionZ = vertices[(int)verticeAttribute[i][j] - 1].z;

				geometryBuffer.push_back(positionX);
				geometryBuffer.push_back(positionY);
				geometryBuffer.push_back(positionZ);

				float normalX = normals[(int)normalAttribute[i][j] - 1].x;
				float normalY = normals[(int)normalAttribute[i][j] - 1].y;
				float normalZ = normals[(int)normalAttribute[i][j] - 1].z;

				geometryBuffer.push_back(normalX);
				geometryBuffer.push_back(normalY);
				geometryBuffer.push_back(normalZ);

				/*float textureX = textureCoordinates[(int)textureAttribute[i][j] - 1].x;
				float textureY = textureCoordinates[(int)textureAttribute[i][j] - 1].y;

				geometryBuffer.push_back(textureX);
				geometryBuffer.push_back(textureY);*/
			}
		}
	};

	vector<GLuint> loadTexture(string texturePath) {
		vector<string> textureFiles; 

		ifstream mtlStream(texturePath);
		if (mtlStream.is_open()) {
			string line;
			while (getline(mtlStream, line)) {
				stringstream ss(line);
				string prefix;
				ss >> prefix;

				if (prefix == "map_Kd") {
					string textureFile;
					ss >> textureFile;
					textureFiles.push_back(textureFile);
				}
			}
			mtlStream.close();
		}

		vector<GLuint> textures(textureFiles.size());
		for (size_t i = 0; i < textureFiles.size(); ++i) {
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			int width, height, channels;
			unsigned char* image = stbi_load(textureFiles[i].c_str(), &width, &height, &channels, STBI_rgb);


			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(image);
			glBindTexture(GL_TEXTURE_2D, 0);

			textures[i] = texture;
		}
		

		return textures;
	};	
};