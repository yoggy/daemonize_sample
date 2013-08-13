#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

char *pid_file = "./.daemonize_sample.pid";

pid_t daemonize_pid = -1;

void usage()
{
	printf("usage :\n");
	printf("       ./daemon_sample start \n");
	printf("       ./daemon_sample stop \n");
	printf("       ./daemon_sample foreground \n");
	printf("\n");

	exit(1);
}

void daemonize_process_signal_handler()
{
	exit(0);
}

void finish_daemonize_process()
{
	unlink(pid_file);
	printf("finish daemonize_sample...pid=%d\n", daemonize_pid);
}

int exists_pid_file()
{
	struct stat st;
	if (stat(pid_file, &st) != 0) {
		return 0;
	}

	if (S_ISREG(st.st_mode)) return 1;
	
	return 0;
}

void start_daemon()
{
	if (exists_pid_file()) {
		printf("daemonize_sample already runnning...\n");
		exit(1);
	}

	FILE *fp = fopen(pid_file, "w+");
	if (fp == NULL) {
		perror("cannot open pid_file...");
		exit(1);
	}

	pid_t pid = fork();
	if (pid) {
		// parent	
		fprintf(fp, "%d", pid);
		fflush(fp);
		fclose(fp);

		printf("start daemonize_sample....pid=%d\n", pid);
		exit(0);
	}
	else {
		// child
		fclose(fp);
		fp = NULL;
		signal(SIGINT, daemonize_process_signal_handler);
		signal(SIGTERM, daemonize_process_signal_handler);
		atexit(finish_daemonize_process);

		daemonize_pid = getpid();
	}
}

void stop_daemon()
{
	FILE *fp = fopen(pid_file, "r+");
	if (fp == NULL) {
		perror("cannot open pid_file...");
		exit(1);
	}

	char buf[32];
	if (fgets(buf, 32, fp) == NULL) {
		perror("read pid failed...");
		exit(1);
	}

	pid_t pid = atoi(buf);
	printf("stop daemonize_sample...pid=%d\n", pid);

	kill(pid, SIGINT);
}

void do_something_main()
{
	while(1) {
		sleep(1);
		printf("do something...\n");
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) usage();

	char *cmd = argv[1];

	if (strcmp("stop", cmd) == 0) {
		stop_daemon();
		return 0;
	}
	else if (strcmp("start", cmd) == 0) {
		start_daemon();
		do_something_main();
	}
	else if (strcmp("foreground", cmd) == 0) {
		do_something_main();
	}
	else {
		usage();
	}

	return 0;
}
