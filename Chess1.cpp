
#include<iostream>
#include<curses.h>
#include <unistd.h>     //_getche*/
#include <termios.h>    //_getche*/
using namespace std;
char getche(){             //Getch not available in linux
    
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c",buf);
    return buf;
 }
int piece_type,playr_type,a,b,c,d,z,x;
static int checkmate,bkx,wkx,bky,wky,pos_x,pos_y; //integers that can be changed throughout the program 
enum { KINGW=1,QUEENW=2,BISHOPW=3,ROOKW=4,KNIGHTW=5,PAWNW=6,KINGB=7,QUEENB=8,BISHOPB=9,ROOKB=10,KNIGHTB=11,PAWNB=12 };
int Board_Piece[8][8]={ ROOKW,KNIGHTW,BISHOPW,KINGW,QUEENW,BISHOPW,KNIGHTW,ROOKW, //declare board
			PAWNW,PAWNW,PAWNW,PAWNW,PAWNW,PAWNW,PAWNW,PAWNW,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			PAWNB,PAWNB,PAWNB,PAWNB,PAWNB,PAWNB,PAWNB,PAWNB,
			ROOKB,KNIGHTB,BISHOPB,KINGB,QUEENB,BISHOPB,KNIGHTB,ROOKB};
int Board_Piece_Backup[8][8]={
		        0,0,0,0,0,0,0,0, //declare backup board for implementing accidental cutting of king using check func
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,
			     };

