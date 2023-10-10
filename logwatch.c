#define _GNU_SOURCE
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <regex.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <systemd/sd-journal.h>

#define LOGWATCH_CONF "/etc/logwatch.conf"

// Handle SIGINT and SIGTERM requests
static int request_exit = 0;

void sigint(int signum)
{
    request_exit = 1;
}

void logwatch(const char *match, const char *pattern, const char *action)
{
    sd_journal *journal;
    regex_t regex;
    int ret;

    if(pattern) {
        if(regcomp(&regex, pattern, 0))
            errx(EXIT_FAILURE, "Invalid expression %s\n", pattern);
    }

    ret = sd_journal_open_namespace(&journal, NULL, 0);
    sd_journal_seek_tail(journal);

    if((ret = sd_journal_add_match(journal, match, 0)) < 0)
        errx(EXIT_FAILURE, "sd_journal_add_match");

    while(!request_exit) {
        if((ret = sd_journal_next(journal)) < 0)
            errx(EXIT_FAILURE, "sd_journal_next");

        if(ret == 0) {
            usleep(100000);
            continue;
        }

        if(pattern) {
            const void *message;
            size_t length;
            char *strmessage;

            if((ret = sd_journal_get_data(journal, "MESSAGE", &message, &length)) < 0) {
                errx(EXIT_FAILURE, "sd_journal_get_data");
                continue;
            }
            strmessage = strndup(message, length);
            if(regexec(&regex, strmessage, 0, NULL, 0) == REG_NOMATCH) {
                free(strmessage);
                continue;
            }
            free(strmessage);
        }

        const void *data;
        size_t length;
//        struct timespec realtime;
//        struct tm timestamp;
//        time_t sec;
//        if((ret = sd_journal_get_data(journal, "MESSAGE", &data, &length)) < 0) {
//            errx(EXIT_FAILURE, "sd_journal_get_data");
//            continue;
//        }
//        sd_journal_get_realtime_usec(journal, (uint64_t *)&realtime);
//        sec = realtime.tv_sec;
        sd_journal_restart_data(journal);

        // Need to retrieve data from systemd before the fork
        char *envp[256];
        int i = 0;
        while(sd_journal_enumerate_data(journal, &data, &length) > 0) {
            asprintf(&envp[i++], "%.*s", (int) length, (const char *)data);
        }
        envp[i] = 0;

        int pid;
        if((pid = fork()) == 0) {
            execle(action, action, NULL, envp);
            exit(EXIT_FAILURE);
        }

        // Deallocate memory
        for(i = 0; envp[i]; i++) free(envp[i]);

        waitpid(pid, &ret, 0);
    }
    if(pattern) regfree(&regex);
    sd_journal_close(journal);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sigint);
    signal(SIGTERM, sigint);
    char *line = malloc(BUFSIZ);
    size_t len = BUFSIZ;
    ssize_t nread;
    char watch[BUFSIZ];
    char pattern[BUFSIZ];
    char action[BUFSIZ];

    FILE *fd;
    int ret;
    if((fd = fopen(LOGWATCH_CONF, "r")) == NULL)
        err(EXIT_FAILURE, "open(\"%s\")", LOGWATCH_CONF);

    while((nread = getline(&line, &len, fd)) != -1) {
        ret = sscanf(line, "%s %s %s", watch, pattern, action);
        if(ret != 3) continue;
        if(watch[0] == '#') continue;

        if(fork() == 0) {
            free(line);
            fclose(fd);

            sprintf(argv[0], "%-32s", "logwatch [systemd]");
            logwatch(watch, pattern, action);
        }
    }
    free(line);
    fclose(fd);

    sprintf(argv[0], "%-32s", "logwatch [master]");
    daemon(1, 1);
    while(!request_exit) pause();

    exit(EXIT_SUCCESS);
}
