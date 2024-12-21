#include "Angel.h"
#include "TriMesh.h"
#include "Camera.h"
#include "MeshPainter.h"

#include <vector>
#include <string>

const int WIDTH = 1000;
const int HEIGHT = 800;

int cursorMode = 1;

Camera* camera = new Camera();
Light* light = new Light();
MeshPainter *painter = new MeshPainter();

// 这个用来回收和删除我们创建的物体对象
std::vector<TriMesh *> meshList;
int i = 0;

TriMesh* sphere = new TriMesh();


//#include <windows.h>
//std::vector<std::string> getTextureFiles(const std::string& directoryPath) {
//	std::vector<std::string> textureFiles;
//	WIN32_FIND_DATA findFileData;
//
//	// 构造完整的路径，包括通配符（*）用于匹配所有文件
//	std::string searchPath = directoryPath + "\\*";
//
//	// 调用 FindFirstFile 查找第一个文件
//	HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);
//	if (hFind == INVALID_HANDLE_VALUE) {
//		std::cerr << "Could not open directory " << directoryPath << std::endl;
//		return textureFiles;
//	}
//
//	// 遍历所有文件
//	do {
//		std::string filename = findFileData.cFileName;
//		// 排除 . 和 .. 目录
//		if (filename != "." && filename != "..") {
//			// 过滤出常见的图片扩展名
//			std::string ext = filename.substr(filename.find_last_of('.'));
//			if (ext == ".jpg" || ext == ".png" || ext == ".bmp" || ext == ".jpeg") {
//				textureFiles.push_back(filename);
//			}
//		}
//	} while (FindNextFile(hFind, &findFileData) != 0);
//
//	FindClose(hFind);
//	return textureFiles;
//}

//挖掘机
TriMesh* cube = new TriMesh();
// 关节角
enum {
	Base = 0,
	UpperArm = 1,
	LowerArm = 2,
	Palm = 3,
	NumAngles = 4
};
int     Axis = Base;
GLfloat Theta[NumAngles] = { 0.0 };

// 尺寸参数
const GLfloat BASE_HEIGHT = 0.2;
const GLfloat BASE_WIDTH = 0.5;
const GLfloat UPPER_ARM_HEIGHT = 0.5;
const GLfloat UPPER_ARM_WIDTH = 0.12;
const GLfloat LOWER_ARM_HEIGHT = 0.4;
const GLfloat LOWER_ARM_WIDTH = 0.1;
const GLfloat PALM_HEIGHT = 0.05;
const GLfloat PALM_WIDTH = 0.2;

// 绘制底座
void base(glm::mat4 modelView)
{
	// 按长宽高缩放正方体，平移至合适位置
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, BASE_HEIGHT / 2, 0.0));
	instance = glm::scale(instance, glm::vec3(BASE_WIDTH, BASE_HEIGHT, BASE_WIDTH));

	// 绘制，由于我们只有一个立方体数据，所以这里可以直接指定绘制painter中存储的第0个立方体
	painter->drawMesh(0, modelView * instance, light, camera);//函数重载

}
// 绘制大臂
void upper_arm(glm::mat4 modelView)
{
	// @TODO: 参考底座的绘制，在此添加代码绘制大臂
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, UPPER_ARM_HEIGHT / 2, 0.0));
	instance = glm::scale(instance, glm::vec3(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_WIDTH));

	// 绘制，由于我们只有一个立方体数据，所以这里可以直接指定绘制painter中存储的第0个立方体
	painter->drawMesh(0, modelView * instance, light, camera);

}
// 绘制小臂
void lower_arm(glm::mat4 modelView)
{
	// @TODO: 参考底座的绘制，在此添加代码绘制小臂
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, LOWER_ARM_HEIGHT / 2, 0.0));
	instance = glm::scale(instance, glm::vec3(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH));

	// 绘制，由于我们只有一个立方体数据，所以这里可以直接指定绘制painter中存储的第0个立方体
	painter->drawMesh(0, modelView * instance, light, camera);
}
// 绘制手掌
void palm(glm::mat4 modelView)
{
	// @TODO: 参考底座的绘制，在此添加代码绘制小臂
	glm::mat4 instance = glm::mat4(1.0);
	instance = glm::translate(instance, glm::vec3(0.0, PALM_HEIGHT / 2, 0.0));
	instance = glm::scale(instance, glm::vec3(PALM_WIDTH, PALM_HEIGHT, PALM_WIDTH));

	// 绘制，由于我们只有一个立方体数据，所以这里可以直接指定绘制painter中存储的第0个立方体
	painter->drawMesh(0, modelView * instance, light, camera);
}


