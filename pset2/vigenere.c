#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    char pos[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    string keyword,text;
    int cipher,k,j=0;
    if(argc != 2)
    {
        printf("Expected ./vigenere (keyword)");
        return 1;
    }
    else
    {
        keyword = argv[1];
        for(int i = 0, n = strlen(keyword); i < n; i++)
        {
            if(isdigit(argv[1][i]))
            {
                printf("Your keyword must contain leters only (a - z and A - Z)");
                return 1;
            }
        }
        text = GetString();
        for(int i = 0, n = strlen(text); i < n; i++)
        {
            if(isalpha(text[i]))
            {
                if(keyword[j] != '\0')
                {
                    for(int l = 0; l < 26; l++)
                    {
                        if(tolower(keyword[j]) == pos[l])
                        {
                            k = l;
                        }
                    }
                }
                else
                {
                    j = 0;
                    for(int l = 0; l < 26; l++)
                    {
                        if(tolower(keyword[j]) == pos[l])
                        {
                            k = l;
                        }
                    }
                }
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
                j++;
            }
            else
            {
                printf("%c",text[i]);
            }
         }
    }
    printf("\n");
} 
