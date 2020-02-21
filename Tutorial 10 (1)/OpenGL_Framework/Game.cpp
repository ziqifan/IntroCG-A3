#include "Game.h"
#include "ResourceManager.h"
//#include "TextureCube.h"
#include "UI.h"

#include <vector>
#include <string>
#include <fstream>
#include <random>

Game::Game()
{
	updateTimer = new Timer();
}

Game::~Game()
{
	delete updateTimer;
}

void Game::initializeGame()
{

	ShaderProgram::initDefault();
	meshSphere.initMeshSphere(32U, 32U);
	meshSkybox.initMeshSphere(32U, 32U, true);
	meshLight.initMeshSphere(6U, 6U);
	meshPlane.initMeshPlane(32U, 32U);
	
	int num;
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &num);
	SAT_DEBUG_LOG_SAFE("GL_MAX_UNIFORM_BUFFER_BINDINGS = %d", num);

	shader.load("shader.vert", "shader.frag");
	shaderTexture.load("shader.vert", "shaderTexture.frag");
	shaderTextureJupiter.load("shader.vert", "shaderTextureJupiter.frag");
	shaderTextureAlphaDiscard.load("shader.vert", "shaderTextureAlphaDiscard.frag");
	shaderSky.load("shaderSky.vert", "shaderSky.frag");
	shaderTextureTran.load("shader.vert", "shaderTextureTran.frag");
	shaderDis.load("shaderDis.vert", "shaderDis.frag");

	uniformBufferCamera.allocateMemory(sizeof(mat4) * 2);
	uniformBufferCamera.bind(0);
	uniformBufferTime.allocateMemory(sizeof(float));
	uniformBufferTime.bind(1);
	uniformBufferLight.allocateMemory(sizeof(vec4) * 3);	// Three vec3's
	uniformBufferLight.bind(2);								// vec3's on the GPU are automatically padded to the size of a vec4

	uniformBufferLight.sendVector(vec3(0.2f), 0);
	uniformBufferLight.sendVector(vec3(1.0f), sizeof(vec4) * 2);

	Texture* texBlack = new Texture("black.png");
	Texture* texWhite = new Texture("white.png");
