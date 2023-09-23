#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"
#include "knight2.cpp"

class BaseItem;
class Phoenix;
class Antidote;
class BaseKnight;
class Events;
class ArmyKnights;
// #define DEBUG


enum ItemType { ANTIDOTE = 0, PHOENIX };

struct BagNode {
    BaseItem* items;
    BagNode* next;
    BagNode(BaseItem *inp = nullptr, BagNode *nextptr = nullptr) {
        items = inp;
        next = nextptr;
    }
    ~BagNode();
};

class BaseBag {
private:
    BagNode* head = NULL;
    int count = 0;
    int maxitem;
public:
    BaseBag(int phoenixdown1, int antidote, int maxitem = 0);
    ~BaseBag() {
        while (head->next) {
            head = head->next;
        }
        head = NULL;
        count = 0;
    };

    virtual bool insertFirst(BaseItem* item);
    virtual BaseItem* get(ItemType itemType, BaseKnight* bagknight);
    virtual string toString() const;
    bool canuseitem(ItemType itemType, BaseKnight* bagknight);

    void drop3headitems();
};

class BaseOpponent {
protected:
    int type;
    int ordering;
    int level;
    int gil;
    int baseDamage;
    
public:
    BaseOpponent();
    int getType() {
        return type;
    }
    int getLevel() {
        return level;
    }
    int getGil() {
        return gil;
    }
    int getDame() {
        return baseDamage;
    }

    BaseOpponent* createoppo(int type, int order) {
        BaseOpponent* oppo = NULL;
        switch (type)
        {
        case 1:
            oppo = new MadBear(type, order);
            break;
        case 2:
            oppo = new Bandit(type, order);
            break;
        case 3:
            oppo = new LordLupin(type, order);
            break;
        case 4:
            oppo = new Elf(type, order);
            break;
        case 5:
            oppo = new Troll(type, order);
            break;
        case 6:
            oppo = new Tornbery(type, order);
            break;
        case 7:
            oppo = new QueenOfCards(type, order);
            break;
        }
        return oppo;
    }
};

class MadBear : public BaseOpponent {
public:
    MadBear(int type, int order) {
        this->type = type;
        this->level = (order + type) % 10 + 1;
        this->gil = 100;
        this->baseDamage = 10;
    }
};

class Bandit : public BaseOpponent {
public:
    Bandit(int type, int order) {
        this->type = type;
        this->level = (order + type) % 10 + 1;
        this->gil = 150;
        this->baseDamage = 15;
    }
};

class LordLupin : public BaseOpponent {
public:
    LordLupin(int type, int order) {
        this->type = type;
        this->level = (order + type) % 10 + 1;
        this->gil = 450;
        this->baseDamage = 45;
    }
};

class Elf : public BaseOpponent {
public:
    Elf(int type, int order) {
        this->type = type;
        this->level = (order + type) % 10 + 1;
        this->gil = 750;
        this->baseDamage = 75;
    }
};

class Troll : public BaseOpponent {
public:
    Troll(int type, int order) {
        this->type = type;
        this->level = (order + type) % 10 + 1;
        this->gil = 800;
        this->baseDamage = 95;
    }
};

class Tornbery : public BaseOpponent {
public:
    Tornbery(int type, int order) {
        this->level = (order + type) % 10 + 1;
    };
};

class QueenOfCards : public BaseOpponent {
public:
    QueenOfCards(int type, int order) {
        this->level = (order + type) % 10 + 1;
    }
};

enum KnightType { PALADIN = 0, LANCELOT = 1, DRAGON = 2, NORMAL = 3 };



class BaseKnight {
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    int antidote;
    KnightType knightType;
    double kBD;
    BaseBag* bag;
   
public:
    BaseKnight();
    ~BaseKnight();
    bool checkPaladin(int maxHP) {
        if (maxHP <= 1) return false;
        else if (maxHP == 2) return true;
        else {
            for (int i = 2; i <= sqrt(maxHP); i++) {
                if (maxHP % i == 0) return false;
            }
            return true;
        }
    }
    void fight(BaseOpponent* oppo);

    int getmaxhp() {
        return maxhp;
    }
    int gethp() {
        return hp;
    }
    void sethp(int newhp) {
        this->hp = newhp;
    }
    int getlevel() {
        return level;
    }
    void setlevel(int newlevel) {
        this->level = newlevel;
    }
    int getgil() {
        return gil;
    }
    void setgil(int gils) {
        this->gil = gils;
    }
    int getantidote() {
        return antidote;
    }
    void setantidote(int anti) {
        this->antidote = anti;
    }
    KnightType getknighttype() {
        return knightType;
    }
    void setknightType(KnightType type) {
        this->knightType = type;
    }
    BaseBag* getbag() {
        return bag;
    }
    double getKBD() {
        return kBD;
    }

    BaseKnight* create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
       BaseKnight* knight = NULL;
        if (maxhp == 888) {
            knight = new LancelotKnight(maxhp, level, gil, antidote, phoenixdownI);
        }
        else if (knight->checkPaladin(maxhp)) {
            knight = new PaladinKnight(maxhp, level, gil, antidote, phoenixdownI);
        }
        else if (maxhp == 345 || maxhp == 354 || maxhp == 435 || maxhp == 453 || maxhp == 534 || maxhp == 543) {
            knight = new DragonKnight(maxhp, level, gil, antidote, phoenixdownI);
        }
        else {
            knight = new NormalKnight(maxhp, level, gil, antidote, phoenixdownI);
        }
        return  knight;
    }

    string toString() const;
    void whennohp();

    void winmonster(BaseOpponent* oppo) {
        this->gil += oppo->getGil();
    }

    void losemonster(BaseOpponent* oppo) {
        this->hp -= oppo->getDame() * (oppo->getLevel() - this->level);
        whennohp();
    }
    void wintornbery() {
        this->level += 1;
        if (this->level >= 10) this->level = 10;
    }
    void losetornbery() {
        if (this->antidote > 0) {
            this->antidote--;
            bag->canuseitem(ANTIDOTE, this);
        }
        else {
            bag->drop3headitems();
            this->hp -= 10;
            whennohp();
        }
    }
    void winQC() {
        this->gil *= 2;
    }
    void loseQC() {
        this->gil /= 2;
    }
    void winOW() {
        this->level = 10;
        this->gil = 999;
    }
    void loseOW() {
        this->hp = 0;
        whennohp();
    }
    void loseHades() {
        this->hp = 0;
        whennohp();
    }
};

