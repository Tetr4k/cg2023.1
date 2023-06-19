#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Color.h"
#include "utilsGL.h"
#include "VertexUtils.h"
#include "MixColorShader.h"
#include "MarchingCubes.h"
#include "transforms.h"

ShaderProgram shaderProgram;
VAO vao;
GLBuffer vbo_position, vbo_normal;
int n_verts;
float angle = 0;
unsigned int screen_width = 600;
unsigned int screen_height = 600;
	
float treco(float x, float y, float z){
	float r = (x*x + y*y);
	return 2*y*(y*y - 3*x*x)*(1-z*z) + r*r - (9*z*z - 1)*(1 - z*z);
}

float sphere(float x, float y, float z){
	float r = 1;
	return x*x + y*y + z*z - r*r;
}

vec3 normal_sphere(vec3 p){
	return normalize(p);
}

vec3 normal_treco(float x, float y, float z){
	return normalize(vec3{
		4*(x*x*x + x*y*(y + 3*z*z - 3)),
		x*x*(4*y + 6*z*z -6) + 2*y*y*(2*y - 3*z*z +3),
		4*z*(3*x*x*y - y*y*y + 9*z*z - 5)
	});
}

void init_buffers(const std::vector<vec3>& Position, const std::vector<vec3>& Normal){
	vao = VAO{true};
	glBindVertexArray(vao);

	vbo_position = GLBuffer{GL_ARRAY_BUFFER};
	vbo_position.data(Position, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	vbo_normal = GLBuffer{GL_ARRAY_BUFFER};
	vbo_normal.data(Normal, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void init_surface(int m, int n, int p){
	vec3 pmin = {-2, -2, -2}; 
	vec3 pmax = { 2,  2,  2};

	std::vector<vec3> V = marchingCubes(treco, m, n, p, pmin, pmax);

	n_verts = V.size();

	std::vector<vec3> Normal(n_verts);
	for(int i = 0; i < n_verts; i++)
		Normal[i] = normal_treco(V[i][0], V[i][1], V[i][2]);
		//Normal[i] = normal_sphere(V[i]);

	init_buffers(V, Normal);
}

void init_shader(){
	shaderProgram = ShaderProgram{
		Shader{"PhongShader.vert", GL_VERTEX_SHADER},
		Shader{"PhongShader.frag", GL_FRAGMENT_SHADER}
	};
}

void init(){
	glewInit();
	glEnable(GL_DEPTH_TEST);
	
	init_shader();
	init_surface(50, 50, 50);
}

void desenha(){
	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);
	Uniform{"Model"} = rotate_z(angle);
	Uniform{"View"} = lookAt({3, 3, 1.5}, {0, 0, 0}, {0, 0, 1});
	float a = screen_width/(float)screen_height;
	Uniform{"Projection"} = perspective(50, a, 0.1, 50);

	Uniform{"Ka"}   = vec3{0.7, 0.7, 1.0};
	Uniform{"Kd"}   = vec3{0.7, 0.7, 1.0};
	Uniform{"Ks"}  = vec3{ 1.0, 1.0, 1.0};
	Uniform{"shininess"} = 40.0f;

	Uniform{"light_position"} = vec4{ 0.0, 8.0, 10.0, 0.0 };
	Uniform{"Ia"}  = vec3{ 0.2, 0.2, 0.2};
	Uniform{"Id"}  = vec3{ 1.0, 1.0, 1.0};
	Uniform{"Is"} = vec3{ 1.0, 1.0, 1.0};

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, n_verts);
}


double last_x;

void mouse_button_callback(GLFWwindow* window, int button, int action, int /*mods*/){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		last_x = x;
	}
}

void cursor_position_callback(GLFWwindow* window, double x, double /*y*/){
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS){
		double dx = x - last_x;
		angle += dx*0.01;
		last_x = x;
	}
}

void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height){
    glViewport(0, 0, width, height);
	screen_width = width;
	screen_height = height;
}  

int main(){
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "LearnOpenGL", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	

	init();

	glViewport(0, 0, screen_width, screen_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	
	std::cout << "GL Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

	while(!glfwWindowShouldClose(window)){
		desenha();
    	glfwSwapBuffers(window);
    	glfwPollEvents();    
	}
	glfwTerminate();
}

