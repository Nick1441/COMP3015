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

int OverallLoad = 1;
bool Scene1Toggle = true;
bool Scene4Toggle = false;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f, 10.0f, 100, 100), angle3(0.0f), angle(0.0f), tPrev(0.0f),
rotSpeed(glm::pi<float>() / 8.0f),
sky(100.0f)
{
    CarModel = ObjMesh::load("../Project/media/SportsCar.obj", true);
    CarModelNormal = ObjMesh::load("../Project/media/SportsCar.obj", false, true);
}

void SceneBasic_Uniform::initScene()
{
    //vec4 lightPos = vec4(5.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
    //Shader_4.setUniform("Light.Position", view * lightPos);
    compile();
    glEnable(GL_DEPTH_TEST);

    //view = glm::lookAt(vec3(-2.5f, 2.f, 2.0f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f,
    //    1.0f, 0.0f));
    

    projection = mat4(1.0f);
    angle3 = glm::radians(90.0f); //set the initial angle
    //Shader_3.setUniform("Light[0].Position", vec3(100.0f, 1.0f, 1.0f));
    ////LIGHT SETTINGS

    ////Spot lights.
    //projection = mat4(1.0f);
    //Shader_BlinnPhong.setUniform("Spot.L", vec3(0.0f, 0.0f, 0.9f));
    //Shader_BlinnPhong.setUniform("Spot.La", vec3(0.2f));
    //Shader_BlinnPhong.setUniform("Spot.Exponent", 10.0f); //Blurry Around Circle
    //Shader_BlinnPhong.setUniform("Spot.CutOff", glm::radians(5.0f));  //Size of circle
    ////Spotlight lights.
    //projection = mat4(1.0f);
    //float x, z;
    //std::stringstream name;
    //name << "Lights.Position";
    //x = 2.0f * cosf((glm::two_pi<float>() / 3));
    //z = 2.0f * sinf((glm::two_pi<float>() / 3));
    //Shader_BlinnPhong.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z +
    //    1.0f, 1.0f));

    //Shader_BlinnPhong.setUniform("Lights.L", vec3(1.0f, 1.0f, 1.0f));

    //Shader_BlinnPhong.setUniform("Lights.La", 0.8f, 0.8f, 0.8f);

    GLuint ID1 = Texture::loadTexture("media/texture/skylineColor.png");
    GLuint ID2 = Texture::loadTexture("media/texture/skylineNormal.png");
    GLuint ID3 = Texture::loadTexture("media/texture/moss.png");
    GLuint ID4 = Texture::loadTexture("media/texture/skylineColor2.png");
    GLuint ID5 = Texture::loadTexture("media/texture/skylineColor3.png");

    GLuint cubeTex = Texture::loadCubeMap("media/texture/cube/Skybox/skybox2");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ID1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ID2);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, ID3);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ID4);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, ID5);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

}

