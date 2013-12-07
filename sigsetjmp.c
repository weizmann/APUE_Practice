#include "apue.h"
#include "my_err.h"

#include <setjmp.h>
#include <time.h>

static void sig_usr1(int), sig_alrm(int);

static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjump;

void print_mask(const char*); 

int 
main(void)
{
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
        err_sys("sig sig_usr1 error");
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("sig sig_alrm error");
    print_mask("starting main: ");

    if (sigsetjmp(jmpbuf, 1)) {
        print_mask("end main.....");
        exit(0);
    }
    canjump = 1;
    while(1) {
        pause();
    }
}


static void sig_usr1(int signo) {
    time_t start_time;
    if (canjump == 0) {
        return;
    }
    print_mask("starting sig_usr1: ");
    alarm(3);
    start_time = time(NULL);
    while (1) {
        if (start_time + 5 < time(NULL)) {
            break;
        }
    }
    print_mask("finishing sig_usr1: ");
    canjump = 0;
    siglongjmp(jmpbuf, 1);
}

static void sig_alrm(int signo) {
    print_mask("in sig_alrm: ");
}

void print_mask(const char* str) {
    sigset_t sigset;
    int errno_save;

    errno_save = errno;

    if (sigprocmask(0, NULL, &sigset) < 0)
        err_sys("sigproc error");
    printf("%s \t", str) ;

    if (sigismember(&sigset, SIGINT) ) printf(" SIGINT ");
    if (sigismember(&sigset, SIGQUIT)) printf(" SIGQUIT ");
    if (sigismember(&sigset, SIGUSR1)) printf(" SIGUSR1 ");
    if (sigismember(&sigset, SIGALRM)) printf(" SIGALRM ");
    printf("............");
    printf("\n");
    errno = errno_save;
}











