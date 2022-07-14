#include"gold.h"//本地头文件
#include<graphics.h>
IMAGE images[IMAGE_MAX];
struct Mine mine[MINE_NUM];
struct Role r;
LOGFONT ball_text;//字体
void login();
void GameInit();
void GameDraw();
void HookRock(int angle);
void GameControl(int speed);
void HookMove();
void JudgeGrap();
int distance(Hook hook);
void music();
void button();
void text();
void save(char* name, int* score);
void paihangbang();
int main() {
	clock_t begin, end, pauseBegin, pauseEnd;
	double countdown;//倒计时
	char count[1000];//倒计时转化的字符串
	double time;
	MOUSEMSG mouse = { 0 };
	MOUSEMSG Mouse = { 0 };
	login();
	music();
	GameInit();
	GameDraw();
	while (1) {
		if (MouseHit()) {
			mouse = GetMouseMsg();
			if ((mouse.uMsg == WM_LBUTTONDOWN) && (mouse.x > GAME_WIDTH + 30) && (mouse.x < GAME_WIDTH + 170) && (mouse.y > 30) && (mouse.y < 100))//开始
			{
				begin = clock();
				DWORD t1, t2;
				t1 = t2 = GetTickCount();//返回从操作系统启动所经过的毫秒数
				FlushMouseMsgBuffer();// 清空鼠标消息缓冲区
				BeginBatchDraw();// 开始批量绘制
				while (1) {
					if (MouseHit()) {//检查是否存在鼠标消息
						mouse = GetMouseMsg();// 获取一个鼠标消息
						if ((mouse.uMsg == WM_LBUTTONDOWN) && (mouse.x > GAME_WIDTH + 30) && (mouse.x < GAME_WIDTH + 170) && (mouse.y > 110) && (mouse.y < 180)) {//暂停
							mciSendString("pause mymusic", NULL, 0, NULL);//暂停播放
							EndBatchDraw();// 结束批量绘制，并执行未完成的绘制任务
							setbkmode(TRANSPARENT);//设置文字透明背景
							settextcolor(RED);
							settextstyle(&ball_text);
							outtextxy(GAME_WIDTH + 65, 205, "继续");
							settextcolor(WHITE);
							settextstyle(30, 20, "黑体");
							outtextxy(300, 300, "游戏已暂停，请点击继续按钮");
							FlushMouseMsgBuffer();	// 清空鼠标消息缓冲区
							pauseBegin = clock();
							while ((mouse.uMsg != WM_LBUTTONDOWN) || (mouse.x < GAME_WIDTH + 30) || (mouse.x > GAME_WIDTH + 170) || (mouse.y < 190) || (mouse.y > 260)) {//继续
								mouse = GetMouseMsg();
							}
							pauseEnd = clock();
							begin += pauseEnd - pauseBegin;
							mciSendString("resume mymusic", NULL, 0, NULL);//继续播放
							BeginBatchDraw();// 执行未完成的绘制任务
						}
						if ((mouse.uMsg == WM_LBUTTONDOWN) && (mouse.x > GAME_WIDTH + 30) && (mouse.x < GAME_WIDTH + 170) && (mouse.y > 270) && (mouse.y < 340)) {//退出
							exit(0);
						}
					}
					t2 = GetTickCount();
					if (t2 - t1 > 10) {//钩子两次转动间隔大于10毫秒
						HookRock(1);//勾子转动一度
						HookMove();//出勾或收勾时钩子移动
						t1 = t2;
					}
					GameDraw();//画游戏界面
					GameControl(5);//给绳子定义规则，绳子抓取速度为5
					JudgeGrap();// 判断是否抓住物品
					end = clock();
					time = ((double)end - begin) / CLK_TCK;//因为clock()是以毫秒为单位，要正确输出时间差需要把它换成秒，因此需要除以CLK_TCK。
					countdown = (double)60 - time;
					sprintf(count, "%.3f", countdown);
					if (time < 60)
					{
						settextstyle(30, 0, "楷体");
						setbkmode(TRANSPARENT);
						outtextxy(770, 90, "倒计时：");
						outtextxy(880, 90, count);
						FlushBatchDraw();// 执行未完成的绘制任务
					}
					if (time >= 60)                     //游戏通关界面
					{
						save(r.name, &r.coin);
						putimage(0, 0, &images[over]);
						setbkmode(TRANSPARENT);
						settextstyle(&ball_text);
						if (r.coin >= 1200) {
							settextcolor(RGB(255, 255, 0));
							outtextxy(530, 300, "挑战成功");
						}
						else {
							settextcolor(RGB(220, 220, 220));
							outtextxy(530, 300, "挑战失败");
						}
						outtextxy(530, 400, "得分：");
						char s[5];
						sprintf(s, "%d", r.coin);
						outtextxy(620, 400, s);
						outtextxy(530, 350, "用户名：");
						outtextxy(650, 350, r.name);
						EndBatchDraw();// 结束批量绘制，并执行未完成的绘制任务
						mciSendString("close mymusic", NULL, 0, NULL);//停止播放音乐
						if (MessageBox(NULL, "游戏结束，是否查看排行榜？", "*********Game Over*********", MB_OKCANCEL) == IDOK)
						{
							paihangbang();
							Sleep(5000);
						}
						else
							return 1;
					}
				}
			}
			if ((mouse.uMsg == WM_LBUTTONDOWN) && (mouse.x > GAME_WIDTH + 30) && (mouse.x < GAME_WIDTH + 170) && (mouse.y > 270) && (mouse.y < 340)) {//退出
				exit(0);
			}
			if ((mouse.uMsg == WM_LBUTTONDOWN) && (mouse.x > GAME_WIDTH + 30) && (mouse.x < GAME_WIDTH + 170) && (mouse.y > 350) && (mouse.y < 420)) {
				paihangbang();
			}
		}
	}
	return 0;
}
void login() {
	InputBox(r.name, 10, "请输入用户名");
}
void GameInit() {
	initgraph(GAME_WIDTH + 200, GAME_HEIGHT);
	srand(GetTickCount());//用系统时间做随机种子
	///GetTickCount返回（retrieve）从操作系统启动到现在所经过（elapsed）的毫秒数，它的返回值是DWORD。
	loadimage(&images[bk], "./images/bk.png", GAME_WIDTH, GAME_HEIGHT - 120);
	for (int i = 0; i < IMAGE_MAX - 1; i++) {
		char tempname[20];
		sprintf(tempname, "./images/%d.png", i);//将路径储存为字符串
		loadimage(&images[i], tempname);
	}
	//初始化角色
	r.width = 140;
	r.height = 120;
	r.x = GAME_WIDTH / 2 - r.width / 2;
	r.y = 0;
	r.coin = 0;
	//初始化矿物
	for (int i = 0; i < MINE_NUM; i++) {
		mine[i].x = rand() % (GAME_WIDTH - 220) + 110;
		mine[i].y = rand() % (GAME_HEIGHT - 220) + 110;
		mine[i].flag = true;
		mine[i].type = rand() % 6;
		switch (mine[i].type) {
		case LITTLEGOLD:
			mine[i].sizex = 32;
			mine[i].sizey = 30;
			mine[i].gold = rand() % 50 + 25;
			break;
		case MIDGOLD:
			mine[i].sizex = 59;
			mine[i].sizey = 55;
			mine[i].gold = rand() % 50 + 100;
			break;
		case BIGGOLD:
			mine[i].sizex = 155;
			mine[i].sizey = 144;
			mine[i].gold = rand() % 50 + 200;
			break;
		case LITTLESTONE:
			mine[i].sizex = 67;
			mine[i].sizey = 57;
			mine[i].gold = rand() % 50;
			break;
		case BIGSTONE:
			mine[i].sizex = 94;
			mine[i].sizey = 83;
			mine[i].gold = rand() % 50 + 35;
			break;
		case MONEY:
			mine[i].sizex = 73;
			mine[i].sizey = 83;
			mine[i].gold = rand() % 250;
			break;
		case DIAMOND:
			mine[i].sizex = 33;
			mine[i].sizey = 27;
			mine[i].gold = rand() % 600;
			break;
		}
	}
	//初始化钩子
	hook.x = r.x + 40;
	hook.y = r.y + 100;
	hook.len = 50;
	hook.endx = hook.x;
	hook.endy = hook.y + hook.len;
	hook.vx = 0;
	hook.vy = 0;
	hook.angle = 0;
	hook.dir = LEFT;
	hook.state = M_NORMAL;
	hook.index = -1;
}
void GameDraw() {
	setbkcolor(RGB(230, 230, 250));
	cleardevice();
	putimage(0, r.height, &images[bk]);
	setfillcolor(RGB(255, 208, 52));
	solidrectangle(0, 0, GAME_WIDTH, r.height);
	//绘制角色
	putimage(r.x, r.y, &images[role_up - 1], SRCAND);
	putimage(r.x, r.y, &images[role_up], SRCPAINT);
	//画绳子
	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 5);
	line(hook.x, hook.y, hook.endx, hook.endy);
	//画钩子
	setfillcolor(WHITE);
	solidcircle(hook.endx, hook.endy, 5);
	//画分数
	settextcolor(BLACK);
	char score[30];
	sprintf(score, "分数：%d", r.coin);
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, "楷体");
	outtextxy(20, 20, score);
	//画用户名
	outtextxy(770, 20, r.name);
	//画按钮
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 5);
	button();
	text();
	//画金子
	for (int i = 0; i < MINE_NUM; i++) {
		if (mine[i].flag == true) {
			switch (mine[i].type) {
			case LITTLEGOLD:
				putimage(mine[i].x, mine[i].y, &images[littlegold - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &images[littlegold], SRCPAINT);
				break;
			case MIDGOLD:
				putimage(mine[i].x, mine[i].y, &images[midgold - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &images[midgold], SRCPAINT);
				break;
			case BIGGOLD:
				putimage(mine[i].x, mine[i].y, &images[biggold - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &images[biggold], SRCPAINT);
				break;
			case LITTLESTONE:
				putimage(mine[i].x, mine[i].y, &images[littlestone - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &images[littlestone], SRCPAINT);
				break;
			case BIGSTONE:
				putimage(mine[i].x, mine[i].y, &images[bigstone - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &images[bigstone], SRCPAINT);
				break;
			case MONEY:
				putimage(mine[i].x, mine[i].y, &images[money - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &images[money], SRCPAINT);
				break;
			case DIAMOND:
				putimage(mine[i].x, mine[i].y, &images[diamond - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &images[diamond], SRCPAINT);
				break;
			}
		}
	}
}
void HookRock(int angle) {//度
	if (hook.state == M_NORMAL) {//钩子正常摆动
		if (hook.dir == LEFT) {
			hook.angle -= angle;
		}
		else {
			hook.angle += angle;
		}
		if (hook.angle > MAX_ANGLE) {
			hook.dir = LEFT;
		}
		else if (hook.angle < -MAX_ANGLE) {
			hook.dir = RIGHT;
		}
		hook.endx = hook.x + sin(PI / 180 * hook.angle) * hook.len;//将角度转化成弧度再乘以绳子长度
		hook.endy = hook.y + cos(PI / 180 * hook.angle) * hook.len;
	}
}
void GameControl(int speed) {
	if (GetAsyncKeyState(VK_DOWN) & 0x8000 && hook.state == M_NORMAL) {//绳子正常摆动时按↓
		hook.state = M_LONG;//绳子变成伸长状态
		hook.vx = sin(PI / 180 * hook.angle) * speed;//绳子速度赋值
		hook.vy = cos(PI / 180 * hook.angle) * speed;
	}
	if (hook.endx<0 || hook.endx>GAME_WIDTH || hook.endy > GAME_HEIGHT) {//越界收勾
		hook.state = M_SHORT;
	}
}
void HookMove() {//钩子位置变化
	if (hook.state == M_LONG) {//出勾时，钩子位置一直变化
		hook.endx += hook.vx;
		hook.endy += hook.vy;
	}
	else if (hook.state == M_SHORT) {//收勾时，钩子位置一直变化
		hook.endx -= hook.vx;
		hook.endy -= hook.vy;
		if (distance(hook)) {//钩子复原时，绳子回到正常状态
			hook.state = M_NORMAL;
		}
	}
}
void JudgeGrap() {// 判断是否抓住物品
	for (int i = 0; i < MINE_NUM; i++) {
		if (mine[i].flag == true &&//物品存在，且钩子在金块内部，则视为抓住金块，记录金块下标。
			hook.endx > mine[i].x && hook.endx <(mine[i].x + mine[i].sizex) &&
			hook.endy> mine[i].y && hook.endy < (mine[i].y + mine[i].sizey))
		{
			hook.index = i;
			break;
		}
	}
	if (hook.index != -1) {//如果抓住金块，绳子缩短
		hook.state = M_SHORT;
		mine[hook.index].x = hook.endx - mine[hook.index].sizex / 2;
		mine[hook.index].y = hook.endy - mine[hook.index].sizey / 2;
		if (distance(hook)) {//如果绳子收缩为原长
			mine[hook.index].flag = false;//物品不存在
			r.coin += mine[hook.index].gold;//人物金币增加
			hook.state = M_NORMAL;//绳子状态为原长
			hook.index = -1;//钩子变为没抓金子
		}
	}
}
int distance(struct Hook h) {
	int dis = sqrt(((double)h.endx - h.x) * ((double)h.endx - h.x) + ((double)h.endy - h.y) * ((double)h.endy - h.y));
	return dis <= h.len;//绳子为原长返回1，绳子不为原长返回0
}
void music() {
	mciSendString("open ./music/NaNaNa.mp3 alias mymusic", NULL, 0, NULL);//打开文件
	mciSendString("play mymusic", NULL, 0, NULL);//播放
}
void button()
{
	rectangle(GAME_WIDTH + 30, 30, GAME_WIDTH + 170, 100);    //开始
	rectangle(GAME_WIDTH + 30, 110, GAME_WIDTH + 170, 180);   //暂停
	rectangle(GAME_WIDTH + 30, 190, GAME_WIDTH + 170, 260);  //继续
	rectangle(GAME_WIDTH + 30, 270, GAME_WIDTH + 170, 340);  //退出
	rectangle(GAME_WIDTH + 30, 350, GAME_WIDTH + 170, 420);  //排行榜
}
void text()//设置字体
{
	gettextstyle(&ball_text);// 获取当前字体样式
	strcpy(ball_text.lfFaceName, "黑体");
	ball_text.lfHeight = 32;
	ball_text.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&ball_text);
	outtextxy(GAME_WIDTH + 65, 45, "开始");
	outtextxy(GAME_WIDTH + 65, 125, "暂停");
	outtextxy(GAME_WIDTH + 65, 205, "继续");
	outtextxy(GAME_WIDTH + 65, 285, "退出");
	outtextxy(GAME_WIDTH + 65, 365, "排行榜");
}
void save(char* name, int* coin)//游戏数据存盘
{
	FILE* fp;
	time_t timep;
	struct tm* p;
	time(&timep);
	p = localtime(&timep); //当地时间
	fp = fopen(".\\user.txt", "a");//向文件末尾追加数据
	fprintf(fp, "姓名：%s\n", name);
	fprintf(fp, "分数：%d\n", *coin);
	fprintf(fp, "游戏时间：%d年%d月%d日%d时%d分%d秒\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	fclose(fp);
}
void paihangbang()//排行榜
{
	initgraph(1129, 753);
	putimage(0, 0, &images[19]);
	struct data
	{
		char name[20];
		char score[200];
		char time[200];
	};
	FILE* fp;
	data n[30] = { 0 };
	int i = 0;
	fp = fopen("user.txt", "r");
	while (i < 20)
	{
		fgets(n[i].name, 20, fp);
		fgets(n[i].score, 200, fp);
		fgets(n[i].time, 200, fp);
		i++;
	}
	i = 0;
	while (i < 20) {
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		settextstyle(20, 10, "楷体");
		outtextxy(450, 100 + 25 * i, n[i].name);
		settextcolor(RED);
		outtextxy(600, 100 + 25 * i, n[i].score);
		i++;
	}
}