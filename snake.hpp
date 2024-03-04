/*贪吃蛇(头文件)*/

/**
 * 说明：
 * Windows操作系统, C++11及以上编译器可编译链接。
 * 使用小驼峰命名法
 * “@”：蛇头；“#”：蛇身；“$”：果子；“W”：墙壁（如使用其他字符，需特别注意字符编码问题）
 * 按键: ‘W’‘A’‘S’‘D’/‘上’‘左’‘下’‘右’
 * 																					2024-2-24
*/
#ifndef _SNAKE_
#define _SNAKE_

#include <iostream>
#include <map>
#include <deque>
#include <random>
#include <conio.h>
#include <windows.h>

class snake
{
private:
	int headX; // 蛇头坐标
	int headY;
	int fruitX; // 果子坐标
	int fruitY;
	char headDirect = 'N'; // 蛇头朝向，初始为朝北
	std::map<std::pair<int, int>, bool> obstaclesCoordinate; // key标识地图坐标，value标识是否有墙体以及蛇身（true为有，false为无）
	std::deque<std::pair<int, int>> snakeCoordinate; // 蛇头与蛇身坐标标识（双端队列实现）
	int preSnakeBodyX; // 标识前一个被“出队”的蛇身坐标，以便于当蛇吃到果子时，蛇身能够正确伸长
	int preSnakeBodyY;
	int wallLength; // 墙体长度与宽度（可自定义，不建议低于10）
	int wallWidth;
	int speedms = 150; // 游戏速度
	int score = 0; // 当前游戏分数

public:
	snake(const int length = 100, const int width = 40) : wallLength(length), wallWidth(width) { } // 自定义墙体长度与宽度

	// 用户调用此函数以开始游戏，成功启动返回0，否则返回-1
	int start();

private:
	// 初始化游戏（确定蛇初始位置（居中），果子初始位置，构建墙体，显示分数）
	void initial();

	// 构建墙体
	void wallBuild();

	// 生成果子
	void fruitBuild();

	// 在每一个单位时刻更新绘制蛇头与蛇身
	void snakeUpdate();

	// 碰撞判定（墙体，蛇身，果子），如碰到果子或未碰到任何物体，则返回true，否则返回false
	bool hitJudge();

	// 光标重定位，负责将光标重定位至参数所对应的列x与行y
	void cursorRelocate(const int X, const int Y);

};

#endif