#ifndef SOLVER_H
#define SOLVER_H

#define MAX_PATH_LEN 10000

extern int path_len;
extern int path_x[MAX_PATH_LEN];
extern int path_y[MAX_PATH_LEN];

int solve_maze(char maze[][MAX_COLS], int x, int y);

#endif
