#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "limits.h"
#include "string.h"
#define and &&
#define or ||
#define or ||
#define H 7
#define W 7
#define IM INT_MAX
char grid[H][W]=
{{'#','#','#','#','#','#','#'},{'#','.','.','.','.','#','#'},{'#','.','.','.','.','T','#'},{'#','.','.','.','.','B','S'},{'#','.','.','.','.','.','#'},{'#','.','.','.','.','#','#'},{'#','#','#','#','#','#','#'}};
struct pair{
	int x;
	int y;
	};
typedef struct pair tuple;
struct position{
	tuple box;
	tuple me;
	};
typedef struct position state;
tuple* getTuple(int row,int col)
{
	tuple* t=(tuple*)malloc(sizeof(tuple));
	t->x=row;
	t->y=col;
	return t;
}
state* getState(tuple* t1,tuple* t2)
{
	state* s=(state*)malloc(sizeof(state));
	s->box= *t1;
	s->me= *t2;
	return s;
}
struct dictionary{
	state* arr[H*W*H*W];
	int top;
	};
typedef struct dictionary dt;
//getting an empty array of states
dt* getDict()
{
	dt* d=(dt*)malloc(sizeof(dt));
	d->top=0;
	return d;
}
//pushing a state into an array
void push_dict(dt* d,state* s)
{
	d->arr[d->top++]=s;
}
//we need to build an array of dictionaries
struct qu{
	dt* a[H*W*H*W];
	int size;
	};
typedef struct qu q;
//pushing an array of states into a queue
void push_queue(q* gq,dt* d)
{
	gq->a[gq->size++]=d;
}
int isAvailable(int row,int col)
{
	if(row<0 or row>=H or col<0 or col>=W or grid[row][col]=='#')
		return 0;
	return 1;
}
const int d_row[]={-1,1,0,0};
const int d_col[]={0,0,1,-1};
int main()
{
	int processed[H][W][H][W];
	int distance[H][W][H][W];
	memset(processed,0,H*W*H*W*sizeof(int));
	memset(distance,1,sizeof(int)*H*H*W*W);
	tuple* start_m,*start_b,*target;
	int i,j;
	for(i=0;i<H;i++)
	{
		for(j=0;j<W;j++)
		{
			if(grid[i][j]=='S')
				start_m=getTuple(i,j);
			if(grid[i][j]=='B')
				start_b=getTuple(i,j);
			if(grid[i][j]=='T')
				target=getTuple(i,j);
		}
	}
	//printf("%d",start_m->x);
  state* initial=getState(start_b,start_m);
  distance[start_b->x][start_b->y][start_m->x][start_m->y]=0;
	q* gq=(q*)malloc(sizeof(q));
	gq->size=0;
	dt* d=getDict();
	push_dict(d,initial);
	push_queue(gq,d);
	//gq->a[]->arr[]->box->x
	//printf("%d",gq->a[0]->arr[0]->box.x);
	for(i=0;i<gq->size;i++)
	{
		for(j=0;j<gq->a[i]->top;j++)
		{
			state* curr_state=gq->a[i]->arr[j];
			tuple* curr_box=&(curr_state->box);
			tuple* curr_me=&(curr_state->me);
			//printf("%d,%d\n",curr_box->x,curr_box->y);
			if(curr_box->x==target->x and curr_box->y==target->y)
			{
				printf("%d",i);
				exit(1);
			}
			if(processed[curr_box->x][curr_box->y][curr_me->x][curr_me->y])
				continue;
			processed[curr_box->x][curr_box->y][curr_me->x][curr_me->y]=1;
			int dir;
			for(dir=0;dir<4;dir++)
			{
				int new_rol=curr_me->x +d_row[dir];
				int new_col=curr_me->y +d_col[dir];
				if(!isAvailable(new_rol,new_col))
					continue;
				tuple* new_box=curr_box;
				tuple* new_me=getTuple(new_rol,new_col);
				int L=0;
				if(new_rol==curr_box->x and new_col==curr_box->y)
				{
					L=1;
					new_box=getTuple(curr_box->x +d_row[dir],curr_box->y +d_col[dir]);
					if(!isAvailable(new_box->x,new_box->y))
						continue;
				}
				state* new_state=getState(new_box,new_me);
				int dist2=distance[curr_box->x][curr_box->y][curr_me->x][curr_me->y]+L;
				if(dist2<distance[new_box->x][new_box->y][new_me->x][new_me->y])
					distance[new_box->x][new_box->y][new_me->x][new_me->y]=dist2;
				while(dist2>=gq->size)
				{
					dt* new_dict=getDict();
					push_queue(gq,new_dict);
				}
				push_dict(gq->a[dist2],new_state);
			}
		}
	}
	printf("%d",-1);
}
