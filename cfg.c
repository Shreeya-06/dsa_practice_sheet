#include <stdio.h>
#include <string.h>

#define MAX 100
#define V 10

int dp[MAX][MAX][V];

int main()
{
    char s[MAX];

    char var[] = "SECGAFJXYZ";

    int lhs[] = {0,0,1,1,2,3,3,4,5,5,6};
    int rhs1[] = {1,3,9,9,1,9,9,9,8,8,5};
    int rhs2[] = {2,5,6,7,9,2,-1,4,-1,7,7};

    

    int n, i, j, k, p;

    printf("Enter string: ");
    scanf("%s", s);

    n = strlen(s);

    if(n >= MAX)
    {
        printf("String too long\n");
        return 0;
    }

    memset(dp, 0, sizeof(dp));

    // Terminal productions
    for(i = 0; i < n; i++)
    {
        if(s[i] == 'a')
        {
            dp[i][1][1] = 1;  // A -> a
            dp[i][1][7] = 1;  // X -> a
        }

        else if(s[i] == 'b')
        {
            dp[i][1][8] = 1;  // Y -> b
        }

        else if(s[i] == 'c')
        {
            dp[i][1][2] = 1;  // C -> c
            dp[i][1][9] = 1;  // Z -> c
        }
    }

    // CNF binary productions
    int B[][3] = {
        {0,3,2},  // S -> EC
        {0,1,5},  // S -> AF
        {1,9,3},  // E -> XG
        {1,9,8},  // E -> XY
        {3,1,8},  // G -> EY
        {2,9,2},  // C -> ZC
        {4,9,4},  // A -> XA
        {5,8,6},  // F -> YJ
        {5,8,9},  // F -> YZ
        {6,5,9}   // J -> FZ
    };

    int rules = 10;

    for(i = 2; i <= n; i++)
    {
        for(j = 0; j <= n-i; j++)
        {
            for(k = 1; k < i; k++)
            {
                for(p = 0; p < rules; p++)
                {
                    int A = B[p][0];
                    int X = B[p][1];
                    int Y = B[p][2];

                    if(dp[j][k][X] &&
                       dp[j+k][i-k][Y])
                    {
                        dp[j][i][A] = 1;
                    }
                }
            }
        }
    }

    if(dp[0][n][0])
        printf("String Accepted\n");
    else
        printf("String Rejected\n");

    return 0;
}