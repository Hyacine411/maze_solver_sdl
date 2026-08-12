// src/visualize.c
#include <SDL2/SDL.h>
#include <stdio.h>
#include "maze.h"

#define TILE_SIZE 8
#define DELAY_MS 25   // 老鼠移动速度

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* mouse_texture = NULL;
SDL_Texture* cheese_texture = NULL;

int init_sdl(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL 初始化失败: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("Maze Solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width * TILE_SIZE, height * TILE_SIZE, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "窗口创建失败: %s\n", SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "渲染器创建失败: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

SDL_Texture* load_texture(const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) {
        fprintf(stderr, "加载BMP失败: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void draw_maze(char maze[][MAX_COLS], int path_x[], int path_y[], int path_len) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

            if (maze[y][x] == '#') {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            } else if (maze[y][x] == '.') {
                SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            } else {
                SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
                SDL_RenderFillRect(renderer, &rect);
            }

            if (maze[y][x] == 'E') {
                SDL_RenderCopy(renderer, cheese_texture, NULL, &rect);
            }

            if (maze[y][x] == 'S') {
                SDL_RenderCopy(renderer, mouse_texture, NULL, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);

    // 修正方向：老鼠从起点向终点移动
    for (int i = path_len - 1; i >= 0; i--) {
        if (i < path_len - 1 && maze[path_y[i + 1]][path_x[i + 1]] != 'S' && maze[path_y[i + 1]][path_x[i + 1]] != 'E') {
            maze[path_y[i + 1]][path_x[i + 1]] = '.';
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

                if (maze[y][x] == '#') {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &rect);
                } else if (maze[y][x] == '.') {
                    SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
                    SDL_RenderFillRect(renderer, &rect);
                } else {
                    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
                    SDL_RenderFillRect(renderer, &rect);
                }

                if (maze[y][x] == 'E') {
                    SDL_RenderCopy(renderer, cheese_texture, NULL, &rect);
                }

                if (maze[y][x] == 'S') {
                    SDL_RenderCopy(renderer, mouse_texture, NULL, &rect);
                }
            }
        }

        SDL_Rect mouse_rect = {
            path_x[i] * TILE_SIZE,
            path_y[i] * TILE_SIZE,
            TILE_SIZE,
            TILE_SIZE
        };
        SDL_RenderCopy(renderer, mouse_texture, NULL, &mouse_rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(DELAY_MS);
    }

    if (path_len > 0 && maze[path_y[0]][path_x[0]] != 'E' && maze[path_y[0]][path_x[0]] != 'S') {
        maze[path_y[0]][path_x[0]] = '.';
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

            if (maze[y][x] == '#') {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            } else if (maze[y][x] == '.') {
                SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            } else {
                SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
                SDL_RenderFillRect(renderer, &rect);
            }

            if (maze[y][x] == 'E') {
                SDL_RenderCopy(renderer, cheese_texture, NULL, &rect);
            }

            if (maze[y][x] == 'S') {
                SDL_RenderCopy(renderer, mouse_texture, NULL, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void close_sdl() {
    SDL_DestroyTexture(mouse_texture);
    SDL_DestroyTexture(cheese_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int run_visualization(char maze[][MAX_COLS], int path_x[], int path_y[], int path_len) {
    if (!init_sdl(COLS, ROWS)) return 0;

    mouse_texture = load_texture("assets/mouse.bmp");
    cheese_texture = load_texture("assets/cheese.bmp");
    if (!mouse_texture || !cheese_texture) return 0;

    draw_maze(maze, path_x, path_y, path_len);

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = 1;
        }
    }

    close_sdl();
    return 1;
}