int init_pos_x,init_pos_y,fina_pos_x,fina_pos_y;
int Move_Valid(int init_pos_x,int init_pos_y,int fina_pos_x,int fina_pos_y,int piece_type)//move validity
{
	a=init_pos_x;
	b=init_pos_y;
	c=fina_pos_x;
	d=fina_pos_y;

if((a>8&&a<=0)||(b>8&&b<=0)||(c>8&&c<=0)||(d>8&&d<=0))//invalid moves
	return 0;
else if(piece_type == 1||piece_type ==7)//king
{	
	if(piece_type == 1)
	{
		if(Board_Piece[c][d]==0||Board_Piece[c][d]>=7)//change for testing
		{
			if((a==c&&b==d+1)||(a==c+1&&b==d)||(a==c&&b+1==d)||(a+1==c&&b==d)||
			   (a==c+1&&b==d+1)||(a+1==c&&b==d+1)||(a+1==c&&b+1==d)||(a==c+1&&b+1==d))
			return 1;
			else 
			return 0;
		}
	else
	return 0;
	}
	if(piece_type == 7)
	{
		if(Board_Piece[c][d]==0||Board_Piece[c][d]<=7)//change for testing
		{
			if((a==c&&b==d+1)||(a==c+1&&b==d)||(a==c&&b+1==d)||(a+1==c&&b==d)||
			   (a==c+1&&b==d+1)||(a+1==c&&b==d+1)||(a+1==c&&b+1==d)||(a==c+1&&b+1==d))
			return 1;
			else 
			return 0;
		}
	else
	return 0;
	}
else
return 0;
}
else if(piece_type == 2||piece_type == 8)//queen
{	
	if(piece_type == 2)
	{
		if(Board_Piece[c][d]==0||Board_Piece[c][d]>=7)//change for testing
		{
			if((a==c&&b+1==d)||(a==c&&b+2==d)||(a==c&&b+3==d)||(a==c&&b+4==d)||
			   (a==c&&b+5==d)||(a==c&&b+6==d)||(a==c&&b+7==d)||(a==c&&b+8==d)||
			   (a==c&&b==d+1)||(a==c&&b==d+2)||(a==c&&b==d+3)||(a==c&&b==d+4)||
			   (a==c&&b==d+5)||(a==c&&b==d+6)||(a==c&&b==d+7)||(a==c&&b==d+8)||
			   (a+1==c&&b==d)||(a+2==c&&b==d)||(a+3==c&&b==d)||(a+4==c&&b==d)||
			   (a+5==c&&b==d)||(a+6==c&&b==d)||(a+7==c&&b==d)||(a+8==c&&b==d)||
			   (a==c+1&&b==d)||(a==c+2&&b==d)||(a==c+3&&b==d)||(a==c+4&&b==d)||
			   (a==c+5&&b==d)||(a==c+6&&b==d)||(a==c+7&&b==d)||(a==c+8&&b==d)||
			   (a+1==c&&b+1==d)||(a+2==c&&b+2==d)||(a+3==c&&b+3==d)||(a+4==c&&b+4==d)||
			   (a+5==c&&b+5==d)||(a+6==c&&b+6==d)||(a+7==c&&b+7==d)||(a+8==c&&b+8==d)||
			   (a-1==c&&b-1==d)||(a-2==c&&b-2==d)||(a-3==c&&b-3==d)||(a-4==c&&b-4==d)||
			   (a-5==c&&b-5==d)||(a-6==c&&b-6==d)||(a-7==c&&b-7==d)||(a-8==c&&b-8==d)||
			   (a-1==c&&b+1==d)||(a-2==c&&b+2==d)||(a-3==c&&b+3==d)||(a-4==c&&b+4==d)||
			   (a-5==c&&b+5==d)||(a-6==c&&b+6==d)||(a-7==c&&b+7==d)||(a-8==c&&b+8==d)||
			   (a+1==c&&b-1==d)||(a+2==c&&b-2==d)||(a+3==c&&b-3==d)||(a+4==c&&b-4==d)||
			   (a+5==c&&b-5==d)||(a+6==c&&b-6==d)||(a+7==c&&b-7==d)||(a+8==c&&b-8==d))
			return 1;
			else 
			return 0;
		}
	else
	return 0;
	}
	if(piece_type == 8)
	{
		if(Board_Piece[c][d]==0||Board_Piece[c][d]<=7)//change for testing
		{
			if((a==c&&b+1==d)||(a==c&&b+2==d)||(a==c&&b+3==d)||(a==c&&b+4==d)||
			   (a==c&&b+5==d)||(a==c&&b+6==d)||(a==c&&b+7==d)||(a==c&&b+8==d)||
			   (a==c&&b==d+1)||(a==c&&b==d+2)||(a==c&&b==d+3)||(a==c&&b==d+4)||
			   (a==c&&b==d+5)||(a==c&&b==d+6)||(a==c&&b==d+7)||(a==c&&b==d+8)||
			   (a+1==c&&b==d)||(a+2==c&&b==d)||(a+3==c&&b==d)||(a+4==c&&b==d)||
			   (a+5==c&&b==d)||(a+6==c&&b==d)||(a+7==c&&b==d)||(a+8==c&&b==d)||
			   (a==c+1&&b==d)||(a==c+2&&b==d)||(a==c+3&&b==d)||(a==c+4&&b==d)||
			   (a==c+5&&b==d)||(a==c+6&&b==d)||(a==c+7&&b==d)||(a==c+8&&b==d)||
			   (a+1==c&&b+1==d)||(a+2==c&&b+2==d)||(a+3==c&&b+3==d)||(a+4==c&&b+4==d)||
			   (a+5==c&&b+5==d)||(a+6==c&&b+6==d)||(a+7==c&&b+7==d)||(a+8==c&&b+8==d)||
			   (a-1==c&&b-1==d)||(a-2==c&&b-2==d)||(a-3==c&&b-3==d)||(a-4==c&&b-4==d)||
			   (a-5==c&&b-5==d)||(a-6==c&&b-6==d)||(a-7==c&&b-7==d)||(a-8==c&&b-8==d)||
			   (a-1==c&&b+1==d)||(a-2==c&&b+2==d)||(a-3==c&&b+3==d)||(a-4==c&&b+4==d)||
			   (a-5==c&&b+5==d)||(a-6==c&&b+6==d)||(a-7==c&&b+7==d)||(a-8==c&&b+8==d)||
			   (a+1==c&&b-1==d)||(a+2==c&&b-2==d)||(a+3==c&&b-3==d)||(a+4==c&&b-4==d)||
			   (a+5==c&&b-5==d)||(a+6==c&&b-6==d)||(a+7==c&&b-7==d)||(a+8==c&&b-8==d))
			return 1;
			else 
			return 0;
		}
	else
	return 0;
	}
else
return 0;
}
else if(piece_type == 3||piece_type == 9)//bishop
{	
	if(piece_type == 3)
	{
		if(Board_Piece[c][d]==0||Board_Piece[c][d]>=7)//change for testing
		{
			if((a+1==c&&b+1==d)||(a+2==c&&b+2==d)||(a+3==c&&b+3==d)||(a+4==c&&b+4==d)||
			   (a+5==c&&b+5==d)||(a+6==c&&b+6==d)||(a+7==c&&b+7==d)||(a+8==c&&b+8==d)||
			   (a-1==c&&b-1==d)||(a-2==c&&b-2==d)||(a-3==c&&b-3==d)||(a-4==c&&b-4==d)||
			   (a-5==c&&b-5==d)||(a-6==c&&b-6==d)||(a-7==c&&b-7==d)||(a-8==c&&b-8==d)||
			   (a-1==c&&b+1==d)||(a-2==c&&b+2==d)||(a-3==c&&b+3==d)||(a-4==c&&b+4==d)||
			   (a-5==c&&b+5==d)||(a-6==c&&b+6==d)||(a-7==c&&b+7==d)||(a-8==c&&b+8==d)||
			   (a+1==c&&b-1==d)||(a+2==c&&b-2==d)||(a+3==c&&b-3==d)||(a+4==c&&b-4==d)||
			   (a+5==c&&b-5==d)||(a+6==c&&b-6==d)||(a+7==c&&b-7==d)||(a+8==c&&b-8==d))
			return 1;
			else 
			return 0;
		}
	else
	return 0;
	}
	if(piece_type == 9)
	{
		if(Board_Piece[c][d]==0||Board_Piece[c][d]<=7)//change for testing
		{
			if((a+1==c&&b+1==d)||(a+2==c&&b+2==d)||(a+3==c&&b+3==d)||(a+4==c&&b+4==d)||
			   (a+5==c&&b+5==d)||(a+6==c&&b+6==d)||(a+7==c&&b+7==d)||(a+8==c&&b+8==d)||
			   (a-1==c&&b-1==d)||(a-2==c&&b-2==d)||(a-3==c&&b-3==d)||(a-4==c&&b-4==d)||
			   (a-5==c&&b-5==d)||(a-6==c&&b-6==d)||(a-7==c&&b-7==d)||(a-8==c&&b-8==d)||
			   (a-1==c&&b+1==d)||(a-2==c&&b+2==d)||(a-3==c&&b+3==d)||(a-4==c&&b+4==d)||
			   (a-5==c&&b+5==d)||(a-6==c&&b+6==d)||(a-7==c&&b+7==d)||(a-8==c&&b+8==d)||
			   (a+1==c&&b-1==d)||(a+2==c&&b-2==d)||(a+3==c&&b-3==d)||(a+4==c&&b-4==d)||
			   (a+5==c&&b-5==d)||(a+6==c&&b-6==d)||(a+7==c&&b-7==d)||(a+8==c&&b-8==d))
			return 1;
			else 
			return 0;
		}
	else
	return 0;
	}
else
return 0;
}
else if(piece_type == 4||piece_type ==10)//rook
{	
	if(piece_type == 4)
	{
		if(Board_Piece[c][d]==0||Board_Piece[c][d]>=7)//change for testing
		{
			if((a==c&&b+1==d)||(a==c&&b+2==d)||(a==c&&b+3==d)||(a==c&&b+4==d)||
			   (a==c&&b+5==d)||(a==c&&b+6==d)||(a==c&&b+7==d)||(a==c&&b+8==d)||
			   (a==c&&b==d+1)||(a==c&&b==d+2)||(a==c&&b==d+3)||(a==c&&b==d+4)||
			   (a==c&&b==d+5)||(a==c&&b==d+6)||(a==c&&b==d+7)||(a==c&&b==d+8)||
			   (a+1==c&&b==d)||(a+2==c&&b==d)||(a+3==c&&b==d)||(a+4==c&&b==d)||
			   (a+5==c&&b==d)||(a+6==c&&b==d)||(a+7==c&&b==d)||(a+8==c&&b==d)||
			   (a==c+1&&b==d)||(a==c+2&&b==d)||(a==c+3&&b==d)||(a==c+4&&b==d)||
			   (a==c+5&&b==d)||(a==c+6&&b==d)||(a==c+7&&b==d)||(a==c+8&&b==d))
			return 1;
			else 
			return 0;
		}
	else
	return 0;
	}
	if(piece_type == 10)
	{
		if(Board_Piece[c][d]==0||Board_Piece[c][d]<=7)//change for testing
		{
			if((a==c&&b+1==d)||(a==c&&b+2==d)||(a==c&&b+3==d)||(a==c&&b+4==d)||
			   (a==c&&b+5==d)||(a==c&&b+6==d)||(a==c&&b+7==d)||(a==c&&b+8==d)||
			   (a==c&&b==d+1)||(a==c&&b==d+2)||(a==c&&b==d+3)||(a==c&&b==d+4)||
			   (a==c&&b==d+5)||(a==c&&b==d+6)||(a==c&&b==d+7)||(a==c&&b==d+8)||
			   (a+1==c&&b==d)||(a+2==c&&b==d)||(a+3==c&&b==d)||(a+4==c&&b==d)||
			   (a+5==c&&b==d)||(a+6==c&&b==d)||(a+7==c&&b==d)||(a+8==c&&b==d)||
			   (a==c+1&&b==d)||(a==c+2&&b==d)||(a==c+3&&b==d)||(a==c+4&&b==d)||
			   (a==c+5&&b==d)||(a==c+6&&b==d)||(a==c+7&&b==d)||(a==c+8&&b==d))
			return 1;
			else 
			return 0;
		}
	else
	return 0;
	}
else
return 0;
}
else if(piece_type == 5||piece_type == 11)//knight
{	
	if(piece_type == 5)
	{
		if(Board_Piece[c][d]==0||Board_Piece[c][d]>=7)//change for testing
		{
			if((a+2==c&&b+1==d)||(a+2==c&&b==d+1)||(a==c+2&&b==d+1)||(a==c+2&&b+1==d)||
			   (a+1==c&&b+2==d)||(a==c+1&&b+2==d)||(a+1==c&&b==d+2)||(a==c+2&&b==d+2))
			return 1;
			else 
			return 0;
		}
	else
	return 0;
	}
	if(piece_type == 11)
	{
		if(Board_Piece[c][d]==0||Board_Piece[c][d]<=7)//change for testing
		{
			if((a+2==c&&b+1==d)||(a+2==c&&b==d+1)||(a==c+2&&b==d+1)||(a==c+2&&b+1==d)||
			   (a+1==c&&b+2==d)||(a==c+1&&b+2==d)||(a+1==c&&b==d+2)||(a==c+2&&b==d+2))
			return 1;
			else 
			return 0;
		}
	else
	return 0;
	}
else
return 0;
}
else if(piece_type == 6||piece_type ==12)//pawn
{	
	if(piece_type == 6)
	{
		if(Board_Piece[c][d]==0)//change for testing
		{      
			if(a==1)//specify conditions
				{	
				if((a+2==c&&b==d)||(a+1==c&&b==d))	
				return 1;
				else
				return 0;
				}
			else if(a!=1)
				{
				if(a+1==c&&b==d)
				return 1;
				else
				return 0;
				}
			else 
			return 0;
		}
		else if(Board_Piece[c][d]>=7)//change for testing
		{
			if((a+1==c&&b==d+1)||(a+1==c&&b+1==d))//specify conditions
			return 1;
			else 
			return 0;
		}
		else
		return 0;
	}
	if(piece_type == 12)
	{
		if(Board_Piece[c][d]==0)//change for testing
			{
			if(a==6)//specify conditions
				{	
				if((a==c+2&&b==d)||(a==c+1&&b==d))	
				return 1;
				else 
				return 0;				
				}
			else if(a!=1)
				{
				if(a==c+1&&b==d)
				return 1;
				else
				return 0;
				}
			else 
			return 0;
			}
		else if(Board_Piece[c][d]<=7)//change for testing
			{
			if((a==c+1&&b==d+1)||(a==c+1&&b+1==d))//specify conditions
			return 1;
			else 
			return 0;
			}
		else
		return 0;
	}
	
	else
	return 0;
	
}


else
return 0;
}
int Piece_Exist(int piece_type,int q,int w)
{		
	     if(Board_Piece[q][w]!=piece_type)
		return 0;
	      else 
	{	return 1;	
	 }     
}bool Move_Hindrance(int piece_type,int init_pos_x,int init_pos_y,int fina_pos_x,int fina_pos_y)
{
int aa,ab,bb,ba,qw,wq,we,ew,dump=0;
aa=init_pos_x;
ab=init_pos_y;
bb=fina_pos_x;
ba=fina_pos_y;
if(piece_type==3||piece_type==9)
{
	if(aa>bb)//moving up
	{	
		if(ab<ba)//right diagonal
		{ qw=aa-bb;
			
		
			for(int i=0;i<qw-1;i++)
			{
			aa--;
			ab++;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
		}
		else//left diagonal
		{ qw=aa-bb;
			
		
			for(int i=0;i<qw-1;i++)
			{
			aa--;
			ab--;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
		}
	}
	if(aa<bb)//moving down
	{
		if(ab<ba)//right diagonal
		{
		qw=bb-aa;
			for(int i=0;i<qw-1;i++)
			{
			aa++;
			ab++;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
		}
		else//left diagonal
		{
		qw=bb-aa;
			for(int i=0;i<qw-1;i++)
			{
			aa++;
			ab--;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
		}
	}
	else
	return 1;
}
if(piece_type==4||piece_type==10)
{
 if(aa>bb)//move up
{
qw=aa-bb;
for(int i=0;i<qw-1;i++)
			{
			aa--;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
return 1;
}
 if(aa<bb)//move down
{
qw=bb-aa;
for(int i=0;i<qw-1;i++)
			{
			aa++;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
return 1;
}
 if(ba>ab)//move right
{
qw=ba-ab;
for(int i=0;i<qw-1;i++)
			{
			ab++;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
return 1;
}
 if(ab>ba)//move left
{
qw=ab-ba;
for(int i=0;i<qw-1;i++)
			{
			ab--;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
return 1;
}
 else
 return 1;
}	
else if(piece_type==2|piece_type==8)
{
return 1;
}
else
return 1;
}
bool Move_Hindrance_Check(int piece_type,int init_pos_x,int init_pos_y,int fina_pos_x,int fina_pos_y)
{
int aa,ab,bb,ba,qw,wq,we,ew,dump=0;
aa=init_pos_x;
ab=init_pos_y;
bb=fina_pos_x;
ba=fina_pos_y;
if(piece_type==3||piece_type==9)
{
	if(aa>bb)//moving up
	{	
		if(ab<ba)//right diagonal
		{ qw=aa-bb;
			
		
			for(int i=0;i<qw-1;i++)
			{
			aa--;
			ab++;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
		}
		else//left diagonal
		{ qw=aa-bb;
			
		
			for(int i=0;i<qw-1;i++)
			{
			aa--;
			ab--;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
		}
	}
	if(aa<bb)//moving down
	{
		if(ab<ba)//right diagonal
		{
		qw=bb-aa;
			for(int i=0;i<qw-1;i++)
			{
			aa++;
			ab++;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
		}
		else//left diagonal
		{
		qw=bb-aa;
			for(int i=0;i<qw-1;i++)
			{
			aa++;
			ab--;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
		}
	}
	else
	return 1;
}
if(piece_type==4||piece_type==10)
{
 if(aa>bb)//move up
{
qw=aa-bb;
for(int i=0;i<qw-1;i++)
			{
			aa--;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
return 1;
}
 if(aa<bb)//move down
{
qw=bb-aa;
for(int i=0;i<qw-1;i++)
			{
			aa++;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
return 1;
}
 if(ba>ab)//move right
{
qw=ba-ab;
for(int i=0;i<qw-1;i++)
			{
			ab++;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
return 1;
}
 if(ab>ba)//move left
{
qw=ab-ba;
for(int i=0;i<qw-1;i++)
			{
			ab--;
				if(Board_Piece[aa][ab]==0)
				{dump++;
			if(dump-1==qw)
			return 1;}
				else
				return 0;
			}
return 1;
}
 else
 return 1;
}	
else if(piece_type==2|piece_type==8)
{
int pc,pd;
pc=Move_Hindrance_Check(3,init_pos_x,init_pos_y,fina_pos_x,fina_pos_y);
pd=Move_Hindrance_Check(4,init_pos_x,init_pos_y,fina_pos_x,fina_pos_y);
if(pc==1||pd==1)
return 1;
else 
return 0;
}
else
return 1;
}
/*bool check_move_map(int init_pos_x,int init_pos_y,int piece_id)
{
int a=init_pos_x;
int b=init_pos_y;
if(piece_id<=6)
{
if(piece_id==1)
{
if(Board_Piece[a][b-1]==7||Board_Piece[a-1][b]==7||Board_Piece[a][b+1]==7||
Board_Piece[a+1][b]==7||Board_Piece[a-1][b-1]==7||Board_Piece[a+1][b-1]==7||Board_Piece[a+1][b+1]==7||Board_Piece[a-1][b+1]==7)
return 1;
else
return 0;
}
if(piece_id==2)
{
if(0)
return 1;
else
return 0;
}
if(piece_id==3)
{
if(Board_Piece[a+1][b+1]==7){return 1;}
if(Board_Piece[a+2][b+2]==7){int valu=Move_Hindrance_Check(3,a,b,a+2,b+2); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+3][b+3]==7){int valu=Move_Hindrance_Check(3,a,b,a+3,b+3); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+4][b+4]==7){int valu=Move_Hindrance_Check(3,a,b,a+4,b+4); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+5][b+5]==7){int valu=Move_Hindrance_Check(3,a,b,a+5,b+5); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+6][b+6]==7){int valu=Move_Hindrance_Check(3,a,b,a+6,b+6); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+7][b+7]==7){int valu=Move_Hindrance_Check(3,a,b,a+7,b+7); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+8][b+8]==7){int valu=Move_Hindrance_Check(3,a,b,a+8,b+8); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-1][b-1]==7){return 1;}
if(Board_Piece[a-2][b-2]==7){int valu=Move_Hindrance_Check(3,a,b,a-2,b-2); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-3][b-3]==7){int valu=Move_Hindrance_Check(3,a,b,a-3,b-3); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-4][b-4]==7){int valu=Move_Hindrance_Check(3,a,b,a-4,b-4); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-5][b-5]==7){int valu=Move_Hindrance_Check(3,a,b,a-5,b-5); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-6][b-6]==7){int valu=Move_Hindrance_Check(3,a,b,a-6,b-6); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-7][b-7]==7){int valu=Move_Hindrance_Check(3,a,b,a-7,b-7); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-8][b-8]==7){int valu=Move_Hindrance_Check(3,a,b,a-8,b-8); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-1][b+1]==7){return 1;}
if(Board_Piece[a-2][b+2]==7){int valu=Move_Hindrance_Check(3,a,b,a-2,b+2); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-3][b+3]==7){int valu=Move_Hindrance_Check(3,a,b,a-3,b+3); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-4][b+4]==7){int valu=Move_Hindrance_Check(3,a,b,a-4,b+4); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-5][b+5]==7){int valu=Move_Hindrance_Check(3,a,b,a-5,b+5); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-6][b+6]==7){int valu=Move_Hindrance_Check(3,a,b,a-6,b+6); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-7][b+7]==7){int valu=Move_Hindrance_Check(3,a,b,a-7,b+7); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-8][b+8]==7){int valu=Move_Hindrance_Check(3,a,b,a-8,b+8); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+1][b-1]==7){return 1;}
if(Board_Piece[a+2][b-2]==7){int valu=Move_Hindrance_Check(3,a,b,a+2,b-2); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+3][b-3]==7){int valu=Move_Hindrance_Check(3,a,b,a+3,b-3); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+4][b-4]==7){int valu=Move_Hindrance_Check(3,a,b,a+4,b-4); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+5][b-5]==7){int valu=Move_Hindrance_Check(3,a,b,a+5,b-5); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+6][b-6]==7){int valu=Move_Hindrance_Check(3,a,b,a+6,b-6); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+7][b-7]==7){int valu=Move_Hindrance_Check(3,a,b,a+7,b-7); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+8][b-8]==7){int valu=Move_Hindrance_Check(3,a,b,a+8,b-8); if(valu==1) return 0; else return 1;}
else
return 0;
}
if(piece_id==4)
{
if(0)
return 1;
else
return 0;
}
if(piece_id==5)
{
if(0)
return 1;
else
return 0;
}
if(piece_id==6)
{
if(0)
return 1;
else
return 0;
}
}
else
{
if(piece_id==7)
{
if(Board_Piece[a][b-1]==1||Board_Piece[a-1][b]==1||Board_Piece[a][b+1]==1||
Board_Piece[a+1][b]==1||Board_Piece[a-1][b-1]==1||Board_Piece[a+1][b-1]==1||Board_Piece[a+1][b+1]==1||Board_Piece[a-1][b+1]==1)
return 1;
else
return 0;
}
if(piece_id==8)
{
if(0)
return 1;
else
return 0;
}
if(piece_id==9)
{
if(Board_Piece[a+1][b+1]==1){return 1;}
if(Board_Piece[a+2][b+2]==1){int valu=Move_Hindrance_Check(3,a,b,a+2,b+2); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+3][b+3]==1){int valu=Move_Hindrance_Check(3,a,b,a+3,b+3); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+4][b+4]==1){int valu=Move_Hindrance_Check(3,a,b,a+4,b+4); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+5][b+5]==1){int valu=Move_Hindrance_Check(3,a,b,a+5,b+5); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+6][b+6]==1){int valu=Move_Hindrance_Check(3,a,b,a+6,b+6); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+7][b+7]==1){int valu=Move_Hindrance_Check(3,a,b,a+7,b+7); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+8][b+8]==1){int valu=Move_Hindrance_Check(3,a,b,a+8,b+8); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-1][b-1]==1){return 1;}
if(Board_Piece[a-2][b-2]==1){int valu=Move_Hindrance_Check(3,a,b,a-2,b-2); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-3][b-3]==1){int valu=Move_Hindrance_Check(3,a,b,a-3,b-3); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-4][b-4]==1){int valu=Move_Hindrance_Check(3,a,b,a-4,b-4); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-5][b-5]==1){int valu=Move_Hindrance_Check(3,a,b,a-5,b-5); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-6][b-6]==1){int valu=Move_Hindrance_Check(3,a,b,a-6,b-6); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-7][b-7]==1){int valu=Move_Hindrance_Check(3,a,b,a-7,b-7); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-8][b-8]==1){int valu=Move_Hindrance_Check(3,a,b,a-8,b-8); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-1][b+1]==1){return 1;}
if(Board_Piece[a-2][b+2]==1){int valu=Move_Hindrance_Check(3,a,b,a-2,b+2); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-3][b+3]==1){int valu=Move_Hindrance_Check(3,a,b,a-3,b+3); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-4][b+4]==1){int valu=Move_Hindrance_Check(3,a,b,a-4,b+4); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-5][b+5]==1){int valu=Move_Hindrance_Check(3,a,b,a-5,b+5); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-6][b+6]==1){int valu=Move_Hindrance_Check(3,a,b,a-6,b+6); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-7][b+7]==1){int valu=Move_Hindrance_Check(3,a,b,a-7,b+7); if(valu==1) return 0; else return 1;}
if(Board_Piece[a-8][b+8]==1){int valu=Move_Hindrance_Check(3,a,b,a-8,b+8); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+1][b-1]==1){return 1;}
if(Board_Piece[a+2][b-2]==1){int valu=Move_Hindrance_Check(3,a,b,a+2,b-2); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+3][b-3]==1){int valu=Move_Hindrance_Check(3,a,b,a+3,b-3); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+4][b-4]==1){int valu=Move_Hindrance_Check(3,a,b,a+4,b-4); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+5][b-5]==1){int valu=Move_Hindrance_Check(3,a,b,a+5,b-5); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+6][b-6]==1){int valu=Move_Hindrance_Check(3,a,b,a+6,b-6); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+7][b-7]==1){int valu=Move_Hindrance_Check(3,a,b,a+7,b-7); if(valu==1) return 0; else return 1;}
if(Board_Piece[a+8][b-8]==1){int valu=Move_Hindrance_Check(3,a,b,a+8,b-8); if(valu==1) return 0; else return 1;}
else
return 0;
}
if(piece_id==10)
{
if(0)
return 1;
else
return 0;
}
if(piece_id==11)
{
if(0)
return 1;
else
return 0;
}
if(piece_id==12)
{
if(0)
return 1;
else
return 0;
}
}
return 0;
}*/
/*bool Checkmate_Check()
{
int var,ee,rr;
for(int ee=0;ee<8;ee++)
for(int rr=0;rr<8;rr++)
{
if(Board_Piece[ee][rr]==1)
var=check_move_map(ee,rr,1);
if(var==1)
return 1;
if(Board_Piece[ee][rr]==2)
var=check_move_map(ee,rr,2);
if(var==1)
return 1;
if(Board_Piece[ee][rr]==3)
var=check_move_map(ee,rr,3);
if(var==1)
return 1;
if(Board_Piece[ee][rr]==4)
var=check_move_map(ee,rr,4);
if(var==1)
return 1;
if(Board_Piece[ee][rr]==5)
var=check_move_map(ee,rr,5);
if(var==1)
return 1;
if(Board_Piece[ee][rr]==6)
var=check_move_map(ee,rr,6);
if(var==1)
return 1;
if(Board_Piece[ee][rr]==7)
var=check_move_map(ee,rr,7);
if(var==1)
return 1;
if(Board_Piece[ee][rr]==8)
var=check_move_map(ee,rr,8);
if(var==1)
return 1;
if(Board_Piece[ee][rr]==9)
var=check_move_map(ee,rr,9);
if(var==1)
return 1;
if(Board_Piece[ee][rr]==10)
var=check_move_map(ee,rr,10);
if(var==1)
return 1;
if(Board_Piece[ee][rr]==11)
var=check_move_map(ee,rr,11);
if(var==1)
return 1;
if(Board_Piece[ee][rr]==12)
var=check_move_map(ee,rr,12);
if(var==1)
return 1;
}
return 0;
}*/
void Piece_Move(int init_pos_x,int init_pos_y,int fina_pos_x,int fina_pos_y,int piece_type,int Board_Piece[8][8])
{	int pc;
	int pa=Piece_Exist(piece_type,init_pos_x,init_pos_y);
	int pb=Move_Valid(init_pos_x,init_pos_y,fina_pos_x,fina_pos_y,piece_type);
	pc=Move_Hindrance_Check(piece_type,init_pos_x,init_pos_y,fina_pos_x,fina_pos_y);
	//int pd=Checkmate_Check();
	int pd=0;
	if (pa==1&&pb==1&&pc==1&&pd==0)
	{
	Board_Piece[a][b]=0;
	Board_Piece[c][d]=piece_type;
	
	}
	if(pa==0)
	cout<<"\n\tPiece Does Not Exist at Position\n";
	else if(pb==0)
	cout<<"\n\tInvalid Move\n";
	else
	cout<<"";
cout<<endl<<"Piece_Exists : "<<pa<<endl<<"Move_Valid : "<<pb<<endl<<"Move_Hindrance : "<<pc<<endl;//<<"Check : "<<pd<<endl;
} 
int Find_Piece_Id(int startx,int starty)
{
int Variable=Board_Piece[startx][starty];
return Variable;
}
int main()
{
int l,startx=0,starty=0,endx=0,endy=0,piece_id=0;
char starta,startb,enda,endb,piece_ida;
int clrtoel(void);
cout<<endl<<" Chess Program (c) R.MOSES PAUL 2017       Board Layout "<<endl; 
cout<<"                 \tROOKW KNIGHTW BISHOPW KINGW QUEENW BISHOPW KNIGHTW ROOKW  \n";
cout<<"			PAWNW PAWNW   PAWNW   PAWNW PAWNW  PAWNW   PAWNW   PAWNW  \n";
cout<<"			  0     0      0	0     0      0       0       0    \n";
cout<<"			  0     0      0	0     0      0       0       0    \n";
cout<<"			  0     0      0	0     0      0       0       0    \n";
cout<<"			  0     0      0	0     0      0       0       0    \n";
cout<<"			PAWNB PAWNB   PAWNB   PAWNB PAWNB  PAWNB   PAWNB   PAWNB  \n";
cout<<"                 \tROOKB KNIGHTB BISHOPB KINGB QUEENB BISHOPB KNIGHTB ROOKB \n"
<<"\n\n\tThe enums are KINGW=1,QUEENW=2,BISHOPW=3,ROOKW=4,KNIGHTW=5,PAWNW=6,\nKINGB=7,QUEENB=8,BISHOPB=9,ROOKB=10,KNIGHTB=11,PAWNB=12\n";
cout<<"\n\tEnter no of Turns:"<<endl;
cin>>l;
cout<<"\b";
for(int s=0;s<l;s++)
{	start:
	starta=getche();
	startx=starta-'0';
	cout<<",";
	startb=getche();
	starty=startb-'0';
	cout<<",";
	enda=getche();
	endx=enda-'0';
	cout<<",";
	endb=getche();
	endy=endb-'0';
	cout<<"";
int piece_idq=Find_Piece_Id(startx,starty);
	if(0)
{	
	cout<<"\t\t"<<"Illegal Move"<<endl;
	goto start;
}
	else	
for(int i=0;i<8;i++)
for(int j=0;j<8;j++)
Board_Piece_Backup[i][j]=Board_Piece[i][j];
	Piece_Move(startx,starty,endx,endy,piece_idq,Board_Piece);
	cout<<endl<<endl<<endl;
{	
	for(int p=0;p<8;p++)
{
	if(p==0)
	cout<<"{   ";
		for(int j=0;j<8;j++)
		{
		cout<<Board_Piece[p][j]<<"   ";
		if(Board_Piece[p][j]>=10)
		cout<<"\b";
		}
	if(p!=7)cout<<endl<<"    ";
	else 
	cout<<"   }";
}
cout<<endl;
}
cout<<endl<<endl;
}

return 0;
}
