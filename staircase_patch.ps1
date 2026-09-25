$c = Get-Content staircase.cpp -Raw -Encoding UTF8

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
		if (stairwayUp) { cyText (1, "A stairway leads up, do you take it?"); }
		else { cyText (1, "A stairway leads down, do you take it?"); }
		cyText (3, "( es or  o)");
		SetFontColour(40, 96, 244, 255);
		cyText (3, " Y      N  ");
		SetFontColour(215, 215, 215, 255);
		updateDisplay();
		key = getSingleKey();
		if (key=="N") { staircaseMenu = -1; }
		if (key=="Y") { moveMapLevel(); plyr.status = 1; staircaseMenu = -1; }
	}

	if (staircaseMenu == -1) {
		staircaseMenu = 0;
		leaveShop();
	}
}
'@

$c = $c.Replace($old, $new)
Set-Content staircase.cpp -Value $c -Encoding UTF8 -NoNewline
Write-Host "staircase done"