void SceneBasic_Uniform::compile()
{
    //Scenario 1 Shader
    try {
        Shader_1.compileShader("shader/Scenario1.vert");
        Shader_1.compileShader("shader/Scenario1.frag");
        Shader_1.link();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    //Scenario 2 Shader
    try {
        Shader_2.compileShader("shader/Scenario2.vert");
        Shader_2.compileShader("shader/Scenario2.frag");
        Shader_2.link();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    //Scenario 3 Shader
    try {
        Shader_3.compileShader("shader/Scenario3.vert");
        Shader_3.compileShader("shader/Scenario3.frag");
        Shader_3.link();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    //Scenario 4 Shader
    try {
        Shader_4.compileShader("shader/Scenario4.vert");
        Shader_4.compileShader("shader/Scenario4.frag");
        Shader_4.link();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    //Scenario 5 Shader
    try {
        Shader_5.compileShader("shader/Scenario5.vert");
        Shader_5.compileShader("shader/Scenario5.frag");
        Shader_5.link();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    //SkyBox Shader
    try {
        SkyBox.compileShader("shader/SkyBox.vert");
        SkyBox.compileShader("shader/SkyBox.frag");
        SkyBox.link();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    //Text Shader
    try {
        Text.compileShader("shader/TextShader.vert");
        Text.compileShader("shader/TextShader.frag");
        Text.link();
    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::update(float t)
{
    if (m_animate) {
        angle += 0.05f;
        if (angle >= 360.0f)
            angle -= 360.0f;
    }

    if (m_animate) {
        angle5 += 0.015f;
        if (angle5 >= 360.0f)
            angle5 -= 360.0f;
    }

    //USED FOR SKYBOX ROTATION
    float deltaT2 = t - tPrev;
    if (tPrev == 0.0f)
    {
        deltaT2 = 0.0f;
    }

    tPrev = t;
    angle3 += rotSpeed * deltaT2;

    if (angle3 > glm::two_pi<float>())
    {
        angle3 -= glm::two_pi<float>();
    }


    //
    //COLOUR CHANGE
        float timeValue = t;
    Value = sin(timeValue) / 4.0f + 0.5f;
    Value2 = sin(timeValue) / 4.0f + 0.25f;
    Value3 = sin(timeValue) / 4.0f + 0.75f;
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (OverallLoad == 1)
    {
        Scenario_1();
        RenderSkyBox();
    }
    else if (OverallLoad == 2)
    {
        Scenario_2();
        RenderSkyBox();
    }
    else if (OverallLoad == 3)
    {
        Scenario_3();
        RenderSkyBox();
    }
    else if (OverallLoad == 4)
    {
        Scenario_4();
        RenderSkyBox();
    }
    else if (OverallLoad == 5)
    {
        Scenario_5();
        RenderSkyBox();
    }


}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model;

    Shader_1.setUniform("ModelViewMatrix", mv);
    Shader_1.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    Shader_1.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::setMatrices2()
{
    mat4 mv = view * model;

    Shader_2.setUniform("ModelViewMatrix", mv);
    Shader_2.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    Shader_2.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::setMatrices3()
{
    mat4 mv = view * model;

    Shader_3.setUniform("ModelViewMatrix", mv);
    Shader_3.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    Shader_3.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::setMatrices4()
{
    mat4 mv = view * model;

    Shader_4.setUniform("ModelViewMatrix", mv);
    Shader_4.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    Shader_4.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::setMatrices5()
{
    mat4 mv = view * model;

    Shader_5.setUniform("ModelViewMatrix", mv);
    Shader_5.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    Shader_5.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::setMatricesSky()
{
    mat4 mv = view * model;

    SkyBox.setUniform("ModelViewMatrix", mv);
    SkyBox.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    SkyBox.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void  SceneBasic_Uniform::InputPressed(int num)
{
    //KEYBOARD CLICKS, WILL RENDER DIFFERNT ITEMS ON DIFFERNT CLICKS
    if (num < 7)
    {
        OverallLoad = num;
    }
    

    //Scene 1 Camera Movement Toggle
    if (OverallLoad == 1 && num == 10)
    {
        Scene1Toggle = false;
    }
    else if (OverallLoad == 4 && num == 10)
    {
        Scene4Toggle = false;
    }

    if (OverallLoad == 1 && num == 11)
    {
        Scene1Toggle = true;
    }
    else if (OverallLoad == 4 && num == 11)
    {
        Scene4Toggle = true;
    }
}

void  SceneBasic_Uniform::Scenario_1()
{
    //
    // - Normals
    // - BlinnPhong
    // - Moving Lights.
    // - Fog
    //

    vec3 cameraPos = vec3(0.0f);
    if (Scene1Toggle)
    {
        cameraPos = vec3(7.0f * cos(angle3), 2.0f, 7.0f * sin(angle3));
        view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f,
            0.0f));
    }
    else
    {
        cameraPos = vec3(7.0f * cos(90), 2.0f, 7.0f * sin(90));
        view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f,
            0.0f));
    }

    //Setting Shader To Use
    Shader_1.use();

    //Setting Lights Positions
    vec4 lightPos = vec4(10.0f * cos(angle), 20.0f, 10.0f * sin(angle), 6.0f);
    Shader_1.setUniform("Light.Position", view * lightPos);
    Shader_1.setUniform("Light.La", vec3(0.1f, 0.1f, 0.1f));
    Shader_1.setUniform("Light.L", vec3(1.0f, 1.0f, 1.0f));

    Shader_1.setUniform("Material.Ks", 0.2f, 0.2f, 0.2f);
    Shader_1.setUniform("Material.Kd", 0.5f, 0.5f, 0.5f);
    Shader_1.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    Shader_1.setUniform("Material.Shininess", 300.0f);

    Shader_1.setUniform("Pass", 1);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    //model = glm::rotate(model, glm::radians((float)angle * 6), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    plane.render();

    Shader_1.setUniform("Pass", 0);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(-2.0f, -0.45f, 0.0f));
    model = glm::scale(model, vec3(0.8f, 0.8f, 0.8f));
    //model = glm::rotate(model, glm::radians((float)angle * 6), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    CarModelNormal->render();

    //Uses differnt pass for differnt Texture!
    Shader_1.setUniform("Pass", 2);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 1.0f));
    model = glm::scale(model, vec3(0.8f, 0.8f, 0.8f));
    //model = glm::rotate(model, glm::radians((float)angle * 6), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    CarModelNormal->render();

    Shader_1.setUniform("Pass", 3);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(2.0f, -0.45f, 2.0f));
    model = glm::scale(model, vec3(0.8f, 0.8f, 0.8f));
    //model = glm::rotate(model, glm::radians((float)angle * 6), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    CarModelNormal->render();

}

void  SceneBasic_Uniform::Scenario_2()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    // - Alpha Test
    // - Texture
    // - Lights
    //

    view = glm::lookAt(vec3(-2.5f, 2.f, 2.0f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f,
        1.0f, 0.0f));

    //Setting Shader To Use
    Shader_2.use();

    //Setting Lights Positions
    Shader_2.setUniform("Light.Position", vec3(1.0f, 0.0f, 0.0f));
    Shader_2.setUniform("Light.La", vec3(0.1f, 0.1f, 0.1f));
    Shader_2.setUniform("Light.L", vec3(0.8f, 0.8f, 0.8f));

    //Setting Material Values for Car/Plane
    Shader_2.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    Shader_2.setUniform("Material.Ka", 0.9f, 0.9f, 0.9f);
    Shader_2.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
    Shader_2.setUniform("Material.Shininess", 180.0f);

    //Sets Pass to Ignore Texture Coords method.
    Shader_2.setUniform("Pass_2", 0);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    model = glm::rotate(model, glm::radians((float)angle * 6), vec3(0.0f, 1.0f, 0.0f));
    setMatrices2();
    plane.render();

    //Set Pass to Include Alpha Discarding
    Shader_2.setUniform("Pass_2", 1);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.5f, 0.0f));
    model = glm::scale(model, vec3(0.8f, 0.8f, 0.8f));
    model = glm::rotate(model, glm::radians((float)angle * 6), vec3(0.0f, 1.0f, 0.0f));
    setMatrices2();
    CarModel->render();

}

void  SceneBasic_Uniform::Scenario_3()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 
    // - BlinnPhong
    // - MultiTexture
    // - Multiple Lights
    //

    view = glm::lookAt(vec3(-2.5f, 2.f, 2.0f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f,
        1.0f, 0.0f));

    //Setting Shader To Use
    Shader_3.use();

    //Setting Lights Positions
    Shader_3.setUniform("Light[0].Position", vec4(-5.0f, -5.0f, 3.0f, 0.0f));
    Shader_3.setUniform("Light[1].Position", vec4(5.0f, 5.0f, 3.0f, 0.0f));
    Shader_3.setUniform("Light[2].Position", vec4(-5.0f, -5.0f, 3.0f, 0.0f));

    for (int i = 0; i < 3; i++)
    {
        //Setting Lights Ambient
        stringstream Label;
        Label << "Light[" << i << "].La";
        Shader_3.setUniform(Label.str().c_str(), vec3(0.2f, 0.2f, 0.2f));
    }

    //Setting Lights Colour
    Shader_3.setUniform("Light[0].L", vec3(0.06f, 0.34f, 0.81f));
    Shader_3.setUniform("Light[1].L", vec3(0.06f, 0.18f, 0.27f));
    Shader_3.setUniform("Light[2].L", vec3(0.78f, 0.14f, 0.06f));

    Shader_3.setUniform("Material.Ks", 0.2f, 0.2f, 0.2f);
    Shader_3.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    Shader_3.setUniform("Material.Kd", 0.2f, 0.2f, 0.2f);
    Shader_3.setUniform("Material.Shininess", 180.0f);

    //Sets Pass to Ignore Texture Coords method.
    Shader_3.setUniform("Pass_3", 1);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    model = glm::rotate(model, glm::radians((float)angle * 6), vec3(0.0f, 1.0f, 0.0f));
    setMatrices3();
    plane.render();

    //Sets Pass to Load Textures.
    Shader_3.setUniform("Pass_3", 0);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.5f, 0.0f));
    model = glm::scale(model, vec3(0.8f, 0.8f, 0.8f));
    model = glm::rotate(model, glm::radians((float)angle * 6), vec3(0.0f, 1.0f, 0.0f));
    setMatrices3();
    CarModel->render();
}