//	Texture* texYellow = new Texture("yellow.png");
	//Texture* texGray = new Texture("gray.png");
	Texture* texEarthAlbedo = new Texture("earth.jpg");
	Texture* texEarthEmissive = new Texture("earthEmissive.png");
	Texture* texEarthSpecular = new Texture("earthSpec.png");
	Texture* texRings = new Texture("saturnRings.png");
	//Texture* texMoonAlbedo = new Texture("8k_moon.jpg");
	//Texture* texJupiterAlbedo = new Texture("jupiter.png");
	//Texture* texSaturnAlbedo = new Texture("8k_saturn.jpg");
	//Texture* texCheckerboard = new Texture("checkboard.png");
	Texture* texFloor= new Texture("floor.png");
	Texture* texWall= new Texture("wall.png");
	Texture* texCeilling = new Texture("ceilling.png");
	Texture* stone = new Texture("stone.png");
	Texture* discard = new Texture("stoneDis.png");


	std::vector<std::string> skyboxTexPaths;
	skyboxTexPaths.push_back("sky2/sky_c00.bmp");
	skyboxTexPaths.push_back("sky2/sky_c01.bmp");
	skyboxTexPaths.push_back("sky2/sky_c02.bmp");
	skyboxTexPaths.push_back("sky2/sky_c03.bmp");
	skyboxTexPaths.push_back("sky2/sky_c04.bmp");
	skyboxTexPaths.push_back("sky2/sky_c05.bmp");
	cubeMap = new TextureCube(skyboxTexPaths);
	goSkybox = GameObject(&meshSkybox, cubeMap);
	goSkybox.setShaderProgram(&shaderSky);

	std::vector<Texture*> texEarth = { texEarthAlbedo, texEarthEmissive, texEarthSpecular };
	std::vector<Texture*> texCheckboards{ texFloor, texBlack, texWhite };
	std::vector<Texture*> texSun = { texBlack, texWhite, texBlack };
	std::vector<Texture*> texMoon = { texWall ,texBlack, texWhite };
	std::vector<Texture*> texJupiter = { discard, texBlack, texWhite };
	std::vector<Texture*> texPlanet = { stone , texBlack, texWhite };
	std::vector<Texture*> texSaturn = { texCeilling, texBlack, texWhite };
	std::vector<Texture*> texSaturnRings = { texRings, texBlack, texBlack };

	goSun = GameObject(&meshSphere, texSun);
	//goEarth = GameObject(&meshSphere, texEarth);
	goEarthPlane = GameObject(&meshPlane, texCheckboards);
	//goMoon = GameObject(&meshSphere, texMoon);
	//goJupiter = GameObject(&meshSphere, texJupiter);
	//goJupiterMoon[0] = GameObject(&meshSphere, texMoon);
	//goJupiterMoon[1] = GameObject(&meshSphere, texMoon);
	//goSaturn = GameObject(&meshSphere, texSaturn);
	//goSaturnRings = GameObject(&meshPlane, texSaturnRings);

	wall0 = GameObject(&meshPlane, texMoon);
	wall1 = GameObject(&meshPlane, texMoon);
	wall2 = GameObject(&meshPlane, texMoon);
	wall3 = GameObject(&meshPlane, texMoon);
	
	ceilling = GameObject(&meshPlane, texSaturn);

	object0 = GameObject(&meshSphere, texPlanet);
	object1 = GameObject(&meshSphere, texPlanet);
	object2 = GameObject(&meshSphere, texPlanet);
	object3 = GameObject(&meshSphere, texJupiter);




	ResourceManager::addEntity(&goSun);
	//ResourceManager::addEntity(&goEarth);
	ResourceManager::addEntity(&goEarthPlane);
	//ResourceManager::addEntity(&goMoon);
	//ResourceManager::addEntity(&goJupiter);
	//ResourceManager::addEntity(&goJupiterMoon[0]);
	//ResourceManager::addEntity(&goJupiterMoon[1]);
	//ResourceManager::addEntity(&goSaturn);
	//ResourceManager::addEntity(&goSaturnRings);

	ResourceManager::addEntity(&wall0);
	ResourceManager::addEntity(&wall1);
	ResourceManager::addEntity(&wall2);
	ResourceManager::addEntity(&wall3);
	ResourceManager::addEntity(&ceilling);
	ResourceManager::addEntity(&object0);
	ResourceManager::addEntity(&object3);
	ResourceManager::addEntity(&object1);
	ResourceManager::addEntity(&object2);

	goSun.setPosition(vec3(4, 100, 0));
	//goEarth.setPosition(vec3(-2, 0, 0));
	goEarthPlane.setPosition(vec3(0, -50.0f, 0));
	//goMoon.setPosition(vec3(-1, 0, -1));
	//goJupiter.setPosition(vec3(-3, 0, 4));
	//goJupiterMoon[0].setPosition(vec3(-4, 0, 5));
	//goJupiterMoon[1].setPosition(vec3(-2, 0, 3));
	//goSaturn.setPosition(vec3(-2, 0, -3));
	//goSaturnRings.setPosition(vec3(-2, 0, -3));

	wall0.setPosition(vec3(100, 35.0f, 0));
	wall1.setPosition(vec3(0, 35.0f, 100));
	wall2.setPosition(vec3(-100, 35.0f, 0));
	wall3.setPosition(vec3(0, 35.0f, -100));
	ceilling.setPosition(vec3(0, 110.0f, 0));

	object0.setPosition(vec3(50, -10.0f, 50));
	object1.setPosition(vec3(50, -10.0f, -50));
	object2.setPosition(vec3(-50, -10.0f, 50));
	object3.setPosition(vec3(-50, -10.0f, -50));

	wall0.setRotationAngleZ(90);
	wall1.setRotationAngleX(-90);
	wall2.setRotationAngleZ(-90);
	wall3.setRotationAngleX(90);
	ceilling.setRotationAngleX(180);

	/*std::uniform_real_distribution<float> randomPositionX(-100.0f, 100.0f);
	std::uniform_real_distribution<float> randomPositionY(-100.0f, 100.0f);
	std::uniform_real_distribution<float> randomPositionZ(-100.0f, -10.0f);
	std::uniform_real_distribution<float> randomRotation(0.0f, 360.0f);
	std::uniform_real_distribution<float> randomScale(0.5f, 4.0f);
	std::default_random_engine generator(std::_Random_device());
*/
	/*for (int i = 0; i < 500; i++)
	{
		GameObject *object = new GameObject(&meshSphere, texMoon);
		object->setPosition(vec3(randomPositionX(generator), randomPositionY(generator), randomPositionZ(generator)));
		object->setScale(randomScale(generator));
		object->setRotationAngleX(randomRotation(generator));
		object->setRotationAngleY(randomRotation(generator));
		object->setRotationAngleZ(randomRotation(generator));
		object->setShaderProgram(&shaderTexture);
		ResourceManager::addEntity(object);
		goPlanets.push_back(object);
	}*/

	goSun.setScale(1.50f);
	//goEarth.setScale(0.50f);
	goEarthPlane.setScale(100.50f);
	/*goMoon.setScale(0.25f);
	goJupiter.setScale(1.00f);
	goJupiterMoon[0].setScale(0.25f);
	goJupiterMoon[1].setScale(0.20f);
	goSaturn.setScale(1.0f);
	goSaturnRings.setScale(2.0f);
	goSaturnRings.setRotationAngleZ(-20.0f);*/

	wall0.setScale(100.50f);
	wall1.setScale(100.50f);
	wall2.setScale(100.50f);
	wall3.setScale(100.50f);
	ceilling.setScale(100.50f);
	object0.setScale(35.0f);
	object1.setScale(35.0f);
	object2.setScale(35.0f);
	object3.setScale(35.0f);


	goSun			.setShaderProgram(&shaderTexture);
	//goEarth			.setShaderProgram(&shaderTexture);
	goEarthPlane	.setShaderProgram(&shaderTexture);
	/*goMoon			.setShaderProgram(&shaderTexture);
	goJupiter		.setShaderProgram(&shaderTextureJupiter);
	goJupiterMoon[0].setShaderProgram(&shaderTexture);
	goJupiterMoon[1].setShaderProgram(&shaderTexture);
	goSaturn		.setShaderProgram(&shaderTexture);
	goSaturnRings	.setShaderProgram(&shaderTextureAlphaDiscard);*/

	wall0.setShaderProgram(&shaderTexture);
	wall1.setShaderProgram(&shaderTexture);
	wall2.setShaderProgram(&shaderTexture);
	wall3.setShaderProgram(&shaderTexture);
	ceilling.setShaderProgram(&shaderTexture);
	object0.setShaderProgram(&shaderTexture);
	object3.setShaderProgram(&shaderDis);

	object1.setShaderProgram(&shaderTextureTran);
	object2.setShaderProgram(&shaderTextureTran);

	   	 
	// These Render flags can be set once at the start (No reason to waste time calling these functions every frame).
	// Tells OpenGL to respect the depth of the scene. Triangles will not render when they are behind other geometry.
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	   
	// Basic clear color used by glClear().
	glClearColor(0, 0, 0, 0); // Black.

	// Setup our main scene objects...
	float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	camera.perspective(90.0f, aspect, 0.05f, 1000.0f);
	camera.setPosition(vec3(0.0f, 30.0f, 4.0f));
	camera.setRotationAngleX(-15.0f);	
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	float cameraSpeedMult = 2.0f;
	float cameraRotateSpeed = 90.0f;
	if (input.shiftL || input.shiftR)
	{
		cameraSpeedMult *= 4.0f;
	}

	if (input.ctrlL || input.ctrlR)
	{
		cameraSpeedMult *= 0.5f;
	}

	if (input.moveUp)
	{
		camera.m_pLocalPosition.y += cameraSpeedMult;
	}
	if (input.moveDown)
	{
		camera.m_pLocalPosition.y -= cameraSpeedMult;
	}
	if (input.moveForward)
	{
		float pitch = camera.m_pRotX;
		float yaw = camera.m_pRotY;
		float pitchRadian = pitch * (PI / 180); // X rotation
		float yawRadian = yaw * (PI / 180); // Y rotation
		camera.m_pLocalPosition.x -= cameraSpeedMult * sinf(yawRadian) * cosf(pitchRadian);
		//camera.m_pLocalPosition.y -= cameraSpeedMult * -sinf(pitchRadian);
		camera.m_pLocalPosition.z -= cameraSpeedMult * cosf(yawRadian) * cosf(pitchRadian);
	}
	if (input.moveBackward)
	{
		float pitch = camera.m_pRotX;
		float yaw = camera.m_pRotY;
		float pitchRadian = pitch * (PI / 180); // X rotation
		float yawRadian = yaw * (PI / 180); // Y rotation
		camera.m_pLocalPosition.x += cameraSpeedMult * sinf(yawRadian) * cosf(pitchRadian);
		//camera.m_pLocalPosition.y += cameraSpeedMult * -sinf(pitchRadian);
		camera.m_pLocalPosition.z += cameraSpeedMult * cosf(yawRadian) * cosf(pitchRadian);
	}
	if (input.moveRight)
	{
		float pitch = camera.m_pRotX;
		float yaw = camera.m_pRotY - 90;
		float pitchRadian = pitch * (PI / 180); // X rotation
		float yawRadian = yaw * (PI / 180); // Y rotation
		camera.m_pLocalPosition.x -= cameraSpeedMult * sinf(yawRadian) * cosf(pitchRadian);
		camera.m_pLocalPosition.z -= cameraSpeedMult * cosf(yawRadian) * cosf(pitchRadian);
	}
	if (input.moveLeft)
	{
		float pitch = camera.m_pRotX;
		float yaw = camera.m_pRotY + 90;
		float pitchRadian = pitch * (PI / 180); // X rotation
		float yawRadian = yaw * (PI / 180); // Y rotation
		camera.m_pLocalPosition.x -= cameraSpeedMult * sinf(yawRadian) * cosf(pitchRadian);
		camera.m_pLocalPosition.z -= cameraSpeedMult * cosf(yawRadian) * cosf(pitchRadian);
	}
	if (input.rotateUp)
	{
		camera.m_pRotX += cameraRotateSpeed * deltaTime;
	}
	if (input.rotateDown)
	{
		camera.m_pRotX -= cameraRotateSpeed * deltaTime;
	}
	if (input.rotateRight)
	{
		camera.m_pRotY -= cameraRotateSpeed * deltaTime;
	}
	if (input.rotateLeft)
	{
		camera.m_pRotY += cameraRotateSpeed * deltaTime;
	}

	// Give the cube some motion over time.
	//goEarth.setRotationAngleY(TotalGameTime * 15.0f);

	// Give our Transforms a chance to compute the latest matrices
	camera.update(deltaTime);

	for (Transform* object : ResourceManager::Transforms)
	{
		object->update(deltaTime);
	}
	goSkybox.update(deltaTime);
}