class PaladinKnight :public BaseKnight
{
public:
    PaladinKnight(int maxhp, int level, int gil, int antidote, int phoenixdownI) {
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        this->knightType = PALADIN;
        this->kBD = 0.06;
        bag = new BaseBag(phoenixdownI, antidote);
    }
    ~PaladinKnight() {
        delete bag;
        bag = NULL;
    }
    void fight(BaseOpponent* opponent);
};

class LancelotKnight :public BaseKnight
{
public:
    LancelotKnight(int maxhpla, int level, int gil, int antidote, int phoenixdownI) {
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        this->knightType = LANCELOT;
        this->kBD = 0.05;
        bag = new BaseBag(phoenixdownI, antidote, 16);
    }
    ~LancelotKnight() {
        delete bag;
        bag = NULL;
    }
    void fight(BaseOpponent* opponent);
};

class DragonKnight : public BaseKnight
{
public:
    DragonKnight(int maxhpdra, int level, int gil, int antidote, int phoenixdownI) {
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        this->knightType = DRAGON;
        this->kBD = 0.075;
        bag = new BaseBag(phoenixdownI, antidote, 14);
    }
    ~DragonKnight() {
        delete bag;
        bag = NULL;
    }
    void fight(BaseOpponent* opponent);
};

class NormalKnight : public BaseKnight
{
public:
    NormalKnight(int maxhpnor, int level, int gil, int antidote, int phoenixdownI) {
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        this->knightType = NORMAL;
        bag = new BaseBag(phoenixdownI, antidote, 19);
    }
    ~NormalKnight() {
        delete bag;
        bag = NULL;
    }
    void fight(BaseOpponent* opponent);
};

class ArmyKnights :public  BaseKnight {
protected:
    BaseKnight** anarmy;
    int getinnum;
    string* getinfoarmy;
    int countarmy = 0;
    bool shield = false;
    bool spear = false;
    bool hair = false;
    bool sword = false;
    BaseOpponent* mons;
public:
    ArmyKnights(const string& file_armyknights);
    ~ArmyKnights();

    int count() const;
    BaseKnight* lastKnight() const;
    void fight(BaseOpponent* opponent);

    bool adventure(Events* events) {
        int countpath = events->countevent();
        int countow = 0, counthades = 0;
        for (int i = 0; i < countpath; i++)
        {
            bool suc;
            int eventnum = events->get(i);
            if (eventnum == 10) countow++;
            else if (eventnum == 11) counthades++;

            if (getinnum == 0) {
                printInfo();
                return false;
            }
            if (eventnum == 10) {
                if (countow == 1)  suc = journey(i, eventnum);
            }
            else if (eventnum == 11) {
                if (counthades == 1) suc = journey(i, eventnum);
            }
            else {
                suc = journey(i, eventnum);
            }

            printInfo();
            if (!suc) return false;
        }
        return true;
    }

    bool hasPaladinShield() const { return shield; };
    bool hasLancelotSpear() const { return spear; };
    bool hasGuinevereHair() const { return hair; };
    bool hasExcaliburSword() const { return sword; };

    void printInfo() const;
    void printResult(bool result) const;

    bool journey(int i, int eventnum);
    void deleteknight(int at);
    void passgiltofront(int remain, int i);

    bool fightminimons(int type, int eventnum);
    bool fighttornbery(int type, int eventnum);
    bool fightqc(int type, int eventnum);
    void rings();
    void lostindurian();
    bool fightOW();
    bool fightHades();
    void take3items(int eventnum);
    void takeexcalibur();
    void takephoenixdown(int type);

    bool fightboss();

};

class BaseItem {
protected:
    ItemType itemt;
public:
    ItemType getitemtype() { return itemt; };
    virtual bool canUse(BaseKnight* knight) = 0;
    virtual void use(BaseKnight* knight) = 0;
    virtual string toString() = 0;
    ~BaseItem();
};

class Phoenix : public BaseItem {
private:
    int phoenixnum;
public:
    Phoenix(int num = 1) {
        this->itemt = PHOENIX;
        phoenixnum = num;
    }
    ~Phoenix();

    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);

    string toString() {
        string arr[4]{ "I", "II", "III", "IV" };
        return "ANTIDOTE" + arr[phoenixnum - 1];
    }
};

class Antidote : public BaseItem {
public:
    Antidote() {
        this->itemt = ANTIDOTE;
    }
    ~Antidote();

    bool canUse(BaseKnight* knight) {
        return true;
    }
    void use(BaseKnight* knight);

    string toString() {
        return "ANTIDOTE";
    }
};

class Events {
private:
    int* numofevents;
    int quanti;
public:
    Events(const string& file_events);
    ~Events();
    int countevent() const;
    int get(int i) const;
};

class KnightAdventure {
private:
    ArmyKnights* armyKnights;
    Events* events;

public:
    KnightAdventure();
    ~KnightAdventure(); // TODO:

    void loadArmyKnights(const string&);
    void loadEvents(const string&);
    void run();
};

#endif // __KNIGHT2_H__