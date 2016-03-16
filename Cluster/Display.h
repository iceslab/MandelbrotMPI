#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <GL/gl.h>
#include <GL/glut.h>
#include <gmpxx.h>
#include <iostream>

void magnify(int x, int y, mpf_class magnitude);

void RenderScene(void);

void ChangeSize(GLsizei horizontal, GLsizei vertical);

void mouse(int button, int state, int x, int y);

void keys(unsigned char key, int x, int y);

#endif