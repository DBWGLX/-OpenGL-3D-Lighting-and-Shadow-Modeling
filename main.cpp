#include "Angel.h"
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        if (cursorMode) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    glViewport(0, 0, width, height);
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
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT,
        u8"2022150054_王雅贤_期末大作业", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    init();
    printHelp();
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanData();
    return 0;
}

