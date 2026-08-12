#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

int ROWS, COLS;
char maze[MAX_ROWS][MAX_COLS];

int load_maze_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("❌ 无法打开迷宫文件");
        return 0;
    }

    fscanf(file, "%d %d\n", &COLS, &ROWS);

    for (int i = 0; i < ROWS; i++) {
        fgets(maze[i], MAX_COLS, file);
        // 去除换行符（可选）
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == '\n') maze[i][j] = '\0';
        }
    }

    fclose(file);
    return 1;
}

void print_maze() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
}
