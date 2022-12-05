// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<time.h>

#define HEIGHT 450
#define WIDTH 800   //窗口大小
IMAGE temp_img, img_bg, img_littlep, img_pipixia1, img_pipixia2, img_lazhu1, img_dizidan;

int speed = 10;


//枚举按键
enum GAMEINPUT {
	NOINPUT = 0x0,
	UPINPUT = 0x1,
	DOWNINPUT = 0x2,
	LEFTINPUT = 0x4,
	RIGHTINPUT = 0x8,
	FIREINPUT =0x10

};

int input = NOINPUT;


void drawAlpha(IMAGE* dstimg, int x, int y, IMAGE* srcimg); //x为要载入图片的X坐标，y为Y坐标
void dealInput();
void moveP();
void Drawp();
void picture();
void guangInput();
void ctrlFps(int start_time);

//运动结构体
struct FlyP
{
	int x, y;
};
FlyP littlep, bg , pipixia1 , pipixia2, lazhu1, dizidan;

//运动初始化
void moveP()
{
	littlep = {250,200};
	pipixia1 = { 0,0 };
	pipixia2 = { 0,100 };
	lazhu1 = { 0,200 };
	dizidan = { 0,300 };
}


//绘制图片
void Drawp() {
	drawAlpha(&temp_img, 0, 0, &img_bg);
	drawAlpha(&temp_img, littlep.x, littlep.y, &img_littlep);
	drawAlpha(&temp_img, pipixia1.x, pipixia1.y, &img_pipixia1);
	drawAlpha(&temp_img, pipixia2.x, pipixia2.y, &img_pipixia2);
	drawAlpha(&temp_img, lazhu1.x, lazhu1.y, &img_lazhu1);
	drawAlpha(&temp_img, dizidan.x, dizidan.y, &img_dizidan);
}

void drawAlpha(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	// 变量初始化
	DWORD *dst = GetImageBuffer(dstimg);
	DWORD *src = GetImageBuffer(srcimg);
	int src_width = srcimg->getwidth();
	int src_height = srcimg->getheight();
	int dst_width = (dstimg == NULL ? getwidth() : dstimg->getwidth());
	int dst_height = (dstimg == NULL ? getheight() : dstimg->getheight());

	// 计算贴图的实际长宽
	int iwidth = (x + src_width > dst_width) ? dst_width - x : src_width;		// 处理超出右边界
	int iheight = (y + src_height > dst_height) ? dst_height - y : src_height;	// 处理超出下边界
	if (x < 0) { src += -x;				iwidth -= -x;	x = 0; }				// 处理超出左边界
	if (y < 0) { src += src_width * -y;	iheight -= -y;	y = 0; }				// 处理超出上边界

	// 修正贴图起始位置
	dst += dst_width * y + x;

	// 实现透明贴图
	for (int iy = 0; iy < iheight; iy++)
	{
		for (int ix = 0; ix < iwidth; ix++)
		{
			int sa = ((src[ix] & 0xff000000) >> 24);
			int sr = ((src[ix] & 0xff0000) >> 16);	// 源值已经乘过了透明系数
			int sg = ((src[ix] & 0xff00) >> 8);		// 源值已经乘过了透明系数
			int sb = src[ix] & 0xff;				// 源值已经乘过了透明系数
			int dr = ((dst[ix] & 0xff0000) >> 16);
			int dg = ((dst[ix] & 0xff00) >> 8);
			int db = dst[ix] & 0xff;

			dst[ix] = ((sr + dr * (255 - sa) / 255) << 16)
				| ((sg + dg * (255 - sa) / 255) << 8)
				| (sb + db * (255 - sa) / 255);
		}
		dst += dst_width;
		src += src_width;
	}
}

//加载图片
void picture() {
	loadimage(&img_bg, _T("picture\\bg.png"));
	loadimage(&img_lazhu1, _T("picture\\lazhu1.png"));
	loadimage(&img_littlep, _T("picture\\littlep.png"));
	loadimage(&img_pipixia1, _T("picture\\pipixia1.png"));
	loadimage(&img_pipixia2, _T("picture\\pipixia2.png"));
	loadimage(&img_dizidan, _T("picture\\dizidan.png"));
	loadimage(&temp_img, _T("picture\\bg.png"));
}


//光之子运动规则
void guangInput() {
	if (GetAsyncKeyState('W') & 0x8000)  input |= UPINPUT;
	if (GetAsyncKeyState('S') & 0x8000)  input |= DOWNINPUT;
	if (GetAsyncKeyState('A') & 0x8000)  input |= LEFTINPUT;
	if (GetAsyncKeyState('D') & 0x8000)  input |= RIGHTINPUT;
	if (GetAsyncKeyState('K') & 0x8000)  input |= FIREINPUT;

}

//让光之子运动
void dealInput() {
	if ((input & UPINPUT)&&(littlep.y>=-100))   littlep.y -= speed;
	if ((input & DOWNINPUT)&&(littlep.y<=HEIGHT-200))   littlep.y += speed;
	if ((input & LEFTINPUT)&&(littlep.x>=-100))   littlep.x -= speed;
	if (input & RIGHTINPUT&&(littlep.x<=WIDTH-200))   littlep.x += speed;
	input = NOINPUT;
}

//显示fps
void ctrlFps(int start_time) 
{
	clock_t running_time = clock() - start_time;
	if (13 - running_time >= 0)   Sleep(13 - running_time);
	TCHAR  time_text[50];
	int FPS = 1000 / (clock() - start_time);
	_stprintf_s(time_text, _T("FPS:%d"), FPS);
	settextstyle(20, 0, _T("黑体"));
	outtextxy(0, 0, time_text);
}	
	


int main() {
	initgraph(WIDTH, HEIGHT);
	moveP();
	picture();
	clock_t start_time;
	BeginBatchDraw();
	while (1) {
		start_time = clock();
		guangInput();
		dealInput();
		Drawp();
		putimage(0, 0, &temp_img);
		//Sleep(10);
		ctrlFps(start_time);
		FlushBatchDraw();
	}
	EndBatchDraw();
	_getch();
	closegraph();

	return 0;
}