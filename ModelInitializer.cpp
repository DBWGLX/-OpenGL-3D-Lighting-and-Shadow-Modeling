#include "ModelInitializer.h"
#include "Config.h"

#include "Angel.h"
#include "TriMesh.h"
#include "Camera.h"
#include "MeshPainter.h"

ModelInitializer::ModelInitializer(MeshPainter* painter,
                                   std::vector<TriMesh*>& meshList,
                                   Light* light,
                                   Camera* camera)
    : painter_(painter)
    , meshList_(meshList)
    , light_(light)
    , camera_(camera)
{}

void ModelInitializer::initAll() {
    loadShaderPaths();
    configureLight();

    initLightSphere();
    initTable();
    initWawa();
    initSaber();
    initSaber2();
    initCow();
    initCube2();
    initBackground();
    initBackground2();
    initGround();
}

void ModelInitializer::loadShaderPaths() {
#ifdef __APPLE__
    vshader_ = "shaders/vshader_mac.glsl";
    fshader_ = "shaders/fshader_mac.glsl";
#else
    vshader_ = "shaders/vshader_win.glsl";
    fshader_ = "shaders/fshader_win.glsl";
#endif
}

void ModelInitializer::configureLight() {
    light_->setTranslation(glm::vec3(5, 5, 5));
    light_->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0));
    light_->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0));
    light_->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0));
    light_->setAttenuation(1.0, 0.045, 0.0075);

    camera_->setAspect(1.0 * WINDOW_WIDTH / WINDOW_HEIGHT);
}

void ModelInitializer::initLightSphere() {
    lightSphere_ = new TriMesh();
    lightSphere_->readOff("./assets/off/sphere.off");
    lightSphere_->setNormalize(true);
    lightSphere_->storeFacesPoints();
    lightSphere_->setTranslation(light_->getTranslation());
    lightSphere_->setRotation(glm::vec3(-90, 0, 0));
    lightSphere_->setScale(glm::vec3(1.0, 1.0, 1));
    lightSphere_->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
    lightSphere_->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0));
    lightSphere_->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0));
    lightSphere_->setShininess(1.0);
    lightSphere_->setShadowCaster(false);
    painter_->addMesh(lightSphere_, "light_sphere", {}, vshader_, fshader_);
    meshList_.push_back(lightSphere_);
}


void ModelInitializer::initTable() {
    TriMesh* table = new TriMesh();
    table->setNormalize(true);
    table->readObj("./assets/table.obj");
    table->storeFacesPoints();
    table->setTranslation(glm::vec3(-2, 0.22, 0.0));
    table->setRotation(glm::vec3(-90, 0.0, 0.0));
    table->setScale(glm::vec3(1.0, 1.0, 1.0));
    table->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
    table->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0));
    table->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0));
    table->setShininess(1.0);
    painter_->addMesh(table, "table", { "./assets/table.png" }, vshader_, fshader_);
    meshList_.push_back(table);
}

void ModelInitializer::initWawa() {
    TriMesh* wawa = new TriMesh();
    wawa->setNormalize(true);
    wawa->readObj("./assets/wawa.obj");
    wawa->storeFacesPoints();
    wawa->setTranslation(glm::vec3(-1, 0.37, 0));
    wawa->setRotation(glm::vec3(-90, 0, 0));
    wawa->setScale(glm::vec3(1.0, 1.0, 1));
    wawa->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
    wawa->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0));
    wawa->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0));
    wawa->setShininess(1.0);
    painter_->addMesh(wawa, "wawa", { "./assets/wawa.png" }, vshader_, fshader_);
    meshList_.push_back(wawa);
}

void ModelInitializer::initSaber() {
    TriMesh* saber = new TriMesh();
    saber->setNormalize(true);
    saber->readObj("./assets/saber/A pose.obj");
    saber->reverse_vertex_normals();
    saber->storeFacesPoints();
    saber->setTranslation(glm::vec3(0, 0.36, 0));
    saber->setRotation(glm::vec3(0, 0, 0));
    saber->setScale(glm::vec3(1.0, 1.0, 1));
    saber->setAmbient(glm::vec4(0.3, 0.3, 0.3, 1.0));
    saber->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0));
    saber->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0));
    saber->setShininess(50.0);
    painter_->addMesh(saber, "saber",
                      { "./assets/saber/textures/uv_cha.tga.png" },
                      vshader_, fshader_);
    meshList_.push_back(saber);
}

