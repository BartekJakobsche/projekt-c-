#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <pigpio.h>




//gcc -Wall -pthread -o servo_demo servo_demo.c -lpigpio     <--- linijka do kompilacji




#define NUM_GPIO 40

#define MIN_WIDTH 1000
#define MAX_WIDTH 2000
using namespace std;

int run=1;

int step[NUM_GPIO];
int width[NUM_GPIO];
int used[NUM_GPIO];

int randint(int from, int to)
{
   return (random() % (to - from + 1)) + from;
}

void stop(int signum)
{
   run = 0;
}

void prosto (int &width , int g )
{
    width[g] = randint(MAX_WIDTH/2, MAX_WIDTH);
    time_sleep(0.1);
}

void tyl (int &width ,int g)
{
    width[g] = randint(MAX_WIDTH/2, MIN_WIDTH);
    time_sleep(0.1);
}

void turnRight (int &width, int g )
{
    width[g] = randint(MAX_WIDTH/2, MIN_WIDTH);
    width[g2] = randint(MAX_WIDTH/2, MAX_WIDTH);
    time_sleep(0.1);
}

void turnLeft (int &width, int g )
{
    width[g2] = randint(MAX_WIDTH/2, MIN_WIDTH);
    width[g] = randint(MAX_WIDTH/2, MAX_WIDTH);
    time_sleep(0.1);
}



int main(int argc, char *argv[])
{
   int i, g;

   if (gpioInitialise() < 0) return -1;

   gpioSetSignalFunc(SIGINT, stop);

   if (argc == 1) used[4] = 1;
   else
   {
      for (i=1; i<argc; i++)
      {
         g = atoi(argv[i]);
         if ((g>=0) && (g<NUM_GPIO)) used[g] = 1;
      }
   }

   cout << " wysy³anie sugna³u do serwa" << endl;

   for (g=0; g<NUM_GPIO; g++)
   {
      if (used[g])
      {
         cout << " %d", g << endl;
         step[g] = randint(5, 25);
         if ((step[g] % 2) == 0) step[g] = -step[g];
         width[g] = randint(MAX_WIDTH/2, MAX_WIDTH);
      }
   }

   cout << ", control C aby zakoñczyæ ." << endl;

   while(run)
   {
      for (g=0; g<NUM_GPIO; g++)
      {
         if (used[g])
         {
            gpioServo(g, width[g]);



            width[g] += step[g];

            if ((width[g]<MIN_WIDTH) || (width[g]>MAX_WIDTH))
            {
               step[g] = -step[g];
               width[g] += step[g];
            }
         }
      }

      time_sleep(0.1);
   }



   for (g=0; g<NUM_GPIO; g++)
   {
      if (used[g]) gpioServo(g, 0);
   }

   gpioTerminate();

   return 0;
}

