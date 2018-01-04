#include <SFML/Window.hpp>

void readKey()
{

// Process events
		sf::Event Event;
		while (App.GetEvent(Event))
		{
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Left)) turnLeft();
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Right)) turnRight();
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Up)) moveForward();
			//if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Down)) moveBack();

			// Close window : exit
			if (Event.Type == sf::Event::Closed)
				Running = false;

			// Escape key : exit
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
				Running = false;

		//if (Event.Type == sf::Event::Resized)
    //glViewport(0, 0, Event.Size.Width, Event.Size.Height);

		}

}
