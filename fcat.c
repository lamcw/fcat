#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static const char usage[] =
	"Usage: %s [-u] [-h] files...\n"
	"\n"
	"  -u        write bytes from input file to stdout without delay as each is read.\n"
	"  -h        show help message and quit.\n";

ssize_t fcat(int infile, int pipefd[2])
{
	ssize_t total = 0;

	while (1) {
		ssize_t nbytes =
			splice(infile, NULL, pipefd[1], NULL, BUFSIZ, 0);
		if (nbytes == 0) {
			break;
		}

		if (nbytes == -1) {
			perror("in");
			return -1;
		}

		nbytes =
			splice(pipefd[0], NULL, STDOUT_FILENO, NULL, BUFSIZ, 0);
		if (nbytes == -1) {
			perror("out");
			return -1;
		}
	}

	return total;
}

int main(int argc, char **argv)
{
	int opt;

	while ((opt = getopt(argc, argv, "uh")) != -1) {
		switch (opt) {
		case 'u':
			/**
			 * -u  write immediately to stdout
			 * this option is ignored in this implmentation
			 */
			break;
		case 'h':
			fprintf(stderr, usage, argv[0]);
			return EXIT_SUCCESS;
		default:
			fprintf(stderr, usage, argv[0]);
			return EXIT_FAILURE;
		}
	}

	/* splice needs either one of the fd to refer to a pipe */
	int pipefd[2];
	if (pipe(pipefd)) {
		perror("pipe");
		return EXIT_FAILURE;
	}

	if (argc < 2) {
		ssize_t nbytes = fcat(STDIN_FILENO, pipefd);
		if (nbytes == -1) {
			goto close_fd;
		}
		return EXIT_SUCCESS;
	}

	for (int i = 1; i < argc; i++) {
		int fd;
		if (strncmp(argv[i], "-", 1) == 0) {
			fd = STDIN_FILENO;
		} else {
			fd = open(argv[i], O_RDONLY);
			if (fd == -1) {
				goto close_fd;
			}
		}

		ssize_t ret = fcat(fd, pipefd);

		if (fd != STDIN_FILENO) {
			close(fd);
		}

		if (ret == -1) {
			goto close_fd;
		}
	}

	return EXIT_SUCCESS;

close_fd:
	close(pipefd[0]);
	close(pipefd[1]);
	return EXIT_FAILURE;
}
