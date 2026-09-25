$c = Get-Content smithy.cpp -Raw -Encoding UTF8

$newFunc = @'
void shopSmithy()
{
	static int smithyMenu = 0;
	static int offerStatus = 0;
	static int offerRounds = 0;
	static int itemLowestCost = 0;
	static int smithyOffer = 0;
	static bool musicPlaying = false;
	std::string str, key;

	if (smithyMenu == 0) {
		if (plyr.timeOfDay==1) loadShopImage(8); else loadShopImage(9);
		smithyNo=getSmithyNo(); offerStatus=0; offerRounds=0; musicPlaying=false;
		plyr.status=2; menuStartItem=0;
		smithyMenu = ((Smithies[smithyNo].closingHour<=plyr.hours)||(Smithies[smithyNo].openingHour>plyr.hours)) ? 5 : 1;
		return;
	}

	if (smithyMenu == 5) // closed
	{
		smithyDisplayUpdate();
		cyText(1,"Sorry, we are closed. Come back@during our working hours.");
		str="We are open from "+itos(Smithies[smithyNo].openingHour)+":00 in the morning@to "+itos(Smithies[smithyNo].closingHour)+":00 in the evening.";
		if (Smithies[smithyNo].closingHour==15) str="We are open from "+itos(Smithies[smithyNo].openingHour)+":00 in the morning@to "+itos(Smithies[smithyNo].closingHour)+":00 in the afternoon.";
		cyText(4,str); cyText(9,"( Press a key )"); updateDisplay();
		key=getSingleKey(); if(key!=""&&key!="up") smithyMenu=-1;
	}
	else if (smithyMenu == 1) // main menu
	{
		smithyDisplayUpdate(); bText(13,0,"Welcome Stranger!"); bText(7,3,"Do you wish to see our wares?");
		cyText(5,"( es or  o)"); SetFontColour(40,96,244,255); cyText(5," Y      N  "); SetFontColour(215,215,215,255);
		displayCoins();
		if (!musicPlaying) {
			if (plyr.musicStyle==0) smithyMusic.openFromFile("data/audio/armor.ogg"); else smithyMusic.openFromFile("data/audio/B/armor.ogg");
			loadLyrics("armor.txt"); smithyMusic.play(); musicPlaying=true;
		}
		updateDisplay();
		key=getSingleKey();
		if (key=="Y") smithyMenu=2; if (key=="N"||key=="down") smithyMenu=-1;
	}
	else if (smithyMenu == 2) // browse wares
	{
		offerStatus=0; offerRounds=0;
		smithyDisplayUpdate(); cyText(0,"What would you like? (  to leave)"); SetFontColour(40,96,244,255); cyText(0,"                      0          "); SetFontColour(215,215,215,255);
		smithyNo=getSmithyNo();
		for (int i=0;i<maxMenuItems;i++){int n=smithyDailyWares[smithyNo][menuStartItem+i]; str=") "+smithyWares[n].name; bText(3,2+i,str); bText(1,2+i,"                                 coppers");}
		displayCoins();
		for (int i=0;i<maxMenuItems;i++){int n=smithyDailyWares[smithyNo][menuStartItem+i]; int c2=static_cast<int>(Smithies[smithyNo].initialPriceFactor*smithyWares[n].basePrice); int x=28; if(c2<1000)x=30; if(c2>9999)x=27; bText(x,i+2,toCurrency(c2));}
		SetFontColour(40,96,244,255); bText(2,2,"1");bText(2,3,"2");bText(2,4,"3");bText(2,5,"4");bText(2,6,"5");bText(2,7,"6");
		if(menuStartItem!=0)bText(2,1,"}"); if(menuStartItem!=4)bText(2,8,"{"); SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if(key=="1"){itemChoice=0;smithyMenu=20;} if(key=="2"){itemChoice=1;smithyMenu=20;} if(key=="3"){itemChoice=2;smithyMenu=20;}
		if(key=="4"){itemChoice=3;smithyMenu=20;} if(key=="5"){itemChoice=4;smithyMenu=20;} if(key=="6"){itemChoice=5;smithyMenu=20;}
		if((key=="up")&&(menuStartItem>0))menuStartItem--; if((key=="down")&&(menuStartItem<4))menuStartItem++;
		if(key=="ESC"||key=="0") smithyMenu=-1;
	}
	else if (smithyMenu == 20) // calculate price
	{
		smithyNo=getSmithyNo(); itemNo=smithyDailyWares[smithyNo][menuStartItem+itemChoice];
		itemCost=static_cast<int>(Smithies[smithyNo].initialPriceFactor*smithyWares[itemNo].basePrice);
		float tmp=(Smithies[smithyNo].initialPriceFactor*smithyWares[itemNo].basePrice/100)*75;
		itemLowestCost=static_cast<int>(tmp); smithyOffer=itemCost; smithyMenu=3;
	}
	else if (smithyMenu == 3) // negotiate
	{
		smithyDisplayUpdate();
		if(offerStatus==0){str="The cost for "+smithyWares[itemNo].name; cyText(0,str); str="is "+toCurrency(smithyOffer)+" coppers. Agreed?"; cyText(1,str);}
		if(offerStatus==1){str="I demand at least "+toCurrency(smithyOffer)+" silvers!"; cyText(1,str);}
		if(offerStatus==2){str="Would you consider "+toCurrency(smithyOffer)+"?"; cyText(1,str);}
		bText(11,3," ) Agree to price"); bText(11,4," ) Make an offer"); bText(11,5," ) No sale"); bText(11,6," ) Leave");
		displayCoins(); SetFontColour(40,96,244,255); bText(11,3,"1");bText(11,4,"2");bText(11,5,"3");bText(11,6,"0"); SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if(key=="1"){smithyMenu=checkCoins(0,0,smithyOffer)?4:5;}
		if(key=="2") smithyMenu=16; if(key=="3") smithyMenu=2; if(key=="0") smithyMenu=-1;
	}
	else if (smithyMenu == 16) // counter offer
	{
		int coppers=inputValue("What is your offer? (in coppers)",9);
		if(coppers==0) smithyMenu=2;
		else if(coppers>=itemCost){smithyOffer=coppers;offerStatus=2;smithyMenu=20;}
		else if(coppers>=itemLowestCost){offerStatus=2;offerRounds++;if(offerRounds>2){smithyOffer=coppers;smithyMenu=20;}else{smithyOffer=randn(coppers,itemCost);itemLowestCost=coppers;smithyMenu=3;}}
		else{offerStatus=1;offerRounds++;smithyOffer=itemLowestCost;if(offerRounds>1)smithyMenu=19;else smithyMenu=3;}
	}
	else if (smithyMenu == 4) // confirmed buy
	{
		smithyDisplayUpdate(); cText("An excellent choice!"); cyText(9,"( Press a key )"); updateDisplay();
		key=getSingleKey();
		if(key!=""){
			deductCoins(0,0,smithyOffer);
			int on=smithyWares[itemNo].itemRef;
			if((on>10)||(on==0)) createCitySmithyInventoryItem(on);
			if(on==1){createCitySmithyInventoryItem(0x1CA);createCitySmithyInventoryItem(0x1EE);createCitySmithyInventoryItem(0x217);createCitySmithyInventoryItem(0x23E);}
			if(on==2){createCitySmithyInventoryItem(0x263);createCitySmithyInventoryItem(0x288);createCitySmithyInventoryItem(0x2B2);createCitySmithyInventoryItem(0x2DA);}
			if(on==3){createCitySmithyInventoryItem(0x300);createCitySmithyInventoryItem(0x325);createCitySmithyInventoryItem(0x34F);createCitySmithyInventoryItem(0x377);}
			if(on==4){createCitySmithyInventoryItem(0x39D);createCitySmithyInventoryItem(0x3C1);createCitySmithyInventoryItem(0x3E5);}
			if(on==5){createCitySmithyInventoryItem(0x40D);createCitySmithyInventoryItem(0x432);createCitySmithyInventoryItem(0x457);}
			if(on==6){createCitySmithyInventoryItem(0x480);createCitySmithyInventoryItem(0x4A6);createCitySmithyInventoryItem(0x4CC);}
			if(on==7){createCitySmithyInventoryItem(0x4F6);createCitySmithyInventoryItem(0x51B);createCitySmithyInventoryItem(0x540);}
			if(on==8){createCitySmithyInventoryItem(0x569);createCitySmithyInventoryItem(0x58D);createCitySmithyInventoryItem(0x5B6);createCitySmithyInventoryItem(0x5DD);}
			if(on==9){createCitySmithyInventoryItem(0x602);createCitySmithyInventoryItem(0x626);createCitySmithyInventoryItem(0x64F);createCitySmithyInventoryItem(0x676);}
			plyr.smithyFriendships[smithyNo]++; if(plyr.smithyFriendships[smithyNo]>4)plyr.smithyFriendships[smithyNo]=4;
			smithyMenu=2;
		}
	}
	else if (smithyMenu == 5) // insufficient funds
	{
		smithyDisplayUpdate(); cText("THAT OFFENDS ME DEEPLY!@Why don't you get serious and only@agree to something that you can afford!"); cyText(9,"( Press a key )"); updateDisplay();
		key=getSingleKey(); if(key!="") smithyMenu=2;
	}
	else if (smithyMenu == 19) // thrown out
	{
		smithyDisplayUpdate(); cText("Leave my shoppe and don't return@@until you are ready to make a decent@@offer!"); updateDisplay();
		key=getSingleKey(); if(key!=""){plyr.smithyFriendships[smithyNo]--;if(plyr.smithyFriendships[smithyNo]<0)plyr.smithyFriendships[smithyNo]=0;smithyMenu=-1;}
	}

	if (smithyMenu == -1) { smithyMenu=0; smithyMusic.stop(); leaveShop(); }
}

'@

$si = $c.IndexOf("void shopSmithy()")
$ei = $c.IndexOf("int getSmithyNo()")
$c = $c.Substring(0,$si) + $newFunc + $c.Substring($ei)
Set-Content smithy.cpp -Value $c -Encoding UTF8 -NoNewline
Write-Host "smithy done, length: $($c.Length)"
