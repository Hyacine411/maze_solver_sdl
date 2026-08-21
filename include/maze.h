#ifndef MAZE_H
#define MAZE_H

#define MAX_ROWS 100 /* Maximum number of rows accepted from a maze file. */
#define MAX_COLS 100 /* Maximum number of columns accepted from a maze file. */

extern int ROWS, COLS;
/* Each row includes one extra byte for its terminating '\0'. */
extern char maze[MAX_ROWS][MAX_COLS + 1];

/* Load and validate the complete text maze format into the global maze grid. */
int load_maze_from_file(const char* filename);
/* Return nonzero only if marker occurs exactly once, then write its position. */
int find_maze_marker(char marker, int* x, int* y);
/* Print the currently loaded maze without any solver or display overlay. */
void print_maze(void);

#endif
