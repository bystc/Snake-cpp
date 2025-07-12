
#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>
#define H 22
#define W 22
using namespace std;

class chessboard
{
public:
  char qp[H][W];
  int i, j, x1, y1;
  chessboard(); // 构造函数
  void food();
  void prt(int grade, int score, int gamespeed);
};
chessboard::chessboard() // 初始化地图
{
  for (i = 1; i <= H - 2; i++)
    for (j = 1; j <= W - 2; j++)
      qp[i][j] = ' '; // 中间空白区域
  for (i = 0; i <= H - 1; i++)
    qp[0][i] = qp[H - 1][i] = '#';
  for (i = 1; i <= H - 2; i++)
    qp[i][0] = qp[i][W - 1] = '#'; // 边框
  food();                          // 初始化食物
}
void chessboard::food() // 随机产生食物
{
  srand(time(0));
  do
  {
    x1 = rand() % W - 2 + 1;
    y1 = rand() % H - 2 + 1;
  } while (qp[x1][y1] != ' ');
  qp[x1][y1] = '$'; // 生成食物
}
void chessboard::prt(int grade, int score, int gamespeed) // 打印
{
  system("cls");//实现动画效果
  cout << endl;
  for (i = 1; i < H-1; i++)
  {
    cout << "\t";
    for (j = 1; j < W-1; j++)
      cout << qp[i][j] << ' ';
    if (i == 0)
      cout << "\tGrade:" << grade;
    if (i == 2)
      cout << "\tScore:" << score;
    if (i == 4)
      cout << "\tAutomatic forward";
    if (i == 5)
      cout << "\ttime interval:" << gamespeed << "ms";
    cout << endl;
  }
}
class snake : public chessboard // 继承了chessboard函数
{
public:
  int zb[2][100];
  long start;
  int head, tail, grade, score, gamespeed, length, timeover, x, y;
  char direction;
  snake();
  void move();
};
snake::snake() // 构造函数
{
  // cout<<"\n\n\t\tThe game is about to begin!"<<endl;
  // for(i=3;i>=0;i--)//倒计时效果
  // {
  //     start=clock();
  //     while(clock()-start<=1000);
  //     system("cls");//清屏
  //     if(i>0)
  //     cout << "\n\n\t\tCountdown:" << i << endl;
  // }
  for (i = 1; i <= 3; i++) // 初始化尾巴
    qp[1][i] = '*';
  qp[1][4] = '@';         // 初始化头
  for (i = 0; i < 4; i++) // zb数组存放打印点的横纵坐标，第一行x，第二行y
  {
    zb[0][i] = 1;
    zb[1][i] = i + 1;
  }
}
void snake::move()
{
  score = 0;
  head = 3, tail = 0;
  grade = 1, length = 4;
  gamespeed = 500;
  direction = 77; // 右
  while (1)
  {
    timeover = 1;
    start = clock();
    /*     有键盘输入时kbhit函数返回1，反之0。
        该空循环的作用，当到达时间或有键盘输入时跳出循环 */
    while ((timeover = (clock() - start <= gamespeed)) && !kbhit());

    //如果在超时前收到键盘信号，timeover=1，更新方向；如果超时，timeover=0，不更新方向，按原方向运动。
    if (timeover) 
    {
      getch(); // 按下某个字符时，无需回车自动读取
      direction = getch();
    }

    switch (direction) // 控制移动
    {                  // xy存放头的坐标
    case 72:
      x = zb[0][head] - 1;
      y = zb[1][head];
      break; // 上
    case 80:
      x = zb[0][head] + 1;
      y = zb[1][head];
      break; // 下
    case 75:
      x = zb[0][head];
      y = zb[1][head] - 1;
      break; // 左
    case 77:
      x = zb[0][head];
      y = zb[1][head] + 1;
      break; // 右
    }
    // if (x == 0 || x == 21 || y == 0 || y == 21) // 撞墙over
    // {
    //   system("cls");
    //   cout << "\tGame over!" << endl;
    //   system("pause");
    //   break;
    // }
    if (qp[x][y] != ' ' && !(x == x1 && y == y1)) // 到达的目标不为空且没吃到食物（撞墙、撞尾巴）over
    {
      system("cls");
      cout << "\tGame over!" << endl;
      system("pause");
      break;
    }
    if (x == x1 && y == y1) // 吃到食物那一帧的处理
    {
      length++;
      score = score + 100;
      if (length >= 8) // 随着时间推移越来越快，难度增大
      {
        length -= 8; // 每吃8个增加一次难度
        grade++;
        if (gamespeed >= 200)
          gamespeed = 550 - grade * 50;
      }
      qp[x][y] = '@';                     // 吃到食物后，食物处为头
      qp[zb[0][head]][zb[1][head]] = '*'; // 原本头变成*
      head = (head + 1) % 100;            // zb变长，取整数的最后二位数
      zb[0][head] = x;                    // 更新头坐标
      zb[1][head] = y;
      food(); // 吃到食物后更新食物
      prt(grade, score, gamespeed);
    }
    else // 没吃到食物移动
    {//实现贪吃蛇移动方式的算法
      qp[zb[0][tail]][zb[1][tail]] = ' '; // 更新最后的一个点为空
      tail = (tail + 1) % 100;//更新最后一个点后，倒数第二个点就成了尾巴            
      qp[zb[0][head]][zb[1][head]] = '*'; // 更新头点为*
      head = (head + 1) % 100;//%100是在数组溢出时回到开头重新使用
      zb[0][head] = x;
      zb[1][head] = y;
      qp[zb[0][head]][zb[1][head]] = '@'; // 更新头点
      prt(grade, score, gamespeed);
    }
  }
}
int main()
{
  chessboard cb;
  snake s;
  s.move();
}
