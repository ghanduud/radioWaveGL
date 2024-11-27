
#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>
#include <cmath>

using namespace std;
using namespace glm;

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

struct Vertex {
	float x, y, z; // Position
	float r, g, b; // Color
};

const GLint WIDTH = 800, HEIGHT = 600;
GLuint VBO, VAO, BasicProgramId;
float theta = 0.0f;

std::vector<Vertex> SineWaveVertices;

void GenerateSineWave(float amplitude, float frequency)
{
	SineWaveVertices.clear();

	int numPoints = 500; // Number of points in the sine wave
	float step = 2.0f / (numPoints - 1); // x-coordinates step in normalized device coordinates [-1, 1]

	for (int i = 0; i < numPoints; ++i) {
		float x = -1.0f + i * step; // x in NDC [-1, 1]
		float y = 0.0f;             // Initial y (will be calculated in the shader)
		float r = (x + 1.0f) / 2.0f; // Red based on normalized x
		float g = 1.0f - r;          // Green as inverse of red
		float b = 0.5f;              // Constant blue
		SineWaveVertices.push_back({ x, y, 0.0f, r, g, b });
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, SineWaveVertices.size() * sizeof(Vertex), SineWaveVertices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}


void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_namering, GLuint& programId)
{
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_namering);
	glUseProgram(programId);
}

void Init()
{
	glewInit();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	CompileShader("VS.glsl", "FS.glsl", BasicProgramId);

	GenerateSineWave(0.5f, 3.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, SineWaveVertices.size());
}

void Update()
{
	theta += 0.00009f; // Increment theta for animation

	GLuint thetaLocation = glGetUniformLocation(BasicProgramId, "theta");
	glUniform1f(thetaLocation, theta); // Send updated theta to the GPU
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Dynamic Sine Wave", sf::Style::Close, context);

	Init();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		Update();
		Render();

		window.display();
	}

	return 0;
}