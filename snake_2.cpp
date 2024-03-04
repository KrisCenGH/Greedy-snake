/*贪吃蛇(定义)*/

#include "snake.hpp"

int snake::start()
{
	std::cout << "Welcome to this game!" << std::endl;
	std::cout << "'F': Play; 'Q': Quit;" << std::endl;
	char user_key;
	std::cin >> user_key;
	switch (user_key) // 初始用户界面，开始或退出
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
		bool key_valid = false; // 有效键标记
		if (_kbhit()) { // 接受并准备处理键盘有效输入
			char curr_key = _getch();
			if (curr_key == 'w' || curr_key == 'W') { // 只处理拐弯情况
				if (_head_direct == 'W' || _head_direct == 'E')
					_head_direct = 'N', --_head_y, key_valid = true;
			} else if (curr_key == 'a' || curr_key == 'A') {
				if (_head_direct == 'N' || _head_direct == 'S')
					_head_direct = 'W', --_head_x, key_valid = true;
			} else if (curr_key == 's' || curr_key == 'S') {
				if (_head_direct == 'W' || _head_direct == 'E')
					_head_direct = 'S', ++_head_y, key_valid = true;
			} else if (curr_key == 'd' || curr_key == 'D') {
				if (_head_direct == 'N' || _head_direct == 'S')
					_head_direct = 'E', ++_head_x, key_valid = true;
			} else if (curr_key == 'q' || curr_key == 'Q') { // 游戏过程中，可按'Q'退出游戏
				break;
			}
		}

		if (!key_valid) { // 说明无有效输入，蛇正常前进
			if (_head_direct == 'N')
				--_head_y;
			else if (_head_direct == 'S')
				++_head_y;
			else if (_head_direct == 'W')
				--_head_x;
			else if (_head_direct == 'E')
				++_head_x;
		}

		Sleep(_speed_ms); // 刷新频率控制（游戏速度）

		snake_update(); // 渲染

		if (!hit_judge()) // 碰到障碍物或自身，游戏结束
			break;
	}

	cursor_relocate(_wall_length / 2 - 20, _wall_width / 2); // 居中弹出游戏结束提示
	std::cout << "Game Over! Your final score is " << _score << "." << std::endl;

	return 0;
}

void snake::initial()
{
	system("cls"); // 清屏

	wall_build();

	cursor_relocate(_wall_length + 4, 0);
	std::cout << "Score:" << _score; // 分数显示于游戏右上角

	_snake_coordinate.push_back({_wall_length / 2, _wall_width / 2 + 2}); // 蛇初始为三个单位长度(包括蛇头)，居于地图中央
	_obstacles_coordinate[{_wall_length / 2, _wall_width / 2 + 2}] = true;

	_snake_coordinate.push_back({_wall_length / 2, _wall_width / 2 + 1});
	_obstacles_coordinate[{_wall_length / 2, _wall_width / 2 + 1}] = true;

	_snake_coordinate.push_back({_wall_length / 2, _wall_width / 2}), _head_x = _wall_length / 2, _head_y = _wall_width / 2;

	_pre_snake_body_x = _wall_length / 2, _pre_snake_body_y = _wall_width / 2 + 3; // 准备处理蛇身后续的变长行为

	cursor_relocate(_head_x, _head_y);
	std::cout << '@';
	cursor_relocate(_wall_length / 2, _wall_width / 2 + 1);
	std::cout << '#';
	cursor_relocate(_wall_length / 2, _wall_width / 2 + 2);
	std::cout << '#';

	fruit_build();
}

void snake::wall_build()
{
	for (int i = 0; i < _wall_length; ++i) // 画上
		std::cout << 'W', _obstacles_coordinate[{i, 0}] = true;

	cursor_relocate(0, 1);

	for (int i = 0; i < _wall_width - 1; ++i) // 画左
		std::cout << 'W' << std::endl, _obstacles_coordinate[{0, i + 1}] = true;

	for (int i = 0; i < _wall_length - 1; ++i) // 画下
		std::cout << 'W', _obstacles_coordinate[{i + 1, _wall_width - 1}] = true;

	cursor_relocate(_wall_length - 1, 1);

	for (int i = 0; i < _wall_width; ++i) // 画右
		std::cout << 'W', cursor_relocate(_wall_length - 1, i + 2), _obstacles_coordinate[{_wall_length - 1, i + 1}] = true;
}

void snake::fruit_build()
{
	std::random_device rd;
    std::mt19937 seed(rd()); // 生成种子

	while (true) // 随机生成果子坐标
	{
		std::uniform_int_distribution<int> dis_x(1, _wall_length - 2);
		std::uniform_int_distribution<int> dis_y(1, _wall_width - 2);
		_fruit_x = dis_x(seed);
		_fruit_y = dis_y(seed);

		if ((_fruit_x != _head_x || _fruit_y != _head_y) && !_obstacles_coordinate[{_fruit_x, _fruit_y}]) // 如果与蛇或墙体的坐标重合，则重新生成
			break;
	}

	cursor_relocate(_fruit_x, _fruit_y); // 重定位光标，画出水果
	std::cout << '$';
}

void snake::snake_update()
{
	cursor_relocate(_snake_coordinate.back().first, _snake_coordinate.back().second); // 队尾固定为蛇头，每移动一格，蛇头都会被覆盖为身体
	_obstacles_coordinate[{_snake_coordinate.back().first, _snake_coordinate.back().second}] = true;
	std::cout << '#'; // 进行覆盖

	_snake_coordinate.push_back({_head_x, _head_y}); // 放入新蛇头
	cursor_relocate(_head_x, _head_y);
	std::cout << '@'; // 进行覆盖

	_pre_snake_body_x = _snake_coordinate.front().first, _pre_snake_body_y = _snake_coordinate.front().second; // 保存尾巴，方便后续“变长”
	cursor_relocate(_snake_coordinate.front().first, _snake_coordinate.front().second); // 准备消除尾巴
	_obstacles_coordinate[{_snake_coordinate.front().first, _snake_coordinate.front().second}] = false;
	std::cout << ' '; // 进行覆盖
	_snake_coordinate.pop_front(); // 弹出队列，保持原有长度
}

bool snake::hit_judge()
{
	if (_obstacles_coordinate[{_head_x, _head_y}]) { // 碰到障碍物或自身，游戏结束
		return false;
	} else if (_head_x == _fruit_x && _head_y == _fruit_y) { // 吃到果子，加分，身体变长，重新生成果子
		++_score, cursor_relocate(_wall_length + 10, 0), std::cout << _score; // 加分

		_snake_coordinate.push_front({_pre_snake_body_x, _pre_snake_body_y}); // 从尾部伸长
		_obstacles_coordinate[{_pre_snake_body_x, _pre_snake_body_y}] = true;
		cursor_relocate(_pre_snake_body_x, _pre_snake_body_y);
		std::cout << '#';

		fruit_build();
	}

	return true;
}

void snake::cursor_relocate(const int X, const int Y)
{
	COORD crd;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	crd.X = X, crd.Y = Y;
	SetConsoleCursorPosition(handle, crd);
}