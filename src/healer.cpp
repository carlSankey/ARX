
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>
#include <iostream>
#include <sstream>

#include "player.h"
#include "font.h"
#include "display.h"
#include "game.h"
#include "misc.h"
#include "healer.h"
#include "automap.h"

// extern Player plyr;
// extern sf::RenderWindow App;


// Base prices
int healPrice = 0;
int poisonsPrice = 0;
int diseasesPrice = 0;
int alcoholPrice = 0;
int clarityPrice = 0;
int diagnosePrice = 0;



void shopHealer()
{
	//int healerNo = gethealerNo();
	int healerMenu = 1; // high level menu
	string str,key;
	plyr.status = 2; // shopping

    // Both healers have the same base prices
    healerUpdatePrices();

	//loadShopImage(10);

	int healerNo = getHealerNo();
	if (healerNo==1) // One Way
	{
		setAutoMapFlag(plyr.map,28,34);
		setAutoMapFlag(plyr.map,29,34);
		setAutoMapFlag(plyr.map,30,34);
		setAutoMapFlag(plyr.map,28,35);
		setAutoMapFlag(plyr.map,29,35);
		setAutoMapFlag(plyr.map,30,35);
		setAutoMapFlag(plyr.map,28,36);
		setAutoMapFlag(plyr.map,29,36);
		setAutoMapFlag(plyr.map,30,36);
	}
	if (healerNo==2) // Alphe Omega
	{
		// TODO Add automap co-ordinates for 2nd healer!
		setAutoMapFlag(plyr.map, 32, 44);
		setAutoMapFlag(plyr.map, 31, 44);
		setAutoMapFlag(plyr.map, 33, 44);
		setAutoMapFlag(plyr.map, 31, 43);


	}


    // determine odd or even hour and if this healer is open
    bool oddHour = false;
    int h = plyr.hours;
    if  ((h==1)||(h==3)||(h==5)||(h==7)||(h==9)||(h==11)||(h==13)||(h==15)||(h==17)||(h==19)||(h==21)||(h==23)) oddHour = true;
    bool healerOpen = false;
    if ((healerNo==1) && (oddHour)) healerOpen = true;
    if ((healerNo==2) && (!oddHour)) healerOpen = true;

	if (healerOpen) { loadShopImage(15); healerMenu = 1; } else  { loadShopImage(21); healerMenu = 2; }


	while (healerMenu > 0)
	{
		while (healerMenu == 1) // main menu
		{
			clearShopDisplay();

			cyText (0, "Welcome Stranger!");
			cyText (1, "How may I serve you?");
			bText (11,3, "(1) Heal");
			bText (11,4, "(2) Cleanse");
			bText (11,5, "(3) Remove delusions");
			bText (11,6, "(4) Diagnose");
			bText (11,7, "(0) Leave");

			displayCoins();
			updateDisplay();

			key = getSingleKey();

			//if ( key=="1" ) { healerMenu = 2; }
			if ( key=="0" ) { healerMenu = 0; }
			if ( key=="down" ) { healerMenu = 0; }
			if  (key=="1" ) healerShopHealWounds();
			if  (key=="2" ) { healerMenu = 3; }
			if  (key=="3" ) { healerMenu = 4; }
			if  (key=="4" ) { healerMenu = 5; }
		}

		while (healerMenu == 2) // at bar, table or booth menu
		{
            moduleMessage("It looks as though the@@Healer is not here.");
			healerMenu = 0;


		}

		while (healerMenu == 3) // Cleanse menu
		{
			clearShopDisplay();

			cyText (1, "Remove which illnesses?");
			bText (1,3, "(1) Cleanse Poisons");
			bText (1,4, "(2) Cure Diseases");
			bText (1,5, "(3) Remove Alcohol");
			bText (1,6, "(0) None");

            bText (24,3,itos(poisonsPrice)+" coppers");
            bText (24,4,itos(diseasesPrice)+" coppers");
			bText (24,5,itos(alcoholPrice)+" coppers");

			displayCoins();
			updateDisplay();

			key = getSingleKey();

			//if ( key=="1" ) { healerMenu = 2; }
			if ( key=="0" ) { healerMenu = 1; }
            if ( key=="2" )
            {
                if (!checkCoins(0,0,diseasesPrice))
                {
                    moduleMessage("I'm sorry... You have not the funds.");
                }
                else
                {
                    moduleMessage("IT SHALL BE DONE!");
                    deductCoins(0,0,diseasesPrice);
                    plyr.diseases[0] = 0;
                    plyr.diseases[1] = 0;
                    plyr.diseases[2] = 0;
                    plyr.diseases[3] = 0;
                    // CLEAR ANY RELATED EFFECTS
                    healerUpdateLastServiceTime();
                    healerUpdatePrices();
                }
                healerMenu = 1;
            }
            if ( key=="1" )
            {
                if (!checkCoins(0,0,poisonsPrice))
                {
                    moduleMessage("I'm sorry... You have not the funds.");
                }
                else
                {
                    moduleMessage("IT SHALL BE DONE!");
                    deductCoins(0,0,poisonsPrice);
                    // CLEAR ANY RELATED EFFECTS
                    plyr.poison[0] = 0;
                    plyr.poison[1] = 0;
                    plyr.poison[2] = 0;
                    plyr.poison[3] = 0;
                    healerUpdateLastServiceTime();
                    healerUpdatePrices();
                }
                healerMenu = 1;
            }
            if ( key=="3" )
            {
                if (!checkCoins(0,0,alcoholPrice))
                {
                    moduleMessage("I'm sorry... You have not the funds.");
                }
                else
                {
                    moduleMessage("IT SHALL BE DONE!");
                    deductCoins(0,0,alcoholPrice);
                    plyr.alcohol = 0;
                    healerUpdateLastServiceTime();
                    healerUpdatePrices();
                }
                healerMenu = 1;
            }
		}

        while (healerMenu == 4) // Remove delusions
		{
			clearShopDisplay();

			cyText (1, "Shall I remove your delusions");
            cyText (2, "for " + itos(clarityPrice) + " coppers?");
            cyText (5, "(Yes or No)");
			displayCoins();
			updateDisplay();

			key = getSingleKey();

			if ( key=="N" ) { healerMenu = 1; }
			if ( key=="Y" )
            {
                if (!checkCoins(0,0,clarityPrice))
                {
                    moduleMessage("I'm sorry... You have not the funds.");
                }
                else
                {
                    moduleMessage("IT SHALL BE DONE!");
                    deductCoins(0,0,clarityPrice);
                    plyr.delusion = 0;
                    healerUpdateLastServiceTime();
                    healerUpdatePrices();
                }
                healerMenu = 1;
            }
		}

        while (healerMenu == 5) // Diagnose
		{
			clearShopDisplay();

			cyText (1, "Shall I check for and diagnose disease");
            cyText (2, "for " + itos(diagnosePrice) + " coppers?");
            cyText (5, "(Yes or No)");
			displayCoins();
			updateDisplay();

			key = getSingleKey();

			if ( key=="N" ) { healerMenu = 1; }
			if ( key=="Y" )
            {
                if (!checkCoins(0,0,diagnosePrice))
                {
                    moduleMessage("I'm sorry... You have not the funds.");
                }
                else
                {
                    moduleMessage("IT SHALL BE DONE!");
                    deductCoins(0,0,diagnosePrice);
                    // Make diseases visible
                    if ((plyr.diseases[0] > 0) && (plyr.diseases[0] < 15)) { plyr.diseases[0] = 15; }
                    healerUpdateLastServiceTime();
                    healerUpdatePrices();
                }
                healerMenu = 1;
            }
		}
	}
	leaveShop();
}


