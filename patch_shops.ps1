# patch_shops.ps1 - Convert all remaining blocking shop while-loops to per-frame if/else-if state machines

function PatchFile($path, $old, $new) {
    $c = Get-Content $path -Raw -Encoding UTF8
    if ($c.Contains($old)) {
        $c = $c.Replace($old, $new)
        Set-Content $path -Value $c -Encoding UTF8 -NoNewline
        Write-Host "Patched: $path"
    } else {
        Write-Host "WARNING - pattern not found in: $path"
    }
}

# ─── staircase.cpp ───────────────────────────────────────────────────────────

$old = @'
	while (staircaseMenu > 0)
	{
		while (staircaseMenu == 1) // main menu
		{
			clearShopDisplay();
			//drawStatsPanel();

			if (stairwayUp) { cyText (1, "A stairway leads up, do you take it?"); }
			else { cyText (1, "A stairway leads down, do you take it?"); }
			cyText (3, "( es or  o)");
			SetFontColour(40, 96, 244, 255);
			cyText (3, " Y      N  ");
			SetFontColour(215, 215, 215, 255);
			updateDisplay();

			key = getSingleKey();

			//if ( key=="Y" ) { staircaseMenu = 2; }
			if ( key=="N" ) { staircaseMenu = 0; leaveShop();}
			if (key=="Y") { ; moveMapLevel(); plyr.status = 1; staircaseMenu = 0;  };
		}

	}

}
'@
$new = @'
	if (staircaseMenu == 1)
	{
		clearShopDisplay();
		if (stairwayUp) { cyText(1, "A stairway leads up, do you take it?"); }
		else            { cyText(1, "A stairway leads down, do you take it?"); }
		cyText(3, "( es or  o)");
		SetFontColour(40,96,244,255); cyText(3," Y      N  "); SetFontColour(215,215,215,255);
		updateDisplay();
		string key = getSingleKey();
		if (key=="N") { staircaseMenu = -1; }
		if (key=="Y") { moveMapLevel(); plyr.status=1; staircaseMenu=-1; }
	}
	if (staircaseMenu == -1) { staircaseMenu=0; leaveShop(); }
}
'@
PatchFile "staircase.cpp" $old $new

# ─── lift.cpp ────────────────────────────────────────────────────────────────

$old = @'
	while (liftMenu > 0)
	{
		while (liftMenu == 1) // main menu
		{
			clearShopDisplay();
			cyText (1, "In the elevator you find three buttons.");
			cyText (3, "Which do you press?");
			bText (11,5, "(1) The red button");
			bText (11,6, "(2) The green button");
			bText (11,7, "(3) The blue button");
			updateDisplay();


			key = getSingleKey();


			if ( key=="0" ) { liftMenu = 0; }
			if ( key=="down" ) { liftMenu = 0; }
		}



	}
	leaveShop();
}
'@
$new = @'
	if (liftMenu == 1)
	{
		clearShopDisplay();
		cyText(1,"In the elevator you find three buttons.");
		cyText(3,"Which do you press?");
		bText(11,5,"(1) The red button");
		bText(11,6,"(2) The green button");
		bText(11,7,"(3) The blue button");
		updateDisplay();
		string key = getSingleKey();
		if (key=="0" || key=="down") { liftMenu=-1; }
	}
	if (liftMenu == -1) { liftMenu=0; leaveShop(); }
}
'@
PatchFile "lift.cpp" $old $new

# ─── chapel.cpp ──────────────────────────────────────────────────────────────

