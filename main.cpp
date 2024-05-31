#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimension
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

// Vertex Shader
static const char *vShader = "										            	\n\
#version 330														                  	    \n\
																		                            \n\
layout(location = 0) in vec3 pos;										            \n\
																		                            \n\
void main()																                      \n\
{																		                            \n\
	gl_Position = vec4(0.5 * pos.x, 0.5 * pos.y, pos.z, 1.0);			\n\
}";

// Fragment Shader
static const char *fShader = "											            \n\
#version 330															                      \n\
																		                            \n\
out vec4 color;															                    \n\
																		                            \n\
void main()																                      \n\
{																		                            \n\
	color = vec4(1.0f, 0.0, 0.0, 1.0);									          \n\
}";

void CreateTriangle()
{
  GLfloat vertices[] = {
      -1.0f, -1.0, 0.0f,
      1.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f};

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void AddShader(GLint theProgram, const char *shaderCode, GLenum shaderType)
{
  GLint theShader = glCreateShader(shaderType);

  const GLchar *theCode[1];
  theCode[0] = shaderCode;

  GLint codeLength[1];
  codeLength[0] = strlen(shaderCode);

  glShaderSource(theShader, 1, theCode, codeLength);
  glCompileShader(theShader);

  GLint result = 0;
  GLchar eLog[1024] = {0};

  glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
    printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
    return;
  }

  glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
  shader = glCreateProgram();

  if (!shader)
  {
    printf("Error creating shader program!\n");
    return;
  }

  AddShader(shader, vShader, GL_VERTEX_SHADER);
  AddShader(shader, fShader, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar eLog[1024] = {0};

  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error linking program: '%s'\n", eLog);
    return;
  }

  glValidateProgram(shader);
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error validating program: '%s'\n", eLog);
    return;
  }
}

int main()
{
  // Initialize GLFW
  if (!glfwInit())
  {
    printf("GLFW initialization failed!");
    glfwTerminate();
    return 1;
  }

  // Setup GLFW window properties
  // OpenGL version 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Core profile = No backwards Compatibility
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Allow forward compatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

  GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

  if (!mainWindow)
  {
    printf("GLFW window creation failed!");
    glfwTerminate();
    return 1;
  }

  // Get Buffer size information
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Set the context for GLEW to use
  glfwMakeContextCurrent(mainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  // Initialize GLEW
  if (glewInit() != GLEW_OK)
  {
    printf("GLEW initialization failed!");
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 1;
  }

  // Setup view port size
  glViewport(0, 0, bufferWidth, bufferHeight);

  CreateTriangle();
  CompileShaders();

  // Loop until window close
  while (!glfwWindowShouldClose(mainWindow))
  {
    // Get + Handle user input events
    glfwPollEvents();

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glUseProgram(0);

    glfwSwapBuffers(mainWindow);
  }

  return 0;
}