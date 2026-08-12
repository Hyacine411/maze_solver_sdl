#ifndef MAZE_H
#define MAZE_H

#define MAX_ROWS 100
#define MAX_COLS 100

extern int ROWS, COLS;
extern char maze[MAX_ROWS][MAX_COLS];

int load_maze_from_file(const char* filename);

void print_maze();

#endif
