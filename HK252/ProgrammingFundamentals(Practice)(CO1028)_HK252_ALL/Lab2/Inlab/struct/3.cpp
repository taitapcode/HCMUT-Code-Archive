#include "struct.h"

SCP* cloneSCP(SCP* original)
{
  SCP *newSCP = new SCP {
    original->id,
    original->objClass,
    original->speConProcedures,
    original->description,
    new string[original->numAddendums],
    original->numAddendums,
  };

  for(int i = 0; i < newSCP->numAddendums; i++) newSCP->addendums[i] = original->addendums[i];

  return newSCP;
}
