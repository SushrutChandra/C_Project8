#include "safe-fork.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include <sysexits.h>

int main(int argc, char **argv) {
  int retval = 0;
  int pipefd[2];
  pid_t child_pid;
  int cnt_comments = 100, cnt_nocomments = 50;

  if (argc == 2) {

    pipe(pipefd);

    /* Fork to run remove-comments */
    child_pid= safe_fork();

    if (child_pid == 0) { /* this is the child code */
      /* Redirect STDOUT to pipe */
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[0]);
      close(pipefd[1]);

      /* Execute system command */
      execlp("./remove-comments.x", "./remove-comments.x", argv[1],  NULL);

      err(EX_OSERR, "exec error");  /* why no if statement? */

    } else if (child_pid < 0)   /* error case; child_pid == -1 */
      err(EX_OSERR, "fork error");

    /* Parent program */
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);

    scanf("%d", &cnt_nocomments);

    /* Fork to run wc */
    pipe(pipefd);
    child_pid= safe_fork();

    if (child_pid == 0) { /* this is the child code */
      /* Redirect STDOUT to pipe */
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[0]);
      close(pipefd[1]);

      /* Execute system command */
      execlp("wc", "wc", "-m", argv[1],  NULL);

      err(EX_OSERR, "exec error");  /* why no if statement? */

    } else if (child_pid < 0)   /* error case; child_pid == -1 */
      err(EX_OSERR, "fork error");

    /* Parent program */
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    scanf("%d", &cnt_comments);

    /*printf("comments %d     %d\n", cnt_comments, cnt_nocomments);*/
    if (cnt_comments >= 2 * cnt_nocomments)
      printf("Enough comments!\n");
    else {
      printf("Too few comments!\n");
      retval = 1;
    }
  }
  else 
    retval = 2;
  
  return retval;
}
  
    