void init()
{
	std::string vshader, fshader;
	// 读取着色器并使用
#ifdef __APPLE__	// for MacOS
	vshader = "shaders/vshader_mac.glsl";
	fshader = "shaders/fshader_mac.glsl";
#else				// for Windows
	vshader = "shaders/vshader_win.glsl";
	fshader = "shaders/fshader_win.glsl";
#endif

	// 设置光源位置
	light->setTranslation(glm::vec3(5, 5, 5));
	//light->setTranslation(glm::vec3(3, 0, 0));
	light->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	light->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	light->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	light->setAttenuation(1.0, 0.045, 0.0075); // 衰减系数

	camera->setAspect(1.0*WIDTH / HEIGHT);


	//机器臂 【！drawMeshes从1开始了，此层级建模机器臂另外的方式画】
	cube->setNormalize(false);
	cube->generateCube();
	//cube->reverse_vertex_normals();
	cube->setTranslation(glm::vec3(-3.0, 0.0, 0.0));
	cube->setRotation(glm::vec3(0.0, 0.0, 0.0));
	cube->setScale(glm::vec3(0.5, 0.5, 0.5));
	// 设置材质
	cube->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	cube->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	cube->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	cube->setShininess(1.0); //高光系数
	// 加到painter中
	painter->addMesh(cube, "mechanical arm", {}, vshader, fshader);
	meshList.push_back(cube);



 	TriMesh* table = new TriMesh();
	// @TODO: Task2 读取桌子模型
	table->setNormalize(true);
	table->readObj("./assets/table.obj");
	table->storeFacesPoints();
	// 设置物体的旋转位移
	table->setTranslation(glm::vec3(-2, 0.22, 0.0));
	table->setRotation(glm::vec3(-90, 0.0, 0.0));
	table->setScale(glm::vec3(1.0, 1.0, 1.0));
	// 设置材质
	table->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	table->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	table->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	table->setShininess(1.0); //高光系数
	// 加到painter中
	painter->addMesh(table, "table", { "./assets/table.png" }, vshader, fshader);
	meshList.push_back(table);



	TriMesh* wawa = new TriMesh();
	wawa->setNormalize(true);
	wawa->readObj("./assets/wawa.obj");
	wawa->storeFacesPoints();
	// 设置物体的旋转位移
	wawa->setTranslation(glm::vec3(-1, 0.37, 0));
	wawa->setRotation(glm::vec3(-90, 0, 0));
	wawa->setScale(glm::vec3(1.0, 1.0, 1));
	// 设置材质
	wawa->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	wawa->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	wawa->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	wawa->setShininess(1.0); //高光系数
	// 加到painter中
	painter->addMesh(wawa, "wawa", { "./assets/wawa.png" }, vshader, fshader);
	meshList.push_back(wawa);


	//saber模型 （此.obj是四角面片，有s光滑组)
	TriMesh* saber = new TriMesh();
	saber->setNormalize(true);
	saber->readObj("./assets/saber/A pose.obj");
	saber->reverse_vertex_normals();
	saber->storeFacesPoints();
	// 设置物体的旋转位移
	saber->setTranslation(glm::vec3(0, 0.36, 0));
	saber->setRotation(glm::vec3(0, 0, 0));
	saber->setScale(glm::vec3(1.0, 1.0, 1));
	// 设置材质
	saber->setAmbient(glm::vec4(0.3, 0.3, 0.3, 1.0)); // 提高环境光
	saber->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 提高漫反射
	saber->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 提高镜面反射
	saber->setShininess(50.0); // 高光系数增大
	// 加到painter中
	painter->addMesh(saber, "saber", { "./assets/saber/textures/uv_cha.tga.png" }, vshader, fshader);
	meshList.push_back(saber);



	//saber2模型 
	TriMesh* saber2 = new TriMesh();
	saber2->setNormalize(true);
	saber2->readObj("./assets/saber2/Saber_Facials_Finished.obj");
	saber2->storeFacesPoints();
	// 设置物体的旋转位移
	saber2->setTranslation(glm::vec3(1, 0.25, 0));
	saber2->setRotation(glm::vec3(0, 25, 0));
	saber2->setScale(glm::vec3(1.0, 1.0, 1));
	// 设置材质
	saber2->setAmbient(glm::vec4(0.3, 0.3, 0.3, 1.0)); // 提高环境光
	saber2->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 提高漫反射
	saber2->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 提高镜面反射
	saber2->setShininess(50.0); // 高光系数增大
	// 加到painter中
	//std::vector<std::string> textures = getTextureFiles("./assets/saber2/textures");
	//std::cout << "\n[DEBUG]\n";
	//for (auto& x : textures) {
	//	std::cout << x << "\n";
	//}
	//painter->addMesh(saber2, "mesh_saber2", textures, vshader, fshader);
	painter->addMesh(saber2, "saber2", {"./assets/saber2/textures/svt_0001_s01_SVT_0001_S01_clothall.png" }, vshader, fshader);
	meshList.push_back(saber2);



	//牛    牛蹄是黑的，再怎么照也照不亮z
	TriMesh* cow = new TriMesh();
	cow->setNormalize(true);
	cow->readOff("./assets/off/cow.off");
	cow->storeFacesPoints();
	// 设置物体的旋转位移
	cow->setTranslation(glm::vec3(2, 0.25, 0));
	cow->setRotation(glm::vec3(0, -90, 0));
	cow->setScale(glm::vec3(1.0, 1.0, 1));
	// 设置材质
	cow->setAmbient(glm::vec4(0.8, 0.8, 0.8, 1.0)); // 提高环境光
	cow->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 提高漫反射
	cow->setSpecular(glm::vec4(0.5, 0.5, 0.5, 1.0)); // 提高镜面反射
	cow->setShininess(20.0); // 高光系数增大
	// 加到painter中
	painter->addMesh(cow, "cow", {  }, vshader, fshader);
	meshList.push_back(cow);


	TriMesh* cube2 = new TriMesh();
	cube2->setNormalize(true);
	cube2->generateCube();
	//cube2->reverse_vertex_normals();
	// 设置物体的旋转位移
	cube2->setTranslation(glm::vec3(3, 0.3, 0));
	cube2->setRotation(glm::vec3(-90, 0, 0));
	cube2->setScale(glm::vec3(0.5, 0.5, 0.5));
	// 设置材质
	cube2->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	cube2->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	cube2->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	cube2->setShininess(1.0); //高光系数
	// 加到painter中
	painter->addMesh(cube2, "cube2", {  }, vshader, fshader);
	meshList.push_back(cube2);



	// 画一个背景图
	TriMesh* background = new TriMesh();
	background->setNormalize(true);
	background->generateSquare(glm::vec3(0.5, 0.5, 0.5));
	//cube貌似有bug，phong显示的不对。
	background->setScale(glm::vec3(15.0, 15.0, 15));
	//background->setTranslation(glm::vec3(0, 15, -10));
	//background->setRotation(glm::vec3(50, 0, 0));
	background->setTranslation(glm::vec3(0, 3, -8));
	background->setRotation(glm::vec3(0, 0, 0));
	background->setShadowCaster(false);
	// 设置材质
	background->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	background->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	background->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	background->setShininess(1.0); //高光系数
	painter->addMesh(background, "background", { "./assets/InstructionManual.png" }, vshader, fshader);
	meshList.push_back(background);



	// 画一个背景图
	TriMesh* background2 = new TriMesh();
	background2->setNormalize(true);
	background2->generateSquare(glm::vec3(0.5, 0.5, 0.5));
	//cube貌似有bug，phong显示的不对。
	background2->setScale(glm::vec3(10.0, 10.0, 10));
	background2->setTranslation(glm::vec3(0, 3, 10));
	background2->setRotation(glm::vec3(0, 180, 0));
	background2->setShadowCaster(false);
	// 设置材质
	background2->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	background2->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	background2->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	background2->setShininess(1.0); //高光系数
	painter->addMesh(background2, "background2", { "./assets/saber.png" }, vshader, fshader);
	meshList.push_back(background2);



	//代表点光源
	sphere->readOff("./assets/off/sphere.off");
	sphere->setNormalize(true);
	sphere->storeFacesPoints();
	sphere->setTranslation(light->getTranslation());
	sphere->setRotation(glm::vec3(-90, 0, 0));
	sphere->setScale(glm::vec3(1.0, 1.0, 1));
	// 设置材质
	sphere->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	sphere->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	sphere->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	sphere->setShininess(1.0); //高光系数
	// 加到painter中
	sphere->setShadowCaster(false);
	//sphere->sethasTextureMap(false);
	painter->addMesh(sphere, "light_sphere", { }, vshader, fshader);
	meshList.push_back(sphere);



	// 画一个地板
	TriMesh* ground = new TriMesh();
	ground->setNormalize(true);
	ground->generateSquare(glm::vec3(0.5,0.5,0.5));
	//cube貌似有bug，phong显示的不对。
	ground->setScale(glm::vec3(15.0, 15.0, 1));
	ground->setTranslation(glm::vec3(0, -0.001, 0));
	ground->setRotation(glm::vec3(-90, 0, 0));
	// 设置材质
	ground->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	ground->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	ground->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	ground->setShininess(1.0); //高光系数
	ground->setShadowCaster(false);
	painter->addMesh(ground, "ground", { "./assets/ground.png" }, vshader, fshader);
	meshList.push_back(ground);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	// glClearColor(0.0, 0.0, 0.0, 1.0);
}


