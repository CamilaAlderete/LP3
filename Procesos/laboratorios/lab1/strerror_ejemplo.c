#include <stdio.h>
#include <errno.h>
#include <string.h>

int main ()
{


  FILE * pFile;

  pFile = fopen ("strerror_ejemplo.xxxx","rb");
  if (pFile == NULL)
  {
  	// strerror(numero de error): devuelve el error
    printf( "Error Value is : %s\n", strerror(errno) );
  }
  else {
	printf("No error\n");
    fclose (pFile);
 } 
return 0;
}
