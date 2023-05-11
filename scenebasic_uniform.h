#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/skybox.h"
#include <GLFW/glfw3.h>

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;
    Plane plane; // plane surface
    std::vector<std::unique_ptr<ObjMesh>> meshes; // pig mesh
    SkyBox sky; //skybox
    float angle, yAngle, tPrev, rotSpeed, yRotSpeed, zoom;
    GLuint textureIDs[5];

    bool turnCameraLeft = false;
    bool turnCameraRight = false;

    bool turnCameraUp = false;
    bool turnCameraDown = false;

    bool moveCameraIn = false;
    bool moveCameraOut = false;

    bool autoCameraRotation = false;
    bool enableFog = true;
    bool enableClouds = true;

    void compile();
    void setMatrices();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void input(int key, int action);
};

#endif // SCENEBASIC_UNIFORM_H
