#include "eniesLobby.h"

template <typename T>
T clamp(const T& value, const T& low, const T& high)
{
  return max(low, min(value, high));
}

int percentageOf(int percent, int value)
{
  return ceil(value / 100.0 * percent);
}

/*
 * BattleContext
 */
BattleContext::BattleContext()
{
  turnCount = 0;
  morale = 0;
  alarmLevel = 0;
  rescueProgress = 0;
  escapeProgress = 0;
  busterCallTimer = 0;
  mainGateDestroyed = false;
  robinRescued = false;
  bridgeOpened = false;
  battleEnded = false;
  resultCode = "";
  minStrawHatHP = INT_MAX;
}

void BattleContext::nextTurn()
{
  turnCount++;
}

void BattleContext::increaseMorale(int value)
{
  morale = clamp(morale + value, 0, 100);
}

void BattleContext::increaseAlarmLevel(int value)
{
  alarmLevel = clamp(alarmLevel + value, 0, 100);
}

void BattleContext::increaseBusterCallTimer(int value)
{
  busterCallTimer = max(busterCallTimer + value, 0);
}

void BattleContext::increaseEscapeProgress(int value)
{
  escapeProgress = clamp(escapeProgress + value, 0, 100);
}

void BattleContext::increaseRescueProgress(int value)
{
  rescueProgress = clamp(rescueProgress + value, 0, 100);
}

/*
 * Character
 */
Character::Character()
{
  name = "";
  hp = 0;
  maxHp = 0;
  atk = 0;
  def = 0;
  speed = 0;
  energy = 0;
  alive = false;
  killedEnemyInTurn = false;
}

Character::Character(string name, int hp, int atk, int def, int speed, int energy)
    : name(name), hp(hp), maxHp(hp), atk(atk), def(def), speed(speed), energy(energy), alive(hp > 0), killedEnemyInTurn(false)
{
}

Character::~Character()
{
}

int Character::attack(Building* target, BattleContext& context)
{
  return 0;
}

int Character::specialSkill(Building* target, BattleContext& context)
{
  return -1;
}

void Character::endTurn(BattleContext& context)
{
  return;
}

void Character::receiveDamage(int damage)
{
  increaseHP(-max(damage - def, 0));
}

bool Character::isAlive() const
{
  return alive;
}

string Character::getName() const
{
  return name;
}

int Character::getHP() const
{
  return hp;
}

int Character::getEnergy() const
{
  return energy;
}

int Character::getDef() const
{
  return def;
}

int Character::getAtk() const
{
  return atk;
}

int Character::getSpeed() const
{
  return speed;
}

int Character::getMaxHP() const
{
  return maxHp;
}

bool Character::isHPBelow(int percent) const
{
  return hp < percentageOf(percent, maxHp);
}

bool Character::isHPAtMost(int percent) const
{
  return hp <= percentageOf(percent, maxHp);
}

bool Character::consumeEnergy(int cost)
{
  if (energy < cost) return false;
  increaseEnergy(-cost);
  return true;
}

void Character::increaseEnergy(int value)
{
  energy = clamp(energy + value, 0, 100);
}

void Character::increaseHP(int value)
{
  hp = clamp(hp + value, 0, maxHp);
  alive = hp > 0;
}
void Character::increaseDef(int value)
{
  def += value;
}

void Character::increaseAtk(int value)
{
  atk += value;
}

void Character::increaseSpeed(int value)
{
  speed += value;
}

void Character::inflictDamage(Character* target, int damage)
{
  if (!target) return;

  bool wasAlive = target->isAlive();
  target->receiveDamage(damage);

  killedEnemyInTurn = wasAlive && !target->isAlive();
}

bool Character::isStrawHat() const
{
  return false;
}

bool Character::isCP9() const
{
  return false;
}

/*
 * StrawHat
 */
StrawHat::StrawHat() : Character()
{
  bounty = 0;
}

