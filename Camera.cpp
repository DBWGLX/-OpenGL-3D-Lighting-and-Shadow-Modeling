#include "Camera.h"

Camera::Camera() { 
	eye = glm::vec4(0.0f, 0.1f, 3.0f, 1.0f); // 默认相机位置
	updateCamera(); };
Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix()
{
	return this->lookAt(eye, at, up);
}

glm::mat4 Camera::getProjectionMatrix(bool isOrtho)
{
	if (isOrtho) {
		return this->ortho(-scale, scale, -scale, scale, this->near, this->far);
	}
	else {
		return this->perspective(fov, aspect, this->near, this->far);
	}
}

glm::mat4 Camera::lookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up)
{
	// @TODO: Task1:请按照实验课内容补全相机观察矩阵的计算
	// 获得相机方向。
	glm::vec4 n = glm::normalize(eye - at);
	// 获得右(x)轴方向。
	glm::vec3 up_3 = up;
	glm::vec3 n_3 = n;
	glm::vec4 u = glm::normalize(glm::vec4(glm::cross(up_3, n_3), 0.0));
	// 获得上(y)轴方向。
	glm::vec3 u_3 = u;
	glm::vec4 v = glm::normalize(glm::vec4(glm::cross(n_3, u_3), 0.0));

	glm::vec4 t = glm::vec4(0.0, 0.0, 0.0, 1.0);
	glm::mat4 c = glm::mat4(u, v, n, t);

	// 处理相机位置向量。
	glm::mat4 p = glm::mat4(1.0f);
	p[0].w = -(eye.x);
	p[1].w = -(eye.y);
	p[2].w = -(eye.z);

	glm::mat4 view = p * c;
	return view;					// 计算最后需要沿-eye方向平移
}

glm::mat4 Camera::ortho(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	// @TODO: Task2:请按照实验课内容补全正交投影矩阵的计算
	glm::mat4 c = glm::mat4(1.0f);
	c[0][0] = 2.0 / (right - left);
	c[1][1] = 2.0 / (top - bottom);
	c[2][2] = -2.0 / (zFar - zNear);
	c[3][3] = 1.0;
	c[0][3] = -(right + left) / (right - left);
	c[1][3] = -(top + bottom) / (top - bottom);
	c[2][3] = -(zFar + zNear) / (zFar - zNear);
	return c;
}

glm::mat4 Camera::perspective(const GLfloat fovy, const GLfloat aspect,
	const GLfloat zNear, const GLfloat zFar)
{
	// @TODO: Task2:请按照实验课内容补全透视投影矩阵的计算
	GLfloat top = tan(fovy * M_PI / 180 / 2) * zNear;
	GLfloat right = top * aspect;

	glm::mat4 c = glm::mat4(1.0f);;
	c[0][0] = zNear / right;
	c[1][1] = zNear / top;
	c[2][2] = -(zFar + zNear) / (zFar - zNear);
	c[2][3] = -(2.0 * zFar * zNear) / (zFar - zNear);
	c[3][2] = -1.0;
	c[3][3] = 0.0;
	return c;
}

glm::mat4 Camera::frustum(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	// 任意视锥体矩阵
	glm::mat4 c = glm::mat4(1.0f);;
	c[0][0] = 2.0 * zNear / (right - left);
	c[0][2] = (right + left) / (right - left);
	c[1][1] = 2.0 * zNear / (top - bottom);
	c[1][2] = (top + bottom) / (top - bottom);
	c[2][2] = -(zFar + zNear) / (zFar - zNear);
	c[2][3] = -2.0 * zFar * zNear / (zFar - zNear);
	c[3][2] = -1.0;
	c[3][3] = 0.0;
	return c;
}


void Camera::updateCamera()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(front);// 存储方向向量

	//eye = glm::vec4(eye, 1.0);
	at = eye + glm::vec4(this->front, .0); // 朝向方向
	up = glm::vec4(0.0, 1.0, 0.0, 0.0); // 固定世界坐标系上方向
}


void Camera::keyboard(int key, int action, int mode)
{
	// 控制移动速度
	float velocity = 0.1f;

	// 按键判断
	if (key == GLFW_KEY_W ) {
		eye += glm::vec4(front * velocity, 0.0f); // 向前
	}
	else if (key == GLFW_KEY_S ) {
		eye -= glm::vec4(front * velocity, 0.0f); // 向后
	}
	else if (key == GLFW_KEY_A ) {
		eye -= glm::vec4(glm::normalize(glm::cross(glm::vec3(front), glm::vec3(up))) * velocity, 0.0f); // 向左
	}
	else if (key == GLFW_KEY_D) {
		eye += glm::vec4(glm::normalize(glm::cross(glm::vec3(front), glm::vec3(up))) * velocity, 0.0f); // 向右
	}

	// 空格键提升高度
	else if (key == GLFW_KEY_SPACE ) {
		eye.y += velocity; // 向上
	}

	// Shift 键降低高度
	else if (key == GLFW_KEY_LEFT_SHIFT ) {
		eye.y -= velocity; // 向下
	}

	// 初始化相机所有参数
	else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		eye = glm::vec4(0.0, 0.0, 3.0, 1.0);
		yaw = -90.0f;
		pitch = 0.0f;
	}

}


//2024年12月12日
void Camera::mouseMovement(float xOffset, float yOffset)
{
	xOffset *= sensitivity;//灵敏度控制 
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	// 限制俯仰角范围，避免翻转
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// 更新摄像机位置和方向
	updateCamera();
}

void Camera::setAspect(float a) {
	aspect = a;
}
