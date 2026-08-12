#include <stdio.h>
#include "maze.h"
#include "solver.h"
#include "visualize.h"

int main() {
    if (!load_maze_from_file("assets/maze.txt")) {
        return 1;
    }

    // printf("原始迷宫:\n");
    // print_maze();

    int start_x = -1, start_y = -1;

    // 寻找 S 起点
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 'S') {
                start_x = j;
                start_y = i;
                break;
            }
        }
    }

    if (start_x == -1) {
        printf("找不到起点 S\n");
        return 1;
    }

    if (solve_maze(maze, start_x, start_y)) {
        // printf("\n路径已找到,路径坐标如下（从终点到起点）:\n");
        // for (int i = path_len - 1; i >= 0; i--) {
            // printf("(%d, %d)\n", path_x[i], path_y[i]);
        // }

        // 动态可视化走迷宫
        run_visualization(maze, path_x, path_y, path_len);

    } else {
        printf("\n未找到路径。\n");
    }

    // printf("\n已找到路径,如下图:\n");
    // print_maze();

    return 0;
}