void ModelInitializer::initSaber2() {
    TriMesh* saber2 = new TriMesh();
    saber2->setNormalize(true);
    saber2->readObj("./assets/saber2/Saber_Facials_Finished.obj");
    saber2->storeFacesPoints();
    saber2->setTranslation(glm::vec3(1, 0.25, 0));
    saber2->setRotation(glm::vec3(0, 25, 0));
    saber2->setScale(glm::vec3(1.0, 1.0, 1));
    saber2->setAmbient(glm::vec4(0.3, 0.3, 0.3, 1.0));
    saber2->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0));
    saber2->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0));
    saber2->setShininess(50.0);
    painter_->addMesh(saber2, "saber2",
                      { "./assets/saber2/textures/svt_0001_s01_SVT_0001_S01_clothall.png" },
                      vshader_, fshader_);
    meshList_.push_back(saber2);
}

void ModelInitializer::initCow() {
    TriMesh* cow = new TriMesh();
    cow->setNormalize(true);
    cow->readOff("./assets/off/cow.off");
    cow->storeFacesPoints();
    cow->setTranslation(glm::vec3(2, 0.25, 0));
    cow->setRotation(glm::vec3(0, -90, 0));
    cow->setScale(glm::vec3(1.0, 1.0, 1));
    cow->setAmbient(glm::vec4(0.8, 0.8, 0.8, 1.0));
    cow->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0));
    cow->setSpecular(glm::vec4(0.5, 0.5, 0.5, 1.0));
    cow->setShininess(20.0);
    painter_->addMesh(cow, "cow", {}, vshader_, fshader_);
    meshList_.push_back(cow);
}

void ModelInitializer::initCube2() {
    TriMesh* cube2 = new TriMesh();
    cube2->setNormalize(true);
    cube2->generateCube_36();
    cube2->setTranslation(glm::vec3(3, 0.3, 0));
    cube2->setRotation(glm::vec3(-90, 0, 0));
    cube2->setScale(glm::vec3(0.5, 0.5, 0.5));
    cube2->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
    cube2->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0));
    cube2->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0));
    cube2->setShininess(1.0);
    painter_->addMesh(cube2, "cube2", {}, vshader_, fshader_);
    meshList_.push_back(cube2);
}

void ModelInitializer::initBackground() {
    TriMesh* background = new TriMesh();
    background->setNormalize(true);
    background->generateSquare(glm::vec3(0.5, 0.5, 0.5));
    background->setScale(glm::vec3(15.0, 15.0, 15));
    background->setTranslation(glm::vec3(0, 3, -8));
    background->setRotation(glm::vec3(0, 0, 0));
    background->setShadowCaster(false);
    background->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
    background->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0));
    background->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0));
    background->setShininess(1.0);
    painter_->addMesh(background, "background",
                      { "./assets/InstructionManual.png" },
                      vshader_, fshader_);
    meshList_.push_back(background);
}

void ModelInitializer::initBackground2() {
    TriMesh* background2 = new TriMesh();
    background2->setNormalize(true);
    background2->generateSquare(glm::vec3(0.5, 0.5, 0.5));
    background2->setScale(glm::vec3(10.0, 10.0, 10));
    background2->setTranslation(glm::vec3(0, 3, 10));
    background2->setRotation(glm::vec3(0, 180, 0));
    background2->setShadowCaster(false);
    background2->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
    background2->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0));
    background2->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0));
    background2->setShininess(1.0);
    painter_->addMesh(background2, "background2",
                      { "./assets/saber.png" },
                      vshader_, fshader_);
    meshList_.push_back(background2);
}

void ModelInitializer::initGround() {
    TriMesh* ground = new TriMesh();
    ground->setNormalize(true);
    ground->generateSquare(glm::vec3(0.5, 0.5, 0.5));
    ground->setScale(glm::vec3(15.0, 15.0, 1));
    ground->setTranslation(glm::vec3(0, -0.001, 0));
    ground->setRotation(glm::vec3(-90, 0, 0));
    ground->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
    ground->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0));
    ground->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0));
    ground->setShininess(1.0);
    ground->setShadowCaster(false);
    painter_->addMesh(ground, "ground", { "./assets/ground.png" },
                      vshader_, fshader_);
    meshList_.push_back(ground);

    glClearColor(1.0, 1.0, 1.0, 1.0);
}