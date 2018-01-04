#include <SFML/Audio.hpp>

sf::SoundBuffer spellSoundBuffer;
sf::Sound spellSound;

sf::SoundBuffer dungeonGate1Buffer;
sf::SoundBuffer dungeonGate2Buffer;
sf::Sound dungeonGate1Sound;
sf::Sound dungeonGate2Sound;

sf::SoundBuffer cityGate1Buffer;
sf::SoundBuffer cityGate2Buffer;
sf::SoundBuffer cityGate3Buffer;
sf::Sound cityGate1Sound;
sf::Sound cityGate2Sound;
sf::Sound cityGate3Sound;

sf::SoundBuffer encounterBuffers[5];
//sf::Sound encounterSounds[5];
sf::Sound encounterThemeSound;

sf::Music shopMusic;

sf::SoundSource::Status encounterThemeStatus;

bool musicPlaying = false;





void initEncounterThemes()
{
    encounterBuffers[0].loadFromFile("data/audio/cityEncounter2.ogg");  //encounterSounds[0].setBuffer(encounterBuffers[0]);
    encounterBuffers[1].loadFromFile("data/audio/cityEncounter1.ogg");  //encounterSounds[1].setBuffer(encounterBuffers[1]);
    encounterBuffers[2].loadFromFile("data/audio/e1.ogg");              //encounterSounds[2].setBuffer(encounterBuffers[2]);
    encounterBuffers[3].loadFromFile("data/audio/e2.ogg");              //encounterSounds[3].setBuffer(encounterBuffers[3]);
    encounterBuffers[4].loadFromFile("data/audio/e3.ogg");              //encounterSounds[4].setBuffer(encounterBuffers[4]);
}




void playEncounterTheme(int number)
{
    encounterThemeSound.setBuffer(encounterBuffers[number]);
    //encounterSounds[number].play();
    encounterThemeSound.play();
}


bool encounterThemeNotPlaying()
{
    encounterThemeStatus = encounterThemeSound.getStatus();
    if (encounterThemeStatus == sf::Sound::Stopped) { return true; }
    else { return false; }
}




void playShopMusic(int musicNo)
{
	if (musicPlaying==false)
	{
		if (musicNo==1) shopMusic.openFromFile("data/audio/trolls.ogg");
		if (musicNo==2) shopMusic.openFromFile("data/audio/goblins.ogg");
		if (musicNo==3) shopMusic.openFromFile("data/audio/chapel.ogg");
		if (musicNo==4) shopMusic.openFromFile("data/audio/B/trolls.ogg");
		if (musicNo==5) shopMusic.openFromFile("data/audio/B/goblins.ogg");
		if (musicNo==6) shopMusic.openFromFile("data/audio/B/chapel.ogg");
		shopMusic.play();
		musicPlaying = true;
	}
}

void stopShopMusic()
{
	musicPlaying = false;
	shopMusic.stop();
}



void initCityGateSounds()
{
	cityGate1Buffer.loadFromFile("data/audio/cityGate1.ogg");
	cityGate1Sound.setBuffer(cityGate1Buffer);
	//dungeonGate1Sound.setLoop(true);

	cityGate2Buffer.loadFromFile("data/audio/cityGate3v2.ogg");
	cityGate2Sound.setBuffer(cityGate2Buffer);
	//cityGate2Sound.setLoop(true);

	cityGate3Buffer.loadFromFile("data/audio/cityGate4.ogg");
	cityGate3Sound.setBuffer(cityGate3Buffer);
}

void initDungeonGateSounds()
{
	dungeonGate1Buffer.loadFromFile("data/audio/gate1.wav");
	dungeonGate1Sound.setBuffer(dungeonGate1Buffer);
	dungeonGate1Sound.setLoop(true);

	dungeonGate2Buffer.loadFromFile("data/audio/gate2.wav");
	dungeonGate2Sound.setBuffer(dungeonGate2Buffer);
}

void playDungeonGateSound1()
{
   dungeonGate1Sound.play();
}

void playDungeonGateSound2()
{
   dungeonGate2Sound.play();
}

void stopDungeonGateSound1()
{
   dungeonGate1Sound.stop();
}

void stopDungeonGateSound2()
{
   dungeonGate2Sound.stop();
}


void playCityGateSound1()
{
   cityGate1Sound.play();
}

void playCityGateSound2()
{
   cityGate2Sound.play();
}

void playCityGateSound3()
{
   cityGate3Sound.play();
}

void stopCityGateSound1()
{
   cityGate1Sound.stop();
}

void stopCityGateSound2()
{
   cityGate2Sound.stop();
}

void stopCityGateSound3()
{
   cityGate3Sound.stop();
}

void playSpellSound()
{
	spellSoundBuffer.loadFromFile("data/audio/spell.wav");
	spellSound.setBuffer(spellSoundBuffer);
	spellSound.play();
}
