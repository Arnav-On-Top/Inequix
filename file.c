#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 6
int board[MAX][MAX];
int solution[MAX][MAX];
int fixed[MAX][MAX];
char hsign[MAX][MAX];
char vsign[MAX][MAX];
int n;
time_t startTime;
int lastRow=-1;
int lastCol=-1;
int oldValue=0;
int canUndo=0;
int relationOK(int a, char sign, int b)
{
    if(sign=='<')
    return a<b;
    else
    return a>b;
}
void swap(int*a, int*b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}
void shuffle(int arr[], int size)
{
    int i,j;
    for(i=size-1; i>0; i--)
    {
        j=rand()%(i+1);
        swap(&arr[i], &arr[j]);
    }
}
void generatePuzzle(int cluePercent)
{
    int rows[MAX], cols[MAX], nums[MAX];
    int i,j;
    for(i=0; i<n; i++)
    {
        rows[i]=i;
        cols[i]=i;
        nums[i]=i+1;
    }
    shuffle(rows, n);
    shuffle(cols, n);
    shuffle(nums, n);
    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            solution[i][j]=nums[(rows[i]+cols[j])%n];
        }
    }
    for (i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            if(rand()%100<cluePercent)
            {
                board[i][j]=solution[i][j];
                fixed[i][j]=1;
            }
            else
            {
                board[i][j]=0;
                fixed[i][j]=0;
            }
        }
    }
    for (i=0; i<n; i++)
    {
        for(j=0; j<n-1; j++)
        {
            if(solution[i][j]<solution[i][j+1])
            hsign[i][j]='<';
            else
            hsign[i][j]='>';
        }
    }
    for(i=0; i<n-1; i++)
    {
        for(j=0; j<n; j++)
        {
            if(solution[i][j]<solution[i+1][j])
            vsign[i][j]='<';
            else
            vsign[i][j]='>';
        }
    }
    lastRow=-1;
    lastCol=-1;
    oldValue=0;
    canUndo=0;
    startTime=time(NULL);
}
void displayBoard()
{
    int i,j;
    printf("\n");
    for(j=0; j<n; j++)
    printf("%d", j+1);
    printf("\n");
    for(i=0; i<n; i++)
    {
        printf("%d |", i+1);
        for(j=0; j<n; j++)
        {
            if(board[i][j]==0)
            printf(".");
            else
            printf("%d", board[i][j]);
            if(j<n-1)
            printf("%c", hsign[i][j]);
        }
        printf("\n");
        if(i<n-1)
        {
            for(j=0; j<n; j++)
            printf("%c", vsign[i][j]);
            printf("\n");
        }
    }
}
int validMove(int r, int c, int num)
{
    int i, j;
    for (j=0; j<n; j++)
    {
        if(j!=c&&board[r][j]==num)
        return 0;
    }
    for(i=0; i<n; i++)
    {
        if(i!=r&&board[i][c]==num)
        return 0;
    }
    if(c>0 && board[r][c-1]!=0)
    {
        if(!relationOK(board[r][c-1],
        hsign[r][c-1], num))
        return 0;
    }
    if(c<n-1 && board[r][c+1]!=0)
    {
        if(!relationOK(num, hsign[r][c],
        board[r][c+1]))
        return 0;
    }
    if(r>0 && board[r-1][c]!=0)
    {
        if(!relationOK(board[r-1][c],
        vsign[r-1][c], num))
        return 0;
    }
    if(r<n-1 && board[r+1][c]!=0)
    {
        if(!relationOK(num, vsign[r][c],
        board[r+1][c]))
        return 0;
    }
    return 1;
}
int checkWin(){
    int i, j;
    for (i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            if(board[i][j]==0)
            return 0;
        }
    }
    for(i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
        {
            int k;
            for(k=j+1; k<n; k++)
            {
                if(board[i][j]==board[i][k])
                return 0;
            }
        }
    }
    for(j=0; j<n; j++)
    {
        for(i=0; i<n; i++)
        {
            int k;
            for (k=i+1; k<n; k++)
            {
                if(board[i][j]==board[k][j])
                return 0;
            }
        }

    }
    for(i=0; i<n; i++)
    {
        for(j=0; j<n-1; j++)
        {
            if(!relationOK(board[i][j],
            hsign[i][j],
        board[i][j+1]))
        return 0;
        }
    }
    for(i=0; i<n-1; i++)
    {
        for(j=0; j<n; j++)
        {
            if(!relationOK(board[i][j],
            vsign[i][j],
        board[i+1][j]))
        return 0;
        }
    }
    return 1;
}
int main()
{
    int row, col, num, difficulty, elapsed;
    srand((unsigned int)time(NULL));
    printf("Inequix\n");
    printf("1. Easy [4x4]\n");
    printf("2. Medium [5x5]\n");
    printf("3. Hard [6x6]\n");
    printf("Choose difficulty: ");
    if(scanf("%d", &difficulty)!=1)
    return 0;
    if(difficulty==1)
    {
        n=4;
        generatePuzzle(45);
    }
    else if (difficulty==2)
    {
        n=5;
        generatePuzzle(35);
    }
    else if(difficulty==3)
    {
        n=6;
        generatePuzzle(25);
    }
    else
    {
        printf("Invalid Entry\n");
        return 0;
    }
    while (1)
    {
        displayBoard();
        elapsed=(int)difftime(time(NULL), startTime);
        printf("\nTime: %d minutes %d seconds\n",
        elapsed/60, elapsed%60);
        printf("\n Enter row and column number and then the number u want to add\n");
        printf("Example: 2 3 4\n");
        printf("-1 -1 -1: Undo last move\n");
        printf("0 0 0: New random puzzle\n");
        printf("9 9 9: Quit\n");
        printf("You move: ");
        if(scanf("%d%d%d", &row, &col, &num)!=3)
        {
            printf("Invalid input. Restart the program.\n");
            break;
        }
        if(row==9 && col==9 && num==9)
        {
            printf("Game ended\n");
            break;
        }
        if (row==0 &&col==0 &&num==0)
        {
            int clues;
            if(n==4)
            clues=45;
            else if(n==5)
            clues=35;
            else
            clues=25;
            generatePuzzle(clues);
            printf("New random puzzle generated! \n");
            continue;
        }
        if(row==-1 && col==-1 && num==-1)
        {
            if(canUndo==1)
            {
                board[lastRow][lastCol]=oldValue;
                canUndo=0;
                printf("Last move undone\n");
            }
            else
            {
                printf("Nothing to undo!\n");
            }
            continue;
        }
        if(row<1 || row>n || col<1 || col>n || num<1 || num>n)
        {
            printf("Invalid row, column or number!\n");
            continue;
        }
        row--;
        col--;
        if(fixed[row][col]==1)
        {
            printf("Fixed number cannot be changed\n");
            continue;
        }
        if(validMove(row, col, num)==1)
        {
            lastRow=row;
            lastCol=col;
            oldValue=board[row][col];
            canUndo=1;
            board[row][col]=num;
            printf("Correct move!\n");
        }
        else
        {
            printf("Invalid move!");
            continue;
        }
        if(checkWin()==1)
        {
            displayBoard();
            elapsed=(int)difftime(time(NULL),startTime);
            printf("\n You Win :) \n");
            printf("Time taken: %d minutes %d seconds \n",
            elapsed/60, elapsed%60);
            break;
        }
    }
    return 0;
}
