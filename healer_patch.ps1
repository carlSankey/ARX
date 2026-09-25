$c = Get-Content healer.cpp -Raw -Encoding UTF8

$newFunc = @'
void shopHealer()
{
	static int healerMenu = 0;
	string str, key;

	if (healerMenu == 0) {
		plyr.status=2; healerUpdatePrices();
		int healerNo=getHealerNo();
		if(healerNo==1){setAutoMapFlag(plyr.map,28,34);setAutoMapFlag(plyr.map,29,34);setAutoMapFlag(plyr.map,30,34);setAutoMapFlag(plyr.map,28,35);setAutoMapFlag(plyr.map,29,35);setAutoMapFlag(plyr.map,30,35);setAutoMapFlag(plyr.map,28,36);setAutoMapFlag(plyr.map,29,36);setAutoMapFlag(plyr.map,30,36);}
		if(healerNo==2){setAutoMapFlag(plyr.map,32,44);setAutoMapFlag(plyr.map,31,44);setAutoMapFlag(plyr.map,33,44);setAutoMapFlag(plyr.map,31,43);}
		bool oddHour=false; int h=plyr.hours; if((h==1)||(h==3)||(h==5)||(h==7)||(h==9)||(h==11)||(h==13)||(h==15)||(h==17)||(h==19)||(h==21)||(h==23)) oddHour=true;
		bool open=false; if((healerNo==1)&&oddHour)open=true; if((healerNo==2)&&!oddHour)open=true;
		if(open){loadShopImage(15);healerMenu=1;}else{loadShopImage(21);healerMenu=2;}
		return;
	}

	if (healerMenu == 1)
	{
		clearShopDisplay(); cyText(0,"Welcome Stranger!"); cyText(1,"How may I serve you?");
		bText(11,3,"(1) Heal"); bText(11,4,"(2) Cleanse"); bText(11,5,"(3) Remove delusions"); bText(11,6,"(4) Diagnose"); bText(11,7,"(0) Leave");
		displayCoins(); updateDisplay();
		key=getSingleKey();
		if(key=="0"||key=="down") healerMenu=-1;
		if(key=="1") { healerShopHealWounds(); }
		if(key=="2") healerMenu=3; if(key=="3") healerMenu=4; if(key=="4") healerMenu=5;
	}
	else if (healerMenu == 2)
	{
		moduleMessage("It looks as though the@@Healer is not here."); healerMenu=-1;
	}
	else if (healerMenu == 3) // cleanse
	{
		clearShopDisplay(); cyText(1,"Remove which illnesses?");
		bText(1,3,"(1) Cleanse Poisons"); bText(1,4,"(2) Cure Diseases"); bText(1,5,"(3) Remove Alcohol"); bText(1,6,"(0) None");
		bText(24,3,itos(poisonsPrice)+" coppers"); bText(24,4,itos(diseasesPrice)+" coppers"); bText(24,5,itos(alcoholPrice)+" coppers");
		displayCoins(); updateDisplay();
		key=getSingleKey();
		if(key=="0") healerMenu=1;
		if(key=="1"){if(!checkCoins(0,0,poisonsPrice)){moduleMessage("I'm sorry... You have not the funds.");}else{moduleMessage("IT SHALL BE DONE!");deductCoins(0,0,poisonsPrice);plyr.poison[0]=0;plyr.poison[1]=0;plyr.poison[2]=0;plyr.poison[3]=0;healerUpdateLastServiceTime();healerUpdatePrices();}healerMenu=1;}
		if(key=="2"){if(!checkCoins(0,0,diseasesPrice)){moduleMessage("I'm sorry... You have not the funds.");}else{moduleMessage("IT SHALL BE DONE!");deductCoins(0,0,diseasesPrice);plyr.diseases[0]=0;plyr.diseases[1]=0;plyr.diseases[2]=0;plyr.diseases[3]=0;healerUpdateLastServiceTime();healerUpdatePrices();}healerMenu=1;}
		if(key=="3"){if(!checkCoins(0,0,alcoholPrice)){moduleMessage("I'm sorry... You have not the funds.");}else{moduleMessage("IT SHALL BE DONE!");deductCoins(0,0,alcoholPrice);plyr.alcohol=0;healerUpdateLastServiceTime();healerUpdatePrices();}healerMenu=1;}
	}
	else if (healerMenu == 4) // delusions
	{
		clearShopDisplay(); cyText(1,"Shall I remove your delusions"); cyText(2,"for "+itos(clarityPrice)+" coppers?"); cyText(5,"(Yes or No)"); displayCoins(); updateDisplay();
		key=getSingleKey(); if(key=="N")healerMenu=1;
		if(key=="Y"){if(!checkCoins(0,0,clarityPrice)){moduleMessage("I'm sorry... You have not the funds.");}else{moduleMessage("IT SHALL BE DONE!");deductCoins(0,0,clarityPrice);plyr.delusion=0;healerUpdateLastServiceTime();healerUpdatePrices();}healerMenu=1;}
	}
	else if (healerMenu == 5) // diagnose
	{
		clearShopDisplay(); cyText(1,"Shall I check for and diagnose disease"); cyText(2,"for "+itos(diagnosePrice)+" coppers?"); cyText(5,"(Yes or No)"); displayCoins(); updateDisplay();
		key=getSingleKey(); if(key=="N")healerMenu=1;
		if(key=="Y"){if(!checkCoins(0,0,diagnosePrice)){moduleMessage("I'm sorry... You have not the funds.");}else{moduleMessage("IT SHALL BE DONE!");deductCoins(0,0,diagnosePrice);if(plyr.diseases[0]>0&&plyr.diseases[0]<15)plyr.diseases[0]=15;if(plyr.diseases[1]>0&&plyr.diseases[1]<48)plyr.diseases[1]=48;if(plyr.diseases[2]>0&&plyr.diseases[2]<48)plyr.diseases[2]=48;healerUpdateLastServiceTime();healerUpdatePrices();}healerMenu=1;}
	}

	if (healerMenu == -1) { healerMenu=0; leaveShop(); }
}

'@

$si = $c.IndexOf("void shopHealer()")
$ei = $c.IndexOf("void healerUpdatePrices()")
$c = $c.Substring(0,$si) + $newFunc + $c.Substring($ei)
Set-Content healer.cpp -Value $c -Encoding UTF8 -NoNewline
Write-Host "healer done, length: $($c.Length)"
