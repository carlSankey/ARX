#ifndef _smithyh 
#define _smithyh 


extern unsigned char citySmithyBinary[noOfCitySmithyFile];

void loadCitySmithyBinary();

void shopSmithy();
int getSmithyNo();
void stockSmithyWares();
void smithyDisplayUpdate();

int createCitySmithyInventoryItem(int startByte);
string readSmithyItemString(int stringOffset);

 
// Your existing header content goes here 
 
#endif // _smithyh 

