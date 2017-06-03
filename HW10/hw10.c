#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define what_err(msg) \
    { perror(msg); exit(EXIT_FAILURE); }

int main(int argc, char *argv[]) {
    sigset_t mask;
    int sfd;
    struct signalfd_siginfo fdsi; // Use to receive signal informations
    ssize_t s;
    printf("This program listens to 3 signals: SIGINT, SIGTSTP, SIGQUIT\n");
    printf("Press Ctrl+C to send SIGINT\n");
    printf("Press Ctrl+Z to send SIGTSTP\n");
    printf("Press Ctrl+\\ to send SIGQUIT\n");
    printf("Note: Pressing Ctrl+\\ will terminate this program!\n");

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT); // Ctrl+C (Interrupt)
    sigaddset(&mask, SIGQUIT);/* Ctrl+\ (Quit) */
    sigaddset(&mask, SIGTSTP);// Ctrl+Z (Stop)

    /* Block signals so that they aren't handled
       according to their default dispositions */

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
        what_err("sigprocmask");

    sfd = signalfd(-1, &mask, 0); // listen new signal fd
    if (sfd == -1)
        what_err("signalfd");

    while(1) {
        s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo)); // Okay, Blocking is seted
        if (s != sizeof(struct signalfd_siginfo)) // Abnormal readed
            what_err("read");

        // Print which signal read
        switch (fdsi.ssi_signo) {
            case SIGINT:
            printf("Receive SIGINT\n");
            break;
            case SIGQUIT:
            printf("Receive SIGQUIT\n");
            return EXIT_SUCCESS;
            break;
            case SIGTSTP:
            printf("Receive SIGTSTP\n");
            break;
            default:
            printf("Something strange...\n");
            break;
        }
    }
    return EXIT_SUCCESS;
}
