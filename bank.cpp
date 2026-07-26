#include <string>
#include <iostream>
#include <sstream>

#include "game.h"
#include "player.h"
#include "font.h"
#include "display.h"

#include "misc.h"
#include "bank.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

// extern Player plyr;
// extern sf::RenderWindow App;

int bankNo;
int accountType;
int accountRef;
int currentAccount;
int findValue;

struct account
{
	float interest;
	float failProb;
	int minBalance;
	int failures;
};


struct bank
{
	string name;
	account accounts[3];
	int location; // match with location text description number
	int openingHour;
	int closingHour;
	int jobProbability;
	int gemCost;
	int	jewelCost;
};


//	name						accounts									loc		op	cl	job		gem		jewel
bank banks[3] =
{
	{"First City Bank",			0.5f,0.3f,1,0,0.9f,0.9f,1,0,2.6f,3.0f,1,0,		37,		0,	23,	51,		0,		0},
	{"Granite Bank",			0.5f,0.2f,1,0,1.0f,1.0f,1,0,2.7f,3.1f,1,0,		38,		11,	2,	51,		3000,	1},
	{"Gram's Gold Exchange",	0.6f,0.1f,1,0,1.3f,1.0f,1,0,3.2f,3.3f,1,0,		39,		0,	23,	46,		0,		0}
};


struct bankJobOpening
{
	int jobNumber;
	int jobHoursRequired;
	int jobHourlyIncome;
};


bankJobOpening bankJobOpenings[3];

struct bankJob
{
	string name;
	int minIncome;
	int maxIncome;
	string statRequirementName;
	int statRequirementValue;
	float fatigueRate;
	float minorWoundProbability;
	float majorWoundProbability;
};

bankJob bankJobs[3] =
{
	{"Guard",		30,	36,	"Strength",		15,		0.6875f,		15.95f,	3.97f},
	{"File Clerk",	50,	56,	"Intelligence",	20,		0.5625f,		2.29f,	0.05f},
	{"Coin Roller",	22,	28,	"Alignment",	144,	0.59375f,	2.29f,	0.05f }
};


inline std::string concat( int n, const char* str )
{
std::ostringstream ss;
ss << n;
ss << str;
return ss.str();
}

/*
string itos(int i)	// convert int to string
{
	stringstream s;
	s << i;
	return s.str();
}
*/


