#include <stdio.h>
#include <signal.h>

void foo() {
	
	static int count = 0;
	printf("%d: Count = %d\n", time(NULL), ++count);
}

int main() {
	
	sigset_t newsigset, oldsigset;

	/* Initialize a new signal set. */
	sigemptyset(&newsigset);

	/* Add SIGINT to the signal set. */
	sigaddset(&newsigset, SIGINT);
	
	/* Add SIGINT to the signal mask. */
	sigprocmask(SIG_BLOCK, &newsigset, &oldsigset);
	
	/* Print infinitely. */
	while (1) foo();
}
