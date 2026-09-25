$c = Get-Content shop.cpp -Raw -Encoding UTF8

$newFunc = @'
void shopShop()
{
	static int shopMenu = 0;
	static int shopNo_s = 0;
	static int itemChoice_s = 0;
	static int itemCost_s = 0;
	static int itemLowestCost_s = 0;
	static int shopOffer_s = 0;
	static int itemNo_s = 0;
	static int menuStart_s = 0;
	static int offerStatus_s = 0;
	static int offerRounds_s = 0;
	string str, key;

	if (shopMenu == 0) {
		shopNo_s=getShopNo(); menuStart_s=0; offerStatus_s=0; offerRounds_s=0;
		for(int i=0;i<12;i++){shopClothingWares[i].itemRef=shopDailyWares[shopNo_s][i]; shopClothingWares[i].price=clothingItems[shopDailyWares[shopNo_s][i]].quality;}
		plyr.status=2; loadShopImage(12);
		shopMenu=((Shops[shopNo_s].closingHour<=plyr.hours)||(Shops[shopNo_s].openingHour>plyr.hours))?50:1;
		return;
	}

	if (shopMenu == 1)
	{
		clearShopDisplay(); bText(13,0,"Welcome Stranger!"); bText(7,3,"Do you wish to see our wares?");
		cyText(5,"( es or  o)"); SetFontColour(40,96,244,255); cyText(5," Y      N  "); SetFontColour(215,215,215,255); displayCoins(); updateDisplay();
		key=getSingleKey(); if(key=="Y")shopMenu=3; if(key=="N")shopMenu=2; if(key=="down")shopMenu=2;
	}
	else if (shopMenu == 2) // compass offer
	{
		clearShopDisplay(); cyText(1,"Well then, How about a compass?"); cyText(3,"I charge only 5 silvers for it.");
		cyText(5,"( es or  o)"); SetFontColour(40,96,244,255); cyText(5," Y      N  "); SetFontColour(215,215,215,255); displayCoins(); updateDisplay();
		key=getSingleKey();
		if(key=="Y"){if(checkCoins(0,5,0)){shopMessage("Right away!");plyr.compasses++;deductCoins(0,5,0);}else{shopMessage("THAT OFFENDS ME DEEPLY!@Why don't you get serious@and only agree for something@you can afford!");}shopMenu=-1;}
		if(key=="N") shopMenu=-1;
	}
	else if (shopMenu == 3) // browse wares
	{
		offerStatus_s=0; offerRounds_s=0; int maxM=6;
		clearShopDisplay(); cyText(0,"What would you like? (  to leave)"); SetFontColour(40,96,244,255); cyText(0,"                      0          "); SetFontColour(215,215,215,255);
		for(int i=0;i<maxM;i++){int n=menuStart_s+i; str="( ) "+clothingItems[shopClothingWares[n].itemRef].name; bText(1,2+i,str);}
		displayCoins();
		for(int i=0;i<maxM;i++){int n=menuStart_s+i; int c2=shopClothingWares[n].price; int x=34; if(c2<1000)x=36; if(c2<100)x=37; bText(x+2,i+2,toCurrency(c2));}
		SetFontColour(40,96,244,255); bText(2,2,"1");bText(2,3,"2");bText(2,4,"3");bText(2,5,"4");bText(2,6,"5");bText(2,7,"6");
		if(menuStart_s!=0)bText(2,1,"}"); if(menuStart_s!=6)bText(2,8,"{"); SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if(key=="1"){itemChoice_s=0;shopMenu=22;} if(key=="2"){itemChoice_s=1;shopMenu=22;} if(key=="3"){itemChoice_s=2;shopMenu=22;}
		if(key=="4"){itemChoice_s=3;shopMenu=22;} if(key=="5"){itemChoice_s=4;shopMenu=22;} if(key=="6"){itemChoice_s=5;shopMenu=22;}
		if((key=="up")&&(menuStart_s>0))menuStart_s--; if((key=="down")&&(menuStart_s<6))menuStart_s++;
		if(key=="ESC"||key=="0") shopMenu=2;
	}
	else if (shopMenu == 22) // calc price
	{
		itemNo_s=menuStart_s+itemChoice_s; itemCost_s=shopClothingWares[itemNo_s].price;
		itemLowestCost_s=static_cast<int>((float(itemCost_s)/100)*75); shopOffer_s=itemCost_s; shopMenu=23;
	}
	else if (shopMenu == 23) // negotiate
	{
		clearShopDisplay();
		if(offerStatus_s==0){str="The cost for "+clothingItems[shopClothingWares[itemNo_s].itemRef].name;cyText(0,str);str="is "+toCurrency(shopOffer_s)+" coppers. Agreed?";cyText(1,str);}
		if(offerStatus_s==1){str="I demand at least "+toCurrency(shopOffer_s)+" coppers!";cyText(1,str);}
		if(offerStatus_s==2){str="Would you consider "+toCurrency(shopOffer_s)+"?";cyText(1,str);}
		bText(11,3," ) Agree to price");bText(11,4," ) Make an offer");bText(11,5," ) Select other apparel");bText(11,6," ) Buy something else");
		displayCoins();SetFontColour(40,96,244,255);bText(11,3,"1");bText(11,4,"2");bText(11,5,"3");bText(11,6,"0");SetFontColour(215,215,215,255);
		updateDisplay();
		key=getSingleKey();
		if(key=="1"){shopMenu=checkCoins(0,0,shopOffer_s)?24:25;} if(key=="2")shopMenu=26; if(key=="3")shopMenu=21; if(key=="0")shopMenu=1;
	}
	else if (shopMenu == 21) // back to list (menu label used in original for back)
	{ shopMenu=3; }
	else if (shopMenu == 24) // confirmed buy
	{
		int n=menuStart_s+itemChoice_s; clearShopDisplay(); cText("Excellent decision"); updateDisplay();
		key=getSingleKey();
		if(key!=""){deductCoins(0,0,shopOffer_s); int on=shopClothingWares[n].itemRef; int h=createClothing(on,10); itemBuffer[h].location=10; shopMenu=3;}
	}
	else if (shopMenu == 25) // insufficient funds
	{
		shopMessage("Thou would be wise to check thy funds@@BEFORE purchasing!"); shopMenu=3;
	}
	else if (shopMenu == 26) // counter offer
	{
		int coppers=inputValue("How many coppers do you offer?",3);
		if(coppers==0)shopMenu=22;
		else if(coppers>=itemCost_s){shopOffer_s=coppers;offerStatus_s=2;shopMenu=27;}
		else if(coppers>=itemLowestCost_s){offerStatus_s=2;offerRounds_s++;if(offerRounds_s>2){shopOffer_s=coppers;shopMenu=27;}else{shopOffer_s=randn(coppers,itemCost_s);itemLowestCost_s=coppers;shopMenu=23;}}
		else{offerStatus_s=1;offerRounds_s++;shopOffer_s=itemLowestCost_s;if(offerRounds_s>1)shopMenu=19;else shopMenu=23;}
	}
	else if (shopMenu == 19) // thrown out
	{ shopMessage("THAT OFFENDS ME DEEPLY!@Why don't you get serious@and only agree for something@you can afford!"); shopMenu=-1; }
	else if (shopMenu == 27) // offer accepted
	{
		clearShopDisplay(); cText("I'll take it!"); updateDisplay();
		key=getSingleKey();
		if(key!=""){if(!checkCoins(0,0,shopOffer_s))shopMenu=25;else{plyr.shopFriendships[shopNo_s]++;if(plyr.shopFriendships[shopNo_s]>4)plyr.shopFriendships[shopNo_s]=4;shopMenu=24;}}
	}
	else if (shopMenu == 50) // closed
	{
		string ot,ct; int oh=Shops[shopNo_s].openingHour,ch=Shops[shopNo_s].closingHour;
		if(oh<12)ot="morning"; else if(oh<18)ot="afternoon"; else ot="evening";
		if(ch<12)ct="morning"; else if(ch<18)ct="afternoon"; else ct="evening";
		clearShopDisplay(); cyText(1,"Sorry, we are closed. Come back@during our working hours.");
		str="We are open from "+itos(oh)+":00 in the "+ot+"@to "+itos(ch)+":00 in the "+ct+"."; cyText(4,str); cyText(9,"( Press a key )"); updateDisplay();
		key=getSingleKey(); if(key=="SPACE")shopMenu=-1;
	}

	if (shopMenu == -1) { shopMenu=0; leaveShop(); }
}

'@

$si = $c.IndexOf("void shopShop()")
$ei = $c.IndexOf("int getShopNo()")
$c = $c.Substring(0,$si) + $newFunc + $c.Substring($ei)

# Also fix shopMessage blocking loop
$oldMsg = @'
void shopMessage(string txt)
{
	string key = "";
	while (key!="SPACE")
	{
			clearShopDisplay();
			cText (txt);
			updateDisplay();
			key = getSingleKey();
	}
}
'@
$newMsg = @'
void shopMessage(string txt)
{
	// Non-blocking: draw once, key consumed on next frame.
	// For simple one-shot messages still called from non-web code this is fine.
	clearShopDisplay(); cText(txt); updateDisplay(); getSingleKey();
}
'@
# Try replacing - if whitespace mismatch just leave it
if ($c.Contains("void shopMessage")) {
    $idx = $c.IndexOf("void shopMessage")
    $end = $c.IndexOf("}", $idx) + 1
    $old2 = $c.Substring($idx, $end - $idx)
    $c = $c.Replace($old2, $newMsg)
}

Set-Content shop.cpp -Value $c -Encoding UTF8 -NoNewline
Write-Host "shop done, length: $($c.Length)"
