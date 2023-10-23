#ifndef _smithyh 
#define _smithyh 

const int citySmithyFileSize = 1691;
extern unsigned char citySmithyBinary[citySmithyFileSize];

void loadCitySmithyBinary();

void shopSmithy();
int getSmithyNo();
void stockSmithyWares();
void smithyDisplayUpdate();

int createCitySmithyInventoryItem(int startByte);
string readSmithyItemString(int stringOffset);

 
// Your existing header content goes here 
 
#endif // _smithyh 

