#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float change = 0;
    int cant = 0;
    do
    {
        printf("How much change is owed?: ");
        change = GetFloat();
    }
    while(change <= 0);
    
    change = change*100;
    change = round(change);
    while(change != 0)
    {
        if(change >= 25)
        {
            change = change - 25;
        }
        else if(change >= 10)
        {
            change = change - 10;
        }
        else if(change >= 5)
        {
            change = change - 5;
        }
        else
        {
            change = change - 1;
        }
        cant = cant + 1;
    }
    printf("%d\n",cant);
}