void display()
{
// #ifdef __APPLE__ // 解决 macOS 10.15 显示画面缩小问题
// 	glViewport(0, 0, WIDTH * 2, HEIGHT * 2);
// #endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	painter->drawMeshes(light, camera);

	glm::mat4 modelView = glm::mat4(1.0);
	// 绘制底座 
	modelView = glm::translate(modelView, glm::vec3(-3, 0, 0.0));// 稍微下移一下，让机器人整体居中在原点	【？】
	modelView = glm::rotate(modelView, glm::radians(Theta[Base]), glm::vec3(0.0, 1.0, 0.0));// 底座旋转矩阵
	base(modelView); // 首先绘制底座

	// @TODO: 在此添加代码完成整个机械手臂绘制
	// 大臂变换矩阵
	modelView = glm::translate(modelView, glm::vec3(0.0, BASE_HEIGHT, 0.0));// 稍微下移一下，让机器人整体居中在原点	
	modelView = glm::rotate(modelView, glm::radians(Theta[UpperArm]), glm::vec3(0.0, 0.0, 1.0));// 底座旋转矩阵
	// 绘制大臂
	upper_arm(modelView);


	// 小臂变换矩阵
	modelView = glm::translate(modelView, glm::vec3(0.0, UPPER_ARM_HEIGHT, 0.0));// 稍微下移一下，让机器人整体居中在原点	
	modelView = glm::rotate(modelView, glm::radians(Theta[LowerArm]), glm::vec3(0.0, 0.0, 1.0));// 底座旋转矩阵
	// 绘制小臂	
	lower_arm(modelView);

	// 手掌
	modelView = glm::translate(modelView, glm::vec3(0.0, LOWER_ARM_HEIGHT, 0.0));// 稍微下移一下，让机器人整体居中在原点	
	modelView = glm::rotate(modelView, glm::radians(Theta[Palm]), glm::vec3(0.0, 0.0, 1.0));// 底座旋转矩阵
	palm(modelView);

	//glutSwapBuffers();
}


