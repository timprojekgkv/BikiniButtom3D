#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint depthMapProgram, shaderProgram, 
		VBO, VAO, EBO, texture, stexture,
		VBO2, VAO2, EBO2, texture2, stexture2, 
		VBO3, VAO3, EBO3, texture3, stexture3,
		VBO4, VAO4, EBO4, texture4, stexture4,
		VBO5, VAO5, EBO5, texture5, stexture5,
		VBO6, VAO6, EBO6, texture6, stexture6,
		VBO7, VAO7, EBO7, texture7, stexture7,
		VBO8, VAO8, EBO8, texture8, stexture8,
		VBO9, VAO9, EBO9, texture9, stexture9,
		VBO10, VAO10, EBO10, texture10, stexture10,
		VBO11, VAO11, EBO11, texture11, stexture11,
		VBO12, VAO12, EBO12, texture12, stexture12,
		VBO13, VAO13, EBO13, texture13, stexture13,
		VBO14, VAO14, EBO14, texture14, stexture14,
		VBO15, VAO15, EBO15, texture15, stexture15,
		VBO16, VAO16, EBO16, texture16, stexture16,
		VBO17, VAO17, EBO17, texture17, stexture17,
		VBO18, VAO18, EBO18, texture18, stexture18;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	float angle = 0;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy, xt, zt;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);
	void BuildHead();
	void BuildFace();
	void BuildFrontBody();
	void BuildBody();
	void BuildHandRight();
	void BuildHandLeft();
	void BuildLegRight();
	void BuildLegLeft();
	void BuildHomePatrick();
	void BuildHomeSponge();
	void BuildFaceSponge();
	void BuildFrontBodySquid();
	void BuildBodySquid();
	void BuildHandRightSquid();
	void BuildHandLeftSquid();
	void BuildPanah();
	void BuildPlane();
	void BuildLangit();
	void DrawHead();
	void DrawFace();
	void DrawFrontBody();
	void DrawBody();
	void DrawHandRight();
	void DrawHandLeft();
	void DrawLegRight();
	void DrawLegLeft();
	void DrawHomePatrick();
	void DrawHomeSponge();
	void DrawFaceSponge();
	void DrawFrontBodySquid();
	void DrawBodySquid();
	void DrawHandRightSquid();
	void DrawHandLeftSquid();
	void DrawPanah();
	void DrawPlane();
	void DrawLangit();
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();

};

