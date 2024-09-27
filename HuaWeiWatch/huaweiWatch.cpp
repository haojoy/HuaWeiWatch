#include<stdio.h>
#include<time.h>
#include"tools.hpp"
#include"lunar.h"
#include<mmsystem.h>
#include<thread> 
#pragma comment(lib,"winmm.lib")
/*
	1,文本字体设置
	2,图片动画效果
	3,绘制表盘
*/
#define WIN_SIZE 500
#define WIN_HALF (WIN_SIZE/2)	//窗口的一半
#define ANIMATION_INTERVAL 20   // 动画帧的间隔时间

IMAGE spaceMan[59];
IMAGE other[6];
const char *week[7] = {"日","一","二","三","四","五","六" };
void setTextStyle(int height,int width,const char *faceName)
{
	LOGFONT f = { 0 };
	f.lfHeight = height;
	f.lfWidth = width;
	f.lfQuality = ANTIALIASED_QUALITY;
	strcpy(f.lfFaceName, faceName);
	settextstyle(&f);
}
void loadImg()
{
	mciSendString("open ./images/风儿吹.mp3", NULL, 0, NULL);
	mciSendString("play ./images/风儿吹.mp3 repeat", NULL, 0, NULL);

	char fileName[50] = { 0 };
	for (int i = 0; i < 59; i++)
	{
		sprintf_s(fileName,"./images/human (%d).jpg", i+2);
		loadimage(spaceMan + i,fileName , 140, 130);
	}
	loadimage(other + 0, "./images/other1.png", 70, 70);
	loadimage(other + 1, "./images/other2.png", 60, 60);
	loadimage(other + 2, "./images/other3.png", 55, 55);
	loadimage(other + 3, "./images/other4.png", 30, 30);
	loadimage(other + 4, "./images/other5.png", 30, 30);
	loadimage(other + 5, "./images/other6.png", 60, 60);
}
//太空人旋转动画
void animation()
{
	static int index = 0;
	static DWORD lastFrameTime = 0;

	// 检查是否到了下一帧的时间
	DWORD currentTime = clock();
	if (currentTime - lastFrameTime > ANIMATION_INTERVAL) {
		index = (index + 1) % 59;  // 循环切换动画帧
		lastFrameTime = currentTime;
	}
	putimage(175, 210, spaceMan + index);  // 显示当前帧
}


void setLunarDate(tm* ltm)
{
	int year = 1900 + ltm->tm_year;
	int month = 1 + ltm->tm_mon;
	int day = ltm->tm_mday;
	Lunar lunar;
	LunarObj* lunarDate = lunar.solar2lunar(year, month, day);

	if (lunarDate) {
		setTextStyle(25, 12, "宋体");
		char szLunar[40] = { 0 };
		sprintf_s(szLunar, "%s%s", lunarDate->lunarMonthChineseName.c_str(), lunarDate->lunarDayChineseName.c_str());
		outtextxy(WIN_HALF + 110, WIN_HALF - 20, szLunar);
		delete lunarDate;
	}
}

void gameDraw()
{
	
	setbkcolor(RGB(223, 230, 240));
	cleardevice();
	//绘制表盘
	setlinecolor(RGB(68, 68, 68));
	setlinestyle(PS_SOLID, 40);
	setfillcolor(RGB(223, 230, 240));
	fillellipse(0, 0, WIN_SIZE, WIN_SIZE);
	
	//绘制线条
	setlinestyle(PS_SOLID, 4);
	setlinecolor(BLACK);
	//最上面竖线
	line(WIN_HALF - 30, 20, WIN_HALF - 30, 130);
	//横线x2
	line(WIN_HALF - 195, WIN_HALF - 120, WIN_HALF + 195, WIN_HALF - 120);
	line(WIN_HALF - 195, WIN_HALF + 120, WIN_HALF + 195, WIN_HALF + 120);
	//下面线条x3
	line(WIN_HALF + 80, WIN_HALF + 120, WIN_HALF + 80, WIN_HALF + 175);
	line(WIN_HALF + 80, WIN_HALF + 175, WIN_HALF - 60, WIN_HALF + 175);
	line(WIN_HALF - 60, WIN_HALF + 175, WIN_HALF - 60, WIN_HALF + 175 + 48);


	setbkmode(TRANSPARENT);
	//左上空气湿度90%
	setTextStyle(55, 23, "Arial");
	settextcolor(BLACK);
	outtextxy(WIN_HALF - 155, 75, "90%");
	drawImg(other + 5, WIN_HALF - 90, 35);	//火箭
	//右上
	setTextStyle(25, 15, "黑体");
	outtextxy(WIN_HALF - 25, 35, "空气良好");

	setTextStyle(25, 13, "宋体");
	outtextxy(WIN_HALF - 25, 65, "晴天");
	outtextxy(WIN_HALF - 25, 95, "25℃");
	outtextxy(WIN_HALF + 38, 65, "26°");
	outtextxy(WIN_HALF + 38, 95, "17°");
	drawImg(other+4, WIN_HALF + 73, 60);	//上面的箭头
	drawImg(other+3, WIN_HALF + 73, 90);	//下面的箭头
	drawImg(other+1, WIN_HALF + 105, 70);	//太阳

	// 下部分
	setTextStyle(37, 17, "宋体");
	outtextxy(100, WIN_HALF + 130, "睡眠");
	outtextxy(WIN_HALF + 90, WIN_HALF + 130, "距离");

	setTextStyle(40, 15, "Arial");
	outtextxy(185, WIN_HALF + 125, "7h30m");
	outtextxy(215, WIN_HALF + 180, "9.88km");

	//中间
	//心率
	setTextStyle(25, 13, "宋体");
	outtextxy(60, WIN_HALF + 30, "80~128");
	drawImg(&other[0], 65, WIN_HALF + 50);	//心率图

	setTextStyle(40, 15, "Arial");
	outtextxy(135, WIN_HALF + 60, "92");
	// 步数
	drawImg(&other[2], WIN_HALF + 65, WIN_HALF + 65);
	outtextxy(WIN_HALF + 125, WIN_HALF + 75, "9527");
	
	//时间、日期相关
	time_t timep = time(NULL);			//获取当前时间
	struct tm* p = localtime(&timep);	//把时间转成格式化时间

	/*setTextStyle(25, 12, "宋体");
	outtextxy(WIN_HALF + 110, WIN_HALF - 20, "三月三十一");*/
	setLunarDate(p);
	char fileName[40] = { 0 };
	sprintf_s(fileName, "周%s %d-%d", week[p->tm_wday], p->tm_mon + 1, p->tm_mday);
	outtextxy(WIN_HALF + 110, WIN_HALF + 10, fileName);

	// 获取字体
	setTextStyle(100, 40, "Arial");
	char szBuf[40] = { 0 };
	sprintf_s(szBuf, "%d:%02d", p->tm_hour, p->tm_min);
	outtextxy(105, 120, szBuf);
	// 秒
	setTextStyle(55, 23, "Arial");
	sprintf(szBuf, "%02d", p->tm_sec);
	outtextxy(335, 160, szBuf);
}
int main()
{
	initgraph(WIN_SIZE, WIN_SIZE/*,EW_SHOWCONSOLE*/);
	SetWindowNewStyle(WIN_SIZE, WIN_SIZE);
	
	std::thread loadImgThread(loadImg); //避免加载资源文件导致的黑窗口
	BeginBatchDraw();	//双缓冲
	while (true)
	{
		gameDraw();
		animation();

		mouseEvent();

		FlushBatchDraw();
	}
	EndBatchDraw();
	return 0;
}