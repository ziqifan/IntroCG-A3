#pragma once
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "Timer.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "IO.h"
#include "GameObject.h"
#include "UniformBuffer.h"
#include "TextureCube.h"

#define WINDOW_SCREEN_WIDTH		640
#define WINDOW_SCREEN_HEIGHT	432
#define WINDOW_WIDTH			800
#define WINDOW_HEIGHT			432
#define FRAMES_PER_SECOND		60

const int FRAME_DELAY_SPRITE = 1000 / FRAMES_PER_SECOND;

class Game
{
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();
	void GUI();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void keyboardSpecialDown(int key, int mouseX, int mouseY);
	void keyboardSpecialUp(int key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);
	void reshapeWindow(int w, int h);

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;
	int windowWidth = WINDOW_WIDTH;
	int windowHeight = WINDOW_HEIGHT;

private:

	int mousePositionx;
	int mousePositiony;

	// Scene Objects.
	Camera camera;
	Mesh meshSphere;
	Mesh meshSkybox;
	Mesh meshLight;
	Mesh meshPlane;

	TextureCube* cubeMap = nullptr;

	GameObject goSun;
	GameObject goEarthPlane;

	GameObject goSkybox;

	GameObject wall0;
	GameObject wall1;
	GameObject wall2;
	GameObject wall3;
	GameObject ceilling;
	GameObject object0;
	GameObject object1;
	GameObject object2;
	GameObject object3;

	// OpenGL Handles
	ShaderProgram shader;
	ShaderProgram shaderTexture;
	ShaderProgram shaderSky;
	ShaderProgram shaderTextureAlphaDiscard;
	ShaderProgram shaderTextureJupiter;
	ShaderProgram shaderTextureTran;
	ShaderProgram shaderDis;

	UniformBuffer uniformBufferCamera;
	UniformBuffer uniformBufferTime;
	UniformBuffer uniformBufferLight;

	bool guiEnabled = false;

	struct KeyInput
	{
		bool moveLeft = false;
		bool moveRight = false;
		bool moveUp = false;
		bool moveDown = false;
		bool moveForward = false;
		bool moveBackward = false;

		bool rotateUp = false;
		bool rotateDown = false;
		bool rotateLeft = false;
		bool rotateRight = false;


		bool ctrlL = false;
		bool ctrlR = false;
		bool shiftL = false;
		bool shiftR = false;
		bool altL = false;
		bool altR = false;
	} input;
};