void printHelp()
{
	std::cout << "\n================================================" << std::endl;
	std::cout << "Author: Ethan Wang   |   Made on 2024/12/21" << std::endl;
	std::cout << "Learn more in \"https://github.com/DBWGLX/-OpenGL-3D-Lighting-and-Shadow-Modeling\"" << std::endl;
	std::cout << "================================================" << std::endl << std::endl;

	std::cout << "Keyboard Usage\n"
		<< "\n[Window]\n"
		<< "ESC:           Exit\n"
		<< "h:             Print help message\n"
		<< "\n[Camera]\n"
		<< "R:             Reset camera parameters\n"
		<< "W A S D:       Move camera\n"
		<< "SPACE/SHIFT:   Increase/Decrease the height\n"
		<< "mouse:         Control the camera view\n"
		<< "Z:             Capture/Release the mouse\n"
		<< "\n[Light]\n"
		<< "I K J L / U O: Move the light along the Z, X, and Y axes\n"
		<< "\n[Mesh Operation]\n"
		<< "1/2:           Change operating mesh\n"
		<< "↑↓←→ KP1/2:Move the meshes along Y, X, and Z axes (KP: on Keypad)\n"
		<< "KP 0:          Rotate all\n"
		<< "+ -:           Change the meshes' scale\n"
		<< "KP 8/5/4/6:    Rotate the meshes vertically and horizontally\n"
		<< "\n[Mechanical Arm] 【Hierarchical Modeling】\n"
		<< "3/4:           Change layer\n"
		<< "5/6:           Revolve Joint\n";


	std::cout << "\n[Meshes Loaded]\n";

	std::vector<std::string>meshNames = painter->getMeshNames();
	for (int i = 0; i < meshNames.size();i++) {
		std::cout << i << ": " << meshNames[i] << std::endl;
	}

	std::cout << std::endl;
}

