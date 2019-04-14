#include<stdio.h>
#define CURR_REQ 143

int absolute(int num)
{
    return (num>0?num:(num*-1));
}
int main()
{
    int queue[] = {86, 1470, 913, 1774, 948, 1509, 1022, 1750, 130};
	int i,distance=0,last = CURR_REQ;

    for(i=0; i<(sizeof(queue)/sizeof(queue[0])); i++)
	{
    	int curr = queue[i];
    	printf("Last : %4d | Curr : %4d | dist : %4d\n",last,curr,absolute(last - curr));
    	distance += absolute(last - curr);
    	last = curr;
    }

    printf("\nTotal Distance = %d\n",distance);

	return 0;
}