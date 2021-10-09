#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

unsigned int seconds = 0, minutes = 1, hours = 1;

void PrintTheTime()
{
    system("CLS");
    printf("%02hu : %02hu : %02hu\n", hours, minutes, seconds);
    Sleep(1000);
}

int main()
{
    PrintTheTime();

    while (1)
    {
        if (seconds == 0)
        {
            if (minutes == 0)
            {
                if (hours == 0)
                {
                    exit(EXIT_SUCCESS);
                }
                hours--;
                minutes = 60;
            }
            minutes--;
            seconds = 60;
        }
        seconds--;

        PrintTheTime();
    }
}