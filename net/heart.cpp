#include <Windows.h>
#include <stdio.h>
#include <math.h>
void Draw1();
void Draw2();
void Draw3();
int nodeNum = 46;
int moveX = 5;
int moveY = 9;
int arr[][2] = {4, 10, 3, 11, 2, 12, 1, 13, 0, 14, 0, 15, 0, 16, 1, 17,
2, 18, 3, 19, 4, 20, 5, 20, 6, 20, 7, 20, 8, 19, 9, 18,
10, 17, 11, 16, 12, 15, 13, 14, 14, 13, 15, 12, 16, 11, 17, 10,
16, 9, 15, 8, 14, 7, 13, 6, 12, 5, 11, 4, 10, 3, 9, 2,
8, 1, 7, 0, 6, 0, 5, 0, 4, 0, 3, 1, 2, 2, 1, 3,
0, 4, 0, 5, 0, 6, 1, 7, 2, 8, 3, 9};
void gotoxy(int x, int y)
{
COORD pos;
pos.X = y * 2;
pos.Y = x;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}
void hidecursor()
{
CONSOLE_CURSOR_INFO cursor_info ={1,0};
SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}
int main()
{
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
hidecursor();
Draw1();
Draw2();
Draw3();
return 0;
}

void Draw1()
{
while(1)
{
for(int i = 0; i < nodeNum; i++)
{
gotoxy(arr[i][0] + moveX, arr[i][1] + moveY);
printf("¡ö");
Sleep(10);
}Draw2();
}
}
void Draw2()
{
int printNodeNum = 30;
while(1)
{
for(int i = 0; i < nodeNum; i++)
{
for(int j = 0; j < printNodeNum; j++)
{
gotoxy(arr[i - j < 0 ? nodeNum + (i - j) : i - j][0] + moveX, arr[i - j < 0 ? nodeNum + (i - j) : i - j][1] + moveY);
printf("¡ö");
}
Sleep(20);
int j;
for( j = 0; j < printNodeNum; j++)
{
gotoxy(arr[i - j < 0 ? nodeNum + (i - j) : i - j][0] + moveX, arr[i - j < 0 ? nodeNum + (i - j) : i - j][1] + moveY);
printf(" ");
}
}Draw3();
}
}
void Draw3()
{
int printNodeNum = nodeNum;
while(1)
{
for(int i = 0; i < nodeNum; i++)
{
for(int j = 0; j < printNodeNum; j += 2)
{
gotoxy(arr[i - j < 0 ? nodeNum + (i - j) : i - j][0] + moveX, arr[i - j < 0 ? nodeNum + (i - j) : i - j][1] + moveY);
printf("¡ö");
}
Sleep(30);

int j;
for( j = 0; j < printNodeNum; j += 2)
{
gotoxy(arr[i - j < 0 ? nodeNum + (i - j) : i - j][0] + moveX, arr[i - j < 0 ? nodeNum + (i - j) : i - j][1] + moveY);
printf(" ");
}
}Draw1();
}
}

