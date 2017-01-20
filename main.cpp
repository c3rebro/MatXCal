#include "qmtrx.h"
#include "graphic.h"
#include <windows.h>		// für konsolen api's
#include <string>			// string from stl
#include <iostream>
#include <fstream>
#include <conio.h>
#include <complex>
#include <math.h>
#include <spirit.hpp>
#include <sstream>			// string stream datentyp für konvertierung von
							// string nach int

using namespace std;
using namespace boost::spirit;

//################################################################################################

// define für bildpuffer groesse des consolen fensters


// define block für menue-daten. positioniert char msg[] konstanten

#define MAIN_MENU		UI.DrawString(1,33,"- MAIN MENU -"); UI.DrawString(3,2," matrixcalculator ");\
						UI.DrawString(5,2,"please select on of the following features:");\
						UI.DrawString(7,2,"- matrix (a) and vector (b) multiply");\
						UI.DrawString(9,2,"- gauss jordan calculation (c)");\
						UI.DrawString(11,2,"- determinant calculation (d)");\
						UI.DrawString(13,2,"- eigenvalue (e) and eigenvector (f) calculation");\
						UI.DrawString(17,2,"cmd>"); UI.DrawString(22,2,"copyright by:");\
						UI.DrawString(20,24,"type 'EXIT' to close the program");\
						UI.DrawString(23,2,"Markus Freitag; Bastian John; Jan Loeser and Steven Rott");\

#define MATRIX_MP_REAL	UI.DrawString(1,28,"- MATRIX MULTIPLICATION -");\
						UI.DrawString(3,2," matrixcalculator ");\
						UI.DrawCharts(&mat_chart1[0],13,59,10,13);

#define VECTOR_MP_REAL	UI.DrawString(1,28,"- VECTOR MULTIPLICATION -");\
						UI.DrawString(3,2," matrixcalculator ");\
						UI.DrawCharts(&vector_chart1[0],16,59,5,13);

#define GAUSS_REAL		UI.DrawString(1,28,"- GAUSS JORDAN CALCULATION -");\
						UI.DrawString(3,2," matrixcalculator ");\
						UI.DrawCharts(&gauss_chart1[0],13,57,9,17);

#define EIGENVL_REAL	UI.DrawString(1,28,"- EIGENVALUE CALCULATION -");\
						UI.DrawString(3,2," matrixcalculator ");

#define EIGENVC_REAL	UI.DrawString(1,28,"- EIGENVECTOR CALCULATION -");\
						UI.DrawString(3,2," matrixcalculator ");

#define DETERM_REAL		UI.DrawString(1,28,"- DETERMINANT CALCULATOR -");\
						UI.DrawString(3,2," matrixcalculator ");\
						UI.DrawCharts(&det_chart1[0],15,59,7,12);


// Farbvorlagen
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define RED 4
#define LIGHTGREY 7
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTRED 12
#define WHITE 15

//################################################################################################

// funktionsprototypen

DWORD __stdcall thread (void *p);

string GetUserInput(Vector *v,matrix *m,int code,int cmd,int err_pos_x,int err_pos_y,\
					int str_pos_x, int str_pos_y,int z,int s,int lower_border, int upper_border);
bool complex_parser(Vector *v,matrix *m,char const* str,int cmd, int z,int s);
bool option_parser(char const* source,char lower_border,char upper_border);
bool range_parser(char const* source, int lower_border, int upper_border);
string CallMenuStructure(string input_str);
string MatrixOutput(matrix *matrix_erg,int row1_count,int col2_count);
string VectorOutput(Vector *v1,Vector *v2,Complex *v_erg,Vector *skalar,int dim,int cmd);

//################################################################################################

// Variablen und Konstanten deklaration und initialisierung

