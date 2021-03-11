#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include "Additional Files/teapot.h"

#include "Additional Files/aabb.h"
#include "Additional Files/utils.h"
#include "Additional Files/objmesh.h"
#include "Additional files/plane.h"
#include "helper/glutils.h"

#include <glm/glm.hpp>

using namespace std;

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;

    GLSLProgram Shader_BlinnPhong;
    GLSLProgram Shader_Toon;
    GLSLProgram Shader_Texture;

    Plane plane;
    unique_ptr<ObjMesh> mesh;
    unique_ptr<ObjMesh> BackGround;


    void setMatrices();

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H