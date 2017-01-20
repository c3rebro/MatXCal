#include <string>
#include <windows.h>

using namespace std;

struct graphics
{
	void DrawFrame(void);
	void DrawBox(int pox_x1, int pox_y1, int pox_x2, int pox_y2);
	void DrawString(int pos_y, int pos_x, string msg);
	void DrawCharts(unsigned char *chart,int pos_y,int pos_x,int dim_y,int dim_x);
	void DrawError(string err_msg,int pos_y, int pos_x);
	void DrawTime(int pos_x, int pos_y);
	void ClearScreen(int X1, int X2, int Y1, int Y2);
	void SetConsoleColor(int fontcolor,int backgroundcolor);
	void GotoXY(int y, int x);
	int CaretPosX(void);
	int CaretPosY(void);
	void DrawVar(string text,int pos_x,int pos_y);
};

