#include <stdio.h>
#include <errno.h>
#include <string.h>

int main ()
{


  FILE * pFile;

  pFile = fopen ("strerror_ejemplo.xxx","rb");
  if (pFile == NULL)
  {
    perror("Error al abrir el archivo");
  }
  else {
	printf("No error");
    fclose (pFile);
 } 
return 0;
}
