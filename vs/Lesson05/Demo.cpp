#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildHead();
	BuildFace();
	BuildFrontBody();
	BuildBody();
	BuildHandRight();
	BuildHandLeft();
	BuildLegRight();
	BuildLegLeft();
	BuildHomePatrick();
	BuildHomeSponge();
	BuildFaceSponge();
	BuildFrontBodySquid();
	BuildBodySquid();
	BuildHandRightSquid();
	BuildHandLeftSquid();
	BuildPanah();
	BuildPlane();
	BuildLangit();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);


}

void Demo::Update(double deltaTime) {
	angle += (float) ((deltaTime * 1.5f) / 1000);
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.9f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	//glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawHead();
	DrawFace();
	DrawFrontBody();
	DrawBody();
	DrawHandRight();
	DrawHandLeft();
	DrawLegRight();
	DrawLegLeft();
	DrawHomePatrick();
	DrawHomeSponge();
	DrawFaceSponge();
	DrawFrontBodySquid();
	DrawBodySquid();
	DrawHandRightSquid();
	DrawHandLeftSquid();
	DrawPanah();
	DrawPlane();
	DrawLangit();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildHead() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("sampingsp-2.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		
		// right
		0.5,  0.5,  0.5, 0, 0,  // 0
		0.5,  0.5, -0.5, 1, 0,  // 1
		0.5, -0.3, -0.5, 1, 1,  // 2
		0.5, -0.3,  0.5, 0, 1,  // 3

		// back
		-0.5, -0.3, -0.5, 0, 0, // 4 
		0.5,  -0.3, -0.5, 1, 0, // 5
		0.5,   0.5, -0.5, 1, 1, // 6
		-0.5,  0.5, -0.5, 0, 1, // 7

		// left
		-0.5, -0.3, -0.5, 0, 0, // 8
		-0.5, -0.3,  0.5, 1, 0, // 9
		-0.5,  0.5,  0.5, 1, 1, // 10
		-0.5,  0.5, -0.5, 0, 1, // 11

		// upper
		0.5, 0.5,  0.5, 0, 0,   // 12
		-0.5, 0.5,  0.5, 1, 0,  // 13
		-0.5, 0.5, -0.5, 1, 1,  // 14
		0.5, 0.5, -0.5, 0, 1,   // 15

		// bottom
		-0.5, -0.3, -0.5, 0, 0, // 16
		0.5, -0.3, -0.5, 1, 0,  // 17
		0.5, -0.3,  0.5, 1, 1,  // 18
		-0.5, -0.3,  0.5, 0, 1, // 19

		
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // right
		4,  5,  6,  4,  6,  7,   // back
		8,  9,  10, 8,  10, 11,  // left
		12, 14, 13, 12, 15, 14,  // upper
		16, 18, 17, 16, 19, 18  // bottom
		   
		
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildFace() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("mukasp.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0.3, 0.5, 0, 0,  // 0
		0.5, -0.3, 0.5, 1, 0,   // 1
		0.5,  0.5, 0.5, 1, 1,   // 2
		-0.5,  0.5, 0.5, 0, 1  // 3

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3   // front

	};

	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glGenBuffers(1, &EBO6);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildFrontBody() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("bawahs.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0, 0.5, 0, 0,  // 0
		0.5, -0, 0.5, 1, 0,   // 1
		0.5,  0.5, 0.5, 1, 1,   // 2
		-0.5,  0.5, 0.5, 0, 1  // 3

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3   // front

	};

	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glGenBuffers(1, &EBO7);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO7);

	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildBody() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image2 = SOIL_load_image("bwhsamping-2.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices3[] = {
		// format position, tex coords
		
		// right
		0.5,  0.5,  0.5, 1, 1,  // 0
		0.5,  0.5, -0.5, 0, 1,  // 1
		0.5, -0, -0.5, 0, 0,  // 2
		0.5, -0,  0.5, 1, 0,  // 3

		// back
		-0.5, -0, -0.5, 0, 0, // 4 
		0.5,  -0, -0.5, 1, 0, // 5
		0.5,   0.5, -0.5, 1, 1, // 6
		-0.5,  0.5, -0.5, 0, 1, // 7

		// left
		-0.5, -0, -0.5, 0, 0, // 8
		-0.5, -0,  0.5, 1, 0, // 9
		-0.5,  0.5,  0.5, 1, 1, // 10
		-0.5,  0.5, -0.5, 0, 1, // 11

		// upper
		0.5, 0.5,  0.5, 0, 0,   // 12
		-0.5, 0.5,  0.5, 1, 0,  // 13
		-0.5, 0.5, -0.5, 1, 1,  // 14
		0.5, 0.5, -0.5, 0, 1,   // 15

		// bottom
		-0.5, -0, -0.5, 0, 0, // 16
		0.5, -0, -0.5, 1, 0,  // 17
		0.5, -0,  0.5, 1, 1,  // 18
		-0.5, -0,  0.5, 0, 1, // 19
	};
	
	unsigned int indices3[] = {
		0,  1,  2,  0,  2,  3,   // right
		4,  5,  6,  4,  6,  7,   // back
		8,  9,  10, 8,  10, 11,  // left
		12, 14, 13, 12, 15, 14,  // upper
		16, 18, 17, 16, 19, 18   // bottom
		

	};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildHandRight() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kakikotak.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {

		// right
		0.5,  0.5,  0.5, 0, 0,  // 0
		0.5,  0.5, -0.5, 1, 0,  // 1
		0.5, -0.3, -0.5, 1, 1,  // 2
		0.5, -0.3,  0.5, 0, 1,  // 3

		1.1,-0.5,0,0,0 //4



	};

	unsigned int indices[] = {
		0,  4, 1,   // front
		2,  4, 3,  // back
		1,  4, 2,  // right
		0,  4, 3  // left

	};

	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	glGenBuffers(1, &EBO8);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO8);

	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildHandLeft() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture9);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kakikotak.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		
		// left
		-0.5, -0.3, -0.5, 0, 0, // 0
		-0.5, -0.3,  0.5, 1, 0, // 1
		-0.5,  0.5,  0.5, 1, 1, // 2
		-0.5,  0.5, -0.5, 0, 1, // 3

		-1.1,-0.5,0,0,0 //4


	};

	unsigned int indices[] = {
		0,  4, 1,   // front
		2,  4, 3,  // back
		1,  4, 2,  // right
		0,  4, 3  // left

	};

	glGenVertexArrays(1, &VAO9);
	glGenBuffers(1, &VBO9);
	glGenBuffers(1, &EBO9);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO9);

	glBindBuffer(GL_ARRAY_BUFFER, VBO9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO9);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildLegRight() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kakikotak.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		
		// bottom
		-0, -0.5, 0.5, 0, 0, // 0
		0.5, -0.5, 0.5, 1, 0,  // 1
		0.5, -0.5,  -0.5, 1, 1,  // 2
		0, -0.5,  -0.5, 0, 1, // 3

	    0.25,-1.1,0,0,0 //4



	};

	unsigned int indices[] = {
		0,  4, 1,   // front
	    2,  4, 3,  // back
		1,  4, 2,  // right
		0,  4, 3  // left
		//16, 18, 17, 16, 19, 18,  // upper
		//20, 22, 21, 20, 23, 22   // bottom

	};

	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildLegLeft() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kakikotak.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// bottom
		-0.5, -0.5, 0.5, 0, 0, // 0
		0, -0.5, 0.5, 1, 0, // 1
		0, -0.5,  -0.5, 1, 1, // 2
		-0.5, -0.5,  -0.5, 0, 1, // 3

		-0.25,-1.1,0,1,1 //4
																																-0.5, -0.3,  0.5, 0, 1, // 23


	};

	unsigned int indices[] = {
		0,  4, 1,   // front
		2,  4, 3,  // back
		1,  4, 2,  // right
		0,  4, 3  // left

	};

	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildHomePatrick() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture10);
	glBindTexture(GL_TEXTURE_2D, texture10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("homepatrick.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords

		// right
		0.5,  0.5,  0.5, 0, 0,  // 0
		0.5,  0.5, -0.5, 1, 0,  // 1
		0.5, -0.5, -0.5, 1, 1,  // 2
		0.5, -0.5,  0.5, 0, 1,  // 3

		// back
		-0.5, -0.5, -0.5, 0, 0, // 4 
		0.5,  -0.5, -0.5, 1, 0, // 5
		0.5,   0.5, -0.5, 1, 1, // 6
		-0.5,  0.5, -0.5, 0, 1, // 7

		// left
		-0.5, -0.5, -0.5, 0, 0, // 8
		-0.5, -0.5,  0.5, 1, 0, // 9
		-0.5,  0.5,  0.5, 1, 1, // 10
		-0.5,  0.5, -0.5, 0, 1, // 11

		// bottom
		-0.5, -0.5, -0.5, 0, 0, // 12
		0.5, -0.5, -0.5, 1, 0,  // 13
		0.5, -0.5,  0.5, 1, 1,  // 14
		-0.5, -0.5,  0.5, 0, 1, // 15

		// front
		-0.5, -0.5, 0.5, 0, 0,  // 16
		0.5, -0.5, 0.5, 1, 0,   // 17
		0.5,  0.5, 0.5, 1, 1,   // 18
		-0.5,  0.5, 0.5, 0, 1,  // 19

							   //Diagonal front
		-0.5,  0.5, 0.5, 0, 0,  // 20
		0.5,  0.5, 0.5, 1, 0,  // 21 
		0.2, 1, 0.25, 1, 1,   // 22 
		-0.2, 1,  0.25, 0, 1, // 23  

							 //Diagonal Back 
		-0.5, 0.5, -0.5, 0, 0,  // 24 
		0.5,  0.5, -0.5, 1, 0,  // 25 
		0.2, 1, -0.25, 1, 1,  // 26 
		-0.2, 1, -0.25, 0, 1,  // 27  

							  //Diagonal Right 
		0.2, 1, 0.25, 0, 0,  // 28 
		0.2, 1, -0.25, 1, 0,  // 29 
		0.5,  0.5, 0.5, 0, 1,  // 30 
		0.5,  0.5, -0.5, 1, 1,  // 31 

							//Diagonal Left 
		-0.5, 0.5, -0.5, 0, 0,  //  32 
		-0.5,  0.5, 0.5, 1, 0,  //  33 
		-0.2, 1,  0.25, 1, 1,  // 34 
		-0.2, 1, -0.25, 0, 1,  // 35 

							  //Atap
		0.2, 1, 0.25, 0, 0,   // 36 
		-0.2, 1,  0.25, 1, 0, // 37 
		0.2, 1, -0.25, 0, 1,  // 38 
		-0.2, 1, -0.25, 1, 1,  // 39  


	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // right
		4,  5,  6,  4,  6,  7,   // back
		8,  9,  10, 8,  10, 11,  // left
		12, 14, 13, 12, 15, 14,  // bottom
		16, 18, 17, 16, 19, 18,  // front
		20, 21, 22, 20, 22, 23,	  // diagonal front
		24, 25, 26, 24, 26, 27,  // diagonal back
		28, 29, 31, 28, 30, 31,   // diagonal right
		32, 33, 34, 32, 34, 35,  // diagonal left
		36, 37, 39, 36, 38, 39,  // atap


	};

	glGenVertexArrays(1, &VAO10);
	glGenBuffers(1, &VBO10);
	glGenBuffers(1, &EBO10);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO10);

	glBindBuffer(GL_ARRAY_BUFFER, VBO10);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO10);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildHomeSponge() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture11);
	glBindTexture(GL_TEXTURE_2D, texture11);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("homebackspng.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		

							// right
							1,  0,  0.5, 0, 0,  // 4
							1,  0, -0.5, 1, 0,  // 5
							1, -2, -0.5, 1, 1,  // 6
							1, -2,  0.5, 0, 1,  // 7

												// back
												-1, -2, -0.5, 0, 0,  // 8 
												1,  -2, -0.5, 1, 0,  // 9
												1,   0, -0.5, 1, 1,  // 10
												-1,  0, -0.5, 0, 1,  // 11

																	 // left
																	 -1, -2, -0.5, 0, 0,  // 12
																	 -1, -2,  0.5, 1, 0,  // 13
																	 -1,  0,  0.5, 1, 1,  // 14
																	 -1,  0, -0.5, 0, 1,  // 15



																											   // bottom
																											   -1, -2, -0.5, 0, 0,  // 16
																											   1, -2, -0.5, 1, 0,   // 17
																											   1, -2,  0.5, 1, 1,   // 18
																											   -1, -2,  0.5, 0, 1,  // 19

		
		//Diagonal front
		-1, 0,  0.5, 0, 0,  // 20
		1,  0,  0.5, 1, 0,  // 21 
		0.5, 1, 0.5, 1, 1,   // 22 
		-0.5, 1,  0.5, 0, 1, // 23  

							 //Diagonal Back 
		-1, 0, -0.5, 0, 0,  // 24 
		1,  0, -0.5, 1, 0,  // 25 
		0.5, 1, -0.5, 1, 1,  // 26 
		-0.5, 1, -0.5, 0, 1,  // 27  

							  //Diagonal Right 
		0.5, 1, 0.5, 0, 0,  // 28 
		0.5, 1, -0.5, 1, 0,  // 29 
		1,  0,  0.5, 0, 1,  // 30 
		1,  0, -0.5, 1, 1,  // 31 

							//Diagonal Left 
		-1, 0, -0.5, 0, 0,  //  32 
		-1, 0,  0.5, 1, 0,  //  33 
		-0.5, 1,  0.5, 1, 1,  // 34 
		-0.5, 1, -0.5, 0, 1,  // 35 

		//Atap
		0.5, 1, 0.5, 0, 0,   // 36 
		-0.5, 1,  0.5, 1, 0, // 37 
		0.5, 1, -0.5, 0, 1,  // 38 
		-0.5, 1, -0.5, 1, 1,  // 39  
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // right
		4,  5,  6,  4,  6,  7,   // back
		8,  9,  10, 8,  10, 11,  // left
		12, 14, 13, 12, 15, 14,  // buttom
		16, 18, 17, 16, 19, 18,  // diagonal front
		20, 21, 22, 20, 22, 23,	  // diagonal back
		24, 25, 27, 24, 26, 27,  // diagonal right
		28, 29, 30, 28, 30, 31,   // diagonal left
		32, 33, 35, 32, 34, 35,  // atap
		

	};

	glGenVertexArrays(1, &VAO11);
	glGenBuffers(1, &VBO11);
	glGenBuffers(1, &EBO11);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO11);

	glBindBuffer(GL_ARRAY_BUFFER, VBO11);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO11);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildFaceSponge() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture12);
	glBindTexture(GL_TEXTURE_2D, texture12);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("homespng.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-1, -2, 0.5, 0, 0,  // 0 m
		1, -2, 0.5, 1, 0,   // 1 n
		1,  0, 0.5, 1, 1,   // 2 y
		-1,  0, 0.5, 0, 1,  // 3 q

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3   // front

	};

	glGenVertexArrays(1, &VAO12);
	glGenBuffers(1, &VBO12);
	glGenBuffers(1, &EBO12);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO12);

	glBindBuffer(GL_ARRAY_BUFFER, VBO12);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO12);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildFrontBodySquid() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture13);
	glBindTexture(GL_TEXTURE_2D, texture13);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("sqdepan.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-1, -2, 0.5, 0, 0,  // 0
		1, -2, 0.5, 1, 0,   // 1
		1,  3, 0.5, 1, 1,   // 2
		-1,  3, 0.5, 0, 1  // 3

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3   // front

	};

	glGenVertexArrays(1, &VAO13);
	glGenBuffers(1, &VBO13);
	glGenBuffers(1, &EBO13);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO13);

	glBindBuffer(GL_ARRAY_BUFFER, VBO13);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO13);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildBodySquid() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture14);
	glBindTexture(GL_TEXTURE_2D, texture14);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image2 = SOIL_load_image("sqsamping-2.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices3[] = {
		// format position, tex coords

		// right
		1,  3,  0.5, 0, 0,  // 0
		1,  3, -0.5, 1, 0,  // 1
		1, -2, -0.5, 1, 1,  // 2
		1, -2,  0.5, 0, 1,  // 3

							// back
							-1, -2, -0.5, 0, 0, // 4 
							1,  -2, -0.5, 1, 0, // 5
							1,   3, -0.5, 1, 1, // 6
							-1,  3, -0.5, 0, 1, // 7

												// left
												-1, -2, -0.5, 0, 0, // 8
												-1, -2,  0.5, 1, 0, // 9
												-1,  3,  0.5, 1, 1, // 10
												-1,  3, -0.5, 0, 1, // 11

																	// upper
																	1, 3,  0.5, 0, 0,   // 12
																	-1, 3,  0.5, 1, 0,  // 13
																	-1, 3, -0.5, 1, 1,  // 14
																	1, 3, -0.5, 0, 1,   // 15

																						// bottom
																						-1, -2, -0.5, 0, 0, // 16
																						1, -2, -0.5, 1, 0,  // 17
																						1, -2,  0.5, 1, 1,  // 18
																						-1, -2,  0.5, 0, 1 // 19
	};

	unsigned int indices3[] = {
		0,  1,  2,  0,  2,  3,   // right
		4,  5,  6,  4,  6,  7,   // back
		8,  9,  10, 8,  10, 11,  // left
		12, 14, 13, 12, 15, 14,  // upper
		16, 18, 17, 16, 19, 18   // bottom
	};

	glGenVertexArrays(1, &VAO14);
	glGenBuffers(1, &VBO14);
	glGenBuffers(1, &EBO14);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO14);

	glBindBuffer(GL_ARRAY_BUFFER, VBO14);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO14);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildHandRightSquid() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture15);
	glBindTexture(GL_TEXTURE_2D, texture15);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("sqtangan-2.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {

		// right
		1.5,  1,  0.5, 0, 0,  // 0
		1.5,  1, -0.5, 1, 0,  // 1
		1.5, -2, -0.5, 1, 1,  // 2
		1.5, -2,  0.5, 0, 1,  // 3

							  // back
							  1, -2, -0.5, 0, 0, // 4 
							  1.5,  -2, -0.5, 1, 0, // 5
							  1.5,   1, -0.5, 1, 1, // 6
							  1,  1, -0.5, 0, 1, // 7

												 // left
												 1, -2, -0.5, 0, 0, // 8
												 1, -2,  0.5, 1, 0, // 9
												 1,  1,  0.5, 1, 1, // 10
												 1,  1, -0.5, 0, 1, // 11

																	// upper
																	1.5, 1,  0.5, 0, 0,   // 12
																	1, 1,  0.5, 1, 0,  // 13
																	1, 1, -0.5, 1, 1,  // 14
																	1.5, 1, -0.5, 0, 1,   // 15

																						  // bottom
																						  1, -2, -0.5, 0, 0, // 16
																						  1.5, -2, -0.5, 1, 0,  // 17
																						  1.5, -2,  0.5, 1, 1,  // 18
																						  1, -2,  0.5, 0, 1, // 19

																											 // front
																											 1, -2, 0.5, 0, 0,  // 20
																											 1.5, -2, 0.5, 1, 0,   // 21
																											 1.5,  1, 0.5, 1, 1,   // 22
																											 1,  1, 0.5, 0, 1  // 23



	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // right
		4,  5,  6,  4,  6,  7,   // back
		8,  9,  10, 8,  10, 11,  // left
		12, 14, 13, 12, 15, 14,  // upper
		16, 18, 17, 16, 19, 18,   // bottom
		20, 21, 22, 20, 22, 23   // front
	};

	glGenVertexArrays(1, &VAO15);
	glGenBuffers(1, &VBO15);
	glGenBuffers(1, &EBO15);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO15);

	glBindBuffer(GL_ARRAY_BUFFER, VBO15);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO15);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildHandLeftSquid() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture16);
	glBindTexture(GL_TEXTURE_2D, texture16);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("sqtangan.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {

		// right
		-1,  1,  0.5, 0, 0,  // 0
		-1,  -2, -0.5, 1, 0,  // 1
		-1, 1, -0.5, 1, 1,  // 2
		-1, -2,  0.5, 0, 1,  // 3

							 // back
							 -1.5, -2, -0.5, 0, 0, // 4 
							 -1,  -2, -0.5, 1, 0, // 5
							 -1,   1, -0.5, 1, 1, // 6
							 -1.5,  1, -0.5, 0, 1, // 7

												   // left
												   -1.5, -2, -0.5, 0, 0, // 8
												   -1.5, -2,  0.5, 1, 0, // 9
												   -1.5,  1,  0.5, 1, 1, // 10
												   -1.5,  1, -0.5, 0, 1, // 11

																		 // upper
																		 -1, 1,  0.5, 0, 0,   // 12
																		 -1.5, 1,  0.5, 1, 0,  // 13
																		 -1.5, 1, -0.5, 1, 1,  // 14
																		 -1, 1, -0.5, 0, 1,   // 15

																							  // bottom
																							  -1.5, -2, -0.5, 0, 0, // 16
																							  -1, -2, -0.5, 1, 0,  // 17
																							  -1, -2,  0.5, 1, 1,  // 18
																							  -1.5, -2,  0.5, 0, 1, // 19

																													// front
																													-1.5, -2, 0.5, 0, 0, // 20
																													-1, -2, 0.5, 1, 0,  // 21
																													-1, 1,  0.5, 1, 1,  // 22
																													-1.5, 1,  0.5, 0, 1 // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // right
		4,  5,  6,  4,  6,  7,   // back
		8,  9,  10, 8,  10, 11,  // left
		12, 14, 13, 12, 15, 14,  // upper
		16, 18, 17, 16, 19, 18,   // bottom
		20, 21, 22, 20, 22, 23   // front

	};

	glGenVertexArrays(1, &VAO16);
	glGenBuffers(1, &VBO16);
	glGenBuffers(1, &EBO16);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO16);

	glBindBuffer(GL_ARRAY_BUFFER, VBO16);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO16);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::BuildPanah()
{
	// Load and create a texture 
	glGenTextures(1, &texture17);
	glBindTexture(GL_TEXTURE_2D, texture17);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kakikotak.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-0.1, 1.0, 0,  0,  0, //0
		-0.1, 1.4, 0, 0,  1, //1
		0.1, 1.4,  0, 1, 1,  //2
		0.1, 1.0,  0,  1, 0, //3

		-0.4, 1.4, 0,  0,  0, //4
		-0.4, 1.6, 0, 0,  1, //5
		0.4, 1.6,  0, 1, 1,  //6
		0.4, 1.4,  0,  1, 0, //7

		0.4, 1.3, 0,  0,  0, //8
		0.4, 1.7, 0, 0,  1, //9
		0.6, 1.5,  0, 1, 1,  //10

		-0.4, 1.4, 0,  0,  0, //12
		-0.4, 1.6, 0, 0,  1, //13
		0.4, 1.6,  0, 1, 1,  //14
		0.4, 1.4,  0,  1, 0, //15


	};

	GLuint indices[] = {
		0,  2,  1,  0,  3,  2,
		4,  5,  6,  4,  6,  7,
		8,  9,  10
	};

	glGenVertexArrays(1, &VAO17);
	glGenBuffers(1, &VBO17);
	glGenBuffers(1, &EBO17);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO17);

	glBindBuffer(GL_ARRAY_BUFFER, VBO17);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO17);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("tanah.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -60.0,  1,  1,
		50.0, -0.5, -60.0, 0,  1,
		50.0, -0.5,  20.0, 0, 0,
		-50.0, -0.5,  20.0,  1, 0,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildLangit()
{
	// Load and create a texture 
	glGenTextures(1, &texture18);
	glBindTexture(GL_TEXTURE_2D, texture18);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("langit.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// langit
		-50.0, -0.5, -40.0,  0,  0,
		50.0, -0.5, -40.0, 1,  0,
		50.0, 40.0,  -40.0, 1, 1,
		-50.0, 40.0,  -40.0,  0, 1,

		//right
		-50.0, -0.5,  20.0,  0,  0,
		-50.0, -0.5, -40.0, 1,  0,
		-50.0, 40.0, -40.0, 1, 1,
		-50.0, 40.0,  20.0,  0, 1,

		//left
		50.0, -0.5, 20.0,  0,  0,
		50.0, -0.5, -40.0, 1,  0,
		50.0, 40.0, -40.0, 1, 1,
		50.0, 40.0, 20.0,  0, 1,


	};

	GLuint indices[] = { 
		0,  2,  1,  0,  3,  2, 
		4,  6,  5,  4,  7,  6,
		8,  10, 9,  8,  11, 10
	};

	glGenVertexArrays(1, &VAO18);
	glGenBuffers(1, &VBO18);
	glGenBuffers(1, &EBO18);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO18);

	glBindBuffer(GL_ARRAY_BUFFER, VBO18);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO18);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::DrawHead()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	
	//transform
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(6, 2, 0)); //posisi objek

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(2, 2, 1)); //ukuran objek

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawFace()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO6); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(6, 2, 0));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(2, 2, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawFrontBody()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO7); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(6, 0.8, 0));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1.8, 1.2, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawBody()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(6, 0.8, 0));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1.8, 1.2, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawHandRight()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO8); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(6, 1.7, 0));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1.7, 0.6, 0.6));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawHandLeft()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO9); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(6, 1.7, 0));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1.7, 0.6, 0.6));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawLegRight()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(6, 2.3, 0));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1.8, 2, 0.9));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawLegLeft()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO5); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(6, 2.3, 0));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1.8, 2, 0.9));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawHomePatrick()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture10);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO10); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-16, 1.5, -8));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(6, 3, 6));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 64, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawHomeSponge()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture11);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO11); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	//transform
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(16, 5.5, -8)); //posisi objek

	model = glm::scale(model, glm::vec3(3, 3, 3)); //ukuran objek

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 64, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawFaceSponge()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture12);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO6); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(16, 1.5, -9));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(6, 8, 5));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawFrontBodySquid()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture13);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO13); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 3.0, -8));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3.5, 3, 3.5));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawBodySquid()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture14);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO14); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 3.0, -8));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3.5, 3, 3.5));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawHandRightSquid()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture15);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO15); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 6, -8));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3.5, 2, 2));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawHandLeftSquid()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture16);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO16); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 6, -8));

	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3.5, 2, 2));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawPanah()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture17);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO17); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-16, 1.5, -8));

	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3, 3, 3));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawLangit()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture18);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO18); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	//initialisasi variabel
	posCamX = 10.0f;
	posCamY = 10.0f;
	posCamZ = 30.0f;
	viewCamX = 0.0f;
	viewCamY = 0.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	xt = 0.0f;
	zt = 0.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	//putar camera
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}


int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Bikini Bottom", 800, 600, false, false);
}