// 键盘响应函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	float tmp;
	glm::vec4 ambient;
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key)
		{
		case GLFW_KEY_ESCAPE: exit(EXIT_SUCCESS); break;
		case GLFW_KEY_H: printHelp(); break;
		case GLFW_KEY_1:
			i = (i + 1 + meshList.size()) % meshList.size();
			break;
		case GLFW_KEY_2:
			i = (i - 1 + meshList.size()) % meshList.size();
			break;
		case GLFW_KEY_3:
			Axis = (Axis + 1 + NumAngles) % NumAngles;
			break;
		case GLFW_KEY_4:
			Axis = (Axis - 1 + NumAngles) % NumAngles;
			break;
		case GLFW_KEY_5:
			Theta[Axis] += 5.0;
			if (Theta[Axis] > 360.0)
				Theta[Axis] -= 360.0;
			break;
		case GLFW_KEY_6:
			Theta[Axis] -= 5.0;
			if (Theta[Axis] < 0.0)
				Theta[Axis] += 360.0;
			break;
		case GLFW_KEY_I:
			light->setTranslation(light->getTranslation() + glm::vec3(0.0, 0.0, 0.5)); sphere->setTranslation(light->getTranslation());
			break;
		case GLFW_KEY_K:
			light->setTranslation(light->getTranslation() + glm::vec3(0.0, 0.0, -0.5)); sphere->setTranslation(light->getTranslation());
			break;
		case GLFW_KEY_J:
			light->setTranslation(light->getTranslation() + glm::vec3(-0.5, 0.0, 0.0)); sphere->setTranslation(light->getTranslation());
			break;
		case GLFW_KEY_L:
			light->setTranslation(light->getTranslation() + glm::vec3(0.5, 0.0, 0.0)); sphere->setTranslation(light->getTranslation());
			break;
		case GLFW_KEY_U:
			light->setTranslation(light->getTranslation() + glm::vec3(0.0, 0.5, 0.0)); sphere->setTranslation(light->getTranslation());
			break;
		case GLFW_KEY_O:
			light->setTranslation(light->getTranslation() + glm::vec3(0.0, -0.5, 0.0)); sphere->setTranslation(light->getTranslation());
			break;
		case GLFW_KEY_UP:  
			meshList[i]->setTranslation(meshList[i]->getTranslation() + glm::vec3(0.0, 0.0, 0.1));
			break;
		case GLFW_KEY_DOWN:  
			meshList[i]->setTranslation(meshList[i]->getTranslation() - glm::vec3(0.0, 0.0, 0.1));
			break;
		case GLFW_KEY_LEFT:  // 向左移动
			meshList[i]->setTranslation(meshList[i]->getTranslation() - glm::vec3(0.1, 0.0, 0.0));
			break;
		case GLFW_KEY_RIGHT:  // 向右移动
			meshList[i]->setTranslation(meshList[i]->getTranslation() + glm::vec3(0.1, 0.0, 0.0));
			break;
		case GLFW_KEY_KP_1:  
			meshList[i]->setTranslation(meshList[i]->getTranslation() + glm::vec3(0.0, 0.1, 0.0));
			break;
		case GLFW_KEY_KP_2:  
			meshList[i]->setTranslation(meshList[i]->getTranslation() - glm::vec3(0.0, 0.1, 0.0));
			break;
		case GLFW_KEY_KP_8:  // 向上旋转
			meshList[i]->setRotation(meshList[i]->getRotation() + glm::vec3(5.0, 0.0, 0.0));
			break;
		case GLFW_KEY_KP_5:  // 向下旋转
			meshList[i]->setRotation(meshList[i]->getRotation() - glm::vec3(5.0, 0.0, 0.0));
			break;
		case GLFW_KEY_KP_0:  // 【全体】向左旋转
			for (int i = 0; i < meshList.size()-1;i++) {
				meshList[i]->setRotation(meshList[i]->getRotation() + glm::vec3(0.0, 5.0, 0.0));
			}
			Theta[Base] += 5.0;
			break;
		case GLFW_KEY_KP_4:  // 向左旋转
			meshList[i]->setRotation(meshList[i]->getRotation() + glm::vec3(0.0, 5.0, 0.0));
			break;
		case GLFW_KEY_KP_6:  // 向右旋转
			meshList[i]->setRotation(meshList[i]->getRotation() - glm::vec3(0.0, 5.0, 0.0));
			break;
		case GLFW_KEY_MINUS:  // 缩小 (普通键盘 -)
		case GLFW_KEY_KP_SUBTRACT:  // 缩小 (小键盘 -) 
			meshList[i]->setScale(meshList[i]->getScale() * 0.9f);
			break;
		case GLFW_KEY_EQUAL:  // 放大 (普通键盘 +)
		case GLFW_KEY_KP_ADD:  // 放大 (小键盘 +)
			meshList[i]->setScale(meshList[i]->getScale() * 1.1f);
			break;
		case GLFW_KEY_Z:
			if (cursorMode) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				cursorMode ^= 1;
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				cursorMode ^= 1;
			}
			break;
		default:
			camera->keyboard(key, action, mode);
			break;
		}
	}

}

