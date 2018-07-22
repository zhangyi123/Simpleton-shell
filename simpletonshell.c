
#include <errno.h>
#include <stdio.h>
#include <stdlib.h> //exit
#include <getopt.h>
#include <fcntl.h> //open
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void print_usage(){
  printf("Usage: follow the instructions.");

}
void signal_handler(int sig){
  fprintf(stderr, "%d caught.\n", sig);
  exit(sig);
}
int flag=0;
int c;
int i=0;
char arg[15];
int* fd;
int num = 0;
int max;
char** args;
int *ptr = NULL;
int input, output, error;
void file(int flag){
  fd[num] = open(optarg, flag);
  if(fd[num] < 0){
    // perror("failed to open.");
    fprintf(stderr,"open file error occurred");
    exit(1);
  }
  num++;
  if(num==max){
    max = max * 5;
    fd = realloc(fd, max*sizeof(int));
  }
}
void waiting(void){
  close(fd[input]);
  close(fd[output]);
  close(fd[error]);
  int status;
  pid_t pid = waitpid(-1, &status, 0);
  if(pid == -1)fprintf(stderr, "wait child process error");
  printf("%d",WEXITSTATUS(status));
  printf(" ");
  int i=0;
  while(args[i]){
    fprintf(stdout, "%s %s",args[i], " ");
    i++;
  }


}
void piping(){
  int pipefd[2];
  if(pipe(pipefd) == -1)
    {
      fprintf(stderr, "Error opening pipe");
    }

  fd[num]=pipefd[0];
  num++;
  fd[num]=pipefd[1];
  num++;

}
void command( int argc, char *argv[]){

  int max_arg = 10;
  int j=0;
  args = malloc(max_arg * sizeof(char *));
  int a = optind +2;
  input = atoi(argv[optind -1]);
  output = atoi(argv[optind]);
  error = atoi(argv[optind+1]);
  if(input >=num || output >=num || error >=num)fprintf(stderr, "file descriptor error");
  if(input ==-1 || output >=-1 || error >=-1)fprintf(stderr, "file descriptor closed already");
  while(a<argc &&argv[a][0]!='-'){
    if(j == max_arg){
      max_arg += max_arg;
      args = realloc(args, max_arg*sizeof(char *));
    }
    args[j]=argv[a];
    j++;
    a++;

  }

  pid_t pid = fork();
  if(pid < 0){
    fprintf(stderr, "Child process create error");
  }
  if(pid == 0){
    //child
    dup2(fd[input],0);
    dup2(fd[output],1);
    dup2(fd[error],2);
    if(execvp(args[0],args) < 0){
      fprintf(stderr, "Execvp error.");
      exit(1);
    }
  }
}
int main (int argc, char **argv){
  max = 5;
  fd = malloc(max*sizeof(int));
      static struct option long_options[] =
	{
	  /* These options don.t set a flag.
	     We distinguish them by their indices. */
	  {"append", no_argument, 0 , '1'},
	  {"cloexec", no_argument, 0 , '2'},
	  {"creat", no_argument, 0 , '3'},
	  {"directory", no_argument, 0 , '4'},
	  {"dsync", no_argument, 0 , '5'},
	  {"excl", no_argument, 0 , '6'},
	  {"nofollow", no_argument, 0 , '7'},
	  {"nonblock", no_argument, 0 , '8'},
	  {"rsync", no_argument, 0 , '9'},
	  {"sync", no_argument, 0 , 's'},
	  {"trunc", no_argument, 0 , 't'},

	  {"rdonly",    required_argument,    0, 'r'},
	  {"rdwr", required_argument, 0, 'b'},
	  {"wronly",  required_argument,       0, 'w'},
	  {"command", required_argument , 0, 'c'},
	  {"verbose",  no_argument, 0, 'v'},
	  {"pipe",  no_argument, 0, 'p'},
	  {"wait",  no_argument, 0, 'i'},
	  {"abort", no_argument, 0 , 'a'},
	  {"close", required_argument, 0 , 'o'},
	  {"catch", required_argument, 0 , 'h'},
	  {"default", required_argument, 0 , 'd'},
	  {"pause", no_argument, 0 , 'u'},
	  {"ignore", required_argument, 0 , 'g'},
	  {0,0,0,0} //need to study
	};
      while(1){
	c = getopt_long (argc, argv, "vc:w:r:a:", long_options, NULL);
	if(c == -1)break;
	switch (c)
	  {
	  case 'b':
	    file(O_RDWR | flag);
	    break;
	  case 'r':
	    file(O_RDONLY | flag);
	    break;
	  case 'w':
	    file(O_WRONLY | flag);
	    break;
	  case 'c':
	    command(argc,argv);
	    break;
	  case 'v':
	    while(1){
	      if(strcmp(argv[i], "--verbose")==0)break;
	      i++;
	    }
	    i++;
	    while(i<argc){
	      strcpy(arg,argv[i]);
	      if(strcmp(arg, "\\")==0)continue;
	      fprintf(stdout,"%s", arg);
	      fprintf(stdout," ");
	      i++;
	    }
	    break;
	  case 'p':
	    piping();
	    break;
	  case 'i':
	    waiting();
	    break;
	  case 'a':
	    *ptr = 1;
	    break;
	  case 'o':
	    //close

	    if(close(fd[atoi(optarg)] < 0)){
	      fprintf(stderr, "Close error");
	      exit(errno);
	    }
	    fd[atoi(optarg)] = -1;
	    break;
	  case 'h':
	    //catch
	    signal(atoi(optarg), signal_handler);
	    break;
	  case 'd':
	    //default
	    signal(atoi(optarg), SIG_DFL);
	    break;
	  case 'u':
	    //pause
	    if(pause() < 0)
	      fprintf(stderr, "Pause error");
	    break;
	  case 'g':
	    //ignore
	    signal(atoi(optarg), SIG_IGN);
	    break;
	  case '1':
	    flag = flag | O_APPEND;
	    break;
	  case '2':
	    flag = flag | O_CLOEXEC;
	    break;
	  case '3':
	    flag = flag | O_CREAT;
	    break;
	  case '4':
	    flag = flag | O_DIRECTORY;
	    break;
	  case '5':
	    flag = flag | O_DSYNC;
	    break;
	  case '6':
	    flag = flag | O_EXCL;
	    break;
	  case '7':
	    flag = flag | O_NOFOLLOW;
	    break;
	  case '8':
	    flag = flag | O_NONBLOCK;
	    break;
	  case '9':
	    flag = flag | O_RSYNC;
	    break;
	  case 's':
	    flag = flag | O_SYNC;
	    break;
	  case 't':
	    flag = flag | O_TRUNC;
	    break;
	  default:
	    print_usage();
	  }

      }
      exit(0);
}
