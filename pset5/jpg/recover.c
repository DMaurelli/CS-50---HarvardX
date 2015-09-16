/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    int cont = 0;
    uint8_t buffer[512];
    
    FILE* fileI = fopen("card.raw", "r");
    FILE* fileO = NULL;
    
    if(fileI == NULL)
    {
        printf("Could not open the file");
    }
   
    while(fread(buffer, 512, 1, fileI))
    {
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
        {
            char photo[8];
            if(fileO != NULL)
            {
                fclose(fileO);
            }
            
            sprintf(photo, "%03d.jpg", cont);
            fileO = fopen(photo, "w");
            cont++;
        
            if(fileO != NULL)
            {
                fwrite(buffer, 512, 1, fileO);
            }
        }
        else if(fileO != NULL)
        {
            fwrite(buffer, 512, 1, fileO);
        }
    }
    fclose(fileI);
}
