#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>

//Helper
#include "helper/glslprogram.h"
#include "helper/texture.h"
#include "helper/stb/stb_image.h"
#include "helper/glutils.h"
#include "helper/skybox.h"

//Additional Files
#include "Additional Files/drawable.h"
#include "Additional Files/trianglemesh.h"
#include "Additional Files/teapot.h"
#include "Additional Files/aabb.h"
#include "Additional Files/utils.h"
#include "Additional Files/objmesh.h"
#include "Additional files/plane.h"
#include "Additional Files/cube.h"


#include "helper/glutils.h"

#include <glm/glm.hpp>

using namespace std;

class SceneBasic_Uniform : public Scene
{
private:
    //GLSLProgram Shader_BlinnPhong;

    GLSLProgram Shader_1;
    GLSLProgram Shader_2;
    GLSLProgram Shader_3;
    GLSLProgram Shader_4;

    SkyBox sky;
    GLSLProgram SkyBox;

    float angle;
    float angle2;
    float angle3;
    float tPrev;
    float tPrev2;
    float rotSpeed;
    Plane plane;
    unique_ptr<ObjMesh> CarModel;
    unique_ptr<ObjMesh> CarModelNormal;
    //unique_ptr<ObjMesh> StreetLamp1;
    //Teapot teapot;
    void setMatrices();
    void setMatrices2();
    void setMatrices3();
    void setMatrices4();
    void setMatricesSky();

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
    void InputPressed(int);
    void RenderSkyBox();

    void Scenario_1();
    void Scenario_2();
    void Scenario_3();
    void Scenario_4();
    void Scenario_5();
    void Scenario_6();
};

#endif // SCENEBASIC_UNIFORM_H