StrawHat::StrawHat(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : Character(name, hp, atk, def, speed, energy), bounty(bounty) {}

bool StrawHat::isStrawHat() const
{
  return true;
}

string StrawHat::str() const
{
  stringstream ss;
  ss << "StrawHat[name=" << name << ", hp=" << hp << ", atk=" << atk << ", def=" << def
     << ", speed=" << speed << ", energy=" << energy << ", bounty=" << bounty << "]";
  return ss.str();
}

/*
 * Luffy
 */
Luffy::Luffy(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Luffy::attack(Character* target, BattleContext& context)
{
  int dmg;
  double ratio = (double)hp / maxHp;

  if (ratio > 0.5)
    dmg = this->atk;
  else if (ratio > 0.3)
    dmg = percentageOf(115, this->atk);
  else
    dmg = percentageOf(130, this->atk);

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(5);

  return max(dmg - target->getDef(), 0);
}

int Luffy::specialSkill(Character* target, BattleContext& context)
{
  if (isHPBelow(15)) return -1;
  if (!consumeEnergy(20)) return -1;

  int dmg = atk * 2;

  increaseAtk(15);
  increaseSpeed(15);

  context.increaseAlarmLevel(10);
  increaseHP(-percentageOf(8, maxHp));

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(5);

  return max(dmg - target->getDef(), 0);
}

int Luffy::attack(Building* target, BattleContext& context)
{
  int dmg = atk;

  if (isHPAtMost(30))
    dmg = percentageOf(130, atk);
  else if (isHPAtMost(50))
    dmg = percentageOf(115, atk);

  target->receiveDamage(dmg);

  return dmg;
}

int Luffy::specialSkill(Building* target, BattleContext& context)
{
  if (isHPBelow(15)) return -1;
  if (!consumeEnergy(20)) return -1;

  int dmg = atk * 2;

  increaseSpeed(15);
  increaseAtk(15);
  context.increaseAlarmLevel(10);
  increaseHP(-percentageOf(8, maxHp));

  target->receiveDamage(dmg);

  return dmg;
}

void Luffy::endTurn(BattleContext& context)
{
  if (isHPAtMost(30)) context.increaseMorale(3);

  if (killedEnemyInTurn)
  {
    increaseEnergy(5);
    killedEnemyInTurn = false;
  }
}

/*
 * Zoro
 */
Zoro::Zoro(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Zoro::attack(Character* target, BattleContext& context)
{
  int dmg = atk + percentageOf(20, def);
  if (target->getHP() < percentageOf(40, target->getMaxHP()))
    dmg = percentageOf(115, dmg);

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(5);
  return max(dmg - target->getDef(), 0);
}

int Zoro::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(15)) return -1;
  int dmg = percentageOf(220, atk);

  if (target->getHP() < percentageOf(50, target->getMaxHP()))
    dmg = percentageOf(150, dmg);

  inflictDamage(target, dmg);
  if (killedEnemyInTurn)
  {
    increaseEnergy(8);
    context.increaseMorale(4);
  }

  return max(dmg - target->getDef(), 0);
}

int Zoro::attack(Building* target, BattleContext& context)
{
  int dmg = atk + percentageOf(20, def);

  if (target->getHP() < percentageOf(40, target->getMaxHP()))
    dmg = percentageOf(115, dmg);

  target->receiveDamage(dmg);

  return dmg;
}

int Zoro::specialSkill(Building* target, BattleContext& context)
{
  if (!consumeEnergy(15)) return -1;

  int dmg = percentageOf(220, atk);

  if (target->getHP() < percentageOf(50, target->getMaxHP()))
    dmg = percentageOf(150, dmg);

  target->receiveDamage(dmg);

  return dmg;
}

void Zoro::endTurn(BattleContext& context)
{
  if (killedEnemyInTurn)
  {
    killedEnemyInTurn = false;
    context.increaseMorale(6);
    increaseAtk(percentageOf(5, atk));
  }
}

/*
 * Sanji
 */
Sanji::Sanji(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Sanji::attack(Character* target, BattleContext& context)
{
  int dmg = atk + percentageOf(50, speed);
  if (target->getDef() < def) dmg = percentageOf(110, dmg);

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(5);

  return max(dmg - target->getDef(), 0);
}

int Sanji::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(18)) return -1;

  int dmg = percentageOf(210, atk);
  int actualDmg = max(dmg - target->getDef(), 0);

  inflictDamage(target, dmg);

  if (!killedEnemyInTurn)
  {
    int defDrop = target->getName() == "Jabra" ? -12 : -8;
    target->increaseDef(defDrop);
  }
  else
    context.increaseMorale(5);

  return actualDmg;
}

