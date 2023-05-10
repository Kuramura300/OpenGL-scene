#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include <sstream>

#include "helper/glutils.h"
#include "helper/texture.h"
#include "helper/noisetex.h"

using glm::vec3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f, 10.0f, 100, 100), angle(0.0f), tPrev(0.0f),
rotSpeed(glm::pi<float>() / 28.0f), sky(100.0f)
{
    // Load models from file
    meshes.push_back(ObjMesh::load("../Project_Template/media/charactermodel.obj", true));
    meshes.push_back(ObjMesh::load("../Project_Template/media/charactermodel2.obj", true));
}

void SceneBasic_Uniform::initScene()
{
    compile();

    glEnable(GL_DEPTH_TEST);

    // Setup camera view
    view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.25f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    // Set initial angle
    angle = glm::radians(0.0f);

    // Setup lights
    float x, z;
    for (int i = 0; i < 3; i++)
    {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z +
            1.0f, 1.0f));
    }

    // Set diffuse and specular light intensity uniforms
    prog.setUniform("lights[0].L", vec3(0.0f, 0.0f, 0.8f));
    prog.setUniform("lights[1].L", vec3(0.0f, 0.8f, 0.0f));
    prog.setUniform("lights[2].L", vec3(0.8f, 0.0f, 0.0f));

    // Set ambient light uniforms
    prog.setUniform("lights[0].La", vec3(0.2f, 0.2f, 0.2f));
    prog.setUniform("lights[1].La", vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("lights[2].La", vec3(0.3f, 0.3f, 0.3f));

    //Set fog uniforms
    prog.setUniform("Fog.MaxDist", 120.0f);
    prog.setUniform("Fog.MinDist", 1.0f);
    prog.setUniform("Fog.Colour", vec3(1.0f, 1.0f, 1.0f));

    // Load textures
    textureIDs[0] = Texture::loadCubeMap("../Project_Template/media/texture/cube/skybox/sky");
    textureIDs[1] = Texture::loadTexture("../Project_Template/media/texture/BlueGradient.png");
    textureIDs[2] = Texture::loadTexture("../Project_Template/media/texture/BlackGradient.png");
    textureIDs[3] = Texture::loadTexture("../Project_Template/media/texture/meadow.jpg");
    textureIDs[4] = NoiseTex::generate2DTex(6.0f);
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    float deltaT = t - tPrev;

    if (tPrev == 0.0f) deltaT = 0.0f;

    tPrev = t;

    angle += rotSpeed * deltaT;

    if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Camera
    vec3 cameraPos = vec3(3.0f * cos(angle), 1.0f, 3.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    // Skybox
    prog.use();
    model = mat4(1.0f);
    setMatrices();

    // Activate and bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureIDs[0]);

    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureIDs[4]);

    sky.render();

    // Draw first character
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);

    prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 180.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0, 0.2f, -0.5f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    setMatrices();
    meshes[0]->render();

    // Draw second character
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-0.45f, 0, -0.5f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    setMatrices();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIDs[2]);

    meshes[1]->render();

    // Draw plane
    prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 180.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIDs[3]);

    plane.render();
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv(view * model);

    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),
        vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
    projection = glm::perspective(glm::radians(70.0f), (float)w / h,
        0.3f, 100.0f);

}
