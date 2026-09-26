int findMax(int* arr, int length)
{
  if (length == 1) return arr[0];
  int maxOfRest = findMax(arr + 1, length - 1);
  return arr[0] > maxOfRest ? arr[0] : maxOfRest;
}
