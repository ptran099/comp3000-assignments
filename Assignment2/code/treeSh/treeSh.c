/* treeSh */
/* v1 Sept 2025 */
/* based of the code of 3000shell.c
   written for COMP 3000 by Anil Somayaji
 */
/* v2 Sept. 15, 2019 */
/* v1 Sept. 24, 2017 */
/* based off of csimpleshell.c, Enrico Franchi © 2005
      https://web.archive.org/web/20170223203852/
      http://rik0.altervista.org/snippets/csimpleshell.html */
/* Original under "BSD" license */
/* 3000shell.c is under GPLv3, copyright 2017, 2019 Anil Somayaji */
/* This version of treeSh.c is under GPLv3, copyright 2025 Zinovi Rabinovich */
/* You really shouldn't be incorporating parts of this in any other code,
   it is meant for teaching, not production */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>

#define BUFFER_SIZE 1<<16
#define ARR_SIZE 1<<16
#define COMM_SIZE 32

const char *proc_prefix = "/proc";


/* Extras for Assignment 2 Begin --- you MAY needs them, but you MAY also find something different useful --- there is No Single correct solution */
/* BRANCH_NUM is how many children there are when grow() works */
#define BRANCH_NUM 2
/* command_count is the counter of how many commands the shell has executed thus far */
static int command_count = 0;
/* this is where the parent treeSh keeps the output ends of pipes to "tree" children */
static int children_feeds_fd[BRANCH_NUM];
/* this is where the parent keeps track of "tree" children ids
static int children_pids[BRANCH_NUM];
/* Extras for Assignment 2 End */

void process_command(int nargs, char *args[], char *path, char *envp[]);

void parse_args(char *buffer, char** args, 
                size_t args_size, size_t *nargs)
{
        char *buf_args[args_size]; /* You need C99 */
        char **cp, *wbuf;
        size_t i, j;
        
        wbuf=buffer;
        buf_args[0]=buffer; 
        args[0] =buffer;
        
        for(cp=buf_args; (*cp=strsep(&wbuf, " \n\t")) != NULL ;){
                if ((*cp != NULL) && (++cp >= &buf_args[args_size]))
                        break;
        }
        
        for (j=i=0; buf_args[i]!=NULL; i++){
                if (strlen(buf_args[i]) > 0)
                        args[j++]=buf_args[i];
        }
        
        *nargs=j;
        args[j]=NULL;
}

/* this is kind of like getenv() */
char *find_env(char *envvar, char *notfound, char *envp[])
{
        const int MAXPATTERN = 128;
        int i, p;
        char c;
        char pattern[MAXPATTERN];
        char *value = NULL;

        p = 0;
        while ((c = envvar[p])) {
                pattern[p] = c;
                p++;
                if (p == (MAXPATTERN - 2)) {
                        break;
                }
        }
        pattern[p] = '=';
        p++;
        pattern[p] = '\0';
        
        i = 0;
        while (envp[i] != NULL) {
                if (strncmp(pattern, envp[i], p) == 0) {                        
                        value = envp[i] + p;
                }
                i++;
        }

        if (value == NULL) {
                return notfound;
        } else {
                return value;
        }
}

void find_binary(char *name, char *path, char *fn, int fn_size) {
        char *n, *p;
        int r, stat_return;

        struct stat file_status;

        if (name[0] == '.' || name[0] == '/') {
                strncpy(fn, name, fn_size);
                return;
        }
        
        p = path;
        while (*p != '\0') {       
                r = 0;
                while (*p != '\0' && *p != ':' && r < fn_size - 1) {
                        fn[r] = *p;
                        r++;
                        p++;
                }

                fn[r] = '/';
                r++;
                
                n = name;
                while (*n != '\0' && r < fn_size) {
                        fn[r] = *n;
                        n++;
                        r++;
                }
                fn[r] = '\0';

                
                stat_return = stat(fn, &file_status);

                if (stat_return == 0) {
                        return;
                }

                if (*p != '\0') {
                        p++;
                }
        }
}

void setup_comm_fn(char *pidstr, char *comm_fn)
{
        char *c;

        strcpy(comm_fn, proc_prefix);
        c = comm_fn + strlen(comm_fn);
        *c = '/';
        c++;
        strcpy(c, pidstr);
        c = c + strlen(pidstr);
        strcpy(c, "/comm");
}


/*
 *
 *    Assignment 2 Functions Start Here
 *
 *
 */