int Sanji::attack(Building* target, BattleContext& context)
{
  int dmg = atk + percentageOf(50, speed);
  target->receiveDamage(dmg);

  return dmg;
}

int Sanji::specialSkill(Building* target, BattleContext& context)
{
  if (!consumeEnergy(18)) return -1;

  int dmg = percentageOf(210, atk);
  target->receiveDamage(dmg);

  return dmg;
}

void Sanji::endTurn(BattleContext& context)
{
  if (killedEnemyInTurn)
  {
    killedEnemyInTurn = false;
    context.increaseMorale(8);
    increaseAtk(percentageOf(10, atk));
  }
}

/*
 * Nami
 */
Nami::Nami(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Nami::attack(Character* target, BattleContext& context)
{
  int dmg = atk + (target->getDef() * 30) / 100;

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(5);
  return max(dmg - target->getDef(), 0);
}

int Nami::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(20)) return -1;

  int dmg = atk + 40;
  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(5);

  if (!killedEnemyInTurn) target->increaseSpeed(-10);

  context.increaseBusterCallTimer(1);
  context.increaseAlarmLevel(-5);

  return max(dmg - target->getDef(), 0);
}

int Nami::attack(Building* target, BattleContext& context)
{
  int dmg = percentageOf(50, atk);
  target->receiveDamage(dmg);

  return dmg;
}

int Nami::specialSkill(Building* target, BattleContext& context)
{
  if (!consumeEnergy(20)) return -1;

  int dmg = percentageOf(150, atk + 40);
  target->receiveDamage(dmg);

  context.increaseBusterCallTimer(1);
  context.increaseAlarmLevel(-5);

  return dmg;
}

void Nami::endTurn(BattleContext& context)
{
  if (killedEnemyInTurn)
  {
    killedEnemyInTurn = false;
    increaseEnergy(6);
  }
}

/*
 * Chopper
 */
Chopper::Chopper(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Chopper::attack(Character* target, BattleContext& context)
{
  inflictDamage(target, atk);
  if (killedEnemyInTurn) context.increaseMorale(5);
  return max(atk - target->getDef(), 0);
}

int Chopper::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(15)) return -1;

  if (target->isStrawHat())
  {
    target->increaseHP(35 + percentageOf(50, atk));
    if (target->getName() == "Luffy") context.increaseMorale(5);
  }

  return 0;
}

int Chopper::attack(Building* target, BattleContext& context)
{
  target->receiveDamage(atk);
  return atk;
}

void Chopper::endTurn(BattleContext& context)
{
  killedEnemyInTurn = false;
}

/*
 * Usopp
 */
Usopp::Usopp(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Usopp::attack(Character* target, BattleContext& context)
{
  int dmg = atk;
  if (target->getSpeed() < 50) dmg = percentageOf(120, dmg);

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(5);

  return max(dmg - target->getDef(), 0);
}

int Usopp::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(16)) return -1;

  int dmg = percentageOf(80, atk);

  inflictDamage(target, dmg);
  if (!killedEnemyInTurn)
    target->increaseSpeed(-12);
  else
    context.increaseMorale(5);

  context.increaseEscapeProgress(8);

  return max(dmg - target->getDef(), 0);
}

int Usopp::attack(Building* target, BattleContext& context)
{
  int dmg = percentageOf(50, atk);
  target->receiveDamage(dmg);

  return dmg;
}

int Usopp::specialSkill(Building* target, BattleContext& context)
{
  if (!consumeEnergy(16)) return -1;

  int dmg = percentageOf(80, atk);
  target->receiveDamage(dmg);
  context.increaseEscapeProgress(8);

  return dmg;
}

void Usopp::endTurn(BattleContext& context)
{
  context.increaseMorale(10);
}

/*
 * Franky
 */
