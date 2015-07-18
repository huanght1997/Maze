#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAZESIZE 48 // 迷宫大小，可以修改。
#define WALL '#'    // 墙壁的表示符号
#define ROAD '.'    // 表示此处可通行，算法中也用来表示这里已经被访问过
#define GOODWAY 'X' // 迷宫正确解法的路径
#define UNVISITED 'R'// 没有被访问的房间
#define ENTRANCE 'I' // 入口标记
#define EXIT 'O'    //  出口标记
// UP后的语句表示，如果上方的房子没有访问过，就把现在房子和上方房子之间的墙变成路，并进入上方的房子，进行与刚才的房子同样的随机挖墙操作
#define UP if (maze[posx-2][posy] == UNVISITED)\
    {\
        maze[posx-1][posy] = ROAD;\
        dig_wall(maze, posx-2, posy);\
    } break     // break用于switch
#define DOWN if (maze[posx+2][posy] == UNVISITED)\
    {\
        maze[posx+1][posy] = ROAD;\
        dig_wall(maze, posx+2, posy);\
    } break
#define LEFT if (maze[posx][posy-2] == UNVISITED)\
    {\
        maze[posx][posy-1] = ROAD;\
        dig_wall(maze, posx, posy-2);\
    } break
#define RIGHT if (maze[posx][posy+2] == UNVISITED)\
    {\
        maze[posx][posy+1] = ROAD;\
        dig_wall(maze, posx, posy+2);\
    } break
void mazeGenerator(char maze[][MAZESIZE]);
void dig_wall(char maze[][MAZESIZE], int posx, int posy);
void mazeTraverse(char maze[][MAZESIZE], int posx, int posy);
int main(void)
{
    char maze[MAZESIZE][MAZESIZE];
    int i, j;
    // 生成一个迷宫
    mazeGenerator(maze);
    printf("按回车键查看解法...\n");
    getchar();
    // 解迷宫并生成路径，用X表示
    mazeTraverse(maze, 0, 0);
    // 显示字符数组
    for (i = 0; i < MAZESIZE; i++)
    {
        for (j = 0; j < MAZESIZE; j++)
            putchar(maze[i][j]);
        putchar('\n');
    }
    // 按任意键继续
    getchar();
    return 0;
}

