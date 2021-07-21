#include <unistd.h>
int main(void) {
	alarm(10);
	for ( ; ; ) ;
	
	return 0;
}
