#include "scenebasic_uniform.h"

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f, 10.0f, 100, 100)
{
    //mesh = ObjMesh::load("../Project/media/pig_triangulated.obj", true);
    BackGround = ObjMesh::load("../Project/media/BackGround.obj", true);
}

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    float x, z;
    std::stringstream name;
    name << "Lights.Position";
    x = 2.0f * cosf((glm::two_pi<float>() / 3));
    z = 2.0f * sinf((glm::two_pi<float>() / 3));
    Shader_BlinnPhong.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z +
        1.0f, 1.0f));

    Shader_BlinnPhong.setUniform("Lights.L", vec3(1.0f, 0.0f, 0.0f));

    Shader_BlinnPhong.setUniform("Light.La", 0.8f, 0.8f, 0.8f);
}

void SceneBasic_Uniform::compile()
{
    //BlinnPhong Shader
    try {
        Shader_BlinnPhong.compileShader("shader/Shader_BlinnPhong.vert");
        Shader_BlinnPhong.compileShader("shader/Shader_BlinnPhong.frag");
        Shader_BlinnPhong.link();
        Shader_BlinnPhong.use();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    ////Toons Shader
    //try {
    //    Shader_Toon.compileShader("shader/basic_uniform.vert");
    //    Shader_Toon.compileShader("shader/basic_uniform.frag");
    //    Shader_Toon.link();
    //}
    //catch (GLSLProgramException& e)
    //{
    //    cerr << e.what() << endl;
    //    exit(EXIT_FAILURE);
    //}

    ////Texture Shader
    //try {
    //    Shader_Texture.compileShader("shader/basic_uniform.vert");
    //    Shader_Texture.compileShader("shader/basic_uniform.frag");
    //    Shader_Texture.link();
    //}
    //catch (GLSLProgramException& e)
    //{
    //    cerr << e.what() << endl;
    //    exit(EXIT_FAILURE);
    //}

}

void SceneBasic_Uniform::update(float t)
{
    
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader_BlinnPhong.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    Shader_BlinnPhong.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    Shader_BlinnPhong.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    Shader_BlinnPhong.setUniform("Material.Shininess", 180.0f);

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    BackGround->render();

    Shader_BlinnPhong.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    Shader_BlinnPhong.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    Shader_BlinnPhong.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    Shader_BlinnPhong.setUniform("Material.Shininess", 180.0f);
    model = mat4(1.0f);

    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices();
    plane.render();
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model;

    Shader_BlinnPhong.setUniform("ModelViewMatrix", mv);

    Shader_BlinnPhong.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

    Shader_BlinnPhong.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}
