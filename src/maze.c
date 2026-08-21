#include <stdio.h>
#include <ctype.h>
#include "maze.h"

/* Runtime dimensions are read from the first line of the maze file. */
int ROWS, COLS;
/* The in-memory grid is shared by loading, solving, and rendering modules. */
char maze[MAX_ROWS][MAX_COLS + 1];

/*
 * Only these four characters are legal inside a maze cell.  Keeping this
 * check in one helper means malformed input is rejected before any solver or
 * renderer indexes the grid.
 */
static int is_valid_cell(int cell) {
    return cell == '#' || cell == ' ' || cell == 'S' || cell == 'E';
}

/*
 * Read exactly one declared-width row.
 *
 * Reading one character at a time intentionally avoids the common fgets()
 * edge case where a maximum-width row leaves its newline in the input stream.
 * Both Unix LF and Windows CRLF endings are accepted.  The extra array slot
 * reserved in maze.h is used for a trailing '\0', making every row safe to
 * inspect or print as a C string.
 */
static int read_maze_row(FILE* file, char row[], int columns) {
    for (int x = 0; x < columns; ++x) {
        int cell = fgetc(file);
        if (!is_valid_cell(cell)) {
            return 0;
        }
        row[x] = (char)cell;
    }

    int line_end = fgetc(file);
    if (line_end == '\r') {
        line_end = fgetc(file);
    }
    if (line_end != '\n' && line_end != EOF) {
        return 0;
    }

    row[columns] = '\0';
    return 1;
}

int load_maze_from_file(const char* filename) {
    /* The file format begins with: <columns> <rows>. */
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("无法打开迷宫文件");
        return 0;
    }

    if (fscanf(file, "%d %d", &COLS, &ROWS) != 2 ||
        ROWS <= 0 || ROWS > MAX_ROWS || COLS <= 0 || COLS > MAX_COLS) {
        fprintf(stderr, "迷宫尺寸无效，允许范围是 1 到 %d 行、1 到 %d 列。\n",
                MAX_ROWS, MAX_COLS);
        fclose(file);
        return 0;
    }

    /* Do not permit maze data to start on the dimensions line. */
    int line_end = fgetc(file);
    if (line_end == '\r') {
        line_end = fgetc(file);
    }
    if (line_end != '\n') {
        fprintf(stderr, "迷宫尺寸后缺少换行符。\n");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < ROWS; i++) {
        if (!read_maze_row(file, maze[i], COLS)) {
            fprintf(stderr, "迷宫第 %d 行格式无效。\n", i + 1);
            fclose(file);
            return 0;
        }
    }

    /* Extra whitespace is harmless, but extra maze data is a format error. */
    int extra;
    while ((extra = fgetc(file)) != EOF) {
        if (!isspace((unsigned char)extra)) {
            fprintf(stderr, "迷宫数据超出声明的尺寸。\n");
            fclose(file);
            return 0;
        }
    }

    fclose(file);

    /* A solvable maze has exactly one start marker and one destination marker. */
    int start_x, start_y, end_x, end_y;
    if (!find_maze_marker('S', &start_x, &start_y) ||
        !find_maze_marker('E', &end_x, &end_y)) {
        fprintf(stderr, "迷宫必须且只能包含一个起点 S 和一个终点 E。\n");
        return 0;
    }

    return 1;
}

/*
 * Locate a marker only when it appears exactly once.
 *
 * Returning false for zero or multiple matches makes callers avoid ambiguous
 * maze states.  Coordinates are written only for matching cells; callers
 * should therefore use them only after a successful return value.
 */
int find_maze_marker(char marker, int* x, int* y) {
    int count = 0;

    if (!x || !y) {
        return 0;
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == marker) {
                *x = j;
                *y = i;
                ++count;
            }
        }
    }

    return count == 1;
}

void print_maze(void) {
    /* The precision prevents output from extending beyond the declared width. */
    for (int i = 0; i < ROWS; i++) {
        printf("%.*s\n", COLS, maze[i]);
    }
}