void delegate(int argc, char *argv[], char *path, char *envp[])
{
  pid_t my_pid;
  int has_children = 0;

  my_pid = getpid();

  if(argc<2) {
    fprintf(stdout,"TreeSh %d can't delegate nothing\n", my_pid);
    fflush(stdout);
    return;
  }

  
  /* Check if there are children here

     has_children = ..... some check that you should do
     
   */
  
  if (has_children) {
    fprintf(stdout,"TreeSh %d delegates to the eldest child\n", my_pid);
    fflush(stdout);
    /*
      If there are "tree" children, delegate the command to the eldest child (who will delegate to their eldest child.....)
    */
  } else {
    /*... process the command yourself */
    fprintf(stdout,"TreeSh %d can't delegate and will do the command on its own\n", my_pid);
    fflush(stdout);
    process_command(argc-1, &(argv[1]), path, envp);
  }
}

void grow(int levels)
{
  pid_t my_pid;
  
  if (levels <= 0) {
    fprintf(stdout,"TreeSh thinks you're funny, growing trees backwards\n");
    fflush(stdout);
    return;
  }
    
   my_pid = getpid();

   fprintf(stdout,"TreeSh %d is growing\n", my_pid);
   fflush(stdout);
   
  /*
    If you do not have enough children (< BRANCH_NUM), fork more...
    Children you have should check if they have enough children ...
    For every new "tree" child setup a common pipe IPC, so that the parent shell can "type in" commands for the child shell. 
    Please notice that if there was a "tree" child shell already, then the newly spawned shell child is "younger" and has to be recorded as such.
  */

   fprintf(stdout,"TreeSh %d: my tree is now fully grown\n", my_pid);
   fflush(stdout);
   
}

void prune()
{
  pid_t my_pid;
  
  my_pid = getpid();

  fprintf(stdout,"TreeSh %d is pruning\n", my_pid);
  fflush(stdout);
   
  /*
    If there are "tree" children, abort the eldest (and all its descendants), so that only one "tree" child remains.
    Make sure that the youngest/remaining "tree" child also "prunes" its subtree.
    Please notice that if there is only 1 "tree" child, then it should survive the pruning.
   */

  fprintf(stdout,"TreeSh %d is now a single branch\n", my_pid);
  fflush(stdout);
  
}

void uproot()
{
  pid_t my_pid;
  
  my_pid = getpid();

  fprintf(stdout,"TreeSh %d is uprooting\n", my_pid);
  fflush(stdout);

  /*
    Abort all "tree" children and their sub-trees.
    Hint: Cascading termination...
   */

  fprintf(stdout,"TreeSh %d is all alone\n", my_pid);
  fflush(stdout);
}

/*
 *
 *    Assignment 2 Functions End Here --- but there are other places where things need to change
 *
 *
 */


void plist()
{
        DIR *proc;
        struct dirent *e;
        int result;
        char comm[COMM_SIZE];  /* seems to just need 16 */        
        char comm_fn[512];
        int fd, i, n;

        proc = opendir(proc_prefix);

        if (proc == NULL) {
                fprintf(stderr, "ERROR: Couldn't open /proc.\n");
        }
        
        for (e = readdir(proc); e != NULL; e = readdir(proc)) {
                if (isdigit(e->d_name[0])) {
                        setup_comm_fn(e->d_name, comm_fn);
                        fd = open(comm_fn, O_RDONLY);
                        if (fd > -1) {                                
                                n = read(fd, comm, COMM_SIZE);
                                close(fd);
                                for (i=0; i < n; i++) {
                                        if (comm[i] == '\n') {
                                                comm[i] = '\0';
                                                break;
                                        }
                                }
                                printf("%s: %s\n", e->d_name, comm);
                        } else {
                                printf("%s\n", e->d_name);
                        }
                }
        }
	fflush(stdout);
        
        result = closedir(proc);
        if (result) {
                fprintf(stderr, "ERROR: Couldn't close /proc.\n");
        }
}

void signal_handler(int the_signal)
{
        int pid, status;

        if (the_signal == SIGHUP) {
                fprintf(stderr, "Received SIGHUP.\n");
                return;
        }
        
        if (the_signal != SIGCHLD) {
                fprintf(stderr, "Child handler called for signal %d?!\n",
                        the_signal);
                return;
        }

        pid = waitpid(-1,&status,WNOHANG);

        if (pid == -1) {
                /* nothing to wait for */
                return;
        }
        
        if (WIFEXITED(status)) {
                fprintf(stderr, "\nProcess %d exited with status %d.\n",
                        pid, WEXITSTATUS(status));
        } else {
                fprintf(stderr, "\nProcess %d aborted.\n", pid);
        }
}

