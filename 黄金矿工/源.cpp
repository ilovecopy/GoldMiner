#include"gold.h"//����ͷ�ļ�
#include<graphics.h>
IMAGE images[IMAGE_MAX];
struct Mine mine[MINE_NUM];
struct Role r;
LOGFONT ball_text;//����
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
	double countdown;//����ʱ
	char count[1000];//����ʱת�����ַ���
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
			if ((mouse.uMsg == WM_LBUTTONDOWN) && (mouse.x > GAME_WIDTH + 30) && (mouse.x < GAME_WIDTH + 170) && (mouse.y > 30) && (mouse.y < 100))//��ʼ
			{
				begin = clock();
				DWORD t1, t2;
				t1 = t2 = GetTickCount();//���شӲ���ϵͳ�����������ĺ�����
				FlushMouseMsgBuffer();// ��������Ϣ������
				BeginBatchDraw();// ��ʼ��������
				while (1) {
					if (MouseHit()) {//����Ƿ���������Ϣ
						mouse = GetMouseMsg();// ��ȡһ�������Ϣ
						if ((mouse.uMsg == WM_LBUTTONDOWN) && (mouse.x > GAME_WIDTH + 30) && (mouse.x < GAME_WIDTH + 170) && (mouse.y > 110) && (mouse.y < 180)) {//��ͣ
							mciSendString("pause mymusic", NULL, 0, NULL);//��ͣ����
							EndBatchDraw();// �����������ƣ���ִ��δ��ɵĻ�������
							setbkmode(TRANSPARENT);//��������͸������
							settextcolor(RED);
							settextstyle(&ball_text);
							outtextxy(GAME_WIDTH + 65, 205, "����");
							settextcolor(WHITE);
							settextstyle(30, 20, "����");
							outtextxy(300, 300, "��Ϸ����ͣ������������ť");
							FlushMouseMsgBuffer();	// ��������Ϣ������
							pauseBegin = clock();
							while ((mouse.uMsg != WM_LBUTTONDOWN) || (mouse.x < GAME_WIDTH + 30) || (mouse.x > GAME_WIDTH + 170) || (mouse.y < 190) || (mouse.y > 260)) {//����
								mouse = GetMouseMsg();
							}
							pauseEnd = clock();
							begin += pauseEnd - pauseBegin;
							mciSendString("resume mymusic", NULL, 0, NULL);//��������
							BeginBatchDraw();// ִ��δ��ɵĻ�������
						}
						if ((mouse.uMsg == WM_LBUTTONDOWN) && (mouse.x > GAME_WIDTH + 30) && (mouse.x < GAME_WIDTH + 170) && (mouse.y > 270) && (mouse.y < 340)) {//�˳�
							exit(0);
						}
					}
					t2 = GetTickCount();
					if (t2 - t1 > 10) {//��������ת���������10����
						HookRock(1);//����ת��һ��
						HookMove();//�������չ�ʱ�����ƶ�
						t1 = t2;
					}
					GameDraw();//����Ϸ����
					GameControl(5);//�����Ӷ����������ץȡ�ٶ�Ϊ5
					JudgeGrap();// �ж��Ƿ�ץס��Ʒ
					end = clock();
					time = ((double)end - begin) / CLK_TCK;//��Ϊclock()���Ժ���Ϊ��λ��Ҫ��ȷ���ʱ�����Ҫ���������룬�����Ҫ����CLK_TCK��
					countdown = (double)60 - time;
					sprintf(count, "%.3f", countdown);
					if (time < 60)
					{
						settextstyle(30, 0, "����");
						setbkmode(TRANSPARENT);
						outtextxy(770, 90, "����ʱ��");
						outtextxy(880, 90, count);
						FlushBatchDraw();// ִ��δ��ɵĻ�������
					}
					if (time >= 60)                     //��Ϸͨ�ؽ���
					{
						save(r.name, &r.coin);
						putimage(0, 0, &images[over]);
						setbkmode(TRANSPARENT);
						settextstyle(&ball_text);
						if (r.coin >= 1200) {
							settextcolor(RGB(255, 255, 0));
							outtextxy(530, 300, "��ս�ɹ�");
						}
						else {
							settextcolor(RGB(220, 220, 220));
							outtextxy(530, 300, "��սʧ��");
						}
						outtextxy(530, 400, "�÷֣�");
						char s[5];
						sprintf(s, "%d", r.coin);
						outtextxy(620, 400, s);
						outtextxy(530, 350, "�û�����");
						outtextxy(650, 350, r.name);
						EndBatchDraw();// �����������ƣ���ִ��δ��ɵĻ�������
						mciSendString("close mymusic", NULL, 0, NULL);//ֹͣ��������
						if (MessageBox(NULL, "��Ϸ�������Ƿ�鿴���а�", "*********Game Over*********", MB_OKCANCEL) == IDOK)
						{
							paihangbang();
							Sleep(5000);
						}
						else
							return 1;
					}
				}
			}
			if ((mouse.uMsg == WM_LBUTTONDOWN) && (mouse.x > GAME_WIDTH + 30) && (mouse.x < GAME_WIDTH + 170) && (mouse.y > 270) && (mouse.y < 340)) {//�˳�
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
	InputBox(r.name, 10, "�������û���");
}
void GameInit() {
	initgraph(GAME_WIDTH + 200, GAME_HEIGHT);
	srand(GetTickCount());//��ϵͳʱ�����������
	///GetTickCount���أ�retrieve���Ӳ���ϵͳ������������������elapsed���ĺ����������ķ���ֵ��DWORD��
	loadimage(&images[bk], "./images/bk.png", GAME_WIDTH, GAME_HEIGHT - 120);
	for (int i = 0; i < IMAGE_MAX - 1; i++) {
		char tempname[20];
		sprintf(tempname, "./images/%d.png", i);//��·������Ϊ�ַ���
		loadimage(&images[i], tempname);
	}
	//��ʼ����ɫ
	r.width = 140;
	r.height = 120;
	r.x = GAME_WIDTH / 2 - r.width / 2;
	r.y = 0;
	r.coin = 0;
	//��ʼ������
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
	//��ʼ������
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
	//���ƽ�ɫ
	putimage(r.x, r.y, &images[role_up - 1], SRCAND);
	putimage(r.x, r.y, &images[role_up], SRCPAINT);
	//������
	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 5);
	line(hook.x, hook.y, hook.endx, hook.endy);
	//������
	setfillcolor(WHITE);
	solidcircle(hook.endx, hook.endy, 5);
	//������
	settextcolor(BLACK);
	char score[30];
	sprintf(score, "������%d", r.coin);
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, "����");
	outtextxy(20, 20, score);
	//���û���
	outtextxy(770, 20, r.name);
	//����ť
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 5);
	button();
	text();
	//������
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
void HookRock(int angle) {//��
	if (hook.state == M_NORMAL) {//���������ڶ�
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
		hook.endx = hook.x + sin(PI / 180 * hook.angle) * hook.len;//���Ƕ�ת���ɻ����ٳ������ӳ���
		hook.endy = hook.y + cos(PI / 180 * hook.angle) * hook.len;
	}
}
void GameControl(int speed) {
	if (GetAsyncKeyState(VK_DOWN) & 0x8000 && hook.state == M_NORMAL) {//���������ڶ�ʱ����
		hook.state = M_LONG;//���ӱ���쳤״̬
		hook.vx = sin(PI / 180 * hook.angle) * speed;//�����ٶȸ�ֵ
		hook.vy = cos(PI / 180 * hook.angle) * speed;
	}
	if (hook.endx<0 || hook.endx>GAME_WIDTH || hook.endy > GAME_HEIGHT) {//Խ���չ�
		hook.state = M_SHORT;
	}
}
void HookMove() {//����λ�ñ仯
	if (hook.state == M_LONG) {//����ʱ������λ��һֱ�仯
		hook.endx += hook.vx;
		hook.endy += hook.vy;
	}
	else if (hook.state == M_SHORT) {//�չ�ʱ������λ��һֱ�仯
		hook.endx -= hook.vx;
		hook.endy -= hook.vy;
		if (distance(hook)) {//���Ӹ�ԭʱ�����ӻص�����״̬
			hook.state = M_NORMAL;
		}
	}
}
void JudgeGrap() {// �ж��Ƿ�ץס��Ʒ
	for (int i = 0; i < MINE_NUM; i++) {
		if (mine[i].flag == true &&//��Ʒ���ڣ��ҹ����ڽ���ڲ�������Ϊץס��飬��¼����±ꡣ
			hook.endx > mine[i].x && hook.endx <(mine[i].x + mine[i].sizex) &&
			hook.endy> mine[i].y && hook.endy < (mine[i].y + mine[i].sizey))
		{
			hook.index = i;
			break;
		}
	}
	if (hook.index != -1) {//���ץס��飬��������
		hook.state = M_SHORT;
		mine[hook.index].x = hook.endx - mine[hook.index].sizex / 2;
		mine[hook.index].y = hook.endy - mine[hook.index].sizey / 2;
		if (distance(hook)) {//�����������Ϊԭ��
			mine[hook.index].flag = false;//��Ʒ������
			r.coin += mine[hook.index].gold;//����������
			hook.state = M_NORMAL;//����״̬Ϊԭ��
			hook.index = -1;//���ӱ�Ϊûץ����
		}
	}
}
int distance(struct Hook h) {
	int dis = sqrt(((double)h.endx - h.x) * ((double)h.endx - h.x) + ((double)h.endy - h.y) * ((double)h.endy - h.y));
	return dis <= h.len;//����Ϊԭ������1�����Ӳ�Ϊԭ������0
}
void music() {
	mciSendString("open ./music/NaNaNa.mp3 alias mymusic", NULL, 0, NULL);//���ļ�
	mciSendString("play mymusic", NULL, 0, NULL);//����
}
void button()
{
	rectangle(GAME_WIDTH + 30, 30, GAME_WIDTH + 170, 100);    //��ʼ
	rectangle(GAME_WIDTH + 30, 110, GAME_WIDTH + 170, 180);   //��ͣ
	rectangle(GAME_WIDTH + 30, 190, GAME_WIDTH + 170, 260);  //����
	rectangle(GAME_WIDTH + 30, 270, GAME_WIDTH + 170, 340);  //�˳�
	rectangle(GAME_WIDTH + 30, 350, GAME_WIDTH + 170, 420);  //���а�
}
void text()//��������
{
	gettextstyle(&ball_text);// ��ȡ��ǰ������ʽ
	strcpy(ball_text.lfFaceName, "����");
	ball_text.lfHeight = 32;
	ball_text.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&ball_text);
	outtextxy(GAME_WIDTH + 65, 45, "��ʼ");
	outtextxy(GAME_WIDTH + 65, 125, "��ͣ");
	outtextxy(GAME_WIDTH + 65, 205, "����");
	outtextxy(GAME_WIDTH + 65, 285, "�˳�");
	outtextxy(GAME_WIDTH + 65, 365, "���а�");
}
void save(char* name, int* coin)//��Ϸ���ݴ���
{
	FILE* fp;
	time_t timep;
	struct tm* p;
	time(&timep);
	p = localtime(&timep); //����ʱ��
	fp = fopen(".\\user.txt", "a");//���ļ�ĩβ׷������
	fprintf(fp, "������%s\n", name);
	fprintf(fp, "������%d\n", *coin);
	fprintf(fp, "��Ϸʱ�䣺%d��%d��%d��%dʱ%d��%d��\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	fclose(fp);
}
void paihangbang()//���а�
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
		settextstyle(20, 10, "����");
		outtextxy(450, 100 + 25 * i, n[i].name);
		settextcolor(RED);
		outtextxy(600, 100 + 25 * i, n[i].score);
		i++;
	}
}