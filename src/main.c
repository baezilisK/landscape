#include <assert.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "GL/glut.h"
#include "cam.h"
#include "kbd.h"

#define N 256
#define NM 255
#define RENDER_DIST 10
#define RENDER_PREC 10
#define NLAYER 3
#define HSCALE 1.0

int p[1 << CHAR_BIT + 1];
float r[1 << CHAR_BIT + 1];

int rng (int n) {
  return rand () / (RAND_MAX/n + 1);
}

void swap (void *u, void *v, int n) {
  char *a = u, *b = v, c;
  while (n-- > 0) {
    c = *a; *a++ = *b; *b++ = c;
  }
}

float interpolate (float a, float b, float t) {
  t = t*t*(3-2*t);
  return a + (b - a) * t;
}

void init (void) {
  int i;
  for (i = 0; i < 1<<CHAR_BIT; ++i) {
    p[i] = i;
    r[i] = (float)rand()/RAND_MAX;
  }
  for (i = 0; i < 1<<CHAR_BIT; ++i)
    swap (p + i, p + rng (1<<CHAR_BIT), sizeof *p);
  for (i = 0; i < 1<<CHAR_BIT; ++i) {
    p[i + (1<<CHAR_BIT)] = p[i];
    r[i + (1<<CHAR_BIT)] = r[i];
  }
}

float noise (float x, float y) {
  int
    xl = floor (x), xr = xl + 1,
    yl = floor (y), yr = yl + 1,
    rx0, rx1, ry0, ry1;
  float 
    a, b, c,
    tx = x - xl, ty = y - yl,
    c00, c10, c01, c11;

  rx0 = xl & NM; rx1 = xr & NM;
  ry0 = yl & NM; ry1 = yr & NM;
  c00 = r[p[p[rx0] + ry0]];
  c01 = r[p[p[rx1] + ry0]];
  c10 = r[p[p[rx0] + ry1]];
  c11 = r[p[p[rx1] + ry1]];
  /*
  c00 = r[p[rx0] + ry0];
  c01 = r[p[rx1] + ry0];
  c10 = r[p[rx0] + ry1];
  c11 = r[p[rx1] + rx1];
  */

  a = interpolate (c00, c01, tx);
  b = interpolate (c10, c11, tx);
  c = interpolate (a, b, ty);
  return c;
}

float h (float x, float y) {
  int i;
  float ret = 0, p = HSCALE, q = 1;
  for (i = 0; i < NLAYER; ++i) {
    ret += p * noise (q*x, q*y);
    p *= 2;
    q /= 2;
  }
  return ret;
}

void initgl (void) {
  glClearColor (0, 0, 0, 0);
  glEnable (GL_DEPTH_TEST);
  kbd_setmap ("assets/kbd/dvp");
}

void reshape (int w, int h) {
  glViewport (0, 0, w, h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (60, 1.0*w/h, 0.1, 50);
  cam_sync ();
}

void passivemotion (int x, int y) {
  float s = 0.001, dx, dy;
  int
    cx = glutGet (GLUT_WINDOW_WIDTH) / 2,
    cy = glutGet (GLUT_WINDOW_HEIGHT) / 2;
  dx = (cx - x) * s; dy = (cy - y) * s;
  cam_rotate (dx, dy);
  cam_sync ();
  glutPostRedisplay ();
}

void centermouse (void) {
  glutWarpPointer (
    glutGet (GLUT_WINDOW_WIDTH) / 2,
    glutGet (GLUT_WINDOW_HEIGHT) / 2
  );
}

void tick (void) {
  float u = 0.01;
  if (kbd_state['w']) cam_mv (u, 0);
  if (kbd_state['s']) cam_mv (-u, 0);
  if (kbd_state['a']) cam_mv (0, -u);
  if (kbd_state['d']) cam_mv (0, u);
  if (kbd_state['c']) cam_z -= u;
  if (kbd_state[' ']) cam_z += u;
  cam_z = h (cam_x, cam_y) + 0.3;
  glutPostRedisplay ();
}

void keydown (unsigned char key, int x, int y) {
  (void) x; (void) y;
  if (kbd_map[key] == 'q') exit (EXIT_SUCCESS);
  kbd_state[kbd_map[key]] = 1;
}

void keyup (unsigned char key, int x, int y) {
  (void) x; (void) y;
  kbd_state[kbd_map[key]] = 0;
}

void vertex (float x, float y) {
  float z = h (x, y), g = z/4;
  g = g*g*g*g;
  glColor3f (g, g, g);
  glVertex3f (x, y, z);
}

void display (void) {
  int i, j, prec = 10, r = prec/2;
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  glBegin (GL_TRIANGLES);
    for (i = 0; i < prec*prec; ++i) {
      for (j = 0; j < prec*prec; ++j) {
        float
          Ox = 0, 
          Oy = 0,
          x = Ox - r + (float)i/prec,
          xx = Ox - r + (float)(i+1)/prec,
          y = Oy - r + (float)j/prec,
          yy = Oy - r + (float)(j+1)/prec;
        vertex (x, y);
        vertex (xx, y);
        vertex (xx, yy);
        vertex (xx, yy);
        vertex (x, yy);
        vertex (x, y);
        /*
        vertex ((float)i/prec, (float)j/prec);
        vertex ((float)(i+1)/prec, (float)j/prec);
        vertex ((float)(i+1)/prec, (float)(j+1)/prec);
        vertex ((float)(i+1)/prec, (float)(j+1)/prec);
        vertex ((float)i/prec, (float)(j+1)/prec);
        vertex ((float)i/prec, (float)j/prec);
        */
      }
    }
  glEnd ();
  centermouse ();
  glFlush ();
}

int main (int argc, char **argv) {
  init ();
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow (argv[0]);
  initgl ();
  glutReshapeFunc (reshape);
  glutDisplayFunc (display);
  glutPassiveMotionFunc (passivemotion);
  glutKeyboardFunc (keydown);
  glutKeyboardUpFunc (keyup);
  glutIdleFunc (tick);
  glutFullScreen ();
  glutMainLoop ();
  exit (EXIT_SUCCESS);
}
