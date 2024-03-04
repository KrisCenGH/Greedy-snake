/*贪吃蛇(定义)*/

#include "snake.hpp"

int snake::start()
{
	std::cout << "Welcome to this game!" << std::endl;
	std::cout << "'F': Play; 'Q': Quit;" << std::endl;
	char userKey;
	std::cin >> userKey;
	switch (userKey) // 初始用户界面，开始或退出
	{
		case 'F':
		case 'f':
			break;

		case 'Q':
		case 'q':
		default:
			return -1;
	}

	initial();

	while (true) // 不断保持游戏运行
	{
		bool keyValid = false; // 有效键标记
		if (_kbhit()) { // 接受并准备处理键盘有效输入
			char currKey = _getch();
			if (currKey == 'w' || currKey == 'W') { // 只处理拐弯情况
				if (headDirect == 'W' || headDirect == 'E')
					headDirect = 'N', --headY, keyValid = true;
			} else if (currKey == 'a' || currKey == 'A') {
				if (headDirect == 'N' || headDirect == 'S')
					headDirect = 'W', --headX, keyValid = true;
			} else if (currKey == 's' || currKey == 'S') {
				if (headDirect == 'W' || headDirect == 'E')
					headDirect = 'S', ++headY, keyValid = true;
			} else if (currKey == 'd' || currKey == 'D') {
				if (headDirect == 'N' || headDirect == 'S')
					headDirect = 'E', ++headX, keyValid = true;
			} else if (currKey == 'q' || currKey == 'Q') { // 游戏过程中，可按'Q'退出游戏
				break;
			}
		}

		if (!keyValid) { // 说明无有效输入，蛇正常前进
			if (headDirect == 'N')
				--headY;
			else if (headDirect == 'S')
				++headY;
			else if (headDirect == 'W')
				--headX;
			else if (headDirect == 'E')
				++headX;
		}

		Sleep(speedms); // 刷新频率控制（游戏速度）

		snakeUpdate(); // 渲染

		if (!hitJudge()) // 碰到障碍物或自身，游戏结束
			break;
	}

	cursorRelocate(wallLength / 2 - 20, wallWidth / 2); // 居中弹出游戏结束提示
	std::cout << "Game Over! Your final score is " << score << "." << std::endl;

	return 0;
}

void snake::initial()
{
	system("cls"); // 清屏

	wallBuild();

	cursorRelocate(wallLength + 4, 0);
	std::cout << "Score:" << score; // 分数显示于游戏右上角

	snakeCoordinate.push_back({wallLength / 2, wallWidth / 2 + 2}); // 蛇初始为三个单位长度(包括蛇头)，居于地图中央
	obstaclesCoordinate[{wallLength / 2, wallWidth / 2 + 2}] = true;

	snakeCoordinate.push_back({wallLength / 2, wallWidth / 2 + 1});
	obstaclesCoordinate[{wallLength / 2, wallWidth / 2 + 1}] = true;

	snakeCoordinate.push_back({wallLength / 2, wallWidth / 2}), headX = wallLength / 2, headY = wallWidth / 2;

	preSnakeBodyX = wallLength / 2, preSnakeBodyY = wallWidth / 2 + 3; // 准备处理蛇身后续的变长行为

	cursorRelocate(headX, headY);
	std::cout << '@';
	cursorRelocate(wallLength / 2, wallWidth / 2 + 1);
	std::cout << '#';
	cursorRelocate(wallLength / 2, wallWidth / 2 + 2);
	std::cout << '#';

	fruitBuild();
}

void snake::wallBuild()
{
	for (int i = 0; i < wallLength; ++i) // 画上
		std::cout << 'W', obstaclesCoordinate[{i, 0}] = true;

	cursorRelocate(0, 1);

	for (int i = 0; i < wallWidth - 1; ++i) // 画左
		std::cout << 'W' << std::endl, obstaclesCoordinate[{0, i + 1}] = true;

	for (int i = 0; i < wallLength - 1; ++i) // 画下
		std::cout << 'W', obstaclesCoordinate[{i + 1, wallWidth - 1}] = true;

	cursorRelocate(wallLength - 1, 1);

	for (int i = 0; i < wallWidth; ++i) // 画右
		std::cout << 'W', cursorRelocate(wallLength - 1, i + 2), obstaclesCoordinate[{wallLength - 1, i + 1}] = true;
}

void snake::fruitBuild()
{
	std::random_device rd;
    std::mt19937 seed(rd()); // 生成种子

	while (true) // 随机生成果子坐标
	{
		std::uniform_int_distribution<int> dis_x(1, wallLength - 2);
		std::uniform_int_distribution<int> dis_y(1, wallWidth - 2);
		fruitX = dis_x(seed);
		fruitY = dis_y(seed);

		if ((fruitX != headX || fruitY != headY) && !obstaclesCoordinate[{fruitX, fruitY}]) // 如果与蛇或墙体的坐标重合，则重新生成
			break;
	}

	cursorRelocate(fruitX, fruitY); // 重定位光标，画出水果
	std::cout << '$';
}

void snake::snakeUpdate()
{
	cursorRelocate(snakeCoordinate.back().first, snakeCoordinate.back().second); // 队尾固定为蛇头，每移动一格，蛇头都会被覆盖为身体
	obstaclesCoordinate[{snakeCoordinate.back().first, snakeCoordinate.back().second}] = true;
	std::cout << '#'; // 进行覆盖

	snakeCoordinate.push_back({headX, headY}); // 放入新蛇头
	cursorRelocate(headX, headY);
	std::cout << '@'; // 进行覆盖

	preSnakeBodyX = snakeCoordinate.front().first, preSnakeBodyY = snakeCoordinate.front().second; // 保存尾巴，方便后续“变长”
	cursorRelocate(snakeCoordinate.front().first, snakeCoordinate.front().second); // 准备消除尾巴
	obstaclesCoordinate[{snakeCoordinate.front().first, snakeCoordinate.front().second}] = false;
	std::cout << ' '; // 进行覆盖
	snakeCoordinate.pop_front(); // 弹出队列，保持原有长度
}

bool snake::hitJudge()
{
	if (obstaclesCoordinate[{headX, headY}]) { // 碰到障碍物或自身，游戏结束
		return false;
	} else if (headX == fruitX && headY == fruitY) { // 吃到果子，加分，身体变长，重新生成果子
		++score, cursorRelocate(wallLength + 10, 0), std::cout << score; // 加分

		snakeCoordinate.push_front({preSnakeBodyX, preSnakeBodyY}); // 从尾部伸长
		obstaclesCoordinate[{preSnakeBodyX, preSnakeBodyY}] = true;
		cursorRelocate(preSnakeBodyX, preSnakeBodyY);
		std::cout << '#';

		fruitBuild();
	}

	return true;
}

void snake::cursorRelocate(const int X, const int Y)
{
	COORD crd;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	crd.X = X, crd.Y = Y;
	SetConsoleCursorPosition(handle, crd);
}