#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int cant, height,cont=0;
    do
    {
        printf("Height: ");
        height = GetInt();
    }
    while((height < 0) || (height>23));
    cant = height;
    cant = cant - 2;
    if(height == 0)
    {
    }
    else
    {
    do
    {
        for(int i=0;i<=cant;i++)
        {
            printf(" ");
        }
        printf("#");
        for(int j=0;j<=cont;j++)
        {
            printf("#");
        }
        printf("\n");
        cant = cant -1;
        height = height-1;
        cont = cont +1;
    }
    while(height != 0);
    }
}
