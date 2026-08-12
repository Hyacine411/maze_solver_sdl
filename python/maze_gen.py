import os
import random
import sys

print("当前工作目录:", os.getcwd())

sys.setrecursionlimit(10000)  # 设置最大递归深度为 10000（默认是 1000）

def generate_maze(width, height):
    # 所有位置初始化为墙（1）
    maze = [[1 for _ in range(width)] for _ in range(height)]

    def dfs(x, y):
        directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]
        random.shuffle(directions)

        for dx, dy in directions:
            nx, ny = x + dx * 2, y + dy * 2
            if 0 < nx < width and 0 < ny < height and maze[ny][nx] == 1:
                maze[ny][nx] = 0
                maze[y + dy][x + dx] = 0
                dfs(nx, ny)

    # 确保起点坐标为奇数（迷宫路径格）
    start_x, start_y = 1, 1
    maze[start_y][start_x] = 0
    dfs(start_x, start_y)

    return maze

def write_maze_to_file(maze, filepath):
    try:
        height = len(maze)
        width = len(maze[0])

        # 创建 assets 目录
        os.makedirs(os.path.dirname(filepath), exist_ok=True)

        with open(filepath, "w") as f:
            f.write(f"{width} {height}\n")
            for y in range(height):
                row = ""
                for x in range(width):
                    if (y == 1 and x == 1):
                        row += 'S'  # 起点
                    elif (y == height - 2 and x == width - 2):
                        row += 'E'  # 终点
                    else:
                        row += ' ' if maze[y][x] == 0 else '#'
                f.write(row + "\n")

        print(f"✅ 迷宫已保存到 {filepath}（大小：{width}x{height}）")
    except Exception as e:
        print(f"❌ 保存迷宫时出错: {e}")

if __name__ == "__main__":
    width, height = 91, 91  # 推荐奇数
    maze = generate_maze(width, height)
    write_maze_to_file(maze, "../assets/maze.txt")

# 在脚本末尾添加测试代码
# test_path = "test_maze.txt"
# with open(test_path, "w") as f:
#     f.write("test")
# print(f"测试文件已保存到: {test_path}")