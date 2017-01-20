#include "graphic.h"
#include <windows.h>		// für konsolen api's
#include <string>			// string from stl
#include <iostream>
#include <iomanip>
#include <time.h>

using namespace std;

// define block für windows api implementation zur steuerung der consolen farben

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define RED 4
#define LIGHTGREY 7
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTRED 12
#define WHITE 15

#define TOPLEFT   "\xC9"
#define HORZLINE  "\xCD"
#define TOPRIGHT  "\xBB"
#define VERTLINE  "\xBA"
#define BOTTLEFT  "\xC8"
#define BOTTRIGHT "\xBC"

#define SCR_BUF_DIM_X 80
#define SCR_BUF_DIM_Y 25

//################################################################################################

// Windows API Handles

HANDLE ConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE ConsoleCaret = GetStdHandle(STD_OUTPUT_HANDLE);

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void graphics::DrawFrame(void){

	DrawBox(0,0,79,24);
	DrawBox(0,0,79,2);
	DrawBox(0,3,79,24);

}
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void graphics::DrawBox(int pos_x1, int pos_y1, int pos_x2, int pos_y2){
	
	for(int j=pos_y1;j<=pos_y2;j++){
		for(int i=pos_x1;i<=pos_x2;i++){

			if(j==pos_y1&&i==pos_x1)
			{ 	GotoXY(j,i); cout<<TOPLEFT;		}

			else if(j==pos_y1&&i==pos_x2)
			{	GotoXY(j,i); cout<<TOPRIGHT;	}

			else if(j==pos_y2&&i==pos_x1)
			{	GotoXY(j,i); cout<<BOTTLEFT;	}
			
			else if(j==pos_y2&&i==pos_x2)
			{	GotoXY(j,i); cout<<BOTTRIGHT;	}
			
			else if((j==pos_y1||j==pos_y2)&&(i!=pos_x1&&i!=pos_x2))	// rahmenelement oben
			{	GotoXY(j,i); cout<<HORZLINE;	}
			
						
			else if((i==pos_x1||i==pos_x2)&&(j!=pos_y1&&j!=pos_y2))	// rahmenelement oben
			{	GotoXY(j,i); cout<<VERTLINE;	}

			else GotoXY(j,i); cout<<"\x20";	// alles andere mit leerzeichen füllen
		}
	}
}
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void graphics::DrawTime(int pos_x, int pos_y)
{
	int cur_pos_x, cur_pos_y;

	cur_pos_x=CaretPosX();	cur_pos_y=CaretPosY();

	time_t Zeitstempel;
    tm *cur;
    Zeitstempel = time(0);
    cur = localtime(&Zeitstempel);

	GotoXY(pos_y,pos_x);


    cout <<" "<<setfill('0')<<setw(2)<<cur->tm_mday<<'.'<< setfill('0')<<setw(2)
		 <<cur->tm_mon+1<<'.'<<setfill('0')<<setw(2)<<cur->tm_year+1900<<" - "<<setfill('0')<<setw(2)
		 <<cur->tm_hour<<':'<<setfill('0')<<setw(2)<<cur->tm_min<<':'<< setfill('0')<<setw(2)<<cur->tm_sec<< " ";

	GotoXY(cur_pos_y-1,cur_pos_x-1);

}
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void graphics::DrawString(int pos_y, int pos_x, string msg)	// welche nachricht (msg_idx) wohin (Y, X)
{
	int i,j,k=0,size=msg.size();			// zaehlvariablen und string laenge
	
	for(j=0;j<=SCR_BUF_DIM_Y-1;j++)					// suche in Y DIM
		for(i=0;i<=SCR_BUF_DIM_X-1;i++)				// suche in X DIM
			if(pos_x==i&&pos_y==j)					// wenn pos. erreicht
				while(size!=k){						// dann uebertrage string auf bildschirm
					GotoXY(j,i++);
					cout<<msg[k++];		// bis zum ende des strings
				}
};
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void graphics::DrawCharts(unsigned char *chart,int pos_y,int pos_x,int dim_y,int dim_x)	// welches, wie gross, wohin
{
	int i,j,k;
	
	for(j=pos_y,k=0;j<pos_y+dim_y;j++)		// beginne bei bildpuffer Y und pictogramm y=0
		for(i=pos_x;i<pos_x+dim_x;i++,k++)	// beginne bei bildpuffer X und pictogramm x=0
			{ GotoXY(j,i); cout<<chart[k]; } 
};
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void graphics::DrawError(string err_msg,int pos_y, int pos_x)		// an position				
{
	GotoXY(pos_y,pos_x);								// x y
	SetConsoleColor(LIGHTRED ,WHITE);					// rot unterlegt
	cout<<err_msg;										// eingabefehler
	
	Beep( 70, 80 );
	Sleep(50);
	Beep( 70, 80 );
	Sleep(50);
	Beep( 70, 300 );
	
	SetConsoleColor(BLACK,WHITE);						// stelle farbstandard wieder her
};
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void graphics::ClearScreen(int X1, int X2, int Y1, int Y2)
{
	int i,j;
	
	for(j=Y1;j<=Y2;j++)				// beginnend bei 0 24 zeilen
		for(i=X1;i<=X2;){				// beginnend bei 0 79 spalten
			GotoXY(j,i++);
			cout<<"\x20";
		}
};
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void graphics::SetConsoleColor(int fontcolor,int backgroundcolor)
{
	int color_attribute;
	color_attribute = backgroundcolor;
	color_attribute = _rotl(color_attribute,4) | fontcolor;
	SetConsoleTextAttribute(ConsoleColor,color_attribute);
}
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void graphics::GotoXY(int y, int x)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(ConsoleCaret,pos);
};
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
void graphics::DrawVar(string text,int pos_x,int pos_y)
{
	int i,j,k=0,size=text.size();			// zaehlvariablen und string laenge
	
	for(j=0;j<=SCR_BUF_DIM_Y-1;j++)			// suche in Y DIM
		for(i=0;i<=SCR_BUF_DIM_X-1;i++)		// suche in X DIM
			if(pos_x==i&&pos_y==j)			// wenn pos. erreicht
				while(size!=k){				// dann uebertrage string auf bildschirm
					GotoXY(j,i++);
					cout<<text;				// bis zum ende des strings
				}
}
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
int graphics::CaretPosX(void)
{
	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(ConsoleCaret, &screenInfo);
	
	return screenInfo.dwCursorPosition.X + 1;
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
int graphics::CaretPosY(void)
{
	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(ConsoleCaret, &screenInfo);
	
	return screenInfo.dwCursorPosition.Y + 1;
}
