/* !
@file    pbo.h
@author  pghali@digipen.edu
@date    10/11/2016

This file contains the declaration of structure Pbo that encapsulates the
functionality required to stream images generated by a 3D graphics pipe 
emulator executed on the CPU for display by the GPU.

*//*__________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef PBO_H
#define PBO_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "../include/app.h"
#include "../include/glslshader.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <array>
#include <vector>
#include <cstring>

class Model
{
public:
  std::vector<glm::vec3> verts;
  std::vector<glm::vec3> screen;
  std::vector<glm::vec3> norms;
  std::vector<glm::vec2> uv;
  std::vector<int> depth;
  std::vector<unsigned short> indices;

  int min = -265;
  int max = 265;
  float deg = 15.0f;
  
private:

};

/*  _________________________________________________________________________ */
struct Pbo
  /*! Pbo structure to encapsulate 3D graphics pipe emulator which will write
  an image composed of RGBA values to PBO, copy the image from PBO to image 
  store of a texture object, and finally use this texture image to render a 
  full-window quad that will display the emulator's output (phew).
  */
{
  // forward declaration
  union Color;

  //-------------- static member function declarations here -----------------
  
  // overloaded functions akin to glClearcolor that set Pbo::clear_clr
  // with user-supplied parameters ...
  static void set_clear_color(Pbo::Color);
  static void set_clear_color(GLubyte r, GLubyte g, GLubyte b, GLubyte a = 255);

  // akin to glClear(GL_COLOR_BUFFER_BIT) - uses the pointer
  static void clear_color_buffer();

  // this is where the emulator does its work of emulating the graphics pipe
  // it generates images using set_pixel to write to the PBO
  static void render();
  
  // render quad using the texture image generated by render()
  static void draw_fullwindow_quad();

  // initialization and cleanup stuff ...
  static void init(GLsizei w, GLsizei h);
  static void setup_quad_vao();
  static void setup_shdrpgm();
  static void cleanup();
  static void set_pixel(int x, int y, Pbo::Color color);
  static void render_triangle0(glm::vec3 const& p0, 
                               glm::vec3 const& p1, 
                               glm::vec3 const& p2,
                               Pbo::Color color);

  // ---------------static data members are declared here ----------------

  // Storage requirements common to emulator, PBO and texture object
  static GLsizei width, height; // dimensions of buffers
  // rather than computing these values many times, compute once in
  // Pbo::init() and then forget ...
  static GLsizei pixel_cnt, byte_cnt; // how many pixels and bytes
  // pointer to PBO's memory chunk - must be set every frame with the value 
  // returned by glMapNamedBuffer()
  static typename Pbo::Color *ptr_to_pbo;

  // geometry and material information ...
  static GLuint vaoid;        // with GL 4.5, VBO & EBO are not required
  static GLuint elem_cnt;     // how many indices in element buffer
  static GLuint pboid;        // id for PBO
  static GLuint texid;        // id for texture object
  static GLSLShader shdr_pgm; // object that abstracts away nitty-gritty
                              // details of shader management
  static void render_linedda(int x1, int y1, int x2, int y2, Pbo::Color color);
  static void render_linebresenham(int x1, int y1, int x2, int y2, Pbo::Color color);
  static void drawModel(Model& model);
  static void transformToScreen();

  // clear color - here we're trying to emulate GL's functions for
  // clearing the colorbuffer ... 
  static typename Pbo::Color clear_clr;

  union Color
    /*! glm doesn't have type unsigned char [4] - therefore we declare our
    own type.
    Since Color is such a common type name in graphics applications, make
    sure to encapsulate the name in scope Pbo!!!
    */
  {
    Color(GLubyte red = 0, GLubyte green = 0, GLubyte blue = 0, GLubyte alpha = 255) :
      r(red), g(green), b(blue), a(alpha) {}
    struct {
      GLubyte r, g, b, a; // a, b, g, r;
    };
    GLubyte val[4];
    GLuint raw;
  };
  

};

#endif /* PBO_H */
