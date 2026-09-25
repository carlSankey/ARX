$c = Get-Content tavern.cpp -Raw -Encoding UTF8

$newFunc = @'
void shopTavern()
{
	static int tavernMenu = 0;
	static int tavernLoc = 0;
	static bool musicPlaying = false;
	static int workingHours = 0;
	static int hourlyRate = 0;
	static int jobIncome = 0;
	string str, key;

	// Entry
	if (tavernMenu == 0) {
		tavernNo = getTavernNo();
		loadShopImage(10);
		if (tavernNo==3) { setAutoMapFlag(plyr.map,7,39); setAutoMapFlag(plyr.map,8,39); setAutoMapFlag(plyr.map,9,39); setAutoMapFlag(plyr.map,10,39); }
		if (tavernNo==6) { setAutoMapFlag(plyr.map,32,44); setAutoMapFlag(plyr.map,31,44); setAutoMapFlag(plyr.map,33,44); setAutoMapFlag(plyr.map,31,43); }
		if (tavernNo==9) { setAutoMapFlag(plyr.map,39,34); setAutoMapFlag(plyr.map,39,33); }
		if (tavernNo==12) { setAutoMapFlag(plyr.map,5,28); setAutoMapFlag(plyr.map,6,28); }
		descriptionPointer=0; eatDrinkDescriptions[0]=""; eatDrinkDescriptions[1]=""; eatDrinkDescriptions[2]=""; eatDrinkDescriptions[3]="";
		tavernMusic.setLooping(false); musicPlaying=false; tavernLoc=0;
		plyr.status=2;
		// Check closed
		bool closed = false;
		if (Taverns[tavernNo].closingHour < Taverns[tavernNo].openingHour) {
			if (Taverns[tavernNo].openingHour > plyr.hours) closed=true;
			if (Taverns[tavernNo].closingHour < plyr.hours && plyr.hours < Taverns[tavernNo].openingHour) closed=true;
		} else {
			if ((Taverns[tavernNo].closingHour<=plyr.hours)||(Taverns[tavernNo].openingHour>plyr.hours)) closed=true;
		}
		if (closed) tavernMenu=20;
		else if (Taverns[tavernNo].membershipFee > 0) tavernMenu=21;
		else tavernMenu=1;
		return;
	}

	if (tavernMenu == 20) // closed
	{
		int openHour=Taverns[tavernNo].openingHour, closeHour=Taverns[tavernNo].closingHour;
		if (openHour<12) openingText="morning"; else if (openHour<18) openingText="afternoon"; else openingText="evening";
		if (closeHour<12) closingText="morning"; else if (closeHour<18) closingText="afternoon"; else closingText="evening";
		tavernDisplayUpdate();
		cyText(1,"Sorry, we are closed. Come back@during our working hours.");
		str="We are open from "+itos(Taverns[tavernNo].openingHour)+":00 in the "+openingText+"@to "+itos(Taverns[tavernNo].closingHour)+":00 in the "+closingText+".";
		cyText(4,str); cyText(9,"( Press a key )"); updateDisplay();
		key=getSingleKey(); if (key=="SPACE") tavernMenu=-1;
	}
	else if (tavernMenu == 21) // membership check
	{
		tavernDisplayUpdate();
		cyText(1,"To enter you must become a member.");
		str="Dues are "+toCurrency(Taverns[tavernNo].membershipFee)+" copper coins."; cyText(3,str);
		cyText(5,"( es or  o)"); SetFontColour(40,96,244,255); cyText(5," Y      N  "); SetFontColour(215,215,215,255);
		displayCoins(); updateDisplay();
		key=getSingleKey();
		if (key=="N") tavernMenu=-1;
		if (key=="Y") {
			if (!checkCoins(0,0,Taverns[tavernNo].membershipFee)) tavernMenu=23;
			else { deductCoins(0,0,Taverns[tavernNo].membershipFee); Taverns[tavernNo].membershipFee=0; tavernMenu=1; }
		}
	}
	else if (tavernMenu == 23) // insufficient for membership
	{
		tavernDisplayUpdate(); cyText(3,"I'm sorry... You have not the funds."); updateDisplay();
		key=getSingleKey(); if (key=="SPACE") tavernMenu=-1;
	}
	else if (tavernMenu == 1) // main menu
	{
		int cc=(plyr.gold*100)+(plyr.silver*10)+plyr.copper;
		if ((plyr.tavernFriendships[tavernNo]>=4)&&(plyr.water==0)&&(cc==0)&&(plyr.thirst>56)) { plyr.thirst=0; plyr.water=1; tavernMessage("Friend, you thirst. Let me get you a drink."); }
		if ((plyr.tavernFriendships[tavernNo]>=4)&&(plyr.food==0)&&(cc==0)&&(plyr.hunger>96))  { plyr.hunger=0; plyr.food=1;  tavernMessage("Friend, you hunger. Let me get you some food."); }
		tavernDisplayUpdate();
		bText(7,0,"Welcome Stranger! You are at "); bText(9,3," ) Go to the bar"); bText(9,4," ) Get a table"); bText(9,5," ) Sit in a private booth"); bText(9,6," ) Apply for a job"); bText(9,7," ) Leave");
		SetFontColour(40,96,244,255); bText(9,3,"1"); bText(9,4,"2"); bText(9,5,"3"); bText(9,6,"4"); bText(9,7,"0"); SetFontColour(215,215,215,255);
		displayCoins();
		if (!musicPlaying) {
			int R=randn(1,5);
			if (plyr.musicStyle==0) {
				if (R==1){tavernMusic.openFromFile("data/audio/dwarfdance.ogg");lyricsFilename="dwarfdance.txt";}
				if (R==2){tavernMusic.openFromFile("data/audio/thoreandan.ogg");lyricsFilename="thoreandan.txt";}
				if (R==3){tavernMusic.openFromFile("data/audio/waves.ogg");lyricsFilename="waves.txt";}
				if (R==4){tavernMusic.openFromFile("data/audio/moments.ogg");lyricsFilename="moments.txt";}
				if (R==5){tavernMusic.openFromFile("data/audio/B/TheNightstalker.ogg");lyricsFilename="TheNightstalker.txt";}
			} else {
				if (R==1){tavernMusic.openFromFile("data/audio/B/dwarfdance.ogg");lyricsFilename="dwarfdance.txt";}
				if (R==2){tavernMusic.openFromFile("data/audio/B/thoreandan.ogg");lyricsFilename="thoreandan.txt";}
				if (R==3){tavernMusic.openFromFile("data/audio/B/waves.ogg");lyricsFilename="waves.txt";}
				if (R==4){tavernMusic.openFromFile("data/audio/B/LetInTheLight.ogg");lyricsFilename="LetInTheLight.txt";}
				if (R==5){tavernMusic.openFromFile("data/audio/B/TheNightstalker.ogg");lyricsFilename="TheNightstalker.txt";}
			}
			loadLyrics(lyricsFilename); tavernMusic.play(); musicPlaying=true;
		}
		updateDisplay();
		key=getSingleKey();
		if (key=="1"){tavernMenu=2;tavernLoc=1;} if (key=="2"){tavernMenu=2;tavernLoc=2;} if (key=="3"){tavernMenu=2;tavernLoc=3;}
		if (key=="4") tavernMenu=11; if (key=="0"||key=="down") tavernMenu=-1;
		if (key=="F1") { tavernMusic.stop(); loadLyrics(lyricsFilename); tavernMusic.play(); }
	}
	else if (tavernMenu == 2) // bar/table/booth
	{
		tavernDisplayUpdate();
		if (tavernLoc==1){bText(7,0,"You are sitting at the bar.");bText(23,3,"A few nuts");}
		if (tavernLoc==2){bText(7,0,"You are at your table.");bText(26,3,"Popcorn");}
		if (tavernLoc==3){bText(7,0,"You are in a private booth.");bText(7,3,"A smokey torch  A few nuts");tavernMusic.stop();musicPlaying=false;}
		bText(7,4," ) Hail the Barkeeper"); bText(7,5," ) Hail the Waitress"); bText(7,6," ) Buy a round for the house"); bText(7,7," ) Leave");
		SetFontColour(40,96,244,255); bText(7,4,"1"); bText(7,5,"2"); bText(7,6,"3"); bText(7,7,"0"); SetFontColour(215,215,215,255);
		displayCoins();
		SetFontColour(208,178,2,255); bText(7,1,eatDrinkDescriptions[0]); bText(23,1,eatDrinkDescriptions[1]); bText(7,2,eatDrinkDescriptions[2]); bText(23,2,eatDrinkDescriptions[3]); SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if (key=="0") tavernMenu=-1; if (key=="1") tavernMenu=3; if (key=="2") tavernMenu=7; if (key=="3") tavernMenu=30;
	}
	else if (tavernMenu == 3) // order drink
	{
		tavernDisplayUpdate(); cyText(0,"What would you like? (  to go back)"); SetFontColour(40,96,244,255); cyText(0,"                      0            "); SetFontColour(215,215,215,255);
		tavernNo=getTavernNo();
		for (int i=0;i<6;i++){int n=tavernDailyDrinks[tavernNo][i]; str=") "+tavernDrinks[n].name; bText(3,2+i,str); bText(1,2+i,"                                 coppers");}
		displayCoins();
		for (int i=0;i<6;i++){int n=tavernDailyDrinks[tavernNo][i]; int c2=static_cast<int>(Taverns[tavernNo].priceFactor*tavernDrinks[n].basePrice); int x=33; if(c2<10)x=34; if(c2>9&&c2<100)x=32; if(c2<1000)x=30; bText(x,i+2,toCurrency(c2));}
		SetFontColour(40,96,244,255); bText(2,2,"1");bText(2,3,"2");bText(2,4,"3");bText(2,5,"4");bText(2,6,"5");bText(2,7,"6"); SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if (key=="1"){drinkChoice=0;tavernMenu=4;} if (key=="2"){drinkChoice=1;tavernMenu=4;} if (key=="3"){drinkChoice=2;tavernMenu=4;}
		if (key=="4"){drinkChoice=3;tavernMenu=4;} if (key=="5"){drinkChoice=4;tavernMenu=4;} if (key=="6"){drinkChoice=5;tavernMenu=4;}
		if (key=="ESC"||key=="0") tavernMenu=2;
	}
	else if (tavernMenu == 4) // attempt buy drink
	{
		tavernNo=getTavernNo(); drinkNo=tavernDailyDrinks[tavernNo][drinkChoice];
		drinkCost=static_cast<int>(Taverns[tavernNo].priceFactor*tavernDrinks[drinkNo].basePrice);
		tavernMenu = checkCoins(0,0,drinkCost) ? 6 : 5;
	}
	else if (tavernMenu == 5) // insufficient funds for drink
	{
		tavernDisplayUpdate(); cyText(3,"I'm sorry... You have not the funds."); updateDisplay();
		key=getSingleKey(); if (key=="SPACE") tavernMenu=3;
	}
	else if (tavernMenu == 6) // successful drink purchase
	{
		tavernDisplayUpdate(); cyText(3,"Right away!"); updateDisplay();
		key=getSingleKey();
		if (key=="SPACE") {
			deductCoins(0,0,drinkCost); plyr.thirst-=tavernDrinks[drinkNo].thirstRemoved; if(plyr.thirst<0)plyr.thirst=0;
			plyr.alcohol+=tavernDrinks[drinkNo].alcoholAdded; plyr.water+=tavernDrinks[drinkNo].waterFlaskAdded;
			eatDrinkDescriptions[descriptionPointer]=tavernDrinks[drinkNo].name;
			if(descriptionPointer==3)descriptionPointer=0; else descriptionPointer++;
			tavernMenu=2;
		}
	}
	else if (tavernMenu == 7) // order food
	{
		tavernDisplayUpdate(); cyText(0,"What would you like? (  to go back)"); SetFontColour(40,96,244,255); cyText(0,"                      0            "); SetFontColour(215,215,215,255);
		tavernNo=getTavernNo();
		for (int i=0;i<6;i++){int n=tavernDailyFoods[tavernNo][i]; str=") "+tavernFoods[n].name; bText(3,2+i,str); bText(1,2+i,"                                 coppers");}
		displayCoins();
		for (int i=0;i<6;i++){int n=tavernDailyFoods[tavernNo][i]; int c2=static_cast<int>(Taverns[tavernNo].priceFactor*tavernFoods[n].basePrice); int x=33; if(c2<10)x=37; if(c2>9&&c2<100)x=34; if(c2<1000)x=30; bText(x,i+2,toCurrency(c2));}
		SetFontColour(40,96,244,255); bText(2,2,"1");bText(2,3,"2");bText(2,4,"3");bText(2,5,"4");bText(2,6,"5");bText(2,7,"6"); SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if (key=="1"){foodChoice=0;tavernMenu=8;} if (key=="2"){foodChoice=1;tavernMenu=8;} if (key=="3"){foodChoice=2;tavernMenu=8;}
		if (key=="4"){foodChoice=3;tavernMenu=8;} if (key=="5"){foodChoice=4;tavernMenu=8;} if (key=="6"){foodChoice=5;tavernMenu=8;}
		if (key=="ESC"||key=="0") tavernMenu=2;
	}
	else if (tavernMenu == 8) // attempt buy food
	{
		tavernNo=getTavernNo(); foodNo=tavernDailyFoods[tavernNo][foodChoice];
		foodCost=static_cast<int>(Taverns[tavernNo].priceFactor*tavernFoods[foodNo].basePrice);
		tavernMenu = checkCoins(0,0,foodCost) ? 10 : 9;
	}
	else if (tavernMenu == 9) // insufficient funds for food
	{
		tavernDisplayUpdate(); cyText(3,"I'm sorry... You have not the funds."); updateDisplay();
		key=getSingleKey(); if (key=="SPACE") tavernMenu=7;
	}
	else if (tavernMenu == 10) // successful food purchase
	{
		tavernDisplayUpdate(); cyText(3,"Right away!"); updateDisplay();
		key=getSingleKey();
		if (key=="SPACE") {
			deductCoins(0,0,foodCost); plyr.hunger-=tavernFoods[foodNo].hungerRemoved; if(plyr.hunger<0)plyr.hunger=0;
			plyr.digestion+=tavernFoods[foodNo].hungerRemoved*2; plyr.food+=tavernFoods[foodNo].foodPacketAdded;
			eatDrinkDescriptions[descriptionPointer]=tavernFoods[foodNo].name;
			if(descriptionPointer==3)descriptionPointer=0; else descriptionPointer++;
			tavernMenu=2;
		}
	}
	else if (tavernMenu == 11) // apply for job
	{
		int jn=tavernJobOpenings[tavernNo].jobNumber;
		tavernDisplayUpdate();
		if (jn==255) { bText(7,0,"I'm sorry but there are no"); cyText(1,"job openings at the moment."); cyText(9,"( Press a key )"); updateDisplay(); key=getSingleKey(); if(key!="") tavernMenu=1; }
		else {
			str="We have an opening for a "+tavernJobs[jn].name; cyText(0,str);
			str="for "+itos(tavernJobOpenings[tavernNo].JobHoursRequired)+" hours at "+itos(tavernJobOpenings[tavernNo].jobHourlyIncome)+" coppers per hour."; cyText(1,str);
			cyText(3,"Would you like to apply?"); cyText(5,"( es or  o)"); SetFontColour(40,96,244,255); cyText(5," Y      N  "); SetFontColour(215,215,215,255); updateDisplay();
			key=getSingleKey(); if(key=="Y") tavernMenu=12; if(key=="N") tavernMenu=1;
		}
	}
	else if (tavernMenu == 12) // check job requirements
	{
		int jn=tavernJobOpenings[tavernNo].jobNumber;
		string req=tavernJobs[jn].statRequirementName; int rv=tavernJobs[jn].statRequirementValue; bool met=false;
		if ((req=="Strength")&&(rv<=plyr.str)) met=true; if ((req=="Charm")&&(rv<=plyr.chr)) met=true; if ((req=="Skill")&&(rv<=plyr.skl)) met=true;
		if (!met) { tavernDisplayUpdate(); str="You will need more "+req; cyText(0,str); cyText(1,"to get the job."); cyText(9,"( Press a key )"); updateDisplay(); key=getSingleKey(); if(key=="SPACE") tavernMenu=1; }
		else { workingHours=tavernJobOpenings[tavernNo].JobHoursRequired; hourlyRate=tavernJobOpenings[tavernNo].jobHourlyIncome; jobIncome=workingHours*hourlyRate; tavernMenu=13; }
	}
	else if (tavernMenu == 13) // working
	{
		if (workingHours>0) { tavernDisplayUpdate(); cyText(2,"WORKING"); updateDisplay(); sf::sleep(sf::seconds(1)); addHour(); workingHours--; }
		else { tavernDisplayUpdate(); cyText(2,"The job is completed."); str="You have earned "+itos(jobIncome)+" coppers."; cyText(3,str); cyText(9,"( Press a key )"); updateDisplay(); key=getSingleKey(); if(key=="SPACE"){plyr.copper+=jobIncome;tavernJobOpenings[tavernNo].jobNumber=255;tavernMenu=1;} }
	}
	else if (tavernMenu == 30) // buy a round
	{
		int rc=static_cast<int>(80*Taverns[tavernNo].priceFactor);
		tavernDisplayUpdate(); str="A round for the house will cost@@"+itos(rc)+" coppers.@@@Dost thou still wish to buy? (Y or N)"; cyText(0,str); updateDisplay();
		key=getSingleKey();
		if (key=="Y") { if(checkCoins(0,0,rc)){deductCoins(0,0,rc);plyr.tavernFriendships[tavernNo]+=1;tavernMenu=31;}else{plyr.tavernFriendships[tavernNo]-=1;tavernMessage("I'm sorry you have not the funds.");tavernMenu=2;} }
		if (key=="N") tavernMenu=2;
	}
	else if (tavernMenu == 31) // round bought message
	{
		int tf=plyr.tavernFriendships[tavernNo]; str="The patrons go up to the bar.";
		if(tf>=0&&tf<3)str="A few people take up your offer.";
		if(tf>3&&tf<6)str="You have won yourself some friends, Adventurer.";
		if(tf>5&&tf<7)str="All the patrons applaud your generosity!";
		if(tf>6)str="A toast to our friend, "+plyr.name+".";
		tavernMessage(str); tavernMenu=2;
	}

	if (tavernMenu == -1) {
		tavernMenu=0;
		if (musicPlaying) { tavernMusic.stop(); musicPlaying=false; }
		leaveShop();
	}
}

'@

$si = $c.IndexOf("void shopTavern()")
$ei = $c.IndexOf("void tavernDisplayUpdate()")
$c = $c.Substring(0,$si) + $newFunc + $c.Substring($ei)
Set-Content tavern.cpp -Value $c -Encoding UTF8 -NoNewline
Write-Host "tavern done, length: $($c.Length)"
