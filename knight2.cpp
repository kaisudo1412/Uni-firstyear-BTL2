#include "knight2.h"

/* * * BEGIN implementation of class BaseBag * * */
bool Phoenix::canUse(BaseKnight* knight)
{
    int temphp = knight->gethp();
    int tempmax = knight->getmaxhp();

    if (temphp <= 0 && phoenixnum == 1) {
        return true;
    }
    else if (temphp < tempmax / 4 && phoenixnum == 2) {
        return true;
    }
    else if (temphp < tempmax / 3 && phoenixnum == 3) {
        return true;
    }
    else if (temphp < tempmax / 2 && phoenixnum == 4) {
        return true;
    }
    return false;
}

void Phoenix::use(BaseKnight* knight) {
    int temphp = knight->gethp();
    int tempmax = knight->getmaxhp();

    switch (phoenixnum)
    {
    case 1:
    case 2:
        knight->sethp(tempmax);
        break;
    case 3:
        if (temphp <= 0) {
            knight->sethp((tempmax / 3));
        }
        else {
            knight->sethp((tempmax / 4) + temphp);
        }
        break;
    case 4:
        if (temphp <= 0) {
            knight->sethp(tempmax / 2);
        }
        else {
            knight->sethp((tempmax / 5) + temphp);
        }
        break;
    default:
        break;
    }
}

BaseBag::BaseBag(int phoenixdown1, int antidote, int maxitem)
{
    this->maxitem = maxitem;

    for (int i = 0; i < phoenixdown1; i++) {
        BaseItem* phoenix = new Phoenix();
        this->insertFirst(phoenix);
    }

    for (int i = 0; i < antidote; i++) {
        BaseItem* anti = new Antidote();
        this->insertFirst(anti);
    }
}

bool BaseBag::insertFirst(BaseItem* item) {
    if (count == 0) {
        head = new BagNode(item);
        count++;
        return true;
    }
    else if (maxitem == 0 || count < maxitem) {
        BagNode* tempbag = new BagNode(item, head);
        head = tempbag;
        count++;
        return true;
    }
    else {
        return false;
    }
}

BaseItem* BaseBag::get(ItemType itemType, BaseKnight* bagknight) {
    BagNode* getitem;
    getitem = head;
    for (int i = 0; i < count; i++) {
        if (getitem->items->getitemtype() == itemType && getitem->items->canUse(bagknight)) {
            BaseItem* canuseitem = getitem->items;
            getitem->items = head->items;
            head->items = canuseitem;
            head = head->next;
            count--;
            return canuseitem;
        }
        else {
            getitem = getitem->next;
        }
    }
    return NULL;
}

bool BaseBag::canuseitem(ItemType itemType, BaseKnight* bagknight) {
    BaseItem* used = get(itemType, bagknight);
    if (used != NULL) {
        used->use(bagknight);
        return true;
    }
    return false;
}

void BaseBag::drop3headitems() {
    int countdrop = 0;
    while (count > 0 && countdrop < 3) {
        head = head->next;
        count--;
        countdrop++;
    }
}

string BaseBag::toString() const {
    string storestring = "Bag[count=" + to_string(count) + ';';
    BagNode* headlist{ head };
    while (headlist)
    {
        storestring += headlist->items->toString() + ',';
        headlist = headlist->next;
    }
    if (storestring[storestring.length() - 1] == ',') {
        storestring[storestring.length() - 1] = ']';
    }
    else storestring += ']';
    return storestring;
}
/* * * END implementation of class BaseBag * * */

/* * * BEGIN implementation of class BaseKnight * * */
string BaseKnight::toString() const {
    string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    // inefficient version, students can change these code
    //      but the format output must be the same
    string s("");
    s += "[Knight:id:" + to_string(id) 
        + ",hp:" + to_string(hp) 
        + ",maxhp:" + to_string(maxhp)
        + ",level:" + to_string(level)
        + ",gil:" + to_string(gil)
        + "," + bag->toString()
        + ",knight_type:" + typeString[knightType]
        + "]";
    return s;
}

void BaseKnight::whennohp() {
    if (bag->canuseitem(PHOENIX, this)) return;
    else if (this->hp <= 0 && this->gil >= 100)
    {
        this->hp = this->maxhp / 2;
        this->gil -= 100;
    }
}

void PaladinKnight::fight(BaseOpponent* oppo) {
    int monstype = oppo->getType();
    if (monstype <= 5) {
        winmonster(oppo);
    }
    else if (this->level >= oppo->getLevel()) {
        if (monstype == 6) wintornbery();
        else if (monstype == 7) winQC();
    }
    else if (this->level < oppo->getLevel()) {
        if (monstype == 6) losetornbery();
        else if (monstype == 7) loseQC();
    }
    else return;
}

