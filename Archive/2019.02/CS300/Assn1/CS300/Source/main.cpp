/******************************************************************************
  Project : load, display and manipulate a 3D object
  Name    : Cody Morgan
  Date    : 16 DEC 2019
******************************************************************************/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
  using glm::vec3;
  using glm::vec4;
#include <iostream>
  using std::cout;
#include <fstream>
  using std::ifstream;
#include <string>
  using std::string;
#include <vector>
  using std::vector;

#include "imGui/imgui.h"
#include "imGui/imgui_impl_glfw.h"
#include "imGui/imgui_impl_opengl3.h"

#include "ObjectManager.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Common.h"
#include "Light.h"
#include "BoundingVolume.h"

// managers and static variables
static ObjectManager* objectMgr = nullptr;
static ShaderManager* shaderMgr = nullptr;
static Object* treeRoot = nullptr;
static Camera* camera = nullptr;

static bool pauseSimulation = false;
static bool pauseModel = true;
static int  selectedModel = 0;

static int  currentDisplayMode = 0;
static int  currentTreeLevel = -10;
static bool currentShape = 0; // 0 = aabb 1 = sphere
static int  currentTreeMode = 0;
static int  currentTDMode = 0;
static int  currentScene = 0;
static int  oldScene = 0;


/////***** Window and OpenGL Management *****/////


