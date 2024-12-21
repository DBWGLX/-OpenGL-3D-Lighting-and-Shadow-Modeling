#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Angel.h"

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix( bool isOrtho );

	glm::mat4 lookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);

	glm::mat4 ortho(const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar);

	glm::mat4 perspective(const GLfloat fovy, const GLfloat aspect,
		const GLfloat zNear, const GLfloat zFar);

	glm::mat4 frustum(const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar);


	// 每次更改相机参数后更新一下相关的数值
	void updateCamera();
	// 处理相机的键盘操作
	void keyboard(int key, int action, int mode);

	void setAspect(float a);

	// 模视矩阵
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	// 相机位置参数
	glm::vec4 eye;
	glm::vec4 at;
	glm::vec4 up;

	// 投影参数
	#undef near
	#undef far
	float near = 0.1;
	float far = 100.0;
	// 透视投影参数
	float fov = 45.0;
	float aspect = 1.0;
	// 正交投影参数
	float scale = 1.5;


	//鼠标控制 2024年12月12日 
	float yaw = -90.0f;   // upAngle偏航角，初始朝向 -Z 方向 （偏航角（Yaw）控制相机在水平平面内的旋转方向，0 度通常对应 X 轴正方向。）
	float pitch = 0.0f;   // rotateAngle俯仰角，初始水平 
	float sensitivity = 0.1f; // 鼠标灵敏度
	void mouseMovement(float xOffset, float yOffset);
	glm::vec3 front;//相机朝向
};
#endif