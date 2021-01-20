#ifndef _H_//防止重复包含
#define _H_
#include<easyx.h>//包含图形库头文件
#include<stdio.h>
#include<math.h>
#include<time.h>
#pragma comment(lib,"WINMM.LIB")// 引用 Windows Multimedia API
#define IMAGE_MAX 21//图片数量
#define GAME_WIDTH 980
#define GAME_HEIGHT 710
#define MINE_NUM 15//物品数量
#define MAX_ANGLE 80//最大角度
#define PI 3.14592653
struct Role {//角色
	int x;//位置
	int y;
	int width;
	int height;
	int coin;
	char name[10];
}role;
enum MINE {//图片数组下标
	littlegold = 1,
	midgold = 3,
	biggold = 5,
	littlestone = 7,
	bigstone = 9,
	money = 11,
	diamond = 13,
	role_down = 15,
	role_up = 17,
	over = 18,
	bk = IMAGE_MAX - 1,
};
struct Mine {//矿
	int x;
	int y;
	bool flag;//物品是否存在
	int gold;//物品价值
	int sizex;//物品宽度
	int sizey;//物品高度
	int type;//物品类型，钱袋，金块，石头
	int weight;//重量
};
struct Hook {
	int x;//钩子绳子起点坐标
	int y;
	int len;
	int endx;//钩子末端坐标
	int endy;
	int vx;//速度分量
	int vy;
	int angle;
	int dir;//钩子摆动方向
	int state;//绳子状态，伸长、缩短、正常
	//bool swing;//钩子是否在摆动
	int index;//抓到的物品的下标
}hook;
enum MINE_TYPE {
	//物品类型
	LITTLEGOLD,
	MIDGOLD,
	BIGGOLD,
	LITTLESTONE,
	BIGSTONE,
	MONEY,
	DIAMOND,
	//钩子摆动方向
	LEFT,
	RIGHT,
	//钩子状态
	M_NORMAL,
	M_LONG,
	M_SHORT,
};
#endif