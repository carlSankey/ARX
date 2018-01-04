

struct spellItem
{
    int no;
    //string name;
    int percentage;
};

struct spellRecord
{
    std::string name;
    int percentage;
	int cost;
	int effect;
	int negativeValue;
	int positiveValue;
	int duration;
	bool guilds[14];
};

void castSpellMessage(std::string spellDesc);
void spellBackfiredMessage(int spellPoints);
void castSpells();
void attemptSpell(int spellRef);

extern spellRecord spells[35];
extern spellItem spellBuffer[35]; // learnt spells that can be cast
