// knight.c - program calculates how many moves it take to move a chess knight
// from one square to another (squares entered by user)
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
enum ver {A=1, B, C, D, E, F, G, H};			// columns
enum hor {H1=1, H2, H3, H4, H5, H6, H7, H8};	// rows
#define SQUARES 64								// number of squares on board
struct square 		// holds square
{
	enum ver v;		// 1..8
	enum hor h;		// 1..8
	char str[3];	// a1..h8
};
typedef struct square S;
void btos(S *s, int i);		/* convert board[]'s element into S */
int stob(S *s);			/* convert S into board[]'s element */ 
int getsq(S *s);		/* get square from user */
void printsq(S *s);		/* print square */
int knightmoves(S *s1, S *s2);  /* how many moves does it take - to move knight from s1 to s2 */
int knightmoveone(int a, int b);	/* can knight move from a to b in one move */
void printboard(int b[]);
int main(int argc, char *argv[])
{
	S s1, s2;
	int result=0;
	int board[SQUARES], i, n;

	printf("enter first square:\n");
	getsq(&s1);
	printf("enter second square:\n");
	getsq(&s2);
	knightmoves(&s1, &s2);	

	return 0;
}


int knightmoves(S *s1, S *s2)
{
	int board[SQUARES]; /* working board */
	int b[SQUARES];	/* array of squares with moves */
	int i, j, k;
	int moves;	/* level of moves */
	int bcurrent;	/* number of squares in b[] array (must be 63 at the end) */
	int bccopy;	/* copy of bcurrent */

	moves=0;
	for(i=0; i<SQUARES; i++)	/* populate board with -1 */
		board[i]=-1;
	board[stob(s1)]=0;		/* here the knight(s1) stand */
	b[moves]=stob(s1);		/* by far we have only one square with number of moves (s1 with 0 moves) */
	bcurrent=1;
	while(bcurrent < 64)
	{
		moves++;
		bccopy=bcurrent;
		for(i=0; i<64; i++)
		{
			if(board[i]>=0)				/* this square already has moves number */
				continue;
			for(j=0; j<bcurrent; j++)		/* the square doesn't have moves number; so we check this square with each element in b[] array */
			{					/* note for further improving: we don't need check all b[]-array, only those elements which have moves-1 */
				if(knightmoveone(i, b[j]))	/* can knight move from i to b[j] in one move? */
				{
					
					board[i]=moves;		/* fill in with moves */
					b[bccopy++]=i;		/* include board[i] into b[] array */
					break;
				}
			}
		}
		bcurrent=bccopy;
	}
	printf("knight can move from %s to %s in %d moves\n", s1->str, s2->str, board[stob(s2)]);
	printboard(board);
	return 1;
}
		


void printboard(int b[])
{
	int i, x, c;

	printf("\n");
	c=8;			// 8..1
	for(x=1; x<=8; x++)
	{
		printf("%d|", c--);
		for(i=SQUARES-8*x; i<(SQUARES-8*x+8); i++)
		{
			printf("%3d", b[i]);
		}
		printf("\n");
	}
	c=97;		// 97 - 'a'
	printf("  ");
	for(i=0; i<8; i++)
		printf("%3c", c++);
	printf("\n");
}

int knightmoveone(int a, int b)		/* can knight move from a to b in one move */
{
	S sa, sb;
	int deltahor, deltaver;

	btos(&sa, a);
	btos(&sb, b);
	deltahor=sa.h - sb.h;	// vertical delta
	if(deltahor<0)
		deltahor*=-1;
	deltaver=sa.v - sb.v;	// horizontal delta
	if(deltaver<0)
		deltaver*=-1;
	if((deltaver==2 && deltahor==1) || (deltaver==1 && deltahor==2))
		return 1;	/* knight can move from a to b in one move */
	
	return 0;		/* can not */
}
	
	

void btos(S *s, int i)	/* convert board[]'s element into S; take integer 0..63 and return S */
{
	s->h=i/8 + 1;
	s->v=i%8 + 1;
}

int stob(S *s)			/* convert S into board[]'s element; return integer 0..63 */
{
	int i;
	
	i=(s->h-1)*8+s->v-1;
	
	return i;
}


void printsq(S *s)
{
	printf("%s\n", s->str);
	printf("v: %d\n", s->v);
	printf("h: %d\n", s->h);
}

int getsq(S *s)			/* get square from user */
{
	char buf[80];
	char *enterstr="enter a square (like e2 for example): \n";
	char *wrongstr="wrong input!!!\n";

	while(1)	// until we get valid input
	{
		write(1, enterstr, strlen(enterstr));
		read(0, buf, sizeof buf);
		buf[2]=0;		// keep only two first chars
		if(buf[0]>'h' || buf[0]<'a')	// validate letter (a..h)
		{
			write(1, wrongstr, strlen(wrongstr));
			continue;
		}
		else if(buf[1]>'8' || buf[1]<'1')	// validate horizontal (1..8)
		{
			write(1, wrongstr, strlen(wrongstr));
			continue;
		}
		s->v=buf[0]-96;
		s->h=buf[1]-48;
		strcpy(s->str, buf);
		break;
	}
	return 1;
}
		
