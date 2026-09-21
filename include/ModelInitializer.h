#pragma once

#include <string>
#include <vector>

class Camera;
class Light;
class MeshPainter;
class TriMesh;

// 统一负责场景中所有模型与光源的初始化
class ModelInitializer {
public:
    ModelInitializer(MeshPainter* painter,
                     std::vector<TriMesh*>& meshList,
                     Light* light,
                     Camera* camera);

    // 加载所有模型（包含加载着色器路径、光源配置）
    void initAll();

    // 光源小球指针（键盘交互时需要同步移动）
    TriMesh* getLightSphere() const { return lightSphere_; }

private:
    void loadShaderPaths();

    void configureLight();
    void initLightSphere();
    
    void initMechanicalArmCube();
    void initTable();
    void initWawa();
    void initSaber();
    void initSaber2();
    void initCow();
    void initCube2();
    void initBackground();
    void initBackground2();
    void initGround();

private:
    MeshPainter* painter_;
    std::vector<TriMesh*>& meshList_;
    Light* light_;
    Camera* camera_;

    std::string vshader_;
    std::string fshader_;

    TriMesh* lightSphere_ = nullptr;
};