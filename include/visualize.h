#ifndef VISUALIZE_H
#define VISUALIZE_H

#include "maze.h"

/*
 * Open an SDL window and animate the supplied E-to-S path in reverse, from S
 * to E. The renderer uses a separate overlay and never modifies source maze.
 */
int run_visualization(char maze[][MAX_COLS + 1], const int path_x[],
                      const int path_y[], int path_len);

#endif