void  SceneBasic_Uniform::Scenario_4()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 
    // - Toon Shading
    // - Moving Lights
    // - Fog (Issues With this)

    if (Scene4Toggle)
    {
        Shader_4.setUniform("Pass", 1);
    }
    else
    {
        Shader_4.setUniform("Pass", 0);
    }

    view = glm::lookAt(vec3(-2.5f, 2.f, 2.0f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f,
        1.0f, 0.0f));

    //Setting Shader To Use
    Shader_4.use();

    vec4 lightPos = vec4(10.0f * cos(angle5), 10.0f, 10.0f * sin(angle5), 1.0f);
    Shader_4.setUniform("Light.Position", view * lightPos);

    //Setting Lights Colour
    Shader_4.setUniform("Light.L", vec3(Value3, Value2, Value));
    Shader_4.setUniform("Light.La", vec3(0.2f, 0.2f, 0.2f));

    Shader_4.setUniform("Material.Kd", 0.2f, 0.55f, 0.9f);
    Shader_4.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    Shader_4.setUniform("Material.Ka", 0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f);
    Shader_4.setUniform("Material.Shininess", 100.0f);
    Shader_4.setUniform("Material.Shininess", 100.0f);

    Shader_4.setUniform("Fog.MaxDist", 50.0f);
    Shader_4.setUniform("Fog.MinDist", 1.0f);
    Shader_4.setUniform("Fog.Color", vec3(0.8f, 0.8f, 0.8f));

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices4();
    plane.render();

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.5f, 0.0f));
    model = glm::scale(model, vec3(0.8f, 0.8f, 0.8f));
    setMatrices4();
    CarModel->render();
}

