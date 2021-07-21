// C program to implement concurrent merge sort 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <time.h> 

void insertionSort(int arr[], int n); 
void merge(int a[], int l1, int h1, int h2); 

void printArray(int A[], int n)
{
    int i;
    // int n = sizeof(A)/sizeof(A[0]);// al usar esto solo imprime dos valores 
    for (i = 0; i < n; i++)
        printf("%d ", A[i]);
    printf("\n");
}

void mergeSort(int a[], int l, int h) 
{ 
	int i, len=(h-l+1); 

	// ARRAY MUY PEQUENO
	if (len<=5) 
	{ 
		insertionSort(a+l, len); 
		return; 
	} 

	// FORK
	pid_t lpid,rpid; 
	lpid = fork(); 
	if (lpid<0) // HIJO IZQUIERDO NO CREADO 
	{ 
		
		perror("Left Child Proc. not created\n"); 
		_exit(-1); 
	} 
	else if (lpid==0) //HIJO
	{ 
		mergeSort(a,l,l+len/2-1); //SUB ARRAY IZQUIERDO
		_exit(0); 
	} 
	else //PADRE
	{ 
		rpid = fork(); 
		if (rpid<0) // HIJO DERECHO NO CREADO 
		{ 
			
			perror("Right Child Proc. not created\n"); 
			_exit(-1); 
		} 
		else if(rpid==0) // HIJO DERECHO
		{ 
			mergeSort(a,l+len/2,h); //SUB ARRAY DERECHO
			_exit(0); 
		} 
	} 

	int status; 

	//ESPERAR A LA FINALIZACION DE LOS HIJOS
	// wait(NULL); no funciona
	waitpid(lpid, &status, 0); 
	waitpid(rpid, &status, 0); 

	// MERGE DE LOS SUBARRAYS 
	merge(a, l, l+len/2-1, h); 
} 

/* INSERTION SORT*/
void insertionSort(int arr[], int n) 
{ 
int i, key, j; 
for (i = 1; i < n; i++) 
{ 
	key = arr[i]; 
	j = i-1; 

	/* Move elements of arr[0..i-1], that are 
		greater than key, to one position ahead 
		of their current position */
	while (j >= 0 && arr[j] > key) 
	{ 
		arr[j+1] = arr[j]; 
		j = j-1; 
	} 
	arr[j+1] = key; 
} 
} 

// Method to merge sorted subarrays 
void merge(int a[], int l1, int h1, int h2) 
{ 
	// We can directly copy the sorted elements 
	// in the final array, no need for a temporary 
	// sorted array. 
	int count=h2-l1+1; 
	int sorted[count]; 
	int i=l1, k=h1+1, m=0; 
	while (i<=h1 && k<=h2) 
	{ 
		if (a[i]<a[k]) 
			sorted[m++]=a[i++]; 
		else if (a[k]<a[i]) 
			sorted[m++]=a[k++]; 
		else if (a[i]==a[k]) 
		{ 
			sorted[m++]=a[i++]; 
			sorted[m++]=a[k++]; 
		} 
	} 

	while (i<=h1) 
		sorted[m++]=a[i++]; 

	while (k<=h2) 
		sorted[m++]=a[k++]; 

	int arr_count = l1; 
	for (i=0; i<count; i++,l1++) 
		a[l1] = sorted[i]; 
} 

// COMPROBAR QUE ESTE ORDENADO
void isSorted(int arr[], int len) 
{ 
	if (len==1) 
	{ 
		printf("Sorting Done Successfully\n"); 
		return; 
	} 

	int i; 
	for (i=1; i<len; i++) 
	{ 
		if (arr[i]<arr[i-1]) 
		{ 
			printf("Sorting Not Done\n"); 
			return; 
		} 
	} 
	printf("Sorting Done Successfully\n"); 

	printArray(arr, len);
	return; 
} 

// To fill randome values in array for testing 
// purpise 
void fillData(int a[], int len) 
{ 
	// Create random arrays 
	int i; 
	for (i=0; i<len; i++) 
		a[i] = rand()%50; 
	return; 
} 

// Driver code 
int main() 
{ 
	int shmid; 
	key_t key = IPC_PRIVATE; 
	int *shm_array; 


	// Using fixed size array. We can uncomment 
	// below lines to take size from user 
	int length = 128; 

	/* printf("Enter No of elements of Array:"); 
	scanf("%d",&length); */

	// Calculate segment length 
	size_t SHM_SIZE = sizeof(int)*length; 

	// Create the segment. 
	if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) 
	{ 
		perror("shmget"); 
		_exit(1); 
	} 

	// Now we attach the segment to our data space. 
	if ((shm_array = shmat(shmid, NULL, 0)) == (int *) -1) 
	{ 
		perror("shmat"); 
		_exit(1); 
	} 

	// Create a random array of given length 
	srand(time(NULL)); 
	fillData(shm_array, length); 

	// Sort the created array 
	mergeSort(shm_array, 0, length-1); 

	// Check if array is sorted or not 
	isSorted(shm_array, length); 

	/* Detach from the shared memory now that we are 
	done using it. */
	if (shmdt(shm_array) == -1) 
	{ 
		perror("shmdt"); 
		_exit(1); 
	} 

	/* Delete the shared memory segment. */
	if (shmctl(shmid, IPC_RMID, NULL) == -1) 
	{ 
		perror("shmctl"); 
		_exit(1); 
	} 

	return 0; 
} 
