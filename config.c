#include <stdio.h>
#include <string.h>
char *configuration(char file[])
{
    FILE *config = fopen("config.txt", "rb");
    char lines[100] = {0};
    while (fgets(lines, 100, config) != 0)
    {
        if (strstr(lines, file) != NULL)
        {
            char *r = strtok(lines, "=");
            r = strtok(NULL, "=");
            return r;
        }
    }
}
int main()
{
    printf("%s",configuration("index"));
    char a [100];
    strcpy(a,configuration("index"));
    printf("%s",a);

}