Franky::Franky(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Franky::attack(Character* target, BattleContext& context)
{
  int dmg = atk + percentageOf(30, def);
  if (target->isCP9()) dmg = percentageOf(110, dmg);

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(5);

  return max(dmg - target->getDef(), 0);
}

int Franky::specialSkill(Character* target, BattleContext& context)
{
  if (consumeEnergy(30))
  {
    int dmg = percentageOf(120, atk);

    inflictDamage(target, dmg);
    if (killedEnemyInTurn) context.increaseMorale(5);

    return max(dmg - target->getDef(), 0);
  }
  if (consumeEnergy(20))
  {
    int dmg = percentageOf(180, atk);

    if (target->getName() == "Lucci") dmg = percentageOf(120, dmg);

    inflictDamage(target, dmg);
    if (killedEnemyInTurn)
      context.increaseMorale(5);
    else
      target->increaseSpeed(-8);

    return max(dmg - target->getDef(), 0);
  }

  return -1;
}

int Franky::attack(Building* target, BattleContext& context)
{
  int dmg = atk + percentageOf(30, def);
  target->receiveDamage(dmg);
  return dmg;
}

int Franky::specialSkill(Building* target, BattleContext& context)
{
  if (consumeEnergy(30))
  {
    int dmg = target->getHP();
    target->receiveDamage(dmg);
    return dmg;
  }
  if (consumeEnergy(20))
  {
    int dmg = percentageOf(180, atk);
    target->receiveDamage(dmg);
    return dmg;
  }

  return -1;
}

void Franky::endTurn(BattleContext& context)
{
  if (!isHPAtMost(70)) increaseDef(5);
  if (isHPBelow(30))
    increaseAtk(percentageOf(10, atk));
}

/*
 * CP9Agent
 */
CP9Agent::CP9Agent() : Character()
{
  doriki = 0;
}

CP9Agent::CP9Agent(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : Character(name, hp, atk, def, speed, energy), doriki(doriki) {}

bool CP9Agent::isCP9() const
{
  return true;
}

string CP9Agent::str() const
{
  stringstream ss;
  ss << "CP9[name=" << name << ", hp=" << hp << ", atk=" << atk << ", def=" << def
     << ", speed=" << speed << ", energy=" << energy << ", doriki=" << doriki << "]";
  return ss.str();
}

/*
 * Lucci
 */
Lucci::Lucci(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Lucci::attack(Character* target, BattleContext& context)
{
  int dmg = atk + percentageOf(5, doriki);
  if (target->isHPBelow(50)) dmg = percentageOf(120, dmg);

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(-5);

  return max(dmg - target->getDef(), 0);
}

int Lucci::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(25)) return -1;
  int dmg = percentageOf(280, atk) + percentageOf(50, target->getDef());

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(-10);

  return max(dmg - target->getDef(), 0);
}

void Lucci::endTurn(BattleContext& context)
{
  if (isHPBelow(40)) increaseAtk(percentageOf(5, atk));
}

/*
 * Kaku
 */
Kaku::Kaku(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kaku::attack(Character* target, BattleContext& context)
{
  inflictDamage(target, atk);
  if (killedEnemyInTurn) context.increaseMorale(-5);
  return max(atk - target->getDef(), 0);
}

int Kaku::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(20)) return -1;

  int hitDamagePercents[] = {120, 100, 80};
  int totalDmg = 0;

  for (int hitDamagePercent : hitDamagePercents)
  {
    if (!target->isAlive()) break;

    int dmg = percentageOf(hitDamagePercent, atk);
    inflictDamage(target, dmg);
    totalDmg += max(dmg - target->getDef(), 0);

    if (killedEnemyInTurn)
    {
      context.increaseMorale(-5);
      break;
    }
  }

  return totalDmg;
}

void Kaku::endTurn(BattleContext& context)
{
}

/*
 * Jabra
 */
Jabra::Jabra(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Jabra::attack(Character* target, BattleContext& context)
{
  inflictDamage(target, atk);
  if (killedEnemyInTurn) context.increaseMorale(-5);
  return max(atk - target->getDef(), 0);
}

int Jabra::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(18)) return -1;

  int dmg = percentageOf(150, atk);
  if (isHPBelow(30)) dmg = percentageOf(125, dmg);

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(-5);

  return max(dmg - target->getDef(), 0);
}

void Jabra::endTurn(BattleContext& context)
{
}

/*
 * Blueno
 */