void shopBank()
{
	// Per-frame tick. bankMenu persists between calls via static.
	// Called every frame by checkShop() while plyr.status == 2.
	static int bankMenu = 0;
	static int workingHours = 0;
	static int hourlyRate = 0;
	static int jobIncome = 0;
	string str, key;

	// Entry: initialise on first call
	if (bankMenu == 0) {
		bankNo = getBankNo();
		if (bankNo==0) accountRef = 0;
		if (bankNo==1) accountRef = 3;
		if (bankNo==2) accountRef = 6;
		plyr.status = 2;
		loadShopImage(13);
		bankMenu = 1;
		return;
	}

	if (bankMenu == 1) {
		clearShopDisplay();
		cyText(0, "How may I be of service?");
		bText(2,2, "1) Open, or 2) Close an account");
		bText(2,3, "3) Make a deposit or 4) Withdrawl");
		bText(2,4, "5) Look at your balances");
		bText(2,5, "6) Examine Bank's books");
		bText(2,6, "7) Sell Gems or Jewelry");
		bText(2,7, "8) Apply for a job");
		bText(2,8, "0) Leave");
		SetFontColour(40,96,244,255);
		bText(2,2,"1"); bText(14,2,"2"); bText(2,3,"3"); bText(23,3,"4");
		bText(2,4,"5"); bText(2,5,"6"); bText(2,6,"7"); bText(2,7,"8"); bText(2,8,"0");
		SetFontColour(215,215,215,255);
		displayCoins(); updateDisplay();
		key = getSingleKey();
		if (key=="0")    bankMenu = -1;
		if (key=="1")    bankMenu = 2;
		if (key=="2")    bankMenu = 8;
		if (key=="3")    bankMenu = 16;
		if (key=="4")    bankMenu = 19;
		if (key=="5")    bankMenu = 15;
		if (key=="6")    bankMenu = 14;
		if (key=="7")    bankMenu = 21;
		if (key=="8")    bankMenu = 11;
		if (key=="down") bankMenu = -1;
	}
	else if (bankMenu == 2) {
		clearShopDisplay();
		cyText(0,"What account would you like to open?");
		str="(1) Low Risk Account    "; if (plyr.bankAccountStatuses[accountRef]==1)   str="(1) Low Risk Account    : Already open"; bText(2,2,str);
		str="(2) Medium Risk Account"; if (plyr.bankAccountStatuses[accountRef+1]==1) str="(2) Medium Risk Account : Already open"; bText(2,3,str);
		str="(3) High Risk Account";   if (plyr.bankAccountStatuses[accountRef+2]==1) str="(3) High Risk Account   : Already open"; bText(2,4,str);
		bText(2,6,"(0) Other banking options"); updateDisplay();
		key = getSingleKey();
		if (key=="1") { accountType=0; bankMenu=3; if (plyr.bankAccountStatuses[accountRef]==1)   bankMenu=4; }
		if (key=="2") { accountType=1; bankMenu=3; if (plyr.bankAccountStatuses[accountRef+1]==1) bankMenu=4; }
		if (key=="3") { accountType=2; bankMenu=3; if (plyr.bankAccountStatuses[accountRef+2]==1) bankMenu=4; }
		if (key=="0") bankMenu=1;
	}
	else if (bankMenu == 3) {
		clearShopDisplay();
		if (accountType==0) str="Bank Policy - Low Risk Account";
		if (accountType==1) str="Bank Policy - Medium Risk Account";
		if (accountType==2) str="Bank Policy - High Risk Account";
		bText(2,0,str);
		str="1. Average interest "+ftos(banks[bankNo].accounts[accountType].interest)+"% per day."; bText(2,3,str);
		str="2. Has a mean failure rate of"; bText(2,5,str);
		str="approximately "+ftos(banks[bankNo].accounts[accountType].failProb)+"% per day."; bText(5,6,str);
		updateDisplay();
		key=getSingleKey(); if (key=="SPACE") bankMenu=5;
	}
	else if (bankMenu == 4)  { clearShopDisplay(); cyText(2,"You already have one."); updateDisplay(); key=getSingleKey(); if(key=="SPACE") bankMenu=1; }
	else if (bankMenu == 5) {
		int coppers=inputValue("Deposit how much (in coppers)?",13);
		if (coppers>0) { if (checkCoins(0,0,coppers)) { currentAccount=accountRef+accountType; plyr.bankAccountStatuses[currentAccount]=1; deductCoins(0,0,coppers); plyr.bankAccountBalances[currentAccount]+=coppers; bankMenu=6; } else bankMenu=7; }
		if (coppers==0) bankMenu=1;
	}
	else if (bankMenu == 6)  { clearShopDisplay(); cyText(2,"We are glad to be of service."); updateDisplay(); key=getSingleKey(); if(key=="SPACE") bankMenu=1; }
	else if (bankMenu == 7)  { clearShopDisplay(); cyText(2,"You have insufficient funds."); updateDisplay(); key=getSingleKey(); if(key=="SPACE") bankMenu=1; }
	else if (bankMenu == 8) {
		clearShopDisplay(); cyText(0,"What account would you like to close?");
		str="(1) Low Risk Account    "; if (!plyr.bankAccountStatuses[accountRef])   str="(1) Low Risk Account    : Already closed"; bText(2,2,str);
		str="(2) Medium Risk Account"; if (!plyr.bankAccountStatuses[accountRef+1]) str="(2) Medium Risk Account : Already closed"; bText(2,3,str);
		str="(3) High Risk Account";   if (!plyr.bankAccountStatuses[accountRef+2]) str="(3) High Risk Account   : Already closed"; bText(2,4,str);
		bText(2,6,"(0) Other banking options"); updateDisplay();
		key=getSingleKey();
		if (key=="1") { accountType=0; bankMenu=9; if (!plyr.bankAccountStatuses[accountRef])   bankMenu=10; }
		if (key=="2") { accountType=1; bankMenu=9; if (!plyr.bankAccountStatuses[accountRef+1]) bankMenu=10; }
		if (key=="3") { accountType=2; bankMenu=9; if (!plyr.bankAccountStatuses[accountRef+2]) bankMenu=10; }
		if (key=="0") bankMenu=1;
	}
	else if (bankMenu == 9) {
		clearShopDisplay(); cyText(2,"This account is now closed."); updateDisplay();
		key=getSingleKey();
		if (key=="SPACE") { currentAccount=accountRef+accountType; plyr.bankAccountStatuses[currentAccount]=0; int c=plyr.bankAccountBalances[currentAccount]; plyr.bankAccountBalances[currentAccount]=0; withdrawCoppers(c); bankMenu=1; }
	}
	else if (bankMenu == 10) { clearShopDisplay(); cyText(2,"Already closed."); updateDisplay(); key=getSingleKey(); if(key=="SPACE") bankMenu=1; }
	else if (bankMenu == 11) {
		int jobNumber=bankJobOpenings[bankNo].jobNumber;
		clearShopDisplay();
		if (jobNumber==255) { bText(7,0,"I'm sorry but there are no"); cyText(2,"job openings at the moment."); cyText(9,"( Press a key )"); updateDisplay(); key=getSingleKey(); if(key!="") bankMenu=1; }
		else {
			str="We have an opening for a "+bankJobs[jobNumber].name; cyText(0,str);
			str="for "+itos(bankJobOpenings[bankNo].jobHoursRequired)+" hours at "+itos(bankJobOpenings[bankNo].jobHourlyIncome)+" coppers per hour."; cyText(1,str);
			cyText(3,"Would you like to apply?"); cyText(5,"( es or  o)"); SetFontColour(40,96,244,255); cyText(5," Y      N  "); SetFontColour(215,215,215,255); updateDisplay();
			key=getSingleKey(); if(key=="Y") bankMenu=12; if(key=="N") bankMenu=1;
		}
	}
	else if (bankMenu == 12) {
		int jn=bankJobOpenings[bankNo].jobNumber;
		string req=bankJobs[jn].statRequirementName; int reqVal=bankJobs[jn].statRequirementValue; bool met=false;
		if ((req=="Strength")     && (reqVal<=plyr.str))       met=true;
		if ((req=="Intelligence") && (reqVal<=plyr.inte))      met=true;
		if ((req=="Alignment")    && (reqVal<=plyr.alignment)) met=true;
		if (!met) { clearShopDisplay(); str="You will need more "+req; if(req=="Alignment") str="You are not righteous enough"; cyText(0,str); cyText(1,"to get the job."); cyText(9,"( Press a key )"); updateDisplay(); key=getSingleKey(); if(key=="SPACE") bankMenu=1; }
		else { workingHours=bankJobOpenings[bankNo].jobHoursRequired; hourlyRate=bankJobOpenings[bankNo].jobHourlyIncome; jobIncome=workingHours*hourlyRate; bankMenu=13; }
	}
	else if (bankMenu == 13) {
		if (workingHours>0) { clearShopDisplay(); cyText(2,"WORKING"); updateDisplay();
#ifdef __EMSCRIPTEN__
		emscripten_sleep(1000);
#else
		arx::sleep(arx::seconds(1));
#endif
		addHour(); workingHours--; }
		else { clearShopDisplay(); cyText(2,"The job is completed."); str="You have earned "+itos(jobIncome)+" coppers."; cyText(3,str); cyText(9,"( Press a key )"); updateDisplay(); key=getSingleKey(); if(key=="SPACE") { plyr.copper+=jobIncome; bankJobOpenings[bankNo].jobNumber=255; bankMenu=1; } }
	}
	else if (bankMenu == 14) { clearShopDisplay(); bText(2,0,"Account failures at our bank"); str="Low Risk: "+itos(banks[bankNo].accounts[0].failures); bText(2,2,str); str="Medium Risk: "+itos(banks[bankNo].accounts[1].failures); bText(2,4,str); str="High Risk: "+itos(banks[bankNo].accounts[2].failures); bText(2,6,str); cyText(9,"(Press SPACE to continue)"); updateDisplay(); key=getSingleKey(); if(key=="SPACE") bankMenu=1; }
	else if (bankMenu == 15) {
		clearShopDisplay(); bText(2,0,"Account Balances");
		str="Low Risk Account:    "+itos(plyr.bankAccountBalances[accountRef]);   if (!plyr.bankAccountStatuses[accountRef])   str="Low Risk Account:    Closed"; bText(2,2,str);
		str="Medium Risk Account: "+itos(plyr.bankAccountBalances[accountRef+1]); if (!plyr.bankAccountStatuses[accountRef+1]) str="Medium Risk Account: Closed"; bText(2,4,str);
		str="High Risk Account:   "+itos(plyr.bankAccountBalances[accountRef+2]); if (!plyr.bankAccountStatuses[accountRef+2]) str="High Risk Account:   Closed"; bText(2,6,str);
		cyText(9,"(Press SPACE to continue)"); updateDisplay(); key=getSingleKey(); if(key=="SPACE") bankMenu=1;
	}
	else if (bankMenu == 16) {
		clearShopDisplay(); cyText(0,"Deposit in which account?");
		str="(1) Low Risk Account    : "+itos(plyr.bankAccountBalances[accountRef]);   if (!plyr.bankAccountStatuses[accountRef])   str="(1) Low Risk Account    : No account"; bText(2,2,str);
		str="(2) Medium Risk Account : "+itos(plyr.bankAccountBalances[accountRef+1]); if (!plyr.bankAccountStatuses[accountRef+1]) str="(2) Medium Risk Account : No account"; bText(2,3,str);
		str="(3) High Risk Account   : "+itos(plyr.bankAccountBalances[accountRef+2]); if (!plyr.bankAccountStatuses[accountRef+2]) str="(3) High Risk Account   : No account"; bText(2,4,str);
		bText(2,6,"(0) Other banking options"); updateDisplay();
		key=getSingleKey();
		if (key=="1") { accountType=0; bankMenu=18; if (!plyr.bankAccountStatuses[accountRef])   bankMenu=17; }
		if (key=="2") { accountType=1; bankMenu=18; if (!plyr.bankAccountStatuses[accountRef+1]) bankMenu=17; }
		if (key=="3") { accountType=2; bankMenu=18; if (!plyr.bankAccountStatuses[accountRef+2]) bankMenu=17; }
		if (key=="0") bankMenu=1;
	}
	else if (bankMenu == 17) { clearShopDisplay(); cyText(2,"You don't have an account of that type."); updateDisplay(); key=getSingleKey(); if(key=="SPACE") bankMenu=1; }
	else if (bankMenu == 18) {
		int c=inputValue("Deposit how much (in coppers)?",13);
		if (c>0) { if (checkCoins(0,0,c)) { currentAccount=accountRef+accountType; plyr.bankAccountStatuses[currentAccount]=1; deductCoins(0,0,c); plyr.bankAccountBalances[currentAccount]+=c; bankMenu=6; } else bankMenu=7; }
		if (c==0) bankMenu=1;
	}
	else if (bankMenu == 19) {
		clearShopDisplay(); cyText(0,"Withdraw from which account?");
		str="(1) Low Risk Account    : "+itos(plyr.bankAccountBalances[accountRef]);   if (!plyr.bankAccountStatuses[accountRef])   str="(1) Low Risk Account    : No account"; bText(2,2,str);
		str="(2) Medium Risk Account : "+itos(plyr.bankAccountBalances[accountRef+1]); if (!plyr.bankAccountStatuses[accountRef+1]) str="(2) Medium Risk Account : No account"; bText(2,3,str);
		str="(3) High Risk Account   : "+itos(plyr.bankAccountBalances[accountRef+2]); if (!plyr.bankAccountStatuses[accountRef+2]) str="(3) High Risk Account   : No account"; bText(2,4,str);
		bText(2,6,"(0) Other banking options"); updateDisplay();
		key=getSingleKey();
		if (key=="1") { accountType=0; bankMenu=20; if (!plyr.bankAccountStatuses[accountRef])   bankMenu=17; }
		if (key=="2") { accountType=1; bankMenu=20; if (!plyr.bankAccountStatuses[accountRef+1]) bankMenu=17; }
		if (key=="3") { accountType=2; bankMenu=20; if (!plyr.bankAccountStatuses[accountRef+2]) bankMenu=17; }
		if (key=="0") bankMenu=1;
	}
	else if (bankMenu == 20) {
		int c=inputValue("Withdraw how much (in coppers)?",13);
		if (c>0) { currentAccount=accountRef+accountType; if (c<=plyr.bankAccountBalances[currentAccount]) { plyr.bankAccountBalances[currentAccount]-=c; withdrawCoppers(c); bankMenu=6; } else bankMenu=7; }
		if (c==0) bankMenu=1;
	}
	else if (bankMenu == 21) {
		clearShopDisplay(); cyText(0,"WELCOME FRIEND TO OUR"); cyText(1,"GEM AND JEWELRY APPRAISAL DEPARTMENT");
		bText(2,3,"Do you wish our experts to appraise your:"); bText(2,5,"(1) Fine gems"); bText(2,6,"(2) Magnificent jewelry"); updateDisplay();
		key=getSingleKey(); if(key=="1") { bankMenu=22; if(plyr.gems>0) bankMenu=26; } if(key=="2") { bankMenu=22; if(plyr.jewels>0) bankMenu=24; }
	}
	else if (bankMenu == 22) { clearShopDisplay(); bText(2,0,"The expert waits patiently..."); bText(2,2,"Then informs you that he will be"); bText(2,4,"glad to give you an appraisal"); bText(2,6,"when you have something to"); bText(2,8,"appraise."); updateDisplay(); key=getSingleKey(); if(key=="SPACE") bankMenu=1; }
	else if (bankMenu == 23) { clearShopDisplay(); bText(2,0,"The expert carefully examines"); bText(2,2,"your find and informs you that"); str="it is worth "+itos(findValue)+" in gold pieces."; bText(2,4,str); updateDisplay(); key=getSingleKey(); if(key=="SPACE") { plyr.gold+=findValue; findValue=0; bankMenu=1; } }
	else if (bankMenu == 24) { plyr.jewels--; int w=randn(0,20); if(w<2){findValue=0;bankMenu=25;}else{findValue=(randn(1,23))+16;bankMenu=23;} }
	else if (bankMenu == 25) { clearShopDisplay(); bText(2,0,"The expert carefully examines"); bText(2,2,"your find and informs you that"); bText(2,4,"it is completely worthless!"); updateDisplay(); key=getSingleKey(); if(key=="SPACE") bankMenu=1; }
	else if (bankMenu == 26) { plyr.gems--; int w=randn(0,20); if(w<2){findValue=0;bankMenu=25;}else{findValue=randn(1,13);bankMenu=23;} }

	if (bankMenu == -1) {
		bankMenu = 0; // reset for next visit
		leaveShop();
	}
}


