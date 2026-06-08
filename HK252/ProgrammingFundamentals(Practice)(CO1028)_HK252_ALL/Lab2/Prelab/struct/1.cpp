#include <string>
using namespace std;

struct SCP
{
  int id, objClass, numAddendums;
  string speConProcedures, description;
  string *addendums;
};

void addAddendum(SCP &obj, string addendum)
{
  int n = obj.numAddendums + 1;
  string * newA = new string[n];
  for(int i = 0; i < n - 1; i++) newA[i] = obj.addendums[i];
  newA[n - 1] = addendum;
  delete[] obj.addendums;

  obj.addendums = newA;
  obj.numAddendums = n;
}