Blueno::Blueno(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Blueno::attack(Character* target, BattleContext& context)
{
  inflictDamage(target, atk);
  if (killedEnemyInTurn) context.increaseMorale(-5);
  return max(atk - target->getDef(), 0);
}

int Blueno::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(15)) return -1;

  int dmg = percentageOf(130, atk);
  if (isHPAtMost(50))
    dmg += 40;
  else
    dmg += 20;

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(-5);

  return max(dmg - target->getDef(), 0);
}

void Blueno::endTurn(BattleContext& context)
{
}

/*
 * Kalifa
 */
Kalifa::Kalifa(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kalifa::attack(Character* target, BattleContext& context)
{
  inflictDamage(target, atk);
  if (killedEnemyInTurn) context.increaseMorale(-5);
  return max(atk - target->getDef(), 0);
}

int Kalifa::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(18)) return -1;
  int dmg = percentageOf(140, atk);

  context.increaseMorale(-(target->getName() == "Nami" ? 12 : 8));
  inflictDamage(target, dmg);
  if (!killedEnemyInTurn)
    target->increaseSpeed(-6);
  else
    context.increaseMorale(-5);

  return max(dmg - target->getDef(), 0);
}

void Kalifa::endTurn(BattleContext& context)
{
}

/*
 * Kumadori
 */
Kumadori::Kumadori(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kumadori::attack(Character* target, BattleContext& context)
{
  inflictDamage(target, atk);
  if (killedEnemyInTurn) context.increaseMorale(-5);
  return max(atk - target->getDef(), 0);
}

int Kumadori::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(16)) return -1;
  int dmg = 30 + percentageOf(10, doriki);
  if (isHPBelow(40)) dmg += 25;

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(-5);

  return max(dmg - target->getDef(), 0);
}

void Kumadori::endTurn(BattleContext& context)
{
}

/*
 * Fukurou
 */
