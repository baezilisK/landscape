#ifndef CAM_H
#define CAM_H

float
  cam_x, cam_y, cam_z, /* position */
  cam_ti, cam_tj;      /* rotation, ti real, -PI < tj < PI */

/*
 * move forward/back (x), left/right (y), relative to the direction
 * of the camera
 */
void cam_mv (float x, float y);

/*
 * rotate the camera:
 *  - by a horizontaly
 *  - by b vertically such that -PI < tj < PI
 */
void cam_rotate (float a, float b);

/*
 * synchronize GL modelview matrix with camera state
 */
void cam_sync (void);

#endif /* CAM_H */
