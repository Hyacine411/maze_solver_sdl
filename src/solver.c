#include <stdio.h>
#include "maze.h"
#include "solver.h"

/* Shared output buffers used by both search implementations. */
int path_len = 0;
int path_x[MAX_PATH_LEN];
int path_y[MAX_PATH_LEN];

/* A grid coordinate used by BFS queues and predecessor links. */
typedef struct {
    int x;
    int y;
} Point;

/* Four-neighbour movement; diagonal movement is intentionally unsupported. */
static const int DIRECTIONS[4][2] = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}
};

/*
 * Append one coordinate to the shared path buffers.
 *
 * Both DFS and BFS store a route from the destination back to the start.  A
 * common append helper keeps their bounds checking consistent.
 */
static int append_path_point(int x, int y) {
    if (path_len >= MAX_PATH_LEN) {
        fprintf(stderr, "路径长度超过迷宫允许的最大值。\n");
        return 0;
    }

    path_x[path_len] = x;
    path_y[path_len] = y;
    ++path_len;
    return 1;
}

/*
 * Recursive worker for the optional depth-first-search implementation.
 *
 * A dot marks a cell currently visited by DFS.  On a dead end the original
 * character is restored, which is the backtracking step.  DFS is retained for
 * comparison purposes; unlike BFS it does not guarantee a shortest route.
 */
static int solve_maze_dfs(char maze[][MAX_COLS + 1], int x, int y) {
    if (x < 0 || x >= COLS || y < 0 || y >= ROWS) return 0;
    if (maze[y][x] == '#' || maze[y][x] == '.') return 0;

    if (maze[y][x] == 'E') {
        return append_path_point(x, y);
    }

    char temp = maze[y][x];
    maze[y][x] = '.';

    for (int i = 0; i < 4; ++i) {
        if (solve_maze_dfs(maze, x + DIRECTIONS[i][0], y + DIRECTIONS[i][1])) {
            return append_path_point(x, y);
        }
    }

    maze[y][x] = temp;
    return 0;
}

int solve_maze(char maze[][MAX_COLS + 1], int x, int y) {
    /* Public wrapper: reset output state before every independent DFS run. */
    path_len = 0;
    if (!solve_maze_dfs(maze, x, y)) {
        path_len = 0;
        return 0;
    }
    return 1;
}

/*
 * Breadth-first search (BFS).
 *
 * BFS processes all cells one step away from the start before moving on to
 * cells two steps away, and so on.  Because every move costs one grid step,
 * the first visit to E is guaranteed to be a shortest path.  `previous`
 * records where each cell was first reached; following those links from E
 * reconstructs the route in the end-to-start order expected by the renderer.
 * This algorithm reads the maze but does not modify it.
 */
int solve_maze_bfs(char maze[][MAX_COLS + 1], int start_x, int start_y) {
    Point queue[MAX_PATH_LEN];
    int front = 0;
    int rear = 0;
    int visited[MAX_ROWS][MAX_COLS] = {0};
    Point previous[MAX_ROWS][MAX_COLS];
    Point end = {-1, -1};

    /* Discard a result from any earlier solver invocation. */
    path_len = 0;

    if (start_x < 0 || start_x >= COLS || start_y < 0 || start_y >= ROWS ||
        maze[start_y][start_x] == '#') {
        return 0;
    }

    /* A (-1, -1) predecessor denotes an unreached cell. */
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            previous[y][x].x = -1;
            previous[y][x].y = -1;
        }
    }

    queue[rear++] = (Point){start_x, start_y};
    visited[start_y][start_x] = 1;

    /* The queue uses monotonically increasing front/rear indices. */
    while (front < rear) {
        Point current = queue[front++];

        if (maze[current.y][current.x] == 'E') {
            end = current;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int next_x = current.x + DIRECTIONS[i][0];
            int next_y = current.y + DIRECTIONS[i][1];

            if (next_x < 0 || next_x >= COLS || next_y < 0 || next_y >= ROWS ||
                visited[next_y][next_x] || maze[next_y][next_x] == '#') {
                continue;
            }

            if (rear >= MAX_PATH_LEN) {
                fprintf(stderr, "BFS 队列已满。\n");
                return 0;
            }

            /* Mark at enqueue time so a cell cannot be queued twice. */
            visited[next_y][next_x] = 1;
            previous[next_y][next_x] = current;
            queue[rear++] = (Point){next_x, next_y};
        }
    }

    if (end.x == -1) {
        return 0;
    }

    /* Follow predecessor links from E until the original start is reached. */
    for (Point current = end;; current = previous[current.y][current.x]) {
        if (path_len >= MAX_PATH_LEN) {
            fprintf(stderr, "BFS 路径过长。\n");
            path_len = 0;
            return 0;
        }

        path_x[path_len] = current.x;
        path_y[path_len] = current.y;
        ++path_len;

        if (current.x == start_x && current.y == start_y) {
            break;
        }
    }

    return 1;
}