Fukurou::Fukurou(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Fukurou::attack(Character* target, BattleContext& context)
{
  inflictDamage(target, atk);
  if (killedEnemyInTurn) context.increaseMorale(-5);
  return max(atk - target->getDef(), 0);
}

int Fukurou::specialSkill(Character* target, BattleContext& context)
{
  if (!consumeEnergy(14)) return -1;

  int dmg = percentageOf(130, atk);
  if (target->getHP() == context.minStrawHatHP) dmg += 20;

  inflictDamage(target, dmg);
  if (killedEnemyInTurn) context.increaseMorale(-6);

  return max(dmg - target->getDef(), 0);
}

void Fukurou::endTurn(BattleContext& context)
{
}

/*
 * Building
 */
Building::Building(string name, int hp) : name(name), hp(hp), maxHP(hp), destroyed(hp <= 0) {}

Building::~Building()
{
}

int Building::getHP() const
{
  return hp;
}

int Building::getMaxHP() const
{
  return maxHP;
}

bool Building::isHPBelow(int percent) const
{
  return hp < percentageOf(percent, maxHP);
}

bool Building::isHPAtMost(int percent) const
{
  return hp <= percentageOf(percent, maxHP);
}

void Building::increaseHP(int value)
{
  hp = clamp(hp + value, 0, maxHP);
  destroyed = !hp;
}

void Building::receiveDamage(int damage)
{
  increaseHP(-damage);
}

bool Building::isDestroyed() const
{
  return destroyed;
}

void Building::onDestroyed(BattleContext& context)
{
  return;
}

string Building::str() const
{
  stringstream ss;
  ss << "Building[name=" << name << ", hp=" << hp << ", maxHP=" << maxHP << ", destroyed=" << (destroyed ? "true" : "false") << "]";
  return ss.str();
}

/*
 * MainGate
 */
MainGate::MainGate(string name, int hp) : Building(name, hp) {}

void MainGate::applyEffect(BattleContext& context)
{
}

void MainGate::onDestroyed(BattleContext& context)
{
  context.mainGateDestroyed = true;
  context.increaseRescueProgress(20);
  context.increaseMorale(5);
}

/*
 * Courthouse
 */
Courthouse::Courthouse(string name, int hp) : Building(name, hp) {}

void Courthouse::applyEffect(BattleContext& context)
{
  if (!isDestroyed()) context.increaseAlarmLevel(5);
}

void Courthouse::onDestroyed(BattleContext& context)
{
  context.increaseAlarmLevel(-20);
}

/*
 * TowerOfJustice
 */
TowerOfJustice::TowerOfJustice(string name, int hp) : Building(name, hp) {}

void TowerOfJustice::applyEffect(BattleContext& context)
{
  if (context.mainGateDestroyed && !context.robinRescued)
  {
    context.increaseRescueProgress(5);

    if (context.rescueProgress >= 100)
    {
      context.robinRescued = true;
      context.increaseMorale(10);
    }
  }
}

/*
 * BridgeOfHesitation
 */
BridgeOfHesitation::BridgeOfHesitation(string name, int hp) : Building(name, hp) {}

void BridgeOfHesitation::applyEffect(BattleContext& context)
{
  if (context.robinRescued)
  {
    context.bridgeOpened = true;
    context.increaseEscapeProgress(5);
  }

  if (context.escapeProgress >= 100)
  {
    context.battleEnded = true;
    context.resultCode = "STRAW_HAT_WIN";
  }
}

/*
 * BusterCallShip
 */
BusterCallShip::BusterCallShip(string name, int hp) : Building(name, hp) {}

void BusterCallShip::applyEffect(BattleContext& context)
{
  if (!destroyed) context.increaseBusterCallTimer(-1);
  if (context.busterCallTimer <= 0)
  {
    context.battleEnded = true;
    context.resultCode = "BUSTER_CALL";
  }
}

void BusterCallShip::onDestroyed(BattleContext& context)
{
  if (destroyed) context.increaseBusterCallTimer(3);
}

/*
 * EniesLobbyBattle
 */
EniesLobbyBattle::EniesLobbyBattle(const string& filename)
{
  strawHats = new Character*[7];
  strawHatCount = 0;
  cp9Agents = new Character*[7];
  cp9Count = 0;
  buildings = new Building*[5];
  buildingCount = 0;
  turnOrder = nullptr;
  maxTurns = 0;

  loadFromFile(filename);
}

EniesLobbyBattle::~EniesLobbyBattle()
{
  for (int i = 0; i < strawHatCount; i++) delete strawHats[i];
  delete[] strawHats;

  for (int i = 0; i < cp9Count; i++) delete cp9Agents[i];
  delete[] cp9Agents;

  for (int i = 0; i < buildingCount; i++) delete buildings[i];
  delete[] buildings;

  while (turnOrder)
  {
    TurnNode* temp = turnOrder;
    turnOrder = turnOrder->next;
    delete temp;
  }
}

void EniesLobbyBattle::loadFromFile(const string& filename)
{
  ifstream file(filename);
  if (!file.is_open()) return;

  string line;
  while (getline(file, line))
  {
    if (line.empty()) continue;
    stringstream ss(line);
    string type;
    ss >> type;

    if (type == "CONTEXT")
    {
      ss >> context.morale >> context.alarmLevel >> context.rescueProgress >> context.escapeProgress >> context.busterCallTimer >> maxTurns;
    }
    else if (type == "STRAW_HAT")
    {
      string name;
      int hp, atk, def, speed, energy;
      long long bounty;
      ss >> name >> hp >> atk >> def >> speed >> energy >> bounty;

      if (name == "Luffy")
        addStrawHat(new Luffy(name, hp, atk, def, speed, energy, bounty));
      else if (name == "Zoro")
        addStrawHat(new Zoro(name, hp, atk, def, speed, energy, bounty));
      else if (name == "Sanji")
        addStrawHat(new Sanji(name, hp, atk, def, speed, energy, bounty));
      else if (name == "Nami")
        addStrawHat(new Nami(name, hp, atk, def, speed, energy, bounty));
      else if (name == "Chopper")
        addStrawHat(new Chopper(name, hp, atk, def, speed, energy, bounty));
      else if (name == "Usopp")
        addStrawHat(new Usopp(name, hp, atk, def, speed, energy, bounty));
      else if (name == "Franky")
        addStrawHat(new Franky(name, hp, atk, def, speed, energy, bounty));
    }
    else if (type == "CP9")
    {
      string name;
      int hp, atk, def, speed, energy, doriki;
      ss >> name >> hp >> atk >> def >> speed >> energy >> doriki;

      if (name == "Lucci")
        addCP9Agent(new Lucci(name, hp, atk, def, speed, energy, doriki));
      else if (name == "Kaku")
        addCP9Agent(new Kaku(name, hp, atk, def, speed, energy, doriki));
      else if (name == "Jabra")
        addCP9Agent(new Jabra(name, hp, atk, def, speed, energy, doriki));
      else if (name == "Blueno")
        addCP9Agent(new Blueno(name, hp, atk, def, speed, energy, doriki));
      else if (name == "Kalifa")
        addCP9Agent(new Kalifa(name, hp, atk, def, speed, energy, doriki));
      else if (name == "Kumadori")
        addCP9Agent(new Kumadori(name, hp, atk, def, speed, energy, doriki));
      else if (name == "Fukurou")
        addCP9Agent(new Fukurou(name, hp, atk, def, speed, energy, doriki));
    }
    else if (type == "BUILDING")
    {
      string name;
      int hp;
      ss >> name >> hp;

      if (name == "MainGate")
        addBuilding(new MainGate(name, hp));
      else if (name == "Courthouse")
        addBuilding(new Courthouse(name, hp));
      else if (name == "TowerOfJustice")
        addBuilding(new TowerOfJustice(name, hp));
      else if (name == "BridgeOfHesitation")
        addBuilding(new BridgeOfHesitation(name, hp));
      else if (name == "BusterCallShip")
        addBuilding(new BusterCallShip(name, hp));
    }
  }
  file.close();
  buildTurnOrder();
}

void EniesLobbyBattle::addStrawHat(Character* character)
{
  if (strawHatCount < 7) strawHats[strawHatCount++] = character;
}

void EniesLobbyBattle::addCP9Agent(Character* character)
{
  if (cp9Count < 7) cp9Agents[cp9Count++] = character;
}

void EniesLobbyBattle::addBuilding(Building* building)
{
  if (buildingCount < 5) buildings[buildingCount++] = building;
}

void EniesLobbyBattle::buildTurnOrder()
{
  TurnNode* tail = nullptr;

  auto addNode = [&](Character* c)
  {
    TurnNode* newNode = new TurnNode{c, nullptr};
    if (!turnOrder)
      turnOrder = newNode;
    else
      tail->next = newNode;
    tail = newNode;
  };

  for (int i = 0; i < strawHatCount; i++) addNode(strawHats[i]);
  for (int i = 0; i < cp9Count; i++) addNode(cp9Agents[i]);
}

void EniesLobbyBattle::runBattle()
{
  while (!context.battleEnded && context.turnCount < maxTurns)
  {
    TurnNode* currentTurn = turnOrder;

    if (currentTurn->data->isAlive())
      processTurn(currentTurn->data);

    if (turnOrder && turnOrder->next)
    {
      turnOrder = turnOrder->next;
      currentTurn->next = nullptr;

      TurnNode* tail = turnOrder;
      while (tail->next) tail = tail->next;
      tail->next = currentTurn;
    }

    processBuildings();
    context.nextTurn();
    checkEndCondition();
  }

  if (!context.battleEnded && context.turnCount >= maxTurns)
  {
    context.battleEnded = true;
    context.resultCode = "TIME_OUT";
  }
}

void EniesLobbyBattle::processTurn(Character* character)
{
  if (!character->isAlive()) return;

  context.minStrawHatHP = INT_MAX;
  for (int i = 0; i < strawHatCount; i++)
  {
    if (strawHats[i]->isAlive() && strawHats[i]->getHP() < context.minStrawHatHP)
      context.minStrawHatHP = strawHats[i]->getHP();
  }

  Character* targetCharacter = nullptr;
  Building* targetBuilding = nullptr;
  string name = character->getName();

  if (character->isStrawHat())
  {
    if (name == "Chopper" && character->getEnergy() >= 15)
    {
      for (int i = 0; i < strawHatCount; i++)
        if (strawHats[i]->isAlive() && strawHats[i]->getHP() == context.minStrawHatHP)
        {
          targetCharacter = strawHats[i];
          break;
        }
    }

    if (!targetCharacter)
    {
      Building* mainGate = nullptr;
      Building* courthouse = nullptr;
      Building* ship = nullptr;
      Building* bridge = nullptr;

      for (int i = 0; i < buildingCount; i++)
      {
        if (!buildings[i]->isDestroyed())
        {
          string bName = buildings[i]->str();
          if (bName.find("MainGate") != string::npos)
            mainGate = buildings[i];
          else if (bName.find("Courthouse") != string::npos)
            courthouse = buildings[i];
          else if (bName.find("BusterCallShip") != string::npos)
            ship = buildings[i];
          else if (bName.find("BridgeOfHesitation") != string::npos)
            bridge = buildings[i];
        }
      }

      Character* firstCP9 = nullptr;
      for (int i = 0; i < cp9Count; i++)
      {
        if (cp9Agents[i]->isAlive())
        {
          firstCP9 = cp9Agents[i];
          break;
        }
      }

      if (mainGate)
        targetBuilding = mainGate;
      else if (context.alarmLevel >= 50 && courthouse)
        targetBuilding = courthouse;
      else if (context.busterCallTimer <= 5 && ship)
        targetBuilding = ship;
      else if (!context.robinRescued)
        targetCharacter = firstCP9;
      else
      {
        if (bridge)
          targetBuilding = bridge;
        else
          targetCharacter = firstCP9;
      }
    }
  }
  else
  {
    if (name == "Fukurou" && character->getEnergy() >= 14)
    {
      for (int i = 0; i < strawHatCount; i++)
        if (strawHats[i]->isAlive() && strawHats[i]->getHP() == context.minStrawHatHP)
        {
          targetCharacter = strawHats[i];
          break;
        }
    }

    if (!targetCharacter)
    {
      for (int i = 0; i < strawHatCount; i++)
      {
        if (strawHats[i]->isAlive())
        {
          targetCharacter = strawHats[i];
          break;
        }
      }
    }
  }

  if (targetBuilding)
  {
    bool wasDestroyed = targetBuilding->isDestroyed();

    if (character->specialSkill(targetBuilding, context) == -1)
      character->attack(targetBuilding, context);

    if (!wasDestroyed && targetBuilding->isDestroyed())
      targetBuilding->onDestroyed(context);
  }
  else if (targetCharacter)
  {
    if (character->specialSkill(targetCharacter, context) == -1)
      character->attack(targetCharacter, context);
  }

  character->endTurn(context);
}

void EniesLobbyBattle::processBuildings()
{
  for (int i = 0; i < buildingCount; i++)
    buildings[i]->applyEffect(context);
}

void EniesLobbyBattle::checkEndCondition()
{
  if (context.battleEnded) return;

  if (context.robinRescued && context.escapeProgress >= 100)
  {
    context.battleEnded = true;
    context.resultCode = "STRAW_HAT_WIN";
    return;
  }

  if (context.busterCallTimer <= 0)
  {
    context.battleEnded = true;
    context.resultCode = "BUSTER_CALL";
    return;
  }

  bool allStrawHatsDead = true;
  for (int i = 0; i < strawHatCount; i++)
  {
    if (strawHats[i]->isAlive()) allStrawHatsDead = false;
  }

  if (allStrawHatsDead)
  {
    context.battleEnded = true;
    context.resultCode = "CP9_WIN";
    return;
  }

  bool allCP9Dead = true;
  for (int i = 0; i < cp9Count; i++)
  {
    if (cp9Agents[i]->isAlive()) allCP9Dead = false;
  }

  if (allCP9Dead)
  {
    context.battleEnded = true;
    context.resultCode = "STRAW_HAT_WIN_BY_DEFEAT_CP9";
    return;
  }

  if (context.turnCount >= maxTurns)
  {
    context.battleEnded = true;
    context.resultCode = "TIME_OUT";
  }
}

string EniesLobbyBattle::getResult() const
{
  stringstream ss;
  ss << context.resultCode << " "
     << context.turnCount << " "
     << context.morale << " "
     << context.alarmLevel << " "
     << context.rescueProgress << " "
     << context.escapeProgress << " "
     << context.busterCallTimer;
  return ss.str();
}