void  SceneBasic_Uniform::Scenario_5()
{
    //
    // This is Car 1, This will Display
    // - Toon Shading
    // - Spolight, Changes Colour
    view = glm::lookAt(vec3(-2.5f, 2.f, 2.0f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f,
        1.0f, 0.0f));

    //Setting Shader To Use
    Shader_5.use();

    //Setting Lights Positions
    vec4 lightPos = vec4(10.0f * cos(angle), 20.0f, 10.0f * sin(angle), 6.0f);
    Shader_5.setUniform("Light.Position", view * lightPos);
    Shader_5.setUniform("Light.La", vec3(0.1f, 0.1f, 0.1f));
    Shader_5.setUniform("Light.L", vec3(0.2f, 0.2f, 0.2f));

    //Setting Material Values for Car/Plane
    Shader_5.setUniform("Material.Ks", 0.2f, 0.2f, 0.2f);
    Shader_5.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    Shader_5.setUniform("Material.Kd", 0.2f, 0.2f, 0.2f);
    Shader_5.setUniform("Material.Shininess", 180.0f);

    lightPos = vec4(0.0f, 10.0f, 0.0f, 1.0f);
    Shader_5.setUniform("Spot.Position", vec3(view * lightPos));
    mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
    Shader_5.setUniform("Spot.Direction", normalMatrix * vec3(-lightPos));

    Shader_5.setUniform("Spot.L", vec3(Value3, Value2, Value));
    Shader_5.setUniform("Spot.La", vec3(0.2f));
    Shader_5.setUniform("Spot.Exponent", 10.0f); //Blurry Around Circle
    Shader_5.setUniform("Spot.CutOff", glm::radians(5.0f));  //Size of circle

    Shader_5.setUniform("Pass", 0);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    model = glm::rotate(model, glm::radians((float)angle * 6), vec3(0.0f, 1.0f, 0.0f));
    setMatrices5();
    plane.render();

    //Set Pass to Include Alpha Discarding
    Shader_5.setUniform("Pass", 1);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.5f, 0.0f));
    model = glm::scale(model, vec3(0.8f, 0.8f, 0.8f));
    model = glm::rotate(model, glm::radians((float)angle * 6), vec3(0.0f, 1.0f, 0.0f));
    setMatrices5();
    CarModel->render();
}

void SceneBasic_Uniform::RenderSkyBox()
{
    SkyBox.use();

    model = mat4(1.0f);

    setMatricesSky();
    sky.render();
}