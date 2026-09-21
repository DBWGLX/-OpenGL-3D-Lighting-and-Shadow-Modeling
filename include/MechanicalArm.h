#pragma once

#include "Angel.h"

#include <vector>

class TriMesh;
class MeshPainter;
class Light;
class Camera;

// 四自由度机械臂：底座 + 大臂 + 小臂 + 手掌
// 负责网格创建、关节状态、分层绘制、关节按键响应
class MechanicalArm {
public:
    enum Joint {
        Base = 0,
        UpperArm,
        LowerArm,
        Palm,
        NumJoints
    };

    MechanicalArm(MeshPainter* painter,
                  Light* light,
                  Camera* camera,
                  std::vector<TriMesh*>& meshList);

    // 创建并注册立方体网格（只调用一次）
    void init();

    // 每帧分层绘制整条机械臂
    void draw();

    // ---- 关节控制 ----
    void nextJoint();
    void prevJoint();
    void rotateCurrentJoint(float deltaDegrees);
    void rotateJoint(Joint joint, float deltaDegrees);

    // 供 UI / 调试
    Joint currentJoint() const { return currentJoint_; }
    float jointAngle(Joint joint) const { return theta_[joint]; }

private:
    void drawSegment(const glm::mat4& parent, float height, float width);

private:
    MeshPainter* painter_;
    Light* light_;
    Camera* camera_;
    std::vector<TriMesh*>& meshList_;

    TriMesh* cube_ = nullptr;
    int meshIndex_ = -1;               // painter 中的索引，用于分层绘制

    Joint currentJoint_ = Base;
    GLfloat theta_[NumJoints] = { 0.0f };

    // 尺寸参数
    static constexpr GLfloat BASE_HEIGHT      = 0.2f;
    static constexpr GLfloat BASE_WIDTH       = 0.5f;
    static constexpr GLfloat UPPER_ARM_HEIGHT = 0.5f;
    static constexpr GLfloat UPPER_ARM_WIDTH  = 0.12f;
    static constexpr GLfloat LOWER_ARM_HEIGHT = 0.4f;
    static constexpr GLfloat LOWER_ARM_WIDTH  = 0.1f;
    static constexpr GLfloat PALM_HEIGHT      = 0.05f;
    static constexpr GLfloat PALM_WIDTH       = 0.2f;
};