void mazeGenerator(char maze[][MAZESIZE])
{
    int i, j;
    // 将这一块区域分成一个一个相邻的由一层墙壁隔开的房间
    for (i = 0; i < MAZESIZE; i++)
        for (j = 0; j < MAZESIZE; j++)
        {
            if (i%2 == 1)           // 奇数行全变成墙
                maze[i][j] = WALL;
            else if (j%2 == 1)      // 奇数列全变成墙
                maze[i][j] = WALL;
            else                    // 其余的都是占一格的小房子，都未被访问
                maze[i][j] = UNVISITED;
        }
    // 设置出口通道
    maze[MAZESIZE-1][MAZESIZE-2] = ROAD;
    maze[MAZESIZE-1][MAZESIZE-1] = EXIT;    // 最后一行最后一列为出口

    // 开始挖墙。从起点出发，随机打通隔壁的墙，只要隔壁没有被访问过。
    // 这么做会使迷宫没有一种转圈的结构，保证迷宫只有一条路径正确
    srand((unsigned)time(NULL));    // 随机数种子，使得每次生成的迷宫都不相同
    dig_wall(maze, 0, 0);           // 从起点开始挖墙
    // 迷宫做好后，将第一行第一列作为迷宫入口
    maze[0][0] = ENTRANCE;

    // 展示迷宫
    for (i = 0; i < MAZESIZE; i++)
    {
        for (j = 0; j < MAZESIZE; j++)
            putchar(maze[i][j]);
        putchar('\n');
    }
}
// 解迷宫的函数，posx,posy是现在正在进行尝试的点
void mazeTraverse(char maze[][MAZESIZE], int posx, int posy)
{
    int i, j;
    // 建立一个与迷宫数组元素一一对应的数组，来记住这个地方是否来过，初始化为都没来过
    static int wasHere[MAZESIZE][MAZESIZE];
    if (posx == MAZESIZE-1 && posy == MAZESIZE-1)// 如果到达了终点，成功，返回
    {
        maze[posx][posy] = GOODWAY;
        return;
    }
    if (maze[posx][posy] == WALL || wasHere[posx][posy])
        return;     // 如果尝试的结果是这里是墙或者这里已经来过，退回上一步
    wasHere[posx][posy] = 1;    // 如果没有来过，那么做好标记说明现在到过这里
    if (posx != 0)  // 如果现在尝试的地方不在迷宫的最左边
    {
        mazeTraverse(maze, posx-1, posy);   // 尝试往左边走，对其左边的格子进行相同判断
        if (maze[posx-1][posy] == GOODWAY)  // 如果此路通
        {
            maze[posx][posy] = GOODWAY;     // 做路径正确的标记
            return;
        }
    }
    if (posx != MAZESIZE-1) // 如果不在迷宫的最右边
    {
        mazeTraverse(maze, posx+1, posy);   // 尝试往右边走
        if (maze[posx+1][posy] == GOODWAY)
        {
            maze[posx][posy] = GOODWAY;
            return;
        }
    }
    if (posy != 0)  // 如果不在迷宫最上边
    {
        mazeTraverse(maze, posx, posy-1);   // 尝试往上走
        if (maze[posx][posy-1] == GOODWAY)
        {
            maze[posx][posy] = GOODWAY;
            return;
        }
    }
    if (posy != MAZESIZE-1) // 如果不在迷宫最下边
    {
        mazeTraverse(maze, posx, posy+1);   // 尝试往下走
        if (maze[posx][posy+1] == GOODWAY)
        {
            maze[posx][posy] = GOODWAY;
            return;
        }
    }
    /* 到这里如果依旧没有标记GOODWAY，说明上下左右走都不对，死路，直接返回，从而前面的
       都不标记GOODWAY（因为要尝试的地方四周有一处为GOODWAY才可以标记GOODWAY）。
       如果到了终点，说明这路一直都是对的，终点标为GOODWAY，然后逐层返回，将上一次走的
       地方也标为GOODWAY，最后到达这个递归函数的最开始的入口(0,0)，函数递归完毕，正确
       路径也被标记好了。*/
}

void dig_wall(char maze[][MAZESIZE], int posx, int posy)
{
    // 挖到了这里，则这里就被访问过，也形成了一条路（哪怕是死路）
    maze[posx][posy] = ROAD;
    int tried[4] = {0,0,0,0};   // 这个地方已经尝试过挖墙的四个方向，这里分别代表上下左右，0代表还未尝试，1代表已经尝试
    // 在边界处，令边界外的地方为已经尝试过的地方
    if (posx == 0)      // 上边界
        tried[0] = 1;   // 将上方向定为已经尝试过挖墙，使得其不会再往上挖
    if (posx == MAZESIZE-2) // 下边界，-2的原因是最后一排小房子的行数为此
        tried[1] = 1;
    if (posy == 0)      // 左边界
        tried[2] = 1;
    if (posy == MAZESIZE-2) // 右边界，-2的原因与下边界相似
        tried[3] = 1;
    int direction;  // 表示方向
    while (tried[0]*tried[1]*tried[2]*tried[3] == 0)    // 只要有一个方向没有尝试挖墙
    {
        direction = rand() % 4;         // 随机产生0123中的一个，达到随机挖墙的目的
        if (tried[direction] == 1)      // 如果这个方向已经尝试过，则再来一次，直到获得没有尝试过的方向
            continue;
        tried[direction] = 1;       // 现在这个方向已经尝试过了
        switch (direction)          // 挖墙的动作
        {
            case 0: UP;         // UP代表尝试向上挖，如果上方的房子没有被访问过，则挖通现在所在的房子和上方房子之间的墙，将墙所在地和上方房子所在地作为一条路径，然后进入上边的房子，上边的房子便成为现在所在的房子，进行相同的挖墙行为。这一系列行为在开头用#define表示过了。
            case 1: DOWN;   // 尝试向下挖
            case 2: LEFT;   // 向左
            case 3: RIGHT;  // 向右
        }
    }
}
