#include "MechanicalArm.h"
#include "Config.h"

#include "TriMesh.h"
#include "MeshPainter.h"
#include "Camera.h"

MechanicalArm::MechanicalArm(MeshPainter* painter,
                             Light* light,
                             Camera* camera,
                             std::vector<TriMesh*>& meshList)
    : painter_(painter)
    , light_(light)
    , camera_(camera)
    , meshList_(meshList)
{}

void MechanicalArm::init() {
    cube_ = new TriMesh();
    cube_->setNormalize(false);
    cube_->generateCube();
    cube_->setTranslation(glm::vec3(-3.0f, 0.0f, 0.0f));
    cube_->setRotation(glm::vec3(0.0f));
    cube_->setScale(glm::vec3(0.5f));

    cube_->setAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    cube_->setDiffuse(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    cube_->setSpecular(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    cube_->setShininess(1.0f);

    // 记录它在 painter 里的索引（在 addMesh 之前统计已有的数量）
    meshIndex_ = static_cast<int>(painter_->getMeshNames().size());
    painter_->addMesh(cube_, "mechanical arm", {},
                      vertexShaderPath(), fragmentShaderPath());

    meshList_.push_back(cube_);
}

void MechanicalArm::nextJoint() {
    currentJoint_ = static_cast<Joint>((currentJoint_ + 1) % NumJoints);
}

void MechanicalArm::prevJoint() {
    currentJoint_ = static_cast<Joint>((currentJoint_ - 1 + NumJoints) % NumJoints);
}

void MechanicalArm::rotateCurrentJoint(float deltaDegrees) {
    rotateJoint(currentJoint_, deltaDegrees);
}

void MechanicalArm::rotateJoint(Joint joint, float deltaDegrees) {
    theta_[joint] += deltaDegrees;
    if (theta_[joint] > 360.0f) theta_[joint] -= 360.0f;
    if (theta_[joint] < 0.0f)   theta_[joint] += 360.0f;
}

// 一节手臂 = 平移半高 + 按 长/宽/宽 缩放 的单位立方体
void MechanicalArm::drawSegment(const glm::mat4& parent, float height, float width) {
    glm::mat4 instance = glm::mat4(1.0f);
    instance = glm::translate(instance, glm::vec3(0.0f, height / 2.0f, 0.0f));
    instance = glm::scale(instance, glm::vec3(width, height, width));
    painter_->drawMesh(meshIndex_, parent * instance, light_, camera_);
}

void MechanicalArm::draw() {
    glm::mat4 modelView = glm::mat4(1.0f);
    modelView = glm::translate(modelView, glm::vec3(-3.0f, 0.0f, 0.0f));
    modelView = glm::rotate(modelView, glm::radians(theta_[Base]),
                            glm::vec3(0.0f, 1.0f, 0.0f));
    drawSegment(modelView, BASE_HEIGHT, BASE_WIDTH);

    modelView = glm::translate(modelView, glm::vec3(0.0f, BASE_HEIGHT, 0.0f));
    modelView = glm::rotate(modelView, glm::radians(theta_[UpperArm]),
                            glm::vec3(0.0f, 0.0f, 1.0f));
    drawSegment(modelView, UPPER_ARM_HEIGHT, UPPER_ARM_WIDTH);

    modelView = glm::translate(modelView, glm::vec3(0.0f, UPPER_ARM_HEIGHT, 0.0f));
    modelView = glm::rotate(modelView, glm::radians(theta_[LowerArm]),
                            glm::vec3(0.0f, 0.0f, 1.0f));
    drawSegment(modelView, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH);

    modelView = glm::translate(modelView, glm::vec3(0.0f, LOWER_ARM_HEIGHT, 0.0f));
    modelView = glm::rotate(modelView, glm::radians(theta_[Palm]),
                            glm::vec3(0.0f, 0.0f, 1.0f));
    drawSegment(modelView, PALM_HEIGHT, PALM_WIDTH);
}