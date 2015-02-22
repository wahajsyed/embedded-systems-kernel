/* Solution to nqueens problem
@author : Wahajuddin Syed
@date: 1/5/2015
*/

#include<stdio.h>
#include<stdlib.h>
unsigned int test_avail(unsigned int , unsigned int, unsigned int *);
unsigned int placequeen(unsigned int ,unsigned int ,unsigned int *);
void printresult(unsigned int *);


int size=0;
int main( void )
{
int i=0;
printf("enter the size \n");
scanf("%d",&size);

unsigned int* position=(unsigned int *)(malloc(sizeof(unsigned int)*size));
for(i=0;i<size;i++)
	{
	position[i]=size+1; //store a non row value
	}

placequeen(0,size,position);

return 0;
}

unsigned int test_avail(unsigned int m,unsigned int n, unsigned int * position)
{
int i=0;
	for(i=0;i<m;i++)
	{
		if((position[i]==n)||( abs(i-m)==abs(position[i]-n) ))
		{
			return 0;
		}
	}
return 1;
}

/*This function prints the result of the position*/
void printresult(unsigned int * position)
{
int i;
for(i=0;i<size;i++)
	{
	printf("%d,%d  ",i,position[i]);
	}
	printf("\n");

}
/*place queen*/
unsigned int placequeen(unsigned int row,unsigned int size,unsigned int * position)
{
int column;
for(column=0;column<size;column++) 		//change columns for each recursion of row
	{

	if(test_avail(row,column,position)) 	//if a position is available at  
		{
		//printf("\navailable %d %d:  ",row,column);
		//printresult(position);
		position[row]=column;		//update the position
		if(row==(size-1)) 			//We have found a position at the last row
			{
			printresult(position);
			return 1; 		//return
			}
		else if(!placequeen((row+1),size,position)) //if not the last row, place queen in next row
			{
			position[row]=size+1;	// if the next row return null, then remove the current position
 			}
		}	
	}
return 0; 					//return 0 if unable to find a position
}