void run_program(char *args[], int background, char *stdout_fn,
                 char *path, char *envp[])
{
        pid_t pid;
        int fd, *ret_status = NULL;
        char bin_fn[BUFFER_SIZE];

        pid = fork();
        if (pid) {
                if (background) {
                        fprintf(stderr,
                                "Process %d running in the background.\n",
                                pid);
                } else {
                        pid = wait(ret_status);
                }
        } else {
                find_binary(args[0], path, bin_fn, BUFFER_SIZE);

                if (stdout_fn != NULL) {
                        fd = creat(stdout_fn, 0666);
                        dup2(fd, 1);
                        close(fd);
                }
                
                if (execve(bin_fn, args, envp)!=0) {
                        puts(strerror(errno));
                        exit(127);
                }
        }
}

void process_command(int nargs, char *args[], char *path, char *envp[])
{
  int background;
  int grow_depth;
  char *stdout_fn;
  int i, j;
        
  
  if (!strcmp(args[0], "exit")) {
    exit(0);
  }
       
  if (!strcmp(args[0], "plist")) {
    plist();
    return;
  }

  if (!strcmp(args[0], "delegate")) {
    delegate(nargs,args,path,envp);
    return;
  }
       
  if (!strcmp(args[0], "grow")) {
    if(nargs!=2) {
      fprintf(stderr, "ERROR: Bad list of arguments: grow <number>\n");
      return;
    }
    if(sscanf(args[1],"%d",&grow_depth)==1){
      grow(grow_depth);
    } else {
      fprintf(stderr, "ERROR: Bad list of arguments: grow <number>\n");
    }
    return;
  }
       
  if (!strcmp(args[0], "prune")) {
    prune();
    return;
  }
       
  if (!strcmp(args[0], "uproot")) {
    uproot();
    return;
  }
              
  /* printf("Not internal command, eh? Need a nap.. $ "); */
  /* fflush(stdout); */
  /* sleep(10); */
  /* printf("... ok... now I'm ready to run your request... $"); */
  /* fflush(stdout); */
		
		
  background = 0;            
  if (strcmp(args[nargs-1], "&") == 0) {
    background	= 1;
    nargs--;
    args[nargs]	= NULL;
  }

  stdout_fn = NULL;
  for (i = 1; i < nargs; i++) {
    if (args[i][0] == '>') {
      stdout_fn	= args[i];
      stdout_fn++;
      printf("Set stdout to %s\n", stdout_fn);
      fflush(stdout);
      for (j = i; j < nargs - 1; j++) {
	args[j] = args[j+1];
      }
      nargs--;
      args[nargs] = NULL;
      break;
    }
  }
                
  run_program(args, background, stdout_fn, path, envp);
  //	command_count = command_count + 1;  
}

void prompt_loop(char *username, char *path, char *envp[])
{
        char buffer[BUFFER_SIZE];
        char *args[ARR_SIZE];
        
        size_t nargs;
        char *s;

        while(1){
	        printf("%s [%d]$ ", username, command_count++);
		fflush(stdout);
                s = fgets(buffer, BUFFER_SIZE, stdin);
                
                if (s == NULL) {
                        /* we reached EOF */
                        printf("\n");
			fflush(stdout);
                        exit(0);
                }
                //printf("%s\n",buffer);
		//fflush(stdout);
                
                parse_args(buffer, args, ARR_SIZE, &nargs); 

                if (nargs==0) continue;

		process_command(nargs, args, path, envp);	
	}    
}

int main(int argc, char *argv[], char *envp[])
{
        struct sigaction signal_handler_struct;
                
        char *username;
        char *default_username = "UNKNOWN";
        
        char *path;
        char *default_path = "/usr/bin:/bin";
        
        memset (&signal_handler_struct, 0, sizeof(signal_handler_struct));
        signal_handler_struct.sa_handler = signal_handler;
        signal_handler_struct.sa_flags = SA_RESTART;
        
        if (sigaction(SIGCHLD, &signal_handler_struct, NULL)) {
                fprintf(stderr, "Couldn't register SIGCHLD handler.\n");
        }
        
        if (sigaction(SIGHUP, &signal_handler_struct, NULL)) {
                fprintf(stderr, "Couldn't register SIGHUP handler.\n");
        }
        
        username = find_env("USER", default_username, envp);
        path = find_env("PATH", default_path, envp);

        prompt_loop(username, path, envp);
        
        return 0;
}
