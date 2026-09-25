$c = Get-Content inn.cpp -Raw -Encoding UTF8

$newFunc = @'
void shopInn()
{
	static int InnMenu = 0;
	static int InnNo = 0;
	static int roomChoice = 0;
	static int sleepingHours = 1;
	static int roomCost = 0;
	static int workingHours = 0;
	static int hourlyRate = 0;
	static int jobIncome = 0;
	string str, key;

	if (InnMenu == 0) {
		InnNo=getInnNo();
		if(InnNo==0){setAutoMapFlag(plyr.map,8,44);setAutoMapFlag(plyr.map,9,44);setAutoMapFlag(plyr.map,10,44);setAutoMapFlag(plyr.map,8,45);}
		if(InnNo==3){setAutoMapFlag(plyr.map,31,38);setAutoMapFlag(plyr.map,32,39);setAutoMapFlag(plyr.map,31,39);}
		if(InnNo==4){setAutoMapFlag(plyr.map,31,40);setAutoMapFlag(plyr.map,32,40);setAutoMapFlag(plyr.map,33,40);setAutoMapFlag(plyr.map,31,41);}
		loadShopImage(11); plyr.status=2; InnMenu=1; return;
	}

	if (InnMenu == 1)
	{
		clearShopDisplay(); bText(6,0,"You are at the Inn's counter"); bText(10,2,"Do you wish to");
		bText(12,4," ) Stay the night"); bText(12,5," ) Check the time"); bText(12,6," ) Apply for a job"); bText(12,7," ) Leave");
		displayCoins(); SetFontColour(40,96,244,255); bText(12,4,"1");bText(12,5,"2");bText(12,6,"3");bText(12,7,"0"); SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if(key=="1")InnMenu=2; if(key=="2")InnMenu=7; if(key=="3")InnMenu=8; if(key=="0"||key=="down")InnMenu=-1;
	}
	else if (InnMenu == 2)
	{
		clearShopDisplay(); bText(8,0,"Would you like to sleep in");
		bText(1,2," ) the common area floor         coppers"); bText(1,3," ) a Bed with no bath            coppers");
		bText(1,4," ) a Bed with common Bath        coppers"); bText(1,5," ) a Room, with common Bath      coppers");
		bText(1,6," ) a Room with Bath              coppers"); bText(1,7," ) Something better"); displayCoins();
		for(int i=0;i<5;i++){int x=30,rc=static_cast<int>(Rooms[i].baseCost*Inns[InnNo].costMultiplier); if(rc<100)x=31; if(rc>999)x=29; bText(x,i+2,rc);}
		SetFontColour(40,96,244,255); bText(1,2,"1");bText(1,3,"2");bText(1,4,"3");bText(1,5,"4");bText(1,6,"5");bText(1,7,"6"); SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if(key=="1"){roomChoice=0;InnMenu=4;} if(key=="2"){roomChoice=1;InnMenu=4;} if(key=="3"){roomChoice=2;InnMenu=4;}
		if(key=="4"){roomChoice=3;InnMenu=4;} if(key=="5"){roomChoice=4;InnMenu=4;} if(key=="6")InnMenu=3; if(key=="0")InnMenu=-1;
	}
	else if (InnMenu == 3)
	{
		clearShopDisplay(); bText(8,0,"Would you like to sleep in");
		bText(1,2," ) a Premium Room"); bText(1,3," ) a Deluxe Room"); bText(1,4," ) a Small Suite");
		bText(1,5," ) a Suite"); bText(1,6," ) Our BEST Suite"); bText(1,7," ) Something cheaper"); displayCoins();
		for(int i=5;i<10;i++){int x=28,rc=static_cast<int>(Rooms[i].baseCost*Inns[InnNo].costMultiplier); if(rc<1000)x=30; bText(x,i-3,toCurrency(rc)); bText(34,i-3,"coppers");}
		SetFontColour(40,96,244,255); bText(1,2,"1");bText(1,3,"2");bText(1,4,"3");bText(1,5,"4");bText(1,6,"5");bText(1,7,"6"); SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if(key=="6")InnMenu=2; if(key=="1"){roomChoice=5;InnMenu=4;} if(key=="2"){roomChoice=6;InnMenu=4;} if(key=="3"){roomChoice=7;InnMenu=4;} if(key=="4"){roomChoice=8;InnMenu=4;} if(key=="5"){roomChoice=9;InnMenu=4;} if(key=="0")InnMenu=-1;
	}
	else if (InnMenu == 4)
	{
		clearShopDisplay(); cyText(0,"Our rate for sleeping in");
		str=Rooms[roomChoice].name+" is "+toCurrency(static_cast<int>(Rooms[roomChoice].baseCost*Inns[InnNo].costMultiplier))+" coppers."; cyText(1,str);
		cyText(3,"Do you wish to sign in?"); cyText(5,"( es or  o)"); displayCoins(); SetFontColour(40,96,244,255); cyText(5," Y      N  "); SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey(); if(key=="Y")InnMenu=5; if(key=="N")InnMenu=1;
	}
	else if (InnMenu == 5)
	{
		clearShopDisplay(); roomCost=static_cast<int>(Rooms[roomChoice].baseCost*Inns[InnNo].costMultiplier);
		if (!checkCoins(0,0,roomCost)) { cText("I'm sorry, you have not the funds."); cyText(9,"( Press a key )"); updateDisplay(); key=getSingleKey(); if(key!="")InnMenu=1; }
		else {
			cyText(0,"How many hours from now do you wish"); cyText(1,"your wake-up call to be set for"); cyText(2,"(Maximum of 12 hours)?");
			bText(3,4," ) 1 hour    ) 2 hours   ) 3 hours"); bText(3,5," ) 4 hours   ) 5 hours   ) 6 hours");
			bText(3,6," ) 7 hours   ) 8 hours   ) 9 hours"); bText(3,7," ) 10 hours  ) 11 hours  ) 12 hours");
			SetFontColour(40,96,244,255); bText(3,4,"1           2           3"); bText(3,5,"4           5           6"); bText(3,6,"7           8           9"); bText(3,7,"A           B           C"); SetFontColour(215,215,215,255);
			updateDisplay(); key=getSingleKey();
			if(key=="1"){sleepingHours=1;InnMenu=6;} if(key=="2"){sleepingHours=2;InnMenu=6;} if(key=="3"){sleepingHours=3;InnMenu=6;}
			if(key=="4"){sleepingHours=4;InnMenu=6;} if(key=="5"){sleepingHours=5;InnMenu=6;} if(key=="6"){sleepingHours=6;InnMenu=6;}
			if(key=="7"){sleepingHours=7;InnMenu=6;} if(key=="8"){sleepingHours=8;InnMenu=6;} if(key=="9"){sleepingHours=9;InnMenu=6;}
			if(key=="A"){sleepingHours=10;InnMenu=6;} if(key=="B"){sleepingHours=11;InnMenu=6;} if(key=="C"){sleepingHours=12;InnMenu=6;}
		}
	}
	else if (InnMenu == 6)
	{
		if (sleepingHours>0) {
			clearShopDisplay(); cyText(2,"You are sleeping in"); str=Rooms[roomChoice].name+"."; cyText(3,str); updateDisplay();
			sf::sleep(sf::seconds(1));
			int rp=0; if(roomChoice==0)rp=30; if(roomChoice==1)rp=45; if(roomChoice==2)rp=60; if(roomChoice==3)rp=85; if(roomChoice==4)rp=100; if(roomChoice==5)rp=140; if(roomChoice==6)rp=170; if(roomChoice==7)rp=200; if(roomChoice==8)rp=240; if(roomChoice==9)rp=255;
			if(randn(0,255)<=rp){plyr.hp+=6; if(plyr.hp>plyr.maxhp)plyr.hp=plyr.maxhp;}
			addHour(); sleepingHours--;
		} else {
			clearShopDisplay(); cyText(2,"Brave adventurer..."); cyText(3,"It is time to wake up."); cyText(4,"I hope you rested well."); cyText(9,"( Press a key )"); updateDisplay();
			key=getSingleKey(); if(key!=""){deductCoins(0,0,roomCost);InnMenu=1;}
		}
	}
	else if (InnMenu == 7)
	{
		clearShopDisplay(); str="Hour "+itos(plyr.hours)+" of day "+itos(plyr.days); bText(7,2,str);
		string md; switch(plyr.months){case 1:md="Rebirth";break;case 2:md="Awakening";break;case 3:md="Winds";break;case 4:md="Rains";break;case 5:md="Sowings";break;case 6:md="First Fruits";break;case 7:md="Harvest";break;case 8:md="Final Reaping";break;case 9:md="The Fall";break;case 10:md="Darkness";break;case 11:md="Cold Winds";break;case 12:md="Lights";break;}
		str="In the month of "+md; bText(7,4,str); str="In year "+itos(plyr.years)+" since abduction"; bText(7,6,str); cyText(9,"( Press a key )"); updateDisplay();
		key=getSingleKey(); if(key!="")InnMenu=1;
	}
	else if (InnMenu == 8)
	{
		int jn=innJobOpenings[InnNo].jobNumber; clearShopDisplay();
		if(jn==255){bText(7,0,"I'm sorry but there are no");cyText(1,"job openings at the moment.");cyText(9,"( Press a key )");updateDisplay();key=getSingleKey();if(key!="")InnMenu=1;}
		else{str="We have an opening for a "+innJobs[jn].name;cyText(0,str);str="for "+itos(innJobOpenings[InnNo].JobHoursRequired)+" hours at "+itos(innJobOpenings[InnNo].jobHourlyIncome)+" coppers per hour.";cyText(1,str);cyText(3,"Would you like to apply?");cyText(5,"( es or  o)");SetFontColour(40,96,244,255);cyText(5," Y      N  ");SetFontColour(215,215,215,255);updateDisplay();key=getSingleKey();if(key=="Y")InnMenu=9;if(key=="N")InnMenu=1;}
	}
	else if (InnMenu == 9)
	{
		int jn=innJobOpenings[InnNo].jobNumber; string req=innJobs[jn].statRequirementName; int rv=innJobs[jn].statRequirementValue; bool met=false;
		if((req=="Strength")&&(rv<=plyr.str))met=true; if((req=="Charm")&&(rv<=plyr.chr))met=true; if((req=="Stamina")&&(rv<=plyr.sta))met=true;
		if(!met){clearShopDisplay();str="You will need more "+req;cyText(0,str);cyText(1,"to get the job.");cyText(9,"( Press a key )");updateDisplay();key=getSingleKey();if(key!="")InnMenu=1;}
		else{workingHours=innJobOpenings[InnNo].JobHoursRequired;hourlyRate=innJobOpenings[InnNo].jobHourlyIncome;jobIncome=workingHours*hourlyRate;InnMenu=10;}
	}
	else if (InnMenu == 10)
	{
		if(workingHours>0){clearShopDisplay();cyText(2,"WORKING");updateDisplay();sf::sleep(sf::seconds(1));addHour();workingHours--;}
		else{clearShopDisplay();cyText(2,"The job is completed.");str="You have earned "+itos(jobIncome)+" coppers.";cyText(3,str);cyText(9,"( Press a key )");updateDisplay();key=getSingleKey();if(key!=""){plyr.copper+=jobIncome;innJobOpenings[InnNo].jobNumber=255;InnMenu=1;}}
	}

	if (InnMenu == -1) { InnMenu=0; leaveShop(); }
}

'@

$si = $c.IndexOf("void shopInn()")
$ei = $c.IndexOf("int getInnNo()")
$c = $c.Substring(0,$si) + $newFunc + $c.Substring($ei)
Set-Content inn.cpp -Value $c -Encoding UTF8 -NoNewline
Write-Host "inn done, length: $($c.Length)"
