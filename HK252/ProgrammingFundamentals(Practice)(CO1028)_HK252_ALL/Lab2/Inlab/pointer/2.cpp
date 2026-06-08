void addElement(int*& arr, int n, int val, int index)
{
  int *newArr = new int[n + 1];
  for(int i = 0; i < index; i++) newArr[i] = arr[i];
  newArr[index] = val;
  for(int i = index; i < n; i++) newArr[i + 1] = arr[i];

  delete []arr;
  arr = newArr;
}
