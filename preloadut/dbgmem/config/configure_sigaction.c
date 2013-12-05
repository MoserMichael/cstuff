#include <stdint.h>
#include <signal.h>


static void do_nothing(int sig)
{
  (void) sig;
}


int main()
{
	struct sigaction sa;

	sa.sa_handler = do_nothing;
	sa.sa_flags = 0x0;
	sigemptyset(&sa.sa_mask);
	if (sigaction( SIGINT, &sa, 0) == -1)
	    return -1;
	return 0;
}