void cleanData() {
	// 释放内存
	
	delete camera;
	camera = NULL;

	delete light;
	light = NULL;

	painter->cleanMeshes();

	delete painter;
	painter = NULL;
	
	for (int i=0; i<meshList.size(); i++) {
		delete meshList[i];
	}
	meshList.clear();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX = 1.0*WIDTH/2; // 假设窗口宽度为 800
	static float lastY = 1.0*HEIGHT/2; // 假设窗口高度为 600
	static bool firstMouse = true;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos; // 鼠标Y方向是反的 （注意 y 坐标方向是反的，向上移动时 ypos 减小，向下增大）。

	lastX = xpos;
	lastY = ypos;

	camera->mouseMovement(xOffset, yOffset); // 调用 Camera 类的鼠标移动处理函数
}

int main(int argc, char** argv)
{
	// 初始化GLFW库，必须是应用程序调用的第一个GLFW函数
	glfwInit();

	// 配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 配置窗口属性
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, u8"2022150054_王雅贤_期末大作业", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);//绑定鼠标回调函数。Cursor光标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//将光标模式设置为“隐藏且锁定模式”


	// 调用任何OpenGL的函数之前初始化GLAD
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Init mesh, shaders, buffer
	init();
	// 输出帮助信息
	printHelp();
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		display();
		//reshape();

		// 交换颜色缓冲 以及 检查有没有触发什么事件（比如键盘输入、鼠标移动等）
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cleanData();


	return 0;
}

// 每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理。
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}