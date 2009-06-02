#include <stdio.h>

void insertion_sort( int array[], int array_length )
{
  int i, j, key;
  for(j = 1; j < array_length; j++)
  {
    key = array[j];
    for(i = j - 1; (i >= 0) && (array[i] < key); i--)
    {
      array[i+1] = array[i];
    }
    array[i+1] = key;
  }
  return;
}


void print_array( int array[], int array_length )
{
int i=0;
for(i=0; i<array_length; i++)
  printf("\n%i ", array[i]);
}

int main()
{
  int arr[8];
  arr[0]=1;
  arr[1]=2;
  arr[2]=3;
  arr[3]=4;
  arr[4]=1;
  arr[5]=2;
  arr[6]=3;
  arr[7]=4;
  print_array( arr, 8 );
  
  printf("\nnow sorting\n");

  insertion_sort( arr, 8 );

  print_array( arr, 8 );


  return 0;
}
