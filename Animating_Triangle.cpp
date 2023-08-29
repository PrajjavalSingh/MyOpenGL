#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include "KHR\khrplatform.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "D:\dev\GLFW\glfw\include\stb\stb_image.h"

using namespace std;

float vertices[]
{
    -0.5f, -0.5f, 0.0,
     0.5f, -0.5f, 0.0,
     0.0f,  0.5f, 0.0
};

const char* vShaderSource =
"#version 330 core\n"
"layout (Location=0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"gl_Position = transform * vec4( aPos, 1.0 );\n"
"}\0";


//GLSL Fragment Shader
const char* fShaderSource = 
"#version 330 core\n"
"uniform vec3 inp_Color;\n"
"void main()\n"
"{\n"
"gl_FragColor.rgb = inp_Color;\n"
"gl_FragColor.a = 1.0;\n"
"}\0";

void framebuffer_size_callback( GLFWwindow* win, int w, int h )
{
    glViewport( 0, 0, w, h );
}

float mag_ = 0.001f;
float color_[3] = { 0.1f, 0.1f, 0.1f };
void processInput( GLFWwindow* win )
{
    if ( glfwGetKey(win,GLFW_KEY_ESCAPE) == GLFW_PRESS )
	glfwSetWindowShouldClose( win, true );
    else if ( glfwGetKey(win,GLFW_KEY_UP) == GLFW_PRESS )
	mag_ += 0.001f;
    else if ( glfwGetKey(win,GLFW_KEY_DOWN) == GLFW_PRESS )
    {
	mag_ -= 0.001f;
	if ( mag_ < 0 )
	    mag_ = 0.001f;
    }
    else if ( glfwGetKey(win,GLFW_KEY_R) == GLFW_PRESS )
    {
	if ( color_[0] >= 1.0f )
	    return;

	color_[0] += 0.0001f;
    }
    else if ( glfwGetKey(win,GLFW_KEY_G) == GLFW_PRESS )
    {
	if ( color_[1] >= 1.0f )
	    return;

	color_[1] += 0.0001f;
    }
    else if ( glfwGetKey(win,GLFW_KEY_R) == GLFW_PRESS )
    {
	if ( color_[2] >= 1.0f )
	    return;

	color_[2] += 0.0001f;
    }
}

int main()
{
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    GLFWwindow* win = glfwCreateWindow( 800, 600, "Triangle", NULL, NULL );
    if ( !win )
    {
	cout << "Unable to create Main Window,\n"
			"contact prajjavalsingh@gmail.com" << endl;
	glfwTerminate();
	return -1;
    }

    glfwMakeContextCurrent( win );
    glfwSetFramebufferSizeCallback( win, framebuffer_size_callback );

    if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) )
    {
	cout << "Cannot initialize GLAD" << endl;
	return -1;
    }

//vertex shader
    unsigned int vertexshader_id = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexshader_id, 1, &vShaderSource, NULL );
    glCompileShader( vertexshader_id );

    int success;
    glGetShaderiv( vertexshader_id, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
	char info[1024];
	glGetShaderInfoLog( vertexshader_id, 1024, NULL, info );
	cout << "Cannot create vertex shader : \n" << info << endl;
	glfwTerminate();
	return -1;
    }

//fragment shader
    unsigned int fragmentshader_id = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentshader_id, 1, &fShaderSource, NULL );
    glCompileShader( fragmentshader_id );
    
    glGetShaderiv( fragmentshader_id, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
	char info[1024];
	glGetShaderInfoLog( fragmentshader_id, 1024, NULL, info );
	cout << "Cannot compile fragment shader : \n" << info << endl;
	glfwTerminate();
	return -1;
    }

//Creating shader program
    unsigned int shader_prog_id = glCreateProgram();
    glAttachShader( shader_prog_id, vertexshader_id );
    glAttachShader( shader_prog_id, fragmentshader_id );
    glLinkProgram( shader_prog_id );
    glGetProgramiv( shader_prog_id, GL_LINK_STATUS, &success );
    if ( !success )
    {
	char info[1024];
	glGetProgramInfoLog( shader_prog_id, 1024, NULL, info );
	cout << "Cannot start program, LINK ERROR :\n" << info << endl;
	glfwTerminate();
	return -1;
    }

    glDeleteShader( vertexshader_id );
    glDeleteShader(  fragmentshader_id );

    unsigned int VAO, VBO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0 );
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
    
    glUseProgram( shader_prog_id );
    while ( !glfwWindowShouldClose(win) )
    {
	processInput( win );
	glClearColor( 0.0f, 0.0, 1.0, 1.0f ); //setting up background color
	glClear( GL_COLOR_BUFFER_BIT );
	glUseProgram( shader_prog_id );
	glBindVertexArray( VAO );
	
	GLint color_loc = glGetUniformLocation( shader_prog_id, "inp_Color" );
	if ( color_loc < 0 )
	{
	    cout << "Invalid color location" << endl;
	    continue;
	}

	glm::mat4 trans = glm::mat4( 1.0f );
	trans = glm::scale( trans, glm::vec3(0.5f+mag_,0.5f+mag_,0.5f+mag_) );
	GLint trans_id = glGetUniformLocation( shader_prog_id, "transform" );
	glUniformMatrix4fv( trans_id, 1, GL_FALSE, glm::value_ptr(trans) );

	glUniform3fv( color_loc, 1, color_ );

	glDrawArrays( GL_TRIANGLES, 0, 3 );

	glfwSwapBuffers( win );
	glfwPollEvents();
    }

    glDeleteProgram( shader_prog_id );
    glfwTerminate();
    return 0;
}