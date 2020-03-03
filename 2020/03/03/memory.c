#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// from https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}
int getRAMUsage(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

char * data;

int main(int argc, char ** argv) {
  int N = argc > 1 ? atoi(argv[1]) : 100;
  int maxmem = getRAMUsage();
  for(int k = 0; k < 10; k++) {
    data = (char*) malloc(N*1024);
    if(data == NULL) {
     printf("allocation failure\n");
    }
    memset(data,0,N*1024);
    int tmem = getRAMUsage();
    if(tmem > maxmem) maxmem = tmem;
    free(data);
  }
  printf("final ram usage: %d kB, max is %d kB \n", getRAMUsage(), maxmem);

}