unsigned char mat_chart1[130]=
{
	' ','-',' ',' ',' ','C','O','L',' ',' ',' ',' ','-',
	'|',201,205,205,203,205,205,203,205,205,203,205,205,
	' ',186,'A','a',186,'A','b',186,'A','c',186,'A','*',
	'R',204,205,205,206,205,205,206,205,205,206,' ',' ',
	'O',186,'B','a',186,'B','b',186,'*','*',' ',' ',' ',
	'W',204,205,205,206,205,205,206,' ',' ',' ',' ',' ',
	' ',186,'C','a',186,'*','*',' ',' ',' ',' ',' ',' ',
	' ',204,205,205,206,' ',' ',' ',' ',' ',' ',' ',' ',
	'|',186,'*','a',' ',' ',' ',' ',' ',' ',' ',' ',' ',
	' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '
};

unsigned char vector_chart1[65]=
{
	' ',' ',' ','V','E','C','T','O','R',' ','1',' ',' ',
	'{','A','a',';','A','b',';','A','c','.','.','.','}',
	' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
	' ',' ',' ','V','E','C','T','O','R',' ','2',' ',' ',
	'{','B','a',';','B','b',';','B','c','.','.','.','}',
};

unsigned char gauss_chart1[153]=
{
	201,205,' ',' ',' ',' ',' ',' ',205,187,' ',201,205,' ',' ',205,187,
	186,'A','1',';','A','2',';','A','n',186,' ',186,' ','X','1',' ',186,
	186,' ',' ',' ',' ',' ',' ',' ',' ',186,' ',186,' ',' ',' ',' ',186,
	186,' ',' ',' ','*','*',' ',' ',' ',186,' ',186,' ','*','*',' ',186,
	186,' ',' ',' ',' ',' ',' ',' ',' ',186,'=',186,' ',' ',' ',' ',186,
	186,' ',' ',' ','*','*',' ',' ',' ',186,' ',186,' ','*','*',' ',186,
	186,' ',' ',' ',' ',' ',' ',' ',' ',186,' ',186,' ',' ',' ',' ',186,
	186,'m','1',';','m','2',';','m','n',186,' ',186,' ','X','n',' ',186,
	200,205,' ',' ',' ',' ',' ',' ',205,188,' ',200,205,' ',' ',205,188
};

unsigned char det_chart1[84]=
{
	186,' ','A','1',' ','A','2',' ','A','n',' ',186,
	186,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',186,
	186,' ',' ',' ',' ','*','*',' ',' ',' ',' ',186,
	186,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',186,
	186,' ',' ',' ',' ','*','*',' ',' ',' ',' ',186,
	186,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',186,
	186,' ','m','1',' ','m','2',' ','m','n',' ',186,
};

unsigned char vc_erg_chart1[55]=
{
	186,' ','A','1',' ',' ',' ','B','1',' ',186,
	186,' ',' ',' ',' ',' ',' ',' ',' ',' ',186,
	186,' ','A','2',' ','o',' ','B','2',' ',186,
	186,' ',' ',' ',' ',' ',' ',' ',' ',' ',186,
	186,' ','A','n',' ',' ',' ','B','n',' ',186,
};

unsigned char vc_erg_chart2[55]=
{
	186,' ','A','1',' ',' ',' ','B','1',' ',186,
	186,' ',' ',' ',' ',' ',' ',' ',' ',' ',186,
	186,' ','A','2',' ','X',' ','B','2',' ',186,
	186,' ',' ',' ',' ',' ',' ',' ',' ',' ',186,
	186,' ','A','n',' ',' ',' ','B','n',' ',186,
};
bool busy=true;	//die akualisierung der uhrzeit erfolgt nur wenn keine andere
				//funktion die gotoxy methode belegt um eine fehlpositionierung zu vermeiden

//################################################################################################

