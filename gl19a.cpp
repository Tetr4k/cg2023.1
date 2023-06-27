#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utilsGL.h"
#include "ObjMesh.h"
#include "transforms.h"
#include "Scene.h"
#include "Shaders.h"

PhongShaderTexNormal shader;
Scene scene;
Light light;

GLMesh box;
GLTexture box_texture;
ShaderProgram textureShader;

unsigned int screen_width = 600, screen_height = 600;

unsigned int framebuffer;
unsigned int fwidth = 600, fheight = 600;

float angle = 0;

void init_box(){
	box = {"modelos/box.obj", loadIdentity()};
	
	box_texture = GLTexture{GL_TEXTURE_2D};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fwidth, fheight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(GLEW_EXT_texture_filter_anisotropic){
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	}
	
	textureShader = ShaderProgram{
		Shader{"TextureShader.vert", GL_VERTEX_SHADER},
		Shader{"TextureShader.frag", GL_FRAGMENT_SHADER}
	};
}

void init_framebuffer(){
	unsigned int depthbuffer;
	glGenRenderbuffers(1, &depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, fwidth, fheight);
	
	glGenFramebuffers(1, &framebuffer);  
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, box_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);
}

void init(){
	glewInit();
	glEnable(GL_DEPTH_TEST);
	enable_debug();

	shader.init();
	scene.init();
	light.position = vec4{ 0.0, 8.0, 10.0, 1.0 };
	light.Ia = vec3{ 0.2, 0.2, 0.2 };
	light.Id = vec3{ 1.0, 1.0, 1.0 };
	light.Is = vec3{ 1.0, 1.0, 1.0 };
	
	init_box();
	init_framebuffer();
}

void draw_framebuffer(){
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, fwidth, fheight);

	glClearColor(0.4, 0.4, 0.6, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float aspect = (float)fwidth/(float)fheight;
	mat4 Projection = perspective(50, aspect, 1, 50);
	vec4 pos = rotate_y(0.2*angle)*vec4{0, 8, 20, 1};
	mat4 View = lookAt(toVec3(pos), {0, 4, 0}, {0, 1, 0});

	shader.setup(Projection, View, light);
	scene.draw();
}

void draw_box(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screen_width, screen_height);

	glClearColor(1, 1, 1, 1);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(textureShader);

	float a = screen_width/(float)screen_height;
	mat4 Projection = perspective(45, a, 0.1, 50);
	mat4 View = lookAt({0, 2, 5}, {0, 0, 0}, {0, 1, 0});
	Uniform{"M"} = Projection*View*rotate_y(-0.1*angle);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, box_texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	box.draw();
}

void desenha(){
	draw_framebuffer();
	draw_box();
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
		angle -= dx*0.01;
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

