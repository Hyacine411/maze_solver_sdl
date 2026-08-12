#include <stdio.h>
#include "maze.h"
#include "solver.h"

int path_len = 0;
int path_x[MAX_PATH_LEN];
int path_y[MAX_PATH_LEN];

int solve_maze(char maze[][MAX_COLS], int x, int y) {
    if (x < 0 || x >= COLS || y < 0 || y >= ROWS) return 0;
    if (maze[y][x] == '#' || maze[y][x] == '.') return 0;

    if (maze[y][x] == 'E') {
        path_x[path_len] = x;
        path_y[path_len] = y;
        path_len++;
        return 1;
    }

    char temp = maze[y][x];
    maze[y][x] = '.';

    if (solve_maze(maze, x+1, y) || solve_maze(maze, x-1, y) ||
        solve_maze(maze, x, y+1) || solve_maze(maze, x, y-1)) {
        path_x[path_len] = x;
        path_y[path_len] = y;
        path_len++;
        return 1;
    }

    maze[y][x] = temp;
    return 0;
}
