struct menuItem
{
	int                objRef;
	std::string        menuName;
	std::string        menuPrice;
};

extern menuItem menuItems[20];

void updateModule(int module);
void runModule(int module);
void displayModuleImage(int module);
int inputNumber(std::string message);
std::string inputText(std::string message);
int inputItemChoice(std::string message, int totalItems);
int calculateMaximumMenuPage(int numberOfItems);
void leaveModule();

