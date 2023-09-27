#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>


class Welt {
	public: 
		 

};

class Objekte {
public: 
	// Positionen vom Objekt
	float x_pos, vel_x; // Position + Geschwindigkeit 
	float y_pos, vel_y; // Position + Geschwindigkeit 
	float angle; 

};


class Spieler : public Objekte{
public: 
	unsigned score; // Gibt den Score an 


};
class Hindernis : public Objekte {// Steht nur im Weg 
	
};
class Gegner : public Objekte{ // Kann schieﬂen (evtl. Nur am Bildrand) 

};

class GameWindow : public Gosu::Window
{
	int width = Gosu::screen_width; 
	int height = Gosu::screen_height; 
public:

	GameWindow()
		: Window(width, height)
	{
		set_caption("Tanktastic");

		//std::string filename = Gosu::resource_prefix() + "media/Stasse.png"
	}

	// Wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
	

		
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
	}
};



// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
