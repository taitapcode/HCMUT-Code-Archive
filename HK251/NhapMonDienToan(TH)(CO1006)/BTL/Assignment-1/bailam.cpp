#include<bits/stdc++.h>
#define el '\n'
#define ll long long
#define db double
using namespace std;

int main()
{
  int slayerLevel, hp, hasTalisman, demonPresence, demonRank, allyCount, bossHP, totalDamage, specialMoveReady;
  db breathingMastery, swordSharpness;
  char timeOfDay;

  cin >> slayerLevel >> hp >> breathingMastery >> hasTalisman >> timeOfDay >> demonPresence >> demonRank
      >> swordSharpness >> allyCount >> bossHP >> totalDamage >> specialMoveReady;

  cout << fixed << setprecision(1);

  // Sence 1
  db power = slayerLevel * 10  + hp / 10.0 + breathingMastery * 50;

  cout << "[Scene 1] Rank: ";
  if (power >= 120) cout << "Hashira";
  else if (power >= 80) cout << "Elite";
  else cout << "Novice";
  cout << " (power = " << power << ")" << el;

  // Scene 2
  cout << "[Scene 2] ";
  if (hasTalisman == 0) cout << "Denied: No talisman.";
  else if (timeOfDay != 'D' && timeOfDay != 'N') cout << "Warning: invalid timeOfDay.";
  else if (timeOfDay == 'N' && demonPresence == 1) cout << "Open silently.";
  else cout << "Open cautiously.";
  cout << el;

  // Scene 3
  db adv = (101 - demonRank * 15) + swordSharpness * 0.4 + allyCount * 5.0;

  cout << "[Scene 3] ";
  if (adv >= 100) cout << "Engage head-on";
  else if (adv >= 60) cout << "Harass and probe";
  else cout << "Retreat and regroup";
  cout << " (adv = " << adv << ")" << el;

  // Scene 4
  int finalHP = bossHP - totalDamage;

  cout << "[Scene 4] ";
  if (finalHP <= 0) cout << "Boss defeated!";
  else if (finalHP > 0 && specialMoveReady == 1 && finalHP <= 50) cout << "Use special move to finish!";
  else cout << "Withdraw to heal.";
  cout << " (finalHP = " << max(finalHP, 0) << ")" << el;

  return 0;
}
