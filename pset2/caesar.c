#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    int k,cipher;
    string text;
    if(argc != 2 || atoi(argv[1]) == 0)
    {
        printf("%s",argv[1]);
        return 1;
    }
    else
    {
        k = atoi(argv[1]);
        text = GetString();
        for(int i = 0, n = strlen(text); i < n; i++)
        {
            if(isalpha(text[i]))
            {
                if(isupper(text[i]))
                {
                    cipher = ((text[i] - 65) + k) % 26;
                    printf("%c",cipher + 65);
                }
                else
                {
                    cipher = ((text[i] - 97) + k) % 26;
                    printf("%c",cipher + 97);
                }
            }
            else
            {
                printf("%c",text[i]);
            }
        }
    }
    printf("\n");
}
