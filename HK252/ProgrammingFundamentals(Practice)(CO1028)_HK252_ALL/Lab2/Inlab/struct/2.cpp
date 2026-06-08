#include "struct.h"

string convertToString(SCP obj) {
  string out, id = to_string(obj.id);

  while(id.size() < 3) id = "0" + id;
  out += "Item #: SCP-" + id + "\n";

  out += "Object Class: ";
  switch (obj.objClass) {
    case 0: out += "Safe"; break;
    case 1: out += "Euclid"; break;
    case 2: out += "Keter"; break;
    default: break;
  }
  out += '\n';

  out += "Special Containment Procedures: " + obj.speConProcedures + "\n";

  out += "Description: " + obj.description + "\n";

  for(int i = 0; i < obj.numAddendums; i++)
    out += obj.addendums[i] + "\n";

  return out;
}
