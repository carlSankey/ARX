// In a separate file, e.g., effects.h
#ifndef _effectsh 
#define _effectsh 

#include <string>

struct SearchResult {
    int index;
    bool found;
};



struct SearchArray {
    SearchResult* resultArray;
    int arrSize;
};
void Effect(int spellNo);


void updateStats(std::bitset<8> binaryStats, int positiveValue);
void updateHStats(std::bitset<15> binaryEffects, int positiveValue);
void updateInvuls(std::bitset<13> binaryElems, int positiveValue);
void applyEffect(int hour, int effectid);
void spellEffect(int spellId);

void repairAll();


void changeWeather(int WeatherType);

SearchResult findEffectByNoOrLastFree(int targetNo, int size);
SearchResult findEffectByNameOrLastFree(const std::string& targetName, int size);
SearchResult findEffectBySpellTypeOrLastFree(int targetType, int size);
SearchResult findActiveSpellNoLastFree(int targetNo, int size);
SearchArray findAllEffectByNameOrLastFree(int targetNo, int size);
SearchResult  findSpellByNameOrLastFree(const std::string& targetName, int size);

struct effectItem {
    std::string name;
    int effectNo;
    int negativeValue;
    int positiveValue;
    float duration;
};


struct effectIndex {
    int index;
    bool found;
};

struct spellSlot {
    int index;
    bool found;
};


extern effectItem effectBuffer[1000]; // 

extern effectIndex effectIndexloc;

extern spellSlot spellSlotloc;

#endif // _itemsh 