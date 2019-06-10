#include <stdio.h>
#include <signal.h>

struct sigaction act;
int run = 1;

/* Stop the printing until the next interrupt. */
void handler(int signo) {
	
	run = !run;
}

void foo() {
	
	static int count = 0;
	printf("%d: Count = %d\n", time(NULL), ++count);
}

int main() {
	
	/* Set up the signal handler. */
	act.sa_handler=handler;
	act.sa_flags=0;
	sigemptyset(&act.sa_mask);

	/* Install the signal handler. */
	sigaction(SIGINT, &act, NULL);
	
	/* Print infinitely. */
	while (1) {
	
		while (run) foo();
		while (!run) {}
	}
}
