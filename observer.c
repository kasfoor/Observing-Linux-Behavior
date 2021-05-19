
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#define LB_SIZE 2048

void partC(void);
void partD(void);
void sampleLoadAvg();

char repTypeName[16];
enum setType{SHORT, LONG, STANDARD};
struct timeval now;

int main(int argc, char *argv[]) {
   FILE *thisProcFile;
   char c1, c2;
   char lineBuf[2048];
   char repTypeName[16];
   enum setType reportType;


   reportType = STANDARD;
   int interval, duration;
   int iteration=0;
   strcpy(repTypeName, "Standard");

   if (argc > 1) {
      sscanf(argv[1], "%c%c", &c1, &c2);
      if (c1 != '-') {
         fprintf(stderr, "usage: observer [-s][-l int dur]\n");
         exit(1);
                }
      if (c2 == 's') {
         reportType = SHORT;
         strcpy(repTypeName, "Short");
        }
      if (c2 == 'l') {
         printf("made it to if statment");
         reportType = LONG;
         strcpy(repTypeName, "Long");
         interval = atoi(argv[2]);
         duration = atoi(argv[3]);
        }
	}

/*Part B*/

/* Get the CPU model name and print it */
   thisProcFile = fopen ("/proc/cpuinfo", "r");
   int i=0;
   while(i<5){
      fgets(lineBuf,LB_SIZE+1, thisProcFile);
      i=i+1;
    }
   printf("CPU %s\n", lineBuf);
   fclose(thisProcFile);

/* Get the Kernel version and print it */
   thisProcFile = fopen ("/proc/version", "r");
   fgets(lineBuf,LB_SIZE+1, thisProcFile);
   printf("Kernel Version: %s\n", lineBuf);
   fclose(thisProcFile);

/* Get the time since the system was last booted, in dd:hh:mm:ss format and print it */
   thisProcFile = fopen ("/proc/uptime", "r");
   fgets(lineBuf,LB_SIZE+1, thisProcFile);
   int uptime = atoi(lineBuf);
   int min = 60;
   int hour = min * 60;
   int day = hour * 24;
   int total [4] = {uptime/day, (uptime%day)/hour, (uptime%hour)/min, (uptime%min) };
   printf("Time Since last boot: %d days : %d hours : %d minutes : %d seconds\n", total[0],total[1],total[2],total[3]);
   fclose(thisProcFile);

   gettimeofday(&now, NULL); // Get the current time
   printf("Status report type %s at %s\n", repTypeName, ctime(&(now.tv_sec)));

/* Get the host filename and print it */
   thisProcFile = fopen("/proc/sys/kernel/hostname", "r");
   fgets(lineBuf, LB_SIZE+1, thisProcFile);
   printf("Machine hostname: %s", lineBuf);
   fclose(thisProcFile);


   if(reportType == SHORT){
      partC();
    }
   if(reportType == LONG) {
      partC();
      partD();
      while (iteration < duration) {
         sleep(interval);
         sampleLoadAvg();
         iteration += interval;
        }
    }

   exit(0);
}



/*Part C*/
void partC(void){
  FILE *thisProcFile;
   char name[100], user[100], number[100], sys[100], idle[100];
   char rDisk[100], wDisk[100], ctxt[100], lastB[100], prcss[100];

/* The amount of time all CPUs have spent in user mode, in system mode, and idle */
   thisProcFile = fopen("/proc/stat", "r");
   fscanf(thisProcFile, "%s %s %s %s %s",name, user, number, sys, idle);
   printf("User Mode: %s\nSystem Mode: %s\nIdle Mode: %s\n", user, sys, idle);
   fclose(thisProcFile);
/* The number of disk read/write requests completed on the system */
   fprintf(stderr, "Disk read requests completed: ");
   strcpy(rDisk,"cat /proc/diskstats | grep sda -m 1| awk '{print $6}'");
   system(rDisk);
   fprintf(stderr, "Disk write requests completed: ");
   strcpy(wDisk,"cat /proc/diskstats | grep sda -m 1| awk '{print $10}'");
   system(wDisk);
/* The number of context switches that the kernel has performed */
   fprintf(stderr, "Context switches that the kernel has performed: ");
   strcpy(ctxt,"cat /proc/stat | grep ctxt| awk '{print $2}'");
   system(ctxt);
/* The time when the system was last booted */
   fprintf(stderr, "Time when the system was last booted: ");
   strcpy(lastB,"who -b | awk '{print $3, $4}'");
   system(lastB);
/* The number of processes that have been created since the system was booted */
   fprintf(stderr, "Number of processes that have been created since the system was booted: ");
   strcpy(prcss,"cat /proc/stat | grep processes| awk '{print $2}'");
   system(prcss);
}


/*Part D*/
void partD(void){
   FILE *thisProcFile;
   char lineBuf[2048];

/* The amount of memory configured into this computer */
   thisProcFile = fopen ("/proc/meminfo", "r");
   fgets(lineBuf,LB_SIZE+1, thisProcFile);
   printf("Amount of Memory Configured into this computer: %s\n", lineBuf);
   fclose(thisProcFile);

/* The amount of memory currently available */
   thisProcFile = fopen ("/proc/meminfo", "r");
   fgets(lineBuf,LB_SIZE+1, thisProcFile);
   fgets(lineBuf,LB_SIZE+1, thisProcFile);
   printf("Memory currently available: %s\n", lineBuf);
   fclose(thisProcFile);
/* A list of load averages (each averaged over the last minute) */
   sampleLoadAvg();
}



/*Load Averages*/
void sampleLoadAvg(){
   FILE *thisProcFile;
   char lineBuf[2048];
   float avgLoad;

   thisProcFile = fopen ("/proc/loadavg", "r");
   fgets(lineBuf,LB_SIZE+1, thisProcFile);
   fscanf(thisProcFile,"%f\n",&avgLoad);
   printf("Load Average: %0.3f\n", avgLoad);
   fclose(thisProcFile);
}