void Game::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	// Completely clear the Back-Buffer before doing any work.
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	goSkybox.draw();
	glClear(GL_DEPTH_BUFFER_BIT);
	
	uniformBufferTime.sendFloat(TotalGameTime, 0);
	uniformBufferCamera.sendMatrix(camera.getProjection(), 0);
	uniformBufferCamera.sendMatrix(camera.getView(), sizeof(mat4));
	
	vec3 lightPos = vec3(camera.getView() * vec4(goSun.getPosition(), 1.0f));
	uniformBufferLight.sendVector(lightPos, sizeof(vec4));
	
	cubeMap->bind(3);

	camera.render();

	cubeMap->unbind(3);

	if(guiEnabled)
		GUI();

	// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
	glutSwapBuffers();
}

int minCurrMode = 0;
int magCurrMode = 0;
void Game::GUI()
{
	UI::Start(windowWidth, windowHeight);

	if (ImGui::Combo("Magnification Filter", &magCurrMode, "Nearest\0Linear\0\0"))
	{
		Texture::magFilterOverride = filterModes[magCurrMode];
	}
	if (ImGui::Combo("Minification Filter", &minCurrMode, "Nearest\0Linear\0Nearest Mipmap Nearest\0Linear Mipmap Nearest\0Nearest Mipmap Linear\0Linear Mipmap Linear\0\0"))
	{
		Texture::minFilterOverride = filterModes[minCurrMode];
	}
	
	ImGui::SliderFloat("Anisotropy", &Texture::anisotropyAmount, 1.0f, 16.0f);

	UI::End();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	if (guiEnabled)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[(int)key] = true;
		io.AddInputCharacter((int)key); // this is what makes keyboard input work in imgui
		// This is what makes the backspace button work
		int keyModifier = glutGetModifiers();
		switch (keyModifier)
		{
		case GLUT_ACTIVE_SHIFT:
			io.KeyShift = true;
			break;

		case GLUT_ACTIVE_CTRL:
			io.KeyCtrl = true;
			break;

		case GLUT_ACTIVE_ALT:
			io.KeyAlt = true;
			break;
		}
	}

	switch(key)
	{
	case 27: // the escape key
		break;
	case 'w':
	case 'W':
	case 'w' - 96:
		input.moveForward = true;
		break;
	case 's':
	case 'S':
	case 's' - 96:
		input.moveBackward = true;
		break;
	case 'd':
	case 'D':
	case 'd' - 96:
		input.moveRight = true;
		break;
	case 'a':
	case 'A':
	case 'a' - 96:
		input.moveLeft = true;
		break;
	case 'e':
	case 'E':
	case 'e' - 96:
		input.moveUp = true;
		break;
	case 'q':
	case 'Q':
	case 'q' - 96:
		input.moveDown = true;
		break;
	case 'l':
	case 'L':
	case 'l' - 96:
		input.rotateRight = true;
		break;
	case 'j':
	case 'J':
	case 'j' - 96:
		input.rotateLeft = true;
		break;
	case 'i':
	case 'I':
	case 'i' - 96:
		input.rotateUp = true;
		break;
	case 'k':
	case 'K':
	case 'k' - 96:
		input.rotateDown = true;
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	if (guiEnabled)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = false;

		int keyModifier = glutGetModifiers();
		io.KeyShift = false;
		io.KeyCtrl = false;
		io.KeyAlt = false;
		switch (keyModifier)
		{
		case GLUT_ACTIVE_SHIFT:
			io.KeyShift = true;
			break;

		case GLUT_ACTIVE_CTRL:
			io.KeyCtrl = true;
			break;

		case GLUT_ACTIVE_ALT:
			io.KeyAlt = true;
			break;
		}
	}

	switch(key)
	{
	case 32: // the space bar
		camera.cullingActive = !camera.cullingActive;
		break;
	case 27: // the escape key
		exit(1);
		break;
	case 'w':
	case 'W':
	case 'w' - 96:
		input.moveForward = false;
		break;
	case 's':
	case 'S':
	case 's' - 96:
		input.moveBackward = false;
		break;
	case 'd':
	case 'D':
	case 'd' - 96:
		input.moveRight = false;
		break;
	case 'a':
	case 'A':
	case 'a' - 96:
		input.moveLeft = false;
		break;
	case 'e':
	case 'E':
	case 'e' - 96:
		input.moveUp = false;
		break;
	case 'q':
	case 'Q':
	case 'q' - 96:
		input.moveDown = false;
		break;
	case 'l':
	case 'L':
	case 'l' - 96:
		input.rotateRight = false;
		break;
	case 'j':
	case 'J':
	case 'j' - 96:
		input.rotateLeft = false;
		break;
	case 'i':
	case 'I':
	case 'i' - 96:
		input.rotateUp = false;
		break;
	case 'k':
	case 'K':
	case 'k' - 96:
		input.rotateDown = false;
		break;
	}
}

