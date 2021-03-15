#include "scenebasic_uniform.h"

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::mat4;
using glm::vec4;
using glm::mat3;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(50.0f, 50.0f, 1, 1)
{
    mesh = ObjMesh::load("../Project/media/Truck.obj", true);
}

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);

    view = glm::lookAt(vec3(2.0f, 2.0f, 2.0f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f,
        1.0f, 0.0f));
    


    //LIGHT SETTINGS

    //Indiveidual lights.
    projection = mat4(1.0f);
    Shader_BlinnPhong.setUniform("Spot.L", vec3(0.0f, 0.9f, 0.0f));
    Shader_BlinnPhong.setUniform("Spot.La", vec3(0.9f));
    Shader_BlinnPhong.setUniform("Spot.Exponent", 100.0f); //Blurry Around Circle
    Shader_BlinnPhong.setUniform("Spot.CutOff", glm::radians(15.0f));  //Size of circle


    //Spotlight lights.
    projection = mat4(1.0f);
    float x, z;
    std::stringstream name;
    name << "Lights.Position";
    x = 2.0f * cosf((glm::two_pi<float>() / 3));
    z = 2.0f * sinf((glm::two_pi<float>() / 3));
    Shader_BlinnPhong.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z +
        1.0f, 1.0f));

    Shader_BlinnPhong.setUniform("Lights.L", vec3(1.0f, 0.0f, 0.0f));

    Shader_BlinnPhong.setUniform("Lights.La", 0.8f, 0.8f, 0.8f);

    GLuint texID = Texture::loadTexture("../Project/media/texture/Truck.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
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
}

void SceneBasic_Uniform::update(float t)
{
    
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Directional Light for Moon, Light Shading Etc....
    //Point Light, Car Light heading Outwards.
    //SpotLight, Lamp Post, Will Fade in/out....
    //Skybox, jungle type thing...
    //Some sort of moving light somehow?




    //Spot Light Settings
    vec4 lightPos = vec4(0.0f, 10.0f, 0.0f, 1.0f);
    Shader_BlinnPhong.setUniform("Spot.Position", vec3(view * lightPos));
    mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
    Shader_BlinnPhong.setUniform("Spot.Direction", normalMatrix * vec3(-lightPos));

    //MESH RENDERING
    Shader_BlinnPhong.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    Shader_BlinnPhong.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    Shader_BlinnPhong.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    Shader_BlinnPhong.setUniform("Material.Shininess", 180.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.5f, -0.5f));
    model = glm::scale(model, vec3(0.02f, 0.02f, 0.02f));
    //model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    mesh->render();

    ////PLANE SETTINGS
    Shader_BlinnPhong.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    Shader_BlinnPhong.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    Shader_BlinnPhong.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
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
