
#include <stdio.h>

#define MAX 252

int states[MAX];
int trans[MAX][2];
int part[MAX], newpart[MAX];
int total = 0;

int encode(int r, int p, int m, int z)
{
    return (((r * 3 + p) * 3 + m) * 4 + z);
}

void decode(int x, int *r, int *p, int *m, int *z)
{
    *z = x % 4;
    x /= 4;

    *m = x % 3;
    x /= 3;

    *p = x % 3;
    x /= 3;

    *r = x;
}

int accepting(int x)
{
    int r, p, m, z;

    decode(x, &r, &p, &m, &z);

    return (r == 0 && m == 2 && z != 3);
}

int nextstate(int x, int bit)
{
    int r, p, m, z;

    decode(x, &r, &p, &m, &z);

    if(z == 3)
        return x;

    // Divisibility by 7
    r = (2 * r + bit) % 7;

    // Count occurrences of 101
    if(p == 0)
    {
        if(bit == 1)
            p = 1;
    }
    else if(p == 1)
    {
        if(bit == 0)
            p = 2;
        else
            p = 1;
    }
    else
    {
        if(bit == 1)
        {
            m = (m + 1) % 3;
            p = 1;
        }
        else
            p = 0;
    }

    // Avoid substring 000
    if(bit == 1)
        z = 0;
    else
        z++;

    return encode(r, p, m, z);
}

int same(int i, int j)
{
    if(accepting(states[i]) != accepting(states[j]))
        return 0;

    for(int b = 0; b < 2; b++)
    {
        if(part[trans[i][b]] != part[trans[j][b]])
            return 0;
    }

    return 1;
}

int main()
{
    int id[MAX];

    char s[100];

    for(int i = 0; i < MAX; i++)
        id[i] = -1;

    // Generate reachable DFA states
    states[0] = encode(0, 0, 0, 0);
    id[states[0]] = 0;
    total = 1;

    for(int i = 0; i < total; i++)
    {
        for(int b = 0; b < 2; b++)
        {
            int x = nextstate(states[i], b);

            if(id[x] == -1)
            {
                id[x] = total;
                states[total++] = x;
            }

            trans[i][b] = id[x];
        }
    }


    // Initial partition
    for(int i = 0; i < total; i++)
        part[i] = accepting(states[i]);

    // DFA minimization
    int changed = 1;

    while(changed)
    {
        int groups = 0;
        changed = 0;

        for(int i = 0; i < total; i++)
        {
            int j;

            for(j = 0; j < i; j++)
            {
                if(same(i, j))
                    break;
            }

            if(j < i)
                newpart[i] = newpart[j];
            else
                newpart[i] = groups++;
        }

        for(int i = 0; i < total; i++)
        {
            for(int j = 0; j < i; j++)
            {
                if((part[i] == part[j]) !=
                   (newpart[i] == newpart[j]))
                    changed = 1;
            }
        }

        for(int i = 0; i < total; i++)
            part[i] = newpart[i];
    }

    int minstates = 0;

    for(int i = 0; i < total; i++)
    {
        int first = 1;

        for(int j = 0; j < i; j++)
        {
            if(part[i] == part[j])
            {
                first = 0;
                break;
            }
        }

        if(first)
            minstates++;
    }

   

    // Test input string
    printf("\nEnter binary string: ");
    scanf("%99s", s);

    int state = 0;

    for(int i = 0; s[i] != '\0'; i++)
    {
        if(s[i] != '0' && s[i] != '1')
        {
            printf("Invalid Input\n");
            return 0;
        }

        state = trans[state][s[i] - '0'];
    }

    if(accepting(states[state]))
        printf("String Accepted\n");
    else
        printf("String Rejected\n");

    return 0;
}