void LancelotKnight::fight(BaseOpponent* oppo) {
    int monstype = oppo->getType();
    if (monstype <= 5) {
        winmonster(oppo);
    }
    else if (this->level >= oppo->getLevel()) {
        if (monstype == 6) wintornbery();
        else if (monstype == 7) winQC();
    }
    else if (this->level < oppo->getLevel()) {
        if (monstype == 6) losetornbery();
        else if (monstype == 7) loseQC();
    }
    else return;
}

void DragonKnight::fight(BaseOpponent* oppo) {
    int monstype = oppo->getType();

    if (this->level >= oppo->getLevel()) {
        if (monstype <= 5) {
            winmonster(oppo);
        }
        else if (monstype == 6) wintornbery();
        else if (monstype == 7) winQC();
    }
    else if (this->level < oppo->getLevel()) {
        if (monstype == 5) losemonster(oppo);

        else if (monstype == 7) loseQC();
    }
}

void NormalKnight::fight(BaseOpponent* oppo) {
    int monstype = oppo->getType();

    if (this->level >= oppo->getLevel()) {
        if (monstype <= 5) {
            winmonster(oppo);
        }
        else if (monstype == 6) wintornbery();
        else if (monstype == 7) winQC();
    }
    else if (this->level < oppo->getLevel()) {
        if (monstype == 6) losetornbery();
        else if (monstype == 5) losemonster(oppo);
        else if (monstype == 7) loseQC();
    }
}
/* * * END implementation of class BaseKnight * * */

/* * * BEGIN implementation of class ArmyKnights * * */
void ArmyKnights::printInfo() const {
    cout << "No. knights: " << this->count();
    if (this->count() > 0) {
        BaseKnight * lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
        << ";LancelotSpear:" << this->hasLancelotSpear()
        << ";GuinevereHair:" << this->hasGuinevereHair()
        << ";ExcaliburSword:" << this->hasExcaliburSword()
        << endl
        << string(50, '-') << endl;
}

bool ArmyKnights::fightminimons(int eventnum, int order) {

    BaseOpponent* monster = mons->createoppo(eventnum, order);

        lastKnight()->fight(monster);
        if (lastKnight()->gethp() <= 0) deleteknight(getinnum-1);
        else {
            int takegil = lastKnight()->getgil();

            if (takegil > 999) {
                lastKnight()->setgil(999);
                passgiltofront(takegil - 999, getinnum - 2);
            }
           
        }
    
    delete monster;
    if (getinnum <= 0) return false;
    return true;
}

bool ArmyKnights::fighttornbery(int eventnum, int order)
{
    BaseOpponent* torn = mons->createoppo(eventnum, order);
  
        lastKnight()->fight(torn);
        if (lastKnight()->gethp() <= 0) deleteknight(getinnum-1);

    delete torn;
    if (getinnum <= 0) return false;
    return true;
}

bool ArmyKnights::fightqc(int eventnum, int order) {

    BaseOpponent* qc = mons->createoppo(eventnum, order);

    lastKnight()->fight(qc);
    int takegil = lastKnight()->getgil();

    if (lastKnight()->getgil() > 999) {
        lastKnight()->setgil(999);
        passgiltofront(takegil - 999, getinnum - 2);
    }

    delete qc;
    return true;
}

void ArmyKnights::lostindurian() {

    lastKnight()->sethp(lastKnight()->getmaxhp());
}

void ArmyKnights::rings() {

    if (lastKnight()->getgil() > 50) {
        if (lastKnight()->gethp() < lastKnight()->getmaxhp() / 3) {
            int temphp = lastKnight()->gethp();
            int tempgil = lastKnight()->getgil();
            lastKnight()->setgil(tempgil - 50);
            lastKnight()->sethp(temphp + (lastKnight()->getmaxhp() / 5));
        }
    }
}

bool ArmyKnights::fightOW() {
    
        if (lastKnight()->getknighttype() == DRAGON || (lastKnight()->getlevel() == 10 && lastKnight()->gethp() == lastKnight()->getmaxhp())) {
            winOW();
        }
        else {
            loseOW();
            if (lastKnight()->gethp() <= 0) deleteknight(getinnum - 1);
        }
    
    if (getinnum <= 0) return false;
    return true;
}

bool ArmyKnights::fightHades() {
    
        if ((lastKnight()->getknighttype() == PALADIN && lastKnight()->getlevel() >= 8) || (lastKnight()->getlevel() == 10)) {
            shield = true;
        }
        else {
            loseHades();
            if (lastKnight()->gethp() <= 0) deleteknight(getinnum -1);
        }
    
    if (getinnum <= 0) return false;
    return true;
}

void ArmyKnights::take3items(int eventnum) {
    switch (eventnum) {
    case 95:
        shield = true;
    case 96:
        spear = true;
    case 97:
        sword = true;
    }
}

void ArmyKnights::takeexcalibur() {
    sword = spear && hair && shield;
};

void ArmyKnights::takephoenixdown(int type) {

    BaseItem* morephoenix = new Phoenix(type - 110);
    for (int i = getinnum - 1; i >= 0; --i)
    {
        if (anarmy[i]->getbag()->insertFirst(morephoenix)) return;
    }

    delete morephoenix;
};

bool ArmyKnights::fightboss() {

    int bosshp = 5000;

    if (sword) return true;
    else if (shield && spear && hair) {
        while (int i = getinnum - 1) {
            if (anarmy[i]->getknighttype() == DRAGON || anarmy[i]->getknighttype() == PALADIN || anarmy[i]->getknighttype() == LANCELOT) {
                bosshp -= anarmy[i]->gethp() * anarmy[i]->getlevel() * anarmy[i]->getKBD();
            }
            else {
                deleteknight(i);
            }

            if (bosshp > 0) {
                deleteknight(i);
            }
            else return true;
        }
    }
    return false;
};

bool ArmyKnights::journey(int order, int eventnum) {
    int countOW = 0;
    switch (eventnum)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (fightminimons(eventnum, order)) return true;
        return false;
    case 6:
        if (fighttornbery(eventnum, order)) return true;
        return false;
    case 7:
        return fightqc(eventnum, order);
    case 8:
        rings();
        return true;
    case 9:
        lostindurian();
        return true;
    case 10:
        if (fightOW()) return true;
        return false;
    case 11:
        if (fightHades()) return true;
        return false;
    case 95:
        if (shield = true) return true;
        take3items(eventnum);
        return true;
    case 96:
    case 97:
        take3items(eventnum);
        return true;
    case 98:
        takeexcalibur();
        return true;
    case 99:
        if (fightboss()) return true;
        return false;
    case 112:
    case 113:
    case 114:
        takephoenixdown(eventnum);
        return true;
    default:
        break;
    }
}