bool WindowInit(int major, int minor, GLFWwindow** window)
{
  GLFWvidmode return_struct;
  GLenum error = glfwInit();
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  

  if (error == GLFW_FALSE)
  {
    char buffer[256];
    glfwGetError((const char**)buffer);
    return getchar();
  }

  // instance the window
  *window = glfwCreateWindow(mode->width, mode->height, "CS350 Assignment 1 Cody Morgan", NULL, NULL);
  glfwWindowHint(GLFW_SAMPLES, 1); // change for anti-aliasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create the window obj
  if (*window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(*window);

  error = glewInit();
  if (error)
  {
    cout << glewGetErrorString(error);
    return getchar();
  }
  glViewport(0, 0, mode->width, mode->height);

  // window is gtg
  return true;
}

/******************************************************************************
  brief : cleans up program

******************************************************************************/
void Terminate()
{
  glfwTerminate();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

/******************************************************************************
  brief : error callback

******************************************************************************/
void GLAPIENTRY MessageCallback(GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam)
{
#ifndef DEBUG_SEVERITY_HIGH
#define DEBUG_SEVERITY_HIGH            0x9146
#define DEBUG_SEVERITY_MEDIUM          0x9147
#define DEBUG_SEVERITY_LOW             0x9148
#define DEBUG_SEVERITY_NOTIFICATION    0x826B
#define DEBUG_TYPE_ERROR               0x824C
#define DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define DEBUG_TYPE_PORTABILITY         0x824F
#define DEBUG_TYPE_PERFORMANCE         0x8250
#define DEBUG_TYPE_OTHER               0x8251
#define DEBUG_TYPE_MARKER              0x8268
#endif

  if (severity != DEBUG_SEVERITY_NOTIFICATION)
  {
    string debugTypes[] =
    {
      "DEBUG_TYPE_ERROR",
      "DEBUG_TYPE_DEPRECATED_BEHAVIOR",
      "DEBUG_TYPE_UNDEFINED_BEHAVIOR",
      "DEBUG_TYPE_PORTABILITY",
      "DEBUG_TYPE_PERFORMANCE",
      "DEBUG_TYPE_OTHER",
      "DEBUG_TYPE_MARKER"
    };
    string severityTypes[] =
    {
      "HIGH",
      "MEDIUM",
      "LOW"
    };

    string typeString = debugTypes[(type - DEBUG_TYPE_ERROR) % 7];
    string severityString = severityTypes[(severity - DEBUG_SEVERITY_HIGH) % 3];

    fprintf(stderr, "Error\n   Severity: %s   Type: %s\n   \n   %s\n\n", severityString.c_str(), typeString.c_str(), message);
  }
}

void Window_size_callback(GLFWwindow* window, int width, int height)
{
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
}



void SceneSetup()
{
  Object* obj = objectMgr->addObject("model");
  obj->setShader(ShaderType::Deferred);
  obj->loadFolder(".\\Common\\PowerPlant4");
  //obj->loadModel("./Common/PowerPlant4/ppsection2/part_b/g4.ply");
  //obj->loadSphere(1, 10);
  obj->material.ambient = vec3(0, 0, 1);
  
  Object* obj2 = objectMgr->addLight("light");
  obj2->translate(right * 3.0f);
  dynamic_cast<Light*>(obj2)->lightData.ambient = vec4(1);

  treeRoot = objectMgr->addVolume<AABB>(obj, "root");
  treeRoot->material.ambient = vec3(1, 0, 0);
  dynamic_cast<AABB*>(treeRoot)->drawAsSphere(currentShape);
  dynamic_cast<AABB*>(treeRoot)->heightMax = 7;
  dynamic_cast<AABB*>(treeRoot)->setTopDownMode((AABB::TopDownMode) currentTDMode);
  dynamic_cast<AABB*>(treeRoot)->split(0);
  printf("split complete\n");
  
  auto centroid = objectMgr->addVolume<Centroid>(obj, "Centroid Sphere Method");
  centroid->wiremode = true;
  centroid->drawMe = false;

  auto ritter = objectMgr->addVolume<Ritter>(obj, "Ritter Sphere Method");
  ritter->wiremode = true;
  ritter->drawMe = false;

  auto ellipsoid = objectMgr->addVolume<Ellipsoid>(obj, "Ellipsoid Method");
  ellipsoid->wiremode = true;
  ellipsoid->drawMe = false;
  
  auto Larsson = objectMgr->addVolume<Centroid>(obj, "Larsson Sphere Method");
  Larsson->wiremode = true;
  Larsson->drawMe = false;

  auto pca = objectMgr->addVolume<Ritter>(obj, "PCA Sphere Method");
  pca->wiremode = true;
  pca->drawMe = false;
  
}

void SceneUpdate()
{
  // dont update invalid object managers
  if (!objectMgr->isValid() || pauseSimulation)
    return;

  if (pauseModel == true)
  {
    objectMgr->getFirstObjectByName("model")->rotate(1);
    objectMgr->getFirstObjectByName("light")->rotate(-1, left * 3.0f);
  }
}

void SceneShutdown()
{
  objectMgr->removeAllObjects();
}

/////***** GUI Management *****/////


void InitGUI(GLFWwindow* window)
{
  // GL state setting
#ifdef _DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
#endif
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  
  // context 
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();

  // open GL
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  // setup stuff
  ImGui::StyleColorsDark();
}

void ProcessInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // camera movement
  float speed = 0.1f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera->translate(forward * speed);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera->translate(right * speed);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera->translate(back * speed);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera->translate(left * speed);
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    camera->translate(up * speed);
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    camera->translate(down * speed);
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    camera->rotate(1, up);
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    camera->rotate(-1, up);

  
  // simulation states
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    pauseSimulation = true;
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    pauseSimulation = false;
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    pauseModel = true;
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
    pauseModel = false;
}

void UpdateGUI()
{
  // allowed models
  const std::vector<const char*> modelNames =
  {
    "4Sphere",
    "bunny",
    "cube2",
    "sphere",
  };

  // differed visualization targets
  const std::vector<const char*> displayNames =
  {
    "Phong Illumination",
    "View Position",
    "Normal",
    "Diffuse",
    "Specular",
  };

  const std::vector<const char*> treeNames =
  {
    "Top Down",
    "Bottom Up"
  };

  const std::vector<const char*> shapes =
  {
    "AABB",
    "Unweighted-Center Sphere"
  };

  const std::vector<const char*> scenes =
  {
    "Bounding Volume Hierarchy",
    "Centroid Sphere Method",
    "Ritter Sphere Method",
    "Ellipsoid Method",
    "Larsson Sphere Method",
    "PCA Sphere Method"
  };
  // get all current state data
  int& objectIndex = objectMgr->selectedObject;
  Object* selectedObject = objectMgr->getSelected();
  Light* selectedLight = dynamic_cast<Light*>(selectedObject);

  // dont use any GUI if there are no objects
  if (!selectedObject)
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("ERROR - THERE ARE NO OBJECTS");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    return;
  }

  vec3 currentPosition = selectedObject->getWorldPosition();
  vec3 currentScale = selectedObject->getWorldScale();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // text
  ImGui::Begin("Welcome to CS350 Assignment 2!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

  // object options
  bool changedObject   = ImGui::ListBox("Objects", &objectIndex, &objectMgr->getObjectNames()[0],objectMgr->getSize());
  bool changedPosition = ImGui::DragFloat3("World Position", &currentPosition[0]);
  bool changedScale    = ImGui::DragFloat3("World Scale", &currentScale[0]);
  bool changeModel     = ImGui::ListBox("Models", &selectedModel, &modelNames[0], int(modelNames.size()));


  // single BV options
  ImGui::Text("Scene Selection");
  bool changedScene = ImGui::SliderInt(scenes[currentScene], &currentScene, 0, scenes.size()-1);

  // tree options
  bool changedTreeMode = false;
  bool changedTreeshape = false;
  bool changedTreelevel = false;
  bool changedTreeMethod = false;
  AABB* root = dynamic_cast<AABB*>(treeRoot);
  if (currentScene == 0)
  {
    ImGui::Text("Tree Level View");
    changedTreelevel = ImGui::SliderInt(treeNames[currentTreeMode], &currentTreeLevel, -1, root->maxLevel);
    ImGui::Text("Tree Volume Shape");
    changedTreeshape |= ImGui::RadioButton("AABB", !currentShape);
    changedTreeshape |= ImGui::RadioButton("Sphere", currentShape);

    ImGui::Text("Tree Bounding Volume Building method");
    changedTreeMethod  = ImGui::RadioButton("Top Down", !currentTreeMode);
    changedTreeMethod |= ImGui::RadioButton("Bottom Up", currentTreeMode);

    // just TD options
    if (currentTreeMode == 0)
    {
      ImGui::Text("Top Down Tree Mode");
      changedTreeMode  = ImGui::RadioButton("Max Vertex Count == 500", !currentTDMode);
      changedTreeMode |= ImGui::RadioButton("Max Tree Height == 7", currentTDMode);
    }
  }

  // data
  bool changedLightData = false;
  bool changedObjectData = false;
  if (selectedLight)
  {
    changedLightData |= ImGui::ColorEdit3("light ambient", &(selectedLight->lightData.ambient.x));
    changedLightData |= ImGui::ColorEdit3("light diffuse", &(selectedLight->lightData.diffuse.x));
    changedLightData |= ImGui::ColorEdit3("light specular", &(selectedLight->lightData.specular.x));
    changedLightData |= ImGui::SliderFloat("shininess", &(selectedLight->lightData.ns), 0, 500);
  }
  else
  {
    changedObjectData |= ImGui::ColorEdit3("object ambient", &(selectedObject->material.ambient.x));
    changedObjectData |= ImGui::ColorEdit3("object diffuse", &(selectedObject->material.diffuse.x));
    changedObjectData |= ImGui::ColorEdit3("object specular", &(selectedObject->material.specular.x));
  }

  // drawing mode options 
  ImGui::Text("Drawing Mode");
  bool changetoWire   = ImGui::RadioButton("WireFrame", selectedObject->wiremode);
  bool changeFromWire = ImGui::RadioButton("Shaded", !selectedObject->wiremode);
  bool changeDisplay  = ImGui::ListBox("Deferred display mode", &currentDisplayMode, &displayNames[0], displayNames.size());

  // light obtions
  bool changeDebugMode = ImGui::Button("Toggle Debug Drawing mode");


  // resetting options
  bool recompileShaders = ImGui::Button("Recompile Shaders");
  bool resetCamera      = ImGui::Button("Reset Camera");
  bool resetScene       = ImGui::Button("Reset Scene");


  // end of gui options
  ImGui::End();



  // object effects
  if (changedTreeMethod)
  {
    if(currentTreeMode == 0)
      currentTreeLevel = root->maxLevel;
    else
      currentTreeLevel = 0;

    currentTreeMode = !currentTreeMode;
    changedTreelevel = true;
  }
  if (changedScene)
  {
    // turn off old scene 0==tree though
    if(oldScene -1 >= 0)
      objectMgr->getFirstObjectByName(scenes[oldScene])->drawMe = false;

    switch (currentScene)
    {
    case 0:
      root->drawLevel(currentTreeLevel);
      break;

    default:
      root->drawLevel(-1);
      objectMgr->getFirstObjectByName(scenes[currentScene])->drawMe = true;
      break;
    }
    oldScene = currentScene;
  }
  if (currentTreeLevel == -10)
  {
    currentTreeLevel = 0;
    changedTreelevel = true;
  }
  if(changedTreeMode)
  {
    BoundingVolume::TopDownMode mode;
    if (currentTDMode == 0)
    {
      mode = BoundingVolume::TopDownMode::heightMax;
        currentTDMode = 1;
    }
    else
    {
      mode = BoundingVolume::TopDownMode::vertexMax;
      currentTDMode = 0;
    }
    root->setTopDownMode(mode);
    changedTreelevel = true;
    resetScene = true;
  }
  if (changedTreeshape)
  {
    currentShape = !currentShape;
    resetScene = true;
    root->split(0);
    root->drawAsSphere(currentShape);
  }
  if (changedTreelevel)
  {
    AABB* root = dynamic_cast<AABB*>(treeRoot);
    root->drawLevel(currentTreeLevel);
  }
  if (changeDisplay)
  {
    shaderMgr->getShader(ShaderType::DeferredLighting).updateDisplayMode(currentDisplayMode);
  }

  if(changedPosition)
  {
    vec3 trans = selectedObject->getWorldPosition() - currentPosition;
    selectedObject->translate(trans);
  }
  if (changedScale)
  {
    selectedObject->scale(currentScale);
  }
  if (changetoWire || changeFromWire)
  {
    selectedObject->wiremode = !selectedObject->wiremode;
    if (selectedObject->wiremode == false)
      selectedObject->renderMode = GL_TRIANGLES;
    else
      selectedObject->renderMode = GL_LINE_STRIP;
  }
  if(changeDebugMode)
  {
    objectMgr->debugMode = !objectMgr->debugMode;
  }
  if (changeModel)
  {
    string name = "Common/models/" + string(modelNames[selectedModel]) + ".obj";
    selectedObject->loadModel(name);
  }

  // resetting effects
  if (recompileShaders)
  {
    shaderMgr->reCompile(ShaderType::TypeCount);
    objectMgr->resetUBO();
  }
  if (resetCamera)
  {
    camera->reset();
  }
  if (resetScene)
  {
    SceneShutdown();
    SceneSetup();
    camera->reset();
    currentTreeLevel = -10;
  }

}

void GUIendFrame(GLFWwindow* window, double time)
{
  //maintain viewport
  glfwSetFramebufferSizeCallback(window, Window_size_callback);

  // render scene and GUI window
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(window);
  glfwPollEvents();

  // maintain a max of 120hertz
  double dt = glfwGetTime() - time;
  while (dt < (1 / 120.0))
  {
    dt = glfwGetTime() - time;
  }
}


/////***** Main *****/////


int main()
{
  // make a window
  GLFWwindow* window = nullptr;
  if (WindowInit(4, 0, &window) == false)
    return -1;
  
  // setup GLFW and IMgui
  InitGUI(window);
  
  // managers setup
  objectMgr = ObjectManager::getObjectManager();
  shaderMgr = ShaderManager::getShaderManager();

  // scene setup
  camera =  new Camera(vec3(0, 0, -8), 0.0f, right);

  SceneSetup();
  while (!glfwWindowShouldClose(window))
  {
    // scene loop
    double time = glfwGetTime();
    UpdateGUI();
    ProcessInput(window);

    // sim loop
    SceneUpdate();

    // end of the loop
    objectMgr->render(*camera);
    GUIendFrame(window, time);
  }

  Terminate();

  return 0;
}