// Funktionsbereich
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
int main(int argc, char* argv[])
{
	DWORD id;									//thread id
	CreateThread (0, 0, thread, 0, 0, &id);		//starten des neuen threads zur uhrzeitausgabe
												//in echtzeit

	graphics UI;

	string commstr,filename;					//kommunikation zwischen ein/ausgabe verarbeitung
												//hinsichtlich der menuestruktur

	UI.SetConsoleColor(BLACK, WHITE);			//setzen der globalen farbeigenschaften
	system("cls");								//anwendung auf den gesamten bildbereich (vollbild)

	UI.DrawFrame();								//rahmen

	MAIN_MENU;									//zeichnen der strings für das hauptmenue

	do{											
		UI.SetConsoleColor(LIGHTBLUE,WHITE);	//eingabe erfolgt hervorgehoben

		commstr=CallMenuStructure(GetUserInput(0,0,0,0, 6,18,6,17, 0,0,'a','f'));	//aufruf der menuestruktur

		if(commstr=="exit") break;			//menuestruktur gibt bei endgültigem verlassen des programms
											//"exit" zurueck
		else
		{
			while(commstr[0])						//Endlosschleife im menue
			{
				if(commstr=="return_to_mainmenu")	//...durch rücksprung ins hauptmenue beendet
				{
					UI.DrawFrame();
					MAIN_MENU;
					commstr[0]=0;
				}
				else
				{
					commstr=CallMenuStructure(GetUserInput(0,0,0,0, 6,18,6,17, 0,0,'a','f'));
				}			
			}
		}
	}while(1);

	UI.SetConsoleColor(LIGHTGREY,BLACK);	//farbstandard wiederherstellen
	system("cls");							//bildschirmdarstellung loeschen
	cout<<"Bye Bye"<<endl;					//verabschiedung in bild und ton
	Beep( 550, 50 );
	Beep( 1000, 50 );
	Beep( 550, 50 );

	return 0;
};
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
DWORD __stdcall thread (void *p)			//Api thread aufruf
{
	graphics UI;
	
	while (1)
	{
		Sleep (500);						//warte
		if(!busy)							//wenn im leerlauf
			UI.DrawTime(55,3);				//schreibe zeit
	}

}
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
string GetUserInput(Vector *v,matrix *m,int code,int cmd,int err_pos_x,\
					int err_pos_y,int str_pos_x, int str_pos_y,int z,int s,\
					int lower_border, int upper_border)
{
	bool err=false;
	unsigned char character,keycode;
	int i=1,j=0;
	string temp;
	complex<double> c;
	
	temp.resize(i);							// Anfangsgrösse festlegen
	graphics UI;

	do
	{
	UI.GotoXY(str_pos_y,str_pos_x);
		do
		{
			busy=false;						// keine Positionierungen, Uhrzeit hat zugriff auf goto fkt
				character=_getch();			// tastatureingabe abfangen OHNE echo
			busy=true;

			character=tolower(character);	// und in lowercase konvertieren

			if(((character>=32&&character<=62)||(character>='a'&&character<='}'))) //wenn druckbaren zeichen
			{ 
				UI.SetConsoleColor(LIGHTBLUE,WHITE);	
				putchar(character);					//echo wenn printable
				UI.SetConsoleColor(BLACK,WHITE);		
			}

			else if((character==27)||(character==23)||(character==13)); // Sonderzeichen abfangen

			else if( (!j)&&( (character==13)||(character==8) ) )	//eingabe oder backspace
			{														//ist nicht möglich wenn kein
				Beep(750,100);										//zeichen vorhanden. hinweiston
				character=0;										//und puffer löschen
				continue;
			}
			
			else if(character==8)									//lösch routine für backspace
			{
				UI.GotoXY(str_pos_y,str_pos_x+temp.size()-1);
				putchar(0);
				temp[--j]='\0';										//erst string kürzen
				UI.GotoXY(str_pos_y,str_pos_x+temp.size()-1);
				temp.resize(j);
				i--;
				continue;
			}
			else {  character=0; keycode=_getch();}

			switch(character)
			{
				case 0:
					{
						/*switch(keycode)
						{
						case 80: return "DOWN"; 	case 72: return "UP";
						case 75: return "LEFT"; 	case 77: return "RIGHT";
						case 59: return "F1";		case 60: return "F2";
						case 61: return "F3";		case 62: return "F4";
						case 63: return "F5";		case 64: return "F6";
						case 65: return "F7";		case 66: return "F8";
						case 67: return "F9";		case 68: return "F10";
						case 133: return "F11";		case 134: return "F12";
						default: return "unreg_kc";
						}*/			
					}break;

				case 27: return "ESCAPE"; break;
				
				case 13: 
							if(j)
							{ temp[j]='\0'; }
							else
							{
								Beep(750,100);
								character=0;
							}break;
				
				default: 	temp.resize(i++);
							temp[j++]=character; break;
			}
			fflush(stdin);
		}while(character!=13);

		try
		{
			switch(code)
			{
				case 0:		//char parser grenzbereich definierbar
				{
					if((temp!="exit")&&(!option_parser(temp.c_str(),lower_border,upper_border)))
					{ 
						UI.DrawString(str_pos_y,str_pos_x,"                ");
						err=true;
						UI.DrawString(err_pos_y,err_pos_x,"                 ");
						throw (string) "<SYNTAX ERROR>"; temp.clear();
					}

					else if(temp=="exit")
					{ return temp; }

					else 
					{
						UI.DrawString(err_pos_y,err_pos_x,"                ");
						err=false;
						Beep(500,50); 
					}

				}break;

				case 1:		//int parser grenzbereich definierbar
				{
					if(!range_parser(temp.c_str(),lower_border,upper_border))
					{ 
						UI.DrawString(str_pos_y,str_pos_x,"                ");
						err=true; 
						UI.DrawString(err_pos_y,err_pos_x,"                 ");
						throw (string) "<SYNTAX ERROR>"; 
						temp.clear();
					}

					else 
					{
						UI.DrawString(err_pos_y,err_pos_x,"                  ");
						err=false;
						Beep(500,50); 
					}
				}break;

				case 2:
					{
						if(!complex_parser(v,m,temp.c_str(),cmd,z,s))
						{ 
							UI.DrawString(str_pos_y,str_pos_x,"                ");
							err=true;
							UI.DrawString(err_pos_y,err_pos_x,"                  ");
							throw (string) "<SYNTAX ERROR>"; temp.clear();
						}

						else 
						{
							UI.DrawString(err_pos_y,err_pos_x,"                  ");
							err=false;
							Beep(500,50); 
						}
					}break;

			}
		}

		catch(string& err_msg)
		{
			UI.DrawError(err_msg,err_pos_y,err_pos_x);
			temp.clear();
			i=1; j=0;
		}

	}while(err);

	return temp;
}
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
bool complex_parser(Vector *v,matrix *m,char const* str,int cmd, int z, int s)
{
    double a = 0.0;
    double b = 0.0;

	subrule<0> possibility_1;
	subrule<1> possibility_2;
	subrule<2> possibility_3;

    subrule<20> re;
    subrule<21> im;
	subrule<22> j1;

    if (parse(str,

        (
             possibility_1=!(re >> !(((ch_p('+') >> j1))>>im) | re),
			 re = real_p[assign(a)],
			 im = ureal_p[assign(b)],
			 j1 = ch_p('j')>>ch_p('*')
        ),
        space_p).full)
    {
		Complex temp;
		temp.SetRe(a);
		temp.SetIm(b);
		if(cmd)
			m->SetComp(z,s,temp);
		else
		{
			v->SetElem(s-1,temp);
		}
		return true;
    }

	else if(parse(str,
		(
			possibility_2=!(re >> !(((ch_p('-') >> j1))>>im) | re),
			re = real_p[assign(a)],
			im = ureal_p[assign(b)],
			j1 = ch_p('j')>>ch_p('*')
		),space_p).full)
	{
		Complex temp;
		temp.SetRe(a);
		temp.SetIm(b);
		b*=-1;

		if(cmd)
			m->SetComp(z,s,temp);
		else
		{
			v->SetElem(s-1,temp);
		}

		return true;
	}

	else if(parse(str,
		(
		possibility_3=!(re >> !(im>>(ch_p('j'))) | re),
		re = real_p[assign(a)],
		im = real_p[assign(b)]
		),space_p).full)
	{
		Complex temp;
		temp.SetRe(a);
		temp.SetIm(b);

		if(cmd)
			m->SetComp(z,s,temp);
		else
		{
			v->SetElem(s-1,temp);
		}

		return true;
	}

    else
    {
        return false;
    }
}
bool option_parser(char const* source,char lower_border,char upper_border)
{
	if (parse(source,(range_p<>(lower_border,upper_border)),space_p).full)
	{		return true;}
	else	return false;
}