void Game::keyboardSpecialDown(int key, int mouseX, int mouseY)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		guiEnabled = !guiEnabled;
		if (!UI::isInit)
		{
			UI::InitImGUI();
		}
		break;
	case GLUT_KEY_F5:
		shader.reload();
		shaderTexture.reload();
		break;
	case GLUT_KEY_CTRL_L:
		input.ctrlL = true;
		break;
	case GLUT_KEY_CTRL_R:
		input.ctrlL = true;
		break;
	case GLUT_KEY_SHIFT_L:
		input.shiftL = true;
		break;
	case GLUT_KEY_SHIFT_R:
		input.shiftR = true;
		break;
	case GLUT_KEY_ALT_L:
		input.altL = true;
		break;
	case GLUT_KEY_ALT_R:
		input.altR = true;
		break;
	case GLUT_KEY_UP:
		input.moveForward = true;
		break;
	case GLUT_KEY_DOWN:
		input.moveBackward = true;
		break;
	case GLUT_KEY_RIGHT:
		input.moveRight = true;
		break;
	case GLUT_KEY_LEFT:
		input.moveLeft = true;
		break;
	case GLUT_KEY_PAGE_UP:
		input.moveUp = true;
		break;
	case GLUT_KEY_PAGE_DOWN:
		input.moveDown = true;
		break;
	case GLUT_KEY_END:
		exit(1);
		break;
	}
}

