#include "Angel.h" // 项目封装的 OpenGL/GLFW/GLAD 等基础头文件
#include "TriMesh.h"
#include "Camera.h"
#include "MeshPainter.h"
#include "ModelInitializer.h"
#include "MechanicalArm.h"
#include "Config.h"

#include <vector>
#include <string>

int cursorMode = 1;

Camera*      camera  = new Camera();
Light*       light   = new Light();

std::vector<TriMesh*> meshList;
MeshPainter* painter = new MeshPainter();

int i = 0;

ModelInitializer* modelInit = nullptr;
MechanicalArm*    arm       = nullptr;

// ========== 初始化 ==========
void init() {
    arm = new MechanicalArm(painter, light, camera, meshList);
    arm->init();

    modelInit = new ModelInitializer(painter, meshList, light, camera);
    modelInit->initAll();
}

// ========== 渲染 ==========
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // glClear：清除颜色缓冲和深度缓冲

    painter->drawMeshes(light, camera);
    arm->draw();
}

// ========== 帮助信息 ==========
void printHelp() {
    //
}

// ========== 键盘回调 ==========
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    auto moveLightAndSphere = [&](const glm::vec3& delta) {
        light->setTranslation(light->getTranslation() + delta);
        if (modelInit && modelInit->getLightSphere()) {
            modelInit->getLightSphere()->setTranslation(light->getTranslation());
        }
    };

    switch (key) {
    case GLFW_KEY_ESCAPE: exit(EXIT_SUCCESS); break;
    case GLFW_KEY_H: printHelp(); break;

    case GLFW_KEY_1: i = (i + 1 + meshList.size()) % meshList.size(); break;
    case GLFW_KEY_2: i = (i - 1 + meshList.size()) % meshList.size(); break;

    // ---- 机械臂关节：全部转发给 MechanicalArm ----
    case GLFW_KEY_3: arm->nextJoint();                  break;
    case GLFW_KEY_4: arm->prevJoint();                  break;
    case GLFW_KEY_5: arm->rotateCurrentJoint(+5.0f);    break;
    case GLFW_KEY_6: arm->rotateCurrentJoint(-5.0f);    break;

    case GLFW_KEY_I: moveLightAndSphere(glm::vec3(0.0, 0.0, 0.5)); break;
    case GLFW_KEY_K: moveLightAndSphere(glm::vec3(0.0, 0.0, -0.5)); break;
    case GLFW_KEY_J: moveLightAndSphere(glm::vec3(-0.5, 0.0, 0.0)); break;
    case GLFW_KEY_L: moveLightAndSphere(glm::vec3(0.5, 0.0, 0.0)); break;
    case GLFW_KEY_U: moveLightAndSphere(glm::vec3(0.0, 0.5, 0.0)); break;
    case GLFW_KEY_O: moveLightAndSphere(glm::vec3(0.0, -0.5, 0.0)); break;

    case GLFW_KEY_UP:
        meshList[i]->setTranslation(meshList[i]->getTranslation() + glm::vec3(0.0, 0.0, 0.1));
        break;
    case GLFW_KEY_DOWN:
        meshList[i]->setTranslation(meshList[i]->getTranslation() - glm::vec3(0.0, 0.0, 0.1));
        break;
    case GLFW_KEY_LEFT:
        meshList[i]->setTranslation(meshList[i]->getTranslation() - glm::vec3(0.1, 0.0, 0.0));
        break;
    case GLFW_KEY_RIGHT:
        meshList[i]->setTranslation(meshList[i]->getTranslation() + glm::vec3(0.1, 0.0, 0.0));
        break;

    case GLFW_KEY_KP_1:
        meshList[i]->setTranslation(meshList[i]->getTranslation() + glm::vec3(0.0, 0.1, 0.0));
        break;
    case GLFW_KEY_KP_2:
        meshList[i]->setTranslation(meshList[i]->getTranslation() - glm::vec3(0.0, 0.1, 0.0));
        break;
    case GLFW_KEY_KP_8:
        meshList[i]->setRotation(meshList[i]->getRotation() + glm::vec3(5.0, 0.0, 0.0));
        break;
    case GLFW_KEY_KP_5:
        meshList[i]->setRotation(meshList[i]->getRotation() - glm::vec3(5.0, 0.0, 0.0));
        break;

    case GLFW_KEY_KP_0:
        for (size_t k = 0; k + 1 < meshList.size(); ++k) {
            meshList[k]->setRotation(meshList[k]->getRotation() + glm::vec3(0.0, 5.0, 0.0));
        }
        arm->rotateJoint(MechanicalArm::Base, 5.0f);
        break;

    case GLFW_KEY_KP_4:
        meshList[i]->setRotation(meshList[i]->getRotation() + glm::vec3(0.0, 5.0, 0.0));
        break;
    case GLFW_KEY_KP_6:
        meshList[i]->setRotation(meshList[i]->getRotation() - glm::vec3(0.0, 5.0, 0.0));
        break;

    case GLFW_KEY_MINUS:
    case GLFW_KEY_KP_SUBTRACT:
        meshList[i]->setScale(meshList[i]->getScale() * 0.9f);
        break;
    case GLFW_KEY_EQUAL:
    case GLFW_KEY_KP_ADD:
        meshList[i]->setScale(meshList[i]->getScale() * 1.1f);
        break;

    case GLFW_KEY_Z:
        if (cursorMode) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // glfwSetInputMode：设置光标为正常模式
        else            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // glfwSetInputMode：隐藏并锁定光标，用于鼠标控制视角
        cursorMode ^= 1;
        break;

    default:
        camera->keyboard(key, action, mode);
        break;
    }
}

