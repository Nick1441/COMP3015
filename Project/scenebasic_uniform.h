#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>

//Helper
#include "helper/glslprogram.h"
#include "helper/texture.h"
#include "helper/stb/stb_image.h"
#include "helper/glutils.h"

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
    GLSLProgram Shader_BlinnPhong;

    Plane plane;
    unique_ptr<ObjMesh> mesh;
    unique_ptr<ObjMesh> StreetLamp1;
    Teapot teapot;
    void setMatrices();

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
    void RenderObject();
    void RenderObject2();
    void RenderOther();
};

#endif // SCENEBASIC_UNIFORM_H