void healerUpdatePrices()
{
    int days,hours, minutes;
    int healerNo = getHealerNo();
    healerNo--;
    days = plyr.healerDays[healerNo];
    hours = plyr.healerHours[healerNo];
    minutes = plyr.healerMinutes[healerNo];
    int n = 1;
    if (days==plyr.days) n = 2;
    if ((days==plyr.days) && (hours==plyr.hours)) n = 4;
    if ((days==plyr.days) && (hours==plyr.hours) && (minutes==plyr.minutes)) n = 8;
    if ((days==0) && (hours==0) && (minutes==0)) n = 1; // First time visitor

    healPrice = 10 * n;
    poisonsPrice = 100 * n;
    diseasesPrice = 200 * n;
    alcoholPrice = 20 * n;
    clarityPrice = 200 * n;
    diagnosePrice = 10 * n;
}

void healerUpdateLastServiceTime()
{
    int healerNo = getHealerNo();
    healerNo--;
    plyr.healerDays[healerNo] = plyr.days;
    plyr.healerHours[healerNo] = plyr.hours;
    plyr.healerMinutes[healerNo] = plyr.minutes;
}

void healerShopHealWounds()
{
    string str = "Cure how many hits at@@ " + itos(healPrice) + " coppers each?";
	int hpToHealInput = inputValue(str, 14);
	bool keynotpressed = true;
    int hpToHeal = hpToHealInput * healPrice;
	if (hpToHealInput>0)
	{
	    string key;

		if (!checkCoins(0,0,hpToHeal))
		{
			// Insufficient funds
			moduleMessage("I'm sorry... You have not the funds.");
		}
		else
		{
		    // Sufficient funds
            moduleMessage("IT SHALL BE DONE!");
            deductCoins(0,0,hpToHeal);
            plyr.hp = plyr.hp + hpToHealInput;
            if (plyr.hp > plyr.maxhp) { plyr.hp = plyr.maxhp; }
            healerUpdateLastServiceTime();
            healerUpdatePrices();
        }
	}
}




int getHealerNo()
{
	int healer_no;
	if (plyr.location==74) healer_no = 1; // One Way Soothers
	if (plyr.location==75) healer_no = 2; // Alpha Omega Healers
	return healer_no;
}