// ========== 资源释放 ==========
void cleanData() {
    delete camera;   camera = nullptr;
    delete light;    light = nullptr;

    painter->cleanMeshes();
    delete painter;  painter = nullptr;

    for (auto* m : meshList) delete m;
    meshList.clear();

    delete modelInit; modelInit = nullptr;
    delete arm;       arm = nullptr;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // glViewport：设置 OpenGL 渲染视口为窗口新尺寸
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = 1.0f * WINDOW_WIDTH / 2;
    static float lastY = 1.0f * WINDOW_HEIGHT / 2;
    static bool  firstMouse = true;

    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xOffset = static_cast<float>(xpos) - lastX;
    float yOffset = lastY - static_cast<float>(ypos);
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera->mouseMovement(xOffset, yOffset);
}

int main(int argc, char** argv) {
    glfwInit(); // glfwInit：初始化 GLFW 库
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // glfwWindowHint：设置 OpenGL 主版本号为 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // glfwWindowHint：设置 OpenGL 次版本号为 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // glfwWindowHint：设置 OpenGL 为核心模式
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // glfwWindowHint：macOS 下启用前向兼容
#endif

    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT,
        u8"2022150054_王雅贤_期末大作业", nullptr, nullptr); // glfwCreateWindow：创建 GLFW 窗口和 OpenGL 上下文
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // glfwTerminate：终止 GLFW，释放相关资源
        return -1;
    }
    glfwMakeContextCurrent(window); // glfwMakeContextCurrent：将窗口的 OpenGL 上下文设为当前上下文
    glfwSetKeyCallback(window, key_callback); // glfwSetKeyCallback：注册键盘按键回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // glfwSetFramebufferSizeCallback：注册帧缓冲大小变化回调
    glfwSetCursorPosCallback(window, mouse_callback); // glfwSetCursorPosCallback：注册鼠标位置回调
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // glfwSetInputMode：隐藏并锁定鼠标光标

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // gladLoadGLLoader：加载 OpenGL 函数指针；glfwGetProcAddress：获取 GLFW 提供的函数地址
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    init();
    printHelp();
    glEnable(GL_DEPTH_TEST); // glEnable：启用深度测试

    while (!glfwWindowShouldClose(window)) { // glfwWindowShouldClose：检查窗口是否收到关闭请求
        display();
        glfwSwapBuffers(window); // glfwSwapBuffers：交换前后缓冲区，显示渲染结果
        glfwPollEvents(); // glfwPollEvents：处理窗口和输入事件
    }

    cleanData();
    return 0;
}