void Game::keyboardSpecialUp(int key, int mouseX, int mouseY)
{
	switch (key)
	{
	case GLUT_KEY_CTRL_L:
		input.ctrlL = false;
		break;
	case GLUT_KEY_CTRL_R:
		input.ctrlL = false;
		break;
	case GLUT_KEY_SHIFT_L:
		input.shiftL = false;
		break;
	case GLUT_KEY_SHIFT_R:
		input.shiftR = false;
		break;
	case GLUT_KEY_ALT_L:
		input.altL = false;
		break;
	case GLUT_KEY_ALT_R:
		input.altR = false;
		break;
	case GLUT_KEY_UP:
		input.moveForward = false;
		break;
	case GLUT_KEY_DOWN:
		input.moveBackward = false;
		break;
	case GLUT_KEY_RIGHT:
		input.moveRight = false;
		break;
	case GLUT_KEY_LEFT:
		input.moveLeft = false;
		break;
	case GLUT_KEY_PAGE_UP:
		input.moveUp = false;
		break;
	case GLUT_KEY_PAGE_DOWN:
		input.moveDown = false;
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if (guiEnabled)
	{
		ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);
		ImGui::GetIO().MouseDown[0] = !state;
	}

	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:

			break;
		case GLUT_RIGHT_BUTTON:
		
			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else
	{

	}
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{

	ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);

	if (std::abs(x - mousePositionx) < 200)
	{
		camera.m_pRotY += -(x - mousePositionx) * 0.3f;
		if (camera.m_pRotX<90.0f &&camera.m_pRotX > -90.0f)
			camera.m_pRotX += -(y - mousePositiony) * 0.3f;
		else if (camera.m_pRotX >= 90.0f)
			camera.m_pRotX = 89.9f;
		else if (camera.m_pRotX <= -90.0f)
			camera.m_pRotX = -89.9f;
	}

	mousePositionx = x;
	mousePositiony = y;

}

void Game::reshapeWindow(int w, int h)
{
	windowWidth = w;
	windowHeight = h;

	float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	camera.perspective(90.0f, aspect, 0.05f, 1000.0f);
	glViewport(0, 0, w, h);
}
