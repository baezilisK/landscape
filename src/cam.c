#include <math.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "cam.h"

#define PI 3.14159265359
#define EPSILON 0.001
#define min(a, b) ((a) < (b) ? (a) : (b))

static void cam_dx (float *dx) {
  dx[0] = cos (cam_ti) * cos (cam_tj);
  dx[1] = sin (cam_ti) * cos (cam_tj);
  dx[2] = sin (cam_tj);
}

void cam_mv (float x, float y) {
  cam_x += x * cos (cam_ti) + y * cos (cam_ti - PI/2);
  cam_y += x * sin (cam_ti) + y * sin (cam_ti - PI/2);
}

void cam_rotate (float a, float b) {
  cam_ti += a;
  cam_tj += b;
  if (cam_ti < -PI) cam_ti += 2 * PI;
  if (cam_ti > PI) cam_ti -= 2 * PI;
  if (cam_tj < -PI / 2) cam_tj = -PI / 2 + EPSILON;
  if (cam_tj > PI / 2) cam_tj = PI / 2 - EPSILON;
}

void cam_sync (void) {
  float dx[3];
  cam_dx (dx);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gluLookAt (
    cam_x, cam_y, cam_z,
    cam_x + dx[0], cam_y + dx[1], cam_z + dx[2],
    0, 0, 1
  );
}