void checkDailybankInterest()
{
	// Run at the start of each new day

	for (int i=0 ; i<9 ; i++) // 9 potential bank accounts
	{
		if (plyr.bankAccountBalances[i]>0)
		{
			float interest, failProb;
			if (i==0) { interest = banks[0].accounts[0].interest; failProb = banks[0].accounts[0].failProb; }
			if (i==1) { interest = banks[0].accounts[1].interest; failProb = banks[0].accounts[1].failProb; }
			if (i==2) { interest = banks[0].accounts[2].interest; failProb = banks[0].accounts[2].failProb; }
			if (i==3) { interest = banks[1].accounts[0].interest; failProb = banks[1].accounts[0].failProb; }
			if (i==4) { interest = banks[1].accounts[1].interest; failProb = banks[1].accounts[1].failProb; }
			if (i==5) { interest = banks[1].accounts[2].interest; failProb = banks[1].accounts[2].failProb; }
			if (i==6) { interest = banks[2].accounts[0].interest; failProb = banks[2].accounts[0].failProb; }
			if (i==7) { interest = banks[2].accounts[1].interest; failProb = banks[2].accounts[1].failProb; }
			if (i==8) { interest = banks[2].accounts[2].interest; failProb = banks[2].accounts[2].failProb; }

			float newInterest = ((float(plyr.bankAccountBalances[i]))/100)*interest;
			plyr.bankAccountBalances[i] += (int(newInterest));
		}
	}
}


