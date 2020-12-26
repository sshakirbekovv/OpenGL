#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "stb_image.h"

GLuint tvbo;
GLuint texLoc;

GLuint uvbo;
GLuint uvLoc;
GLuint vbo;
GLuint cbo;
GLuint posLoc;
GLuint cam;
GLuint viewLoc;
glm::mat4 view(1);
int prevx, prevy;
bool right_button = false;
bool left_button = false;

void myInit()

{
	GLuint program = glCreateProgram();
	GLuint vsh = glCreateShader(GL_VERTEX_SHADER);
	const char* vshCode = "#version 120\n\
		attribute vec3 pos;\n\
          uniform mat4 cam;\n\
          uniform mat4 view;\n\
          attribute vec2 uv1;\
	      varying vec2 uv2;\
		void main()\n\
	{\n\
		gl_Position =cam*view*vec4(pos, 1); \n\
		uv2 = uv1; \n\
	}";

	GLint length = strlen(vshCode);

	glShaderSource(vsh, 1, &vshCode, &length);

	glCompileShader(vsh);
	glAttachShader(program, vsh);

	GLuint fsh = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fshCode = "#version 120\n\
        varying vec2 uv2;\
        uniform sampler2D tex;\
		void main()\n\
	{\n\
		gl_FragColor = texture2D(tex, uv2); \n\
	}";



	length = strlen(fshCode);
	glShaderSource(fsh, 1, &fshCode, &length);
	glCompileShader(fsh);
	glAttachShader(program, fsh);
	glValidateProgram(program);
	glLinkProgram(program);
	glUseProgram(program);
	posLoc = glGetAttribLocation(program, "pos");
	viewLoc = glGetUniformLocation(program, "view");
	cam = glGetUniformLocation(program, "cam");
	texLoc = glGetUniformLocation(program, "tex");
	uvLoc = glGetAttribLocation(program, "uv1");


	float vertices[] = {

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float uv[] = {
		1.0, 0.0, 0.0,
		1.0, 0.0, 0,0,
		1.0, 0.0, 0.0,

		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,


		0.0 ,1.0 ,0.0,
		0.0 ,1.0 ,0.0,
		0.0 ,1.0 ,0.0,

		0.0 ,1.0 ,0.0,
		0.0 ,1.0 ,0.0,
		0.0 ,1.0 ,0.0,


		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,

		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,


		1.0, 1.0, 0.0,
		1.0, 1.0, 0.0,
		1.0, 1.0, 0.0,

		1.0, 1.0, 0.0,
		1.0, 1.0, 0.0,
		1.0, 1.0, 0.0,


		0.0, 1.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 1.0, 1.0,

		0.0, 1.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 1.0, 1.0,


		1.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		1.0, 0.0, 1.0,

		1.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		1.0, 0.0, 1.0
	};

	glGenBuffers(1, &uvbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	int w, h, comp;
	unsigned char * image = stbi_load("kaznu.jpg", &w, &h, &comp, 3);

	float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tvbo);
	glBindBuffer(GL_TEXTURE_2D, tvbo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(texLoc, 0);
	stbi_image_free(image);
	glBufferData(GL_TEXTURE_2D, sizeof(color), color, GL_CLAMP_TO_BORDER);


	glm::vec3 position = glm::vec3(4, 4, 4);
	glm::vec3 target = glm::vec3(0, 0, 0);
	glm::vec3 dirc = glm::normalize(position - target);
	glm::vec3 up = glm::vec3(0, 1, 0);

	glm::mat4 MAT = glm::lookAt(position, dirc, up);

	glm::mat4 pMAT = glm::perspective(120.0f, (float)512 / 512, 1.0f, 100.0f);
	//view = glm::rotate(view, 0.001f, glm::vec3(0, 0, 1));
	//view = glm::scale(view, glm::vec3(2.0f, 1.0f, 1.0f));
	glm::mat4 gMAT = pMAT * MAT;

	glUniformMatrix4fv(cam, 1, GL_FALSE, glm::value_ptr(gMAT));
	view = glm::rotate(view, 0.001f, glm::vec3(1, 0, 0));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void myIdle()
{
	view = glm::rotate(view, 0.001f, glm::vec3(1, 0, 0));
	//view = glm::translate(view, glm::vec3(0.001f, 0.001f, 0.001f));
	//view = glm::scale(view, glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glutPostRedisplay();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(uvLoc);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uvbo);
	glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);


	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(posLoc);
	glDisableVertexAttribArray(uvLoc);

	glutSwapBuffers();
}

void myMouse(int button, int state, int x, int y)
{
	prevx = x;
	prevy = y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		left_button = true;
	}
	else
	{
		left_button = false;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		right_button = true;
	}
	else
	{
		right_button = false;
	}

}

void myMotion(int x, int y)
{
	int x1 = x - prevx;
	int y1 = y - prevy;
	if (left_button)
	{
		view = glm::rotate(view, (float)x1 / 100, glm::vec3(1, 0, 0));
		view = glm::rotate(view, (float)y1 / 100, glm::vec3(0, 1, 0));
	}

	if (right_button)
	{
		view = glm::translate(view, glm::vec3(x1 / 500.f, -y1 / 500.0f, -(x1 - y1) / 500.0f));
	}

	/*if (middle_button)
	{
	view = glm::scale(view, glm::vec3(1.1f, 1.1f, 1.1f));
	}*/

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glutPostRedisplay();

	prevx = x;
	prevy = y;
}
int main(int argc, char** argv)

{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);

	glEnable(GL_DEPTH_TEST);

	glewInit();
	myInit();
	/* glutIdleFunc(myIdle); */
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	glutMainLoop();
}