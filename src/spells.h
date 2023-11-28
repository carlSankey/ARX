#ifndef _spellsh 
#define _spellsh 

const int SpellList = 77;

extern unsigned char spellItems[SpellList];




struct spellItem
{
    int no;
    //string name;
    int percentage;
};

struct spellRecord
{
    std::string name;
	int no;
	int spelltype;		//Type 0 = Attribute Change, 1 = inventory change, 2 = Buff, 3 = Damage
	int percentage;		//base
	int cost;			// ring cost
	int effect;			//0 = Blind,1 = paralyse......
	int elementtype;    //Hex value
	int stattype;       //Hex value		
	int negativeValue;	
	int positiveValue;	
	float duration;		//Duration
	int damage;			//Failure Damage
	int lessonsno;
	float lessonboost;
	int uses;
	int maxpercent;
	int guild;
};



void castSpellMessage(std::string spellDesc);
void spellBackfiredMessage(int spellPoints);
void castSpells();
void attemptSpell(int spellRef);
void castSpellAction(int spellRef);



#include <string>





extern spellRecord spells[150];
extern spellItem spellBuffer[76]; // learnt spells that can be cast

struct bonusDamage
{
	int no;
	int positiveValue;
	int negativeValue;
	int blunt;
	int sharp;
	int earth;
	int air;
	int fire;
	int water;
	int power;
	int magic;
	int good;
	int evil;
	int cold;
	int nature;
	int acid;

};




// Your existing header content goes here 
 
#endif // _spellsh 


// Blunt damage - 1 byte = 0000000000000000
// Sharp damage - 1 byte
// Earth damage - 1 byte
// Air damage - 1 byte
// Fire damage - 1 byte
// Water damage - 1 byte
// Power damage - 1 byte
// Magic damage - 1 byte
// Good damage - 1 byte
// Evil damage - 1 byte
// Cold damage - 1 byte
// RFE
// RFE
// RFE
// RFE
// RFE

