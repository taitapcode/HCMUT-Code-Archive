#include "struct.h"

SCP createSCP(
  int id, int objClass, string speConProcedures,
  string description, string* addendums, int numAddendums
) {
  return {id, objClass, speConProcedures, description, addendums, numAddendums};
}
