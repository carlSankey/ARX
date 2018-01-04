struct itemMenuEntry
{
	int                objRef;
	std::string        menuName;
};

extern itemMenuEntry itemSelectEntries[255];



int inputItemRef(std::string message);
int calculateLastMenuPage(int numberOfItems);

//int inputNumber(std::string message);
//void updateModule(int module);
//void runModule(int module);
//void displayModuleImage(int module);
//void leaveModule();