void checkDailybankJobOpenings()
{
	// Run at the start of each new day
	int jobOpeningProbability = 0;
	for (int i=0 ; i<3 ; i++) // 3 banks in total
	{
		jobOpeningProbability  = randn(0,255);
		if (jobOpeningProbability <= banks[i].jobProbability)
		{
			// Create a new job entry for the day
			int newJobNumber = randn(0,2);
			bankJobOpenings[i].jobNumber = newJobNumber;
			bankJobOpenings[i].jobHoursRequired = randn(0,5)+3;
			bankJobOpenings[i].jobHourlyIncome = randn(bankJobs[newJobNumber].minIncome,bankJobs[newJobNumber].maxIncome);
		}
		else
		{
			// No job available today
			bankJobOpenings[i].jobNumber = 255; // 255 for none
		}

	}
}


void withdrawCoppers(int coppersToWithdraw)
{
	// Adds an amount of coppers to plyr in gold, silver and copper coins

	int remainder = 0;
	int goldFromWithdrawal = 0;
	int silverFromWithdrawal = 0;
	int copperFromWithdrawal = 0;
	remainder = coppersToWithdraw % 100;
	goldFromWithdrawal = (coppersToWithdraw - remainder) / 100;
	coppersToWithdraw -= (goldFromWithdrawal*100);
	if (remainder > 0)
	{
		remainder = coppersToWithdraw % 10;
		silverFromWithdrawal = (coppersToWithdraw - remainder) / 10;
		coppersToWithdraw -= (silverFromWithdrawal*10);
		if ( remainder > 0 ) { copperFromWithdrawal = remainder; }
	}

	plyr.copper += copperFromWithdrawal;
	plyr.gold += goldFromWithdrawal;
	plyr.silver += silverFromWithdrawal;
}


int getBankNo()
{
	int bank_no;
	for (int i=0 ; i<3 ; i++) // Max number of bank objects
	{
		if (banks[i].location == plyr.location)
		{
			bank_no = i; // The number of the bank you have entered
		}
	}
	return bank_no;
}