bool range_parser(char const* source, int lower_border, int upper_border)
{
	if (parse(source,+digit_p,space_p).full)
	{		
		int b;
		b=atoi(source);	
		if(b<lower_border||b>upper_border)
			return false;
		else
		return true;
	}
	else	return false;
}
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
string CallMenuStructure(string input_str)
{
	graphics UI;
	Complex CMPX;

	UI.DrawFrame();

	Complex comp;
	string expect,row_input,col_input, temp;
	int col1_count=0,row1_count=0,col2_count=0,row2_count=0;

	if(input_str=="exit")
		return input_str;

	//matrix *Matrix1;

	UI.SetConsoleColor(BLACK,WHITE);		// eingabe hervorgehoben

	switch(input_str[0])
	{				// menueverzweigung
//------------------------------------------------------------------------------------------------
		case 'a':
		{
			bool scrollx=0, scrolly=0, err=false;
			int i,j,l,m, cc=0;
			int dim_x=1, dim_y=1;

			char idx_m='A';
			char idx_n[2];

			UI.DrawBox(2,4,52,23);
			UI.DrawBox(53,4,77,23);

			MATRIX_MP_REAL;

			int menucall=3;

			UI.GotoXY(5,55); cout<<"first matrix";
			UI.GotoXY(6,55); cout<<"row count (MAX 20)";
			UI.GotoXY(7,55); cout<<">"; 

			row1_count=CMPX.from_string<int>(GetUserInput(0,0,1,0, 56,8, 56,7,0,0,2,20));

			UI.GotoXY(9,55); cout<<"col count (MAX 20)";
			UI.GotoXY(10,55); cout<<">"; 

			col1_count=CMPX.from_string<int>(GetUserInput(0,0,1,0, 56,11, 56,10,0,0,2,20));

			UI.GotoXY(5,4);	cout<<"please insert first matrix ";
			
			UI.SetConsoleColor(GREEN,WHITE);

			for(int u=0;u<4;u++)
			{
				UI.DrawString(5,31,"                   ");
				Sleep(100);
				UI.DrawString(5,31,"(FORMAT a+j*b or a)");
				Beep(750,30);
				Sleep(100);
			}

			UI.SetConsoleColor(BLACK,WHITE);

			matrix Matrix1(row1_count,col1_count);

			for(j=1, l=7; j<=row1_count; j++, l++)
			{
				for(i=1, m=4; i<=col1_count; i++)
				{
					UI.GotoXY(l,m);
					_itoa_s(i,idx_n,10);
					cout<<idx_m<<idx_n<<"=";

					temp=GetUserInput(0,&Matrix1,2,1, m+3,l+1, m+3,l, j,i,0,0);
					
					if(i<=9)
						m+=4+temp.size();
					else
						m+=5+temp.size();

					if(m>=40){
						UI.ClearScreen(4,50,l,l); m=4;
					}
				}
				scrollx=0;
				idx_m++;	
				}

			UI.ClearScreen(4,50,5,22);
			UI.ClearScreen(54,76,5,10);

			idx_m='A';

			UI.GotoXY(5,55); cout<<"second matrix";

			UI.GotoXY(6,55); cout<<"row count (MAX 20)";
			UI.GotoXY(7,55); cout<<">"; 

			do
			{
				row2_count=CMPX.from_string<int>(GetUserInput(0,0,1,0, 56,8, 56,7, 0,0,2,20));

				try
				{
					if(col1_count!=row2_count)
						throw (string) "<EXPECT: %  ROWS>";
				}
				catch (string& err_msg)
				{
					char temp_count[2];

					_itoa_s(col1_count,temp_count,10);

					for(unsigned int n=0;n<=err_msg.size();n++)
						if(err_msg[n]=='%')
						{
							for(int o=0; o<=1;)
							{
								if(strlen(temp_count)==2)
									err_msg[n+o]=temp_count[o++];
								else
								{
									err_msg[n+o]=temp_count[o++];
									break;
								}
							}
						}
						UI.DrawString(8,56,"                ");
						UI.DrawError(err_msg,8,56);
						UI.DrawString(7,56,"                ");
				}	
			}while(col1_count!=row2_count);

			UI.GotoXY(9,55); cout<<"col count (MAX 20)";
			UI.GotoXY(10,55); cout<<">";

			col2_count=CMPX.from_string<int>(GetUserInput(0,0,1,0, 56,11, 56,10,0,0,2,20));

			UI.GotoXY(5,4);

			cout<<"please insert second matrix";

			matrix Matrix2(row2_count,col2_count);

			for(j=1, l=7; j<=row2_count; j++, l++)
			{
				for(i=1, m=4; i<=col2_count; i++)
				{
					UI.GotoXY(l,m);
					_itoa_s(i,idx_n,10);
					cout<<idx_m<<idx_n<<"=";

					temp=GetUserInput(0,&Matrix2,2,1,m+3,l+1,m+3,l,j,i,0,0);

					if(i<=9)
						m+=4+temp.size();
					else
						m+=5+temp.size();

					if(m>=40){
						UI.ClearScreen(4,50,l,l); m=4;
					}
				}
				scrollx=0;
				idx_m++;	
			}
			
			matrix matrix_erg=Matrix1*Matrix2;
			return MatrixOutput(&matrix_erg,row1_count, col2_count);
		}break;
//------------------------------------------------------------------------------------------------
		case 'b':
			{
				int i,j=1,l=7,m, cc=0;
				char idx_m='A';
				char idx_n[2];

				string selection;
				stringstream conv;
				int dim;

				UI.DrawBox(2,4,52,23);
				UI.DrawBox(53,4,77,23);

				VECTOR_MP_REAL;

				UI.GotoXY(5,55); cout<<"dot or cross product?";
				UI.GotoXY(6,55); cout<<"'d' = dot; 'c' = cross";
				UI.GotoXY(7,55); cout<<">"; 

				selection=GetUserInput(0,0,0,0, 56,8, 56,7, 0,0,'c','d');

				UI.GotoXY(9,55); cout<<"vector dimension";
				UI.GotoXY(10,55); cout<<">";

				dim=CMPX.from_string<int>(GetUserInput(0,0,1,0, 56,11, 56,10, 0,0,2,20));

				Vector Vector1(dim);

				UI.GotoXY(5,4);

				cout<<"please insert first vector";

				UI.SetConsoleColor(GREEN,WHITE);

				for(int u=0;u<4;u++)
				{
					UI.DrawString(5,31,"                   ");
					Sleep(100);
					UI.DrawString(5,31,"(FORMAT a+j*b or a)");
					Beep(750,30);
					Sleep(100);
				}

				UI.SetConsoleColor(BLACK,WHITE);

				for(i=1, m=4; i<=dim; i++)
				{
					if((m>=40)){
						UI.ClearScreen(4,50,l,l);
						m=4;
					}

					UI.GotoXY(l,m);

					_itoa_s(i,idx_n,10);
					cout<<idx_m<<idx_n<<"=";

					temp=GetUserInput(&Vector1,0,2,0, m+3,l+1,m+3,l,j,i,0,0);

					if(i<=9)
					{
						m+=4+temp.size();

					}
					else
					{
						m+=5+temp.size();
					}
				}

				Vector Vector2(dim);

				UI.GotoXY(9,4);
				idx_m++;
				i=4; j++; l+=4;
				cout<<"please insert second vector";

				for(i=1, m=4; i<=dim; i++)
				{
					if((m>=40)){
						UI.ClearScreen(4,50,l,l);
						m=4;
					}

					UI.GotoXY(l,m);

					_itoa_s(i,idx_n,10);
					cout<<idx_m<<idx_n<<"=";

					temp=GetUserInput(&Vector2,0,2,0, m+3,l+1,m+3,l,j,i,0,0);

					if(i<=9)
					{
						m+=4+temp.size();

					}
					else
					{
						m+=5+temp.size();
					}

				}
				Vector v_erg(dim);
				Complex skalar;
				if(selection=="d")
				{
					skalar=Vector1*Vector2;
					VectorOutput(&Vector1,&Vector2,&skalar,0,dim,0);
				}
				else
				{
					v_erg=Vector1&Vector2;
					VectorOutput(&Vector1,&Vector2,0,&v_erg,dim,1);
				}				
			}return "return_to_mainmenu";
//------------------------------------------------------------------------------------------------
		case 'c':
		{
				int i,j,l,m,n=1;
				char idx_m='A',idx_m_temp='X';
				char idx_n[2], idx_n_temp[2];
				stringstream conv;

				UI.DrawBox(2,4,52,23);
				UI.DrawBox(53,4,77,23);

				GAUSS_REAL;

				int selection;
				
				UI.GotoXY(5,55); cout<<"unknowns count (1-10)";
				UI.GotoXY(6,55); cout<<">";

				conv<<GetUserInput(0,0,1,0, 56,7, 56,6,0,0,1,10);
				conv>>selection;

				UI.GotoXY(5,4);

				cout<<"please insert equation system";

				for(j=1, l=7; j<=selection; j++, l++, n++)
				{
					for(i=1, m=4; i<=selection+1; i++)
					{
						
						UI.GotoXY(l,m);
						_itoa_s(i,idx_n,10);
						_itoa_s(n,idx_n_temp,10);

						if(i==selection+1)
						{
							cout<<idx_m_temp<<idx_n_temp<<"=";
						}

						else
							cout<<idx_m<<idx_n<<"=";

						temp=GetUserInput(0,0,2,0, m+3,l+1,m+3,l,j,i,0,0);
						
						if(i<=9)
							m+=4+temp.size();
						else
							m+=5+temp.size();

						if((m>=45))
						{
							UI.ClearScreen(4,50,l,l);
							m=4;
						}
					}

					UI.GotoXY(8,4);

					idx_m++;
					i=4;
					UI.GotoXY(j,i);
				}

			 }return input_str;
//------------------------------------------------------------------------------------------------
		case 'd': 	
			{
				int dim;
				bool scrollx=0, scrolly=0;
				int i,j,l,m, cc=0;
				char idx_m='A';
				char idx_n[2];
				stringstream conv;

				matrix MAT;

				UI.DrawBox(2,4,52,23);
				UI.DrawBox(53,4,77,23);

				DETERM_REAL;		

				UI.GotoXY(5,55); cout<<"dimension (MAX 20)";
				UI.GotoXY(6,55); cout<<">";

				dim=CMPX.from_string<int>(GetUserInput(0,0,1,0, 56,7, 56,6,0,0,2,20));

				qmatrix qm(dim);

				for(j=1, l=6; j<=dim; j++, l++){
					for(i=1, m=4; i<=dim; i++){
						
						UI.GotoXY(l,m);
						_itoa_s(i,idx_n,10);
						cout<<idx_m<<idx_n<<"=";

						temp=GetUserInput(0,&qm,2,1, m+3,l+1,m+3,l,j,i,0,0);
						
						if(i<=9)
							m+=4+temp.size();
						else
							m+=5+temp.size();
						
						if(m>=40){
							UI.ClearScreen(4,50,l,l); m=4;
						}
					}
					idx_m++;
					i=4;
					UI.GotoXY(j,i);
				}
				UI.DrawFrame();
				UI.DrawString(4,2,"result:");

				UI.DrawCharts(&det_chart1[0],7,7,7,12);
				UI.SetConsoleColor(GREEN,WHITE);
				UI.GotoXY(10,19);
					cout<<" = "<<qm.determinante(dim);
				UI.SetConsoleColor(BLACK,WHITE);
				UI.DrawString(22,2,"Press any key to return to main menu.");
				busy=false;
				_getch();
				busy=true;
				fflush(stdin);
				
			}return "return_to_mainmenu";
//------------------------------------------------------------------------------------------------
				case 'e':{	EIGENVL_REAL; }return input_str;
//------------------------------------------------------------------------------------------------
				case 'f':{	EIGENVC_REAL; }return input_str;
//------------------------------------------------------------------------------------------------
				}
	return "NULL";
}
string MatrixOutput(matrix *matrix_erg,int row1_count,int col2_count)
{
	int i,j,l,m;

	char idx_m='A';
	char idx_n[2];

	graphics UI;

	UI.DrawFrame();
	UI.DrawString(4,2,"result:");

	for(j=1, l=7; j<=row1_count; j++, l++)
	{
		for(i=1, m=4; i<=col2_count; i++)
		{
			UI.GotoXY(l,m);
			_itoa_s(i,idx_n,10);
			cout<<idx_m<<idx_n<<"=";

			UI.SetConsoleColor(GREEN,WHITE);

			cout<<matrix_erg->GetComp(j-1,i-1);

			UI.SetConsoleColor(BLACK,WHITE);

			m+=4+matrix_erg->GetComp(j-1,i-1).CompLen();
		}
		idx_m++;
	}

	UI.DrawString(22,2,"Press any key to return to main menu.");
	busy=false;
	_getch();
	busy=true;
	fflush(stdin);
	return "return_to_mainmenu";
}
string VectorOutput(Vector *v1,Vector *v2,Complex *skalar,Vector *v_erg,int dim,int cmd)
{
	int i,l=7,m=7;

	char idx_m='X';
	char idx_n[2];

	graphics UI;

	UI.DrawFrame();
	UI.DrawString(4,2,"result:");

	if(!cmd)
	{
		UI.DrawCharts(&vc_erg_chart1[0],7,7,5,11);
		UI.GotoXY(m+2,l+11);
		cout<<" = "<<*skalar;
	}

	else
	{
		UI.DrawCharts(&vc_erg_chart2[0],7,7,5,11);
		UI.GotoXY(9,18);
		cout<<" = ";

		for(i=1, m=7, l=21; i<=dim; i++)
		{
			UI.GotoXY(m,l);
			_itoa_s(i,idx_n,10);
			cout<<idx_m<<idx_n<<"=";

			UI.SetConsoleColor(GREEN,WHITE);

			cout<<v_erg->GetElem(i-1);

			UI.SetConsoleColor(BLACK,WHITE);

			m+=2;
		}
	}

	UI.DrawString(22,2,"Press any key to return to main menu.");
	busy=false;
	_getch();
	busy=true;
	fflush(stdin);
	return "return_to_mainmenu";
}