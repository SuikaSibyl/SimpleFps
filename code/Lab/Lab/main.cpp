#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------*****************************************--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------          DEAR imgui Define end          --------------------------------------------------------------------------------
//--------------------------------------------------------------------------------*****************************************--------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Chicken.h"
#include "model.h"

#include "Shader.h"
#include "Texture.h"
#include "collider.h"
#include "Camera.h"
#include "Cube.h"
#include "Material.h"
#include "Light.h"
#include "bullet.h"
#include "BMPOps.h"

#include "resourceManager.h"

// function:: declaration
// ------------------------------------------------------------------
int initialize();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void printScreen();

// globle:: object / Varies
// ------------------------------------------------------------------
GLFWwindow* window;
bool inGame = true;

Light l_full(glm::vec3(2.0f, 2.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
Light l_default(glm::vec3(2.0f, 2.0f, -2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
Light l_nextRoom(glm::vec3(-10.0f, 6.0f, 0), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

Material emerald(glm::vec3(0.0215f, 0.1745f, 0.0215f),
	glm::vec3(0.07568, 0.61424, 0.07568),
	glm::vec3(0.0215f, 0.1745f, 0.0215f),
	0.6f);

Material gold(glm::vec3(1.0f, 0.5f, 0.31f),
	glm::vec3(1.0f, 0.5f, 0.31f),
	glm::vec3(0.5f, 0.5f, 0.5f),
	32.0f);

// settings
// ------------------------------------------------------------------
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 600;

// collider
// ------------------------------------------------------------------
vector<Collider> v_collider;

// camera
// ------------------------------------------------------------------
Camera camera(v_collider, glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// light
// ------------------------------------------------------------------
glm::vec3 lightPos(1.0f, 1.0f, -1.0f);
glm::vec3 lightAmbientColorvec(0.5f, 0.5f, 0.5f);
glm::vec3 lightDiffColorvec(0.5f, 0.5f, 0.5f);
glm::vec3 lightSpecColorvec(0.5f, 0.5f, 0.5f);
float lightPos_x = 1.0f;
float lightPos_y = 1.0f;
float lightPos_z = -1.0f;
float lightAmbientColorfv[3] = { 0.5f,0.5f,0.5f };
float lightDiffColorfv[3] = { 0.5f,0.5f,0.5f };
float lightSpecColorfv[3] = { 0.5f,0.5f,0.5f };
void updateLightPos() { lightPos.x = lightPos_x; lightPos.y = lightPos_y; lightPos.z = lightPos_z; }
void updateLightAmbientvec() {
	lightAmbientColorvec.x = lightAmbientColorfv[0];
	lightAmbientColorvec.y = lightAmbientColorfv[1];
	lightAmbientColorvec.z = lightAmbientColorfv[2];
}
void updateLightDiffvec() {
	lightDiffColorvec.x = lightDiffColorfv[0];
	lightDiffColorvec.y = lightDiffColorfv[1];
	lightDiffColorvec.z = lightDiffColorfv[2];
}
void updateLightSpecvec() {
	lightSpecColorvec.x = lightSpecColorfv[0];
	lightSpecColorvec.y = lightSpecColorfv[1];
	lightSpecColorvec.z = lightSpecColorfv[2];
}
//先利用数组更新vec向量，再利用vec向量更新光源的位置、环境光颜色、漫反射颜色和镜面反射颜色
void updateLight(Light* light);

// timing
// ------------------------------------------------------------------
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float sigmaTime = 0.0f;

// game paras
// ------------------------------------------------------------------
bool in_bossRoom = false;
bool Bullet_Time = false;
bool change_lock = true;
float lock_timer = 0;

vector<bullet> v_bullet;

//Audio Manager
//-------------
AudioManager::audioManager audioAdmin;
ModelManager::modelManager modelAdmin;
bool changeTexture = false;
float texture_rate = 0;
float box_size = 1;
float box_theta = 0;
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------*****************************************--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------                M A I N                  --------------------------------------------------------------------------------
//--------------------------------------------------------------------------------*****************************************--------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	//-----------------------------------------------------
	//                 Library  Initializer
	// ----------------------------------------------------
	int ini_status = initialize();
	if (ini_status == -1) return -1;

	//-----------------------------------------------------
	//                 SHADER  Initializer
	// ----------------------------------------------------
	Shader ourShader("my.vs", "my.fs");
	Shader lightShader("my.vs", "light.fs");
	Shader skyboxShader("skybox.vs", "skybox.fs");
	Shader depthShader("depth.vs", "skybox.fs");
	Shader waterShader("water.vs", "water.fs");
	Shader uiShader("ui.vs", "ui.fs");
	Shader posShader("pos_only.vs", "pos_only.fs", "pos_only.gs");
	Shader modelShader("model.vs", "model.fs");
	Shader screenShader("screen.vs", "screen.fs");
	Shader shadowShader("depthS.vs", "depthS.fs","depthS.gs");
	Shader powerShader("power.vs", "power.fs");
	//设置材质与对象颜色的混合模式
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//-----------------------------------------------------
	//                 VAO  Initializer
	// ----------------------------------------------------
	unsigned int VAO = CVAO_pos_norm_tex();
	unsigned int lightVAO = CVAO_pos();
	unsigned int skyboxVAO = CVAO_skybox();
	unsigned int squareVAO = SVAO_pos_norm_tex_tangent();
	unsigned int squareUI = SUI_pos_tex();
	unsigned int sphereV = SphVAO_pos();
	unsigned int CylinderV = CylVAO_pos();
	unsigned int ConeV = ConVAO_pos();
	float vertices[] = {
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int pVBO, pVAO, pEBO;
	glGenVertexArrays(1, &pVAO);
	glGenBuffers(1, &pVBO);
	glGenBuffers(1, &pEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(pVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//model
	Model frame1("resources/anime/chicken_1.obj");
	Model frame2("resources/anime/chicken_2.obj");
	Model frame3("resources/anime/chicken_3.obj");
	Model frame4("resources/anime/chicken_4.obj");
	Model frame5("resources/anime/chicken_5.obj");
	Model frame6("resources/anime/chicken_6.obj");
	Model frame7("resources/anime/chicken_7.obj");
	Model frame8("resources/anime/chicken_8.obj");
	Model frame9("resources/anime/chicken_9.obj");
	Model frame10("resources/anime/chicken_10.obj");
	Model frame11("resources/anime/chicken_11.obj");
	Model frame12("resources/anime/chicken_12.obj");
	Model frame13("resources/anime/chicken_13.obj");
	Model frame14("resources/anime/chicken_14.obj");
	Model frame15("resources/anime/chicken_15.obj");
	Model frame16("resources/anime/chicken_16.obj");
	Model frame17("resources/anime/chicken_17.obj");
	Model frame18("resources/anime/chicken_18.obj");
	Model frame19("resources/anime/chicken_19.obj");

	Model scene("resources/objects/bossroom.obj");
	Model scene_floor("resources/objects/floor.obj");
	Model scene_wall("resources/objects/wall.obj");
	Model scene_total("resources/objects/total.obj");
	//////////////////////////////////////////////

	//-----------------------------------------------------
	//               TEXTURE  Initializer
	// ----------------------------------------------------
	CubeTexture cubemapTexture("resources/skybox");
	CubeTexture cubemapTexture2("resources/skybox/sky0");
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	Texture texture1("resources/container2.bmp");
	Texture container_brick("resources/container_brick.bmp");
	Texture texture2("resources/container2_specular.bmp");
	Texture texture3("resources/normal.bmp");
	Texture rock_t("resources/rock_t.bmp");
	Texture rock_n("resources/rock_n.bmp");
	Texture floor("resources/floor.bmp");
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	ourShader.setInt("texture3", 2);
	Texture T_Perlin_Noise_M("resources/T_Perlin_Noise_M.bmp");
	Texture T_Water_N("resources/T_Water_N.bmp");
	waterShader.use();
	waterShader.setInt("T_Water_N", 0);
	waterShader.setInt("T_Perlin_Noise_M", 1);
	waterShader.setInt("cubeMap", 2);
	Texture Ui_bottom("resources/ui_bottom.png");
	Texture Ui_chicken("resources/ui_hchicken.png");
	uiShader.use();
	uiShader.setInt("bottom", 0);
	uiShader.setInt("chicken", 1);

	posShader.use();
	posShader.setInt("depthMap", 0);

	//-----------------------------------------------------
	//               Camera  Initializer
	// ----------------------------------------------------
	camera.mode = "fps";

	//-----------------------------------------------------
	//               Collider  Initializer
	// ----------------------------------------------------
	v_collider.push_back(Collider(-5, 5, -5, -2, -10, 10));  //ground1
	v_collider.push_back(Collider(-10, 10, -5, -2, -40, -10));  //ground2
	v_collider.push_back(Collider(-0.5, 0.5, -1.4 - 0.5, -1.4 + 0.5, -0.5, 0.5));
	v_collider.push_back(Collider(-6, -5, -3, 7, -10, 10));
	v_collider.push_back(Collider(5, 6, -3, 7, -10, 10));
	v_collider.push_back(Collider(-5, 5, -3, 7, 10, 12));
	v_collider.push_back(Collider(-1.25, 1.25 , -3, -1, 5 + 1.25 - 0.2, 5 + 1.25 + 0.2));
	v_collider.push_back(Collider(-1.25, 1.25, -3, -1, 5 - 1.25 - 0.2, 5 - 1.25 + 0.2));
	v_collider.push_back(Collider(-1.25 - 0.2, -1.25 + 0.2, -3, -1, 5 - 1.25, 5 + 1.25));
	v_collider.push_back(Collider(1.25 - 0.2, 1.25 + 0.2, -3, -1, 5 - 1.25, 5 + 1.25));
	v_collider.push_back(Collider(3, 5, -2, 4, 5, 8));
	v_collider.push_back(Collider(-5, -2, -2, 6, -10.5, -9.5));
	v_collider.push_back(Collider(2, 5, -2, 6, -10.5, -9.5));

	//-----------------------------------------------------
	//               Shadow  Initializer
	// ----------------------------------------------------
	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);  // Show that we do not do any color render
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	depthShader.use();
	depthShader.setInt("depthMap", 0);

	//Shawdow 2
	GLuint depthMapFBOS;
	glGenFramebuffers(1, &depthMapFBOS);
	// Create depth cubemap texture
	GLuint depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// Attach cubemap as depth map FBO's color buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOS);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Set texture samples

	// framebuffer configuration
	// -------------------------
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	//-----------------------------------------------------
	//               Game  Initializer
	// ----------------------------------------------------
	v_bullet.push_back(bullet(glm::vec3(5, 0, 5), glm::vec3(-1, 0, -1), true));
	chicken boss;

	//-----------------------------------------------------
	//               UI  Initializer
	// ----------------------------------------------------

	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
	bool err = false;
	glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
	bool err = false;
	glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	const char* glsl_version = "#version 330 core";
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Our state
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------*****************************************--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------             Rendering Loop              --------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------*****************************************--------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//glfwWindowShouldClose返回一个标识符，该标识符指示窗口是否应被关闭
	while (!glfwWindowShouldClose(window))
	{
		//-----------------------------------------------------
		//                       UPDATE
		// ----------------------------------------------------
		// Pre-frame Time Logic
		// ----------------------------------------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// ===========================有修改===============================
		updateLight(&l_default);

		camera.Update(deltaTime);
		//按键锁：防止对一次按键多次响应
		if (change_lock == false)
		{
			lock_timer += deltaTime;
			if (lock_timer >= 1)
			{
				lock_timer = 0;
				change_lock = true;
			}
		}

		if (Bullet_Time == false)
			sigmaTime += deltaTime;
		else
			sigmaTime += deltaTime * 0.1;
		// Input        
		// ----------------------------------------------------
		//处理按键
		processInput(window);

		// Render       
		// ----------------------------------------------------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Render       
		// ----------------------------------------------------
		//判断是否在boss房间内，当玩家初次位于boss房间内时，boss开始移动
		if (in_bossRoom == false && camera.Position.z <= -10)
		{
			in_bossRoom = true;
			audioAdmin.play("background", true);
		}
		if (in_bossRoom == true && boss.alive)
		{
			boss.update(camera.Position, deltaTime);
			boss.checkShooted(v_bullet);
			boss.shoot(v_bullet, deltaTime);
		}

		// Shadow Depth Tester       
		// ----------------------------------------------------
		glCullFace(GL_FRONT);

		GLfloat near_plane = 1.0f, far_plane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(glm::vec3(lightPos_x, lightPos_y, lightPos_z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		depthShader.use();
		depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, -2, 0));
		model = glm::scale(model, glm::vec3(10, 0.1f, 10));
		depthShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, -1.4f, 0));
		depthShader.setMat4("model", model);
		// 7.3: VAO: draw
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_BACK);

		GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
		GLfloat near = 0.1f;
		GLfloat far = 25.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		// 1. Render scene to depth cubemap
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOS);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowShader.use();
		for (GLuint i = 0; i < 6; ++i)
			glUniformMatrix4fv(glGetUniformLocation(shadowShader.ID, ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
		glUniform1f(glGetUniformLocation(shadowShader.ID, "far_plane"), far);
		glUniform3fv(glGetUniformLocation(shadowShader.ID, "lightPos"), 1, &lightPos[0]);
		//glm::mat4 model = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, -2, 0));
		model = glm::scale(model, glm::vec3(10, 0.1f, 10));
		shadowShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, -1.4f, 0));
		model = glm::scale(model, glm::vec3(box_size));
		model = glm::rotate(model, box_theta, glm::vec3(0.0, 1.0, 0.0));
		shadowShader.setMat4("model", model);
		// 7.3: VAO: draw
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		shadowShader.setMat4("model", model);
		/*scene_wall.Draw(shadowShader);
		scene.Draw(shadowShader);*/
		scene_total.Draw(shadowShader);
		model = boss.glmGetModel();
		//shadowShader.setMat4("model", model);
		model = boss.glmGetModel();
		shadowShader.setMat4("model", model);

		boss.sigma_time += deltaTime;
		if (boss.sigma_time >= 0.1f)
		{
			boss.sigma_time = 0;
			boss.frame++;
			if (boss.frame == 10)
				boss.frame = 1;
		}
		if (boss.alive == true)
		{

			switch (boss.frame)
			{
			case 1:
				frame1.Draw(shadowShader);
				break;
			case 2:
				frame2.Draw(shadowShader);
				break;
			case 3:
				frame3.Draw(shadowShader);
				break;
			case 4:
				frame4.Draw(shadowShader);
				break;
			case 5:
				frame5.Draw(shadowShader);
				break;
			case 6:
				frame6.Draw(shadowShader);
				break;
			case 7:
				frame7.Draw(shadowShader);
				break;
			case 8:
				frame8.Draw(shadowShader);
				break;
			case 9:
				frame9.Draw(shadowShader);
				break;
			case 10:
				frame10.Draw(shadowShader);
				break;
			case 11:
				frame11.Draw(shadowShader);
				break;
			case 12:
				frame12.Draw(shadowShader);
				break;
			case 13:
				frame13.Draw(shadowShader);
				break;
			case 14:
				frame14.Draw(shadowShader);
				break;
			case 15:
				frame15.Draw(shadowShader);
				break;
			case 16:
				frame16.Draw(shadowShader);
				break;
			case 17:
				frame17.Draw(shadowShader);
				break;
			case 18:
				frame18.Draw(shadowShader);
				break;
			case 19:
				frame19.Draw(shadowShader);
				break;
			}

		}
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(5, 5, 5));
		shadowShader.setMat4("model", model);
		modelAdmin.drawModel("hand");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-----------------------------------------------------
		//                       DRAW
		// ----------------------------------------------------
		// DRAW 0: set default mats
		// ----------------------------------------------------
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		/////////////////////////////////////////////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

		// make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);  // Fragments lways pass and draw
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  // When Stencil and depth test both pass, set ref
		glEnable(GL_CULL_FACE);

		// DRAW 1: render skybox
		// ----------------------------------------------------
		glStencilMask(0x00);
		glDepthMask(GL_FALSE);
		glm::mat4 skyview = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.use();
		skyboxShader.setInt("state", 0);
		skyboxShader.setMat4("view", skyview);
		skyboxShader.setMat4("projection", projection);
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);//cubemapTexture
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);

		// DRAW 4: render bullets
		// ----------------------------------------------------
		ourShader.use();
		ourShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		ourShader.setInt("state", 0);
		glStencilMask(0xFF);
		auto iter = v_bullet.begin();
		while (iter != v_bullet.end())
		{
			if (iter->out_of_range == true)
			{
				iter = v_bullet.erase(iter);
			}
			else
			{
				if (iter->enemy == true)
				{
					ourShader.setInt("state", 2);
					model = glm::mat4(1.0f);
					model = glm::translate(model, iter->Position);
					model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
					ourShader.setMat4("model", model);


					glm::vec3 dis = iter->Position - camera.Position;
					float distance = glm::dot(dis, dis);
					if (distance < 0.5)
					{
						camera.Getshooted(iter->Front);
					}
				}
				else
				{
					ourShader.setInt("state", 3);
					model = glm::mat4(1.0f);
					model = glm::translate(model, iter->Position);
					model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
					ourShader.setMat4("model", model);

				}
				if (Bullet_Time == true)
					iter->update(deltaTime * 0.5);
				else
					iter->update(deltaTime * 10);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				iter++;
			}
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(5, 5, 5));
		ourShader.setMat4("model", model);
		
		glDisable(GL_CULL_FACE);
		posShader.use();
		posShader.setMat4("model", model);
		posShader.setMat4("view", view);
		posShader.setMat4("projection", projection);
		posShader.setVec3("viewPos", camera.Position);
		posShader.setFloat("far_plane", far);
		posShader.setLight(l_default);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		modelAdmin.drawModel("hand");
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3, 1, -4));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		posShader.setMat4("model", model);
		glBindVertexArray(sphereV);
		glDrawArrays(GL_TRIANGLES, 0, 10800);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3,1,-6));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		posShader.setMat4("model", model);
		glBindVertexArray(CylinderV);
		glDrawArrays(GL_TRIANGLES, 0, 144);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3, 1, -2));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		posShader.setMat4("model", model);
		glBindVertexArray(ConeV);
		glDrawArrays(GL_TRIANGLES, 0, 120);
		glEnable(GL_CULL_FACE);

		// DRAW 3: render ground
		// ----------------------------------------------------

		powerShader.use();
		powerShader.setInt("depthMap", 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		powerShader.setMat4("projection", projection);
		powerShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		powerShader.setMat4("model", model);
		powerShader.setLight(l_nextRoom);
		powerShader.setFloat("far_plane", far);
		powerShader.setVec3("viewPos", camera.Position);
		powerShader.setVec3("lightPos", lightPos);
		powerShader.setLight(l_default);
		/*scene_floor.Draw(powerShader);
		scene_wall.Draw(powerShader);
		scene.Draw(powerShader);*/
		scene_total.Draw(powerShader); 

		// Draw boss
		if (boss.alive == true)
		{
			model = boss.glmGetModel();
			powerShader.setMat4("model", model);

			switch (boss.frame)
			{
			case 1:
				frame1.Draw(powerShader);
				break;
			case 2:
				frame2.Draw(powerShader);
				break;
			case 3:
				frame3.Draw(powerShader);
				break;
			case 4:
				frame4.Draw(powerShader);
				break;
			case 5:
				frame5.Draw(powerShader);
				break;
			case 6:
				frame6.Draw(powerShader);
				break;
			case 7:
				frame7.Draw(powerShader);
				break;
			case 8:
				frame8.Draw(powerShader);
				break;
			case 9:
				frame9.Draw(powerShader);
				break;
			case 10:
				frame10.Draw(powerShader);
				break;
			case 11:
				frame11.Draw(powerShader);
				break;
			case 12:
				frame12.Draw(powerShader);
				break;
			case 13:
				frame13.Draw(powerShader);
				break;
			case 14:
				frame14.Draw(powerShader);
				break;
			case 15:
				frame15.Draw(powerShader);
				break;
			case 16:
				frame16.Draw(powerShader);
				break;
			case 17:
				frame17.Draw(powerShader);
				break;
			case 18:
				frame18.Draw(powerShader);
				break;
			case 19:
				frame19.Draw(powerShader);
				break;
			}
		}

		// DRAW 5: render light
		// ----------------------------------------------------
		glStencilMask(0xFF);
		lightShader.use();
		model = glm::mat4(1.0f);
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		lightShader.setVec3("lightColor", l_default.specular);
		glBindVertexArray(lightVAO);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// DRAW 7: render box
		// ----------------------------------------------------
		// 7.1: shader: prepare shader
		ourShader.use();
		ourShader.setInt("state", 0);
		// 2,2: shader: prepare texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, container_brick.ID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, T_Perlin_Noise_M.ID);
		// 7.3: shader: prepare paras
		ourShader.setFloat("rate", texture_rate);
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setLight(l_default);
		ourShader.setMaterial(gold);
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, -1.4f, 0));
		model = glm::scale(model, glm::vec3(box_size));
		model = glm::rotate(model, box_theta, glm::vec3(0.0, 1.0, 0.0));
		ourShader.setMat4("model", model);
		// 7.3: VAO: draw
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// DRAW 6 : render water
		// ----------------------------------------------------
		waterShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, -2, 5));
		model = glm::scale(model, glm::vec3(2.5, 1, 2.5));
		waterShader.setInt("state", 0);
		waterShader.setMat4("projection", projection);
		waterShader.setMat4("view", view);
		waterShader.setMat4("model", model);
		waterShader.setVec3("lightpos", l_default.position);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, T_Water_N.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, T_Perlin_Noise_M.ID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture2.ID);
		waterShader.setVec3("cameraPos", camera.Position);
		waterShader.setVec3("LightLocation", l_default.position);
		waterShader.setFloat("totalTime", sigmaTime);
		glBindVertexArray(squareVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glfwPollEvents();

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use();
		screenShader.setBool("bullet_time", Bullet_Time);
		glBindVertexArray(squareUI);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//-----------------------------------------------------
		//                       DRAW UI
		// ----------------------------------------------------

		glDisable(GL_DEPTH_TEST);
		//使用uiShader
		//状态机：每次只能启用一个shader
		uiShader.use();
		uiShader.setFloat("h_chicken", boss.getH());
		//黑槽
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Ui_bottom.ID);
		//血条
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Ui_chicken.ID);
		//绘制
		glBindVertexArray(squareUI);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glEnable(GL_DEPTH_TEST);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ==============================有修改=============================
		if (!inGame)
		{
			//static int counter = 0;

			ImGui::Begin("Menu"); //将一个名为“Menu”的窗口推入栈
			//Text函数显示一行文字
			//此类添加组件的函数将逐行向窗口添加组件
			ImGui::Text("Welcome to Deep Dark Fantasy I.");               // Display some text (you can use a format strings too)

			//光线调整
			ImGui::Text("Light Adjustment");
			//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			//ImGui::Checkbox("Another Window", &show_another_window);
			//SliderFloat函数添加一个滑动条，并可以用滑动条改变一些变量的值
			//此处利用滑动条改变光源的位置，并将新值赋予lightPos变量。每个分量的范围是[-1,1].
			ImGui::SliderFloat("Position: x", &lightPos_x, -1.0f, 1.0f);
			ImGui::SliderFloat("Position: y", &lightPos_y, -1.0f, 1.0f);
			ImGui::SliderFloat("Position: z", &lightPos_z, -1.0f, 1.0f);
			//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
			ImGui::ColorEdit3("Ambient", lightAmbientColorfv);
			ImGui::ColorEdit3("Difffuse", lightDiffColorfv);
			ImGui::ColorEdit3("Specular", lightSpecColorfv);

			ImGui::SliderFloat("Box: TEXTURE_mix_rate", &texture_rate, 0, 1.0f);
			ImGui::SliderFloat("Box: size", &box_size, 0.5f, 2);
			ImGui::SliderFloat("Box: rotate", &box_theta, 0.5f, 2);
			//补充功能
			ImGui::Text("Supplementary Functions");
			if (ImGui::Button("PrtSc")) {
				printScreen();
			}
			ImGui::SameLine();
			ImGui::Text("Screenshot would be named as \"PrtSc.bmp\".");
			//Button函数绘制一个按钮，同时，如果这个按钮被按下，Button函数将返回true
			if (ImGui::Button("Exit")) {                            // Buttons return true when clicked (most widgets return true when edited/activated)
				exit(0);
			}
			ImGui::SameLine();
			ImGui::Text("Please think twice before you press it :)");
			if (ImGui::Button("PrtObj")) {                            // Buttons return true when clicked (most widgets return true when edited/activated)
				obj_writer(Cube_pos, sizeof(Cube_pos) / sizeof(Cube_pos[0]));
			}
			ImGui::SameLine();
			ImGui::Text("Print a box");
			//不换行显示
			//ImGui::SameLine();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//将窗口弹出栈
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		//glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------*****************************************----------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------           Rendering Loop End            ----------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------*****************************************----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int initialize()
{
	// glfw: initialize and configure
 // ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	// glfw: window creation
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Deep Dark Fantasy I", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// glfw: to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);  // Fragments lways pass and draw
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  // When Stencil and depth test both pass, set ref
	glEnable(GL_CULL_FACE);

	// resourcesManger initialize
	// --------------------------
	audioAdmin.insertPath("background", "resources/audio/Ludwig, the Holy Blade.mp3");
	audioAdmin.insertPath("laser", "resources/audio/laser.wav");

	modelAdmin.insertModel("hand", "resources/model/hand.obj");

	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	//glfwGetKey返回按键的状态（是否被按下）
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		inGame = !inGame;
	if (inGame == true)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		//    glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.ProcessKeyboard(SPACE, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			changeTexture = !changeTexture;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			if (change_lock)
			{
				Bullet_Time = !Bullet_Time;
				change_lock = false;
			}
		}
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever click the mouse, this callback is called
// -------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (inGame == true)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			audioAdmin.play("laser",false);
			v_bullet.push_back(bullet(camera.Position, camera.Front, false));
		}
	}
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (inGame == true)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// ================================有修改==================================
void updateLight(Light* light) {
	updateLightPos();
	updateLightAmbientvec();
	updateLightDiffvec();
	updateLightSpecvec();
	light->position = lightPos;
	light->diffuse = lightDiffColorvec;
	light->ambient = lightAmbientColorvec;
	light->specular = lightSpecColorvec;
}

void printScreen() {
	//要求OpenGL以4字节对齐方式保存数据
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//建立RGBInfo对象并赋予信息
	RGBInfo img;
	img.FileHeader.bfOffBits = 54;
	img.InfoHeader.biBitCount = 24;
	img.InfoHeader.biHeight = SCR_HEIGHT;
	img.InfoHeader.biWidth = SCR_WIDTH;
	img.calcByteWidth();
	img.InfoHeader.biSizeImage = img.InfoHeader.biHeight * img.width_byte;
	img.FileHeader.bfSize = img.InfoHeader.biSizeImage + 54;
	img.InfoHeader.biXPelsPerMeter = 3780;
	img.InfoHeader.biYPelsPerMeter = 3780;
	img.data = new BYTE[img.InfoHeader.biHeight * img.width_byte];

	glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_BGR, GL_UNSIGNED_BYTE, img.data);
	imwrite(img, "PrtSc.bmp");
	delete[] img.data;
} 