void ArmyKnights::printResult(bool win) const {
    cout << (win ? "WIN" : "LOSE") << endl;
}
void ArmyKnights::passgiltofront(int remain, int i) {
    int gils = 0;
    int gilsleft = remain;

    for (int k = i; k >= 0; k--) {
        gils = anarmy[k]->getgil();
        gils += gilsleft;

        if (gils > 999) {
            gilsleft = gils - 999;
            anarmy[k]->setgil(999);
        }
        else {
            anarmy[k]->setgil(gils);
            break;
        }

    }
}

void ArmyKnights::deleteknight(int at) {
    delete anarmy[at];
    anarmy[at] = NULL;
    getinnum--;
}

int ArmyKnights::count() const {
    return getinnum;
}

void KnightAdventure::loadArmyKnights(const string& filearmy)
{
    armyKnights = new ArmyKnights(filearmy);
}

ArmyKnights::ArmyKnights(const string& file_armyknights)
{

    ifstream getfile(file_armyknights);
    getfile >> getinnum;
    getfile.ignore();
    getinfoarmy = new string[getinnum];
    for (int i = 0; i < getinnum; i++) {
        countarmy++;
        getline(getfile, getinfoarmy[i]);
    }
    anarmy = new BaseKnight * [getinnum];
    for (int i = 0; i < getinnum; i++) {
        int phoenixdownI = 0;
        int idd = i + 1;
        id = idd;
        stringstream ss(getinfoarmy[i]);
        string word;
        int k = 0;
        while (ss >> word) {
            switch (k) {
            case 0:
                maxhp = stoi(word);
                k++;
                break;
            case 1:
                level = stoi(word);
                k++;
                break;
            case 2:
                phoenixdownI = stoi(word);
                k++;
                break;
            case 3:
                gil = stoi(word);
                k++;
                break;
            case 4:
                antidote = stoi(word);
                k++;
                break;
            default:
                break;
            }
        }
        anarmy[i] = BaseKnight::create(id, maxhp, level, gil, antidote, phoenixdownI);
    }
}


/* * * END implementation of class ArmyKnights * * */

/* * * BEGIN implementation of class KnightAdventure * * */
KnightAdventure::KnightAdventure() {
    armyKnights = nullptr;
    events = nullptr;
}

void KnightAdventure::loadEvents(const string& fileevent) {
    events = new Events(fileevent);
}

Events::Events(const string& fileevents)
{
    ifstream getnum(fileevents);
    int quanti;
    getnum >> quanti;
    numofevents = new int[quanti];
    for (int i = 0; i < quanti; i++) {
        getnum >> numofevents[i];
    }
}

int Events::get(int i) const {
    return this->numofevents[i];
}

Events::~Events() {
    delete[] this->numofevents;
    this->numofevents = NULL;
    quanti = 0;
}

int Events::countevent() const {
    return quanti;
}

void KnightAdventure::run()
{
    if (armyKnights && events)
    {
        bool final = armyKnights->adventure(events);
        armyKnights->printResult(final);
    }
}
/* * * END implementation of class KnightAdventure * * */


