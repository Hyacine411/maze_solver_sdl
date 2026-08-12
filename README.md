```Maze Solver — Mouse and Cheese  

作者：Chiharu280
版本：1.0

本项目使用 C 语言编写，实现了通过递归算法自动求解迷宫路径，并使用 SDL2 实现动态可视化展示。迷宫起点有一只老鼠，终点为奶酪，过程清晰可视，支持自定义迷宫大小。

---

编译方法（Ubuntu）

确保已安装 SDL2 开发库，终端运行下面的命令来安装或确认是否已安装：

sudo apt install libsdl2-dev

然后在项目根目录下执行：

gcc src/*.c -Iinclude -o maze_solver `sdl2-config --cflags --libs`

进行编译

或者使用Makefile进行便捷编译

make

调用Makefile文件进行快捷编译

编译完成后，运行主程序：

./maze_solver

程序会：

    从 assets/maze.txt 加载迷宫

    执行路径搜索（DFS）

    弹出 SDL2 窗口动态展示“老鼠走向奶酪”的过程

关闭窗口后，终端将输出路径信息，并生成迷宫地图

运行完成后，也可以使用Makefile清除可执行文件

make clean

---

项目自带 Python 脚本自动生成复杂迷宫：

python3 maze_gen.py

默认生成一个 71x71 的迷宫并保存为 assets/maze.txt。

你也可以自定义尺寸（建议奇数且小于100）：

python3 maze_gen.py --width 51 --height 51

    脚本会确保 S 起点 和 E 终点 已正确放置。

---

目录结构说明
目录   文件说明
src/	所有 C 源码文件
include/	头文件目录
assets/maze.txt	当前加载的迷宫文件
assets/mouse.bmp	起点图标（老鼠）
assets/cheese.bmp	终点图标（奶酪）
maze_gen.py	Python 迷宫生成器脚本
README.md	项目说明文件

---

功能特点

    DFS 自动求解迷宫路径

    支持任意大小迷宫（推荐奇数维度）

    SDL2 动态可视化老鼠路径演示

    支持生成路径、路径坐标打印

    图标可替换，支持自定义 BMP 图

---

依赖项

名称	用途
SDL2	动态绘制窗口
Pillow	Python 生成 BMP 图
Python 3	运行 maze_gen.py

---

致谢 & 建议

    若使用非常大的迷宫（>200×200），建议使用非递归 DFS 或 BFS 改进求解效率。

    欢迎反馈 bug 或提出功能增强建议！

© 2025 Chiharu280 — Maze Solver Project