$old = @'
	while (chapelMenu > 0)
	{
		while (chapelMenu == 1) // main menu
		{
			clearShopDisplay();
			cyText (1, "Welcome to the Dungeon Chapel.");
			cyText (3, "What would you like to do?");
			bText (8,5, "(1) Pray");
			bText (8,6, "(2) Listen to a sermon");
			bText (8,7, "(3) Consult with a priest");
			bText (8,8, "(4) Make a donation");
			bText (8,9, "(0) Leave");
			updateDisplay();
			playShopMusic(3);

			key = getSingleKey();

			if ( key=="0" ) { chapelMenu = 0; }
			if ( key=="down" ) { chapelMenu = 0; }
		}



	}
	stopShopMusic();
	leaveShop();
}
'@
$new = @'
	if (chapelMenu == 1)
	{
		clearShopDisplay();
		cyText(1,"Welcome to the Dungeon Chapel.");
		cyText(3,"What would you like to do?");
		bText(8,5,"(1) Pray"); bText(8,6,"(2) Listen to a sermon");
		bText(8,7,"(3) Consult with a priest"); bText(8,8,"(4) Make a donation"); bText(8,9,"(0) Leave");
		updateDisplay();
		playShopMusic(3);
		string key = getSingleKey();
		if (key=="0" || key=="down") { chapelMenu=-1; }
	}
	if (chapelMenu == -1) { chapelMenu=0; stopShopMusic(); leaveShop(); }
}
'@
PatchFile "chapel.cpp" $old $new

# ─── fountain.cpp ─────────────────────────────────────────────────────────────

$old = @'
	while (fountainMenu > 0)
	{
		while (fountainMenu == 1) // main menu
		{
			clearShopDisplay();
			cyText (1, "You're facing a crystal clear fountain.");
			bText (6,3, "Do you (1) Take a drink or");
			bText (13,4, "(0) Leave?");
			updateDisplay();

			key = getSingleKey();
			if ( key=="0" ) { fountainMenu = 0; }
			if ( key=="down" ) { fountainMenu = 0; }
			if ( key=="1" ) { fountainMenu = 2; }
		}


		while (fountainMenu == 2) // main menu
		{
			clearShopDisplay();

			if ( plyr.map == 1 )
			{
				int strNo = 1;
				if (strNo == 1) str = "The water tastes delicious!";
				if (strNo == 2) str = "Ahh! The water tastes@@absolutely marvelous!";
				if (strNo == 3) str = "The water is cool, clear and@@really hits the spot!";
				if (strNo == 4) str = "You feel rejuvenated!";
				if (strNo == 5) str = "Ahh! Now you feel much better!";
				plyr.hp = plyr.maxhp;
				plyr.thirst = 0;
				//Increase consumption FULL and too bloated to drink message
			}

			if ( plyr.map == 2)
			{
				int strNo = 2;
				if (strNo == 1) str = "Your health improves rapidly!";
				if (strNo == 2) str = "Every cell in your body seems purified!";
				if (strNo == 3) str = "You feel a wave of relief@@sweep over you!";
				//TODO Diseases cleansed!
				//Increase consumption FULL
				plyr.thirst = 0;
			}

			if ( plyr.map == 3 )
			{
				str = "The water tastes delicious!";
				//TODO Fatigue removed!
				//Increase consumption FULL
				plyr.thirst = 0;
			}
			cyText (3, str);
			updateDisplay();

			key = getSingleKey();
			if ( key=="SPACE" ) { fountainMenu = 1; }
		}

	}

	leaveShop();
}
'@
$new = @'
	if (fountainMenu == 1)
	{
		clearShopDisplay();
		cyText(1,"You're facing a crystal clear fountain.");
		bText(6,3,"Do you (1) Take a drink or"); bText(13,4,"(0) Leave?");
		updateDisplay();
		string key = getSingleKey();
		if (key=="0"||key=="down") { fountainMenu=-1; }
		if (key=="1") { fountainMenu=2; }
	}
	else if (fountainMenu == 2)
	{
		clearShopDisplay();
		if (plyr.map==1) { str="The water tastes delicious!"; plyr.hp=plyr.maxhp; plyr.thirst=0; }
		if (plyr.map==2) { str="Every cell in your body seems purified!"; plyr.thirst=0; }
		if (plyr.map==3) { str="The water tastes delicious!"; plyr.thirst=0; }
		cyText(3,str); updateDisplay();
		string key = getSingleKey();
		if (key=="SPACE") { fountainMenu=1; }
	}
	if (fountainMenu == -1) { fountainMenu=0; leaveShop(); }
}
'@
PatchFile "fountain.cpp" $old $new

Write-Host "All patches complete."
