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
	Gosu::Image background;

public:
//
	
	GameWindow()
		: Window(Gosu::screen_width(),Gosu::screen_height(),true),background("Strasse.png.png")
	{
		set_caption("Tanktastic");
		
		
	}

	// Wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
	
		background.draw(0, 0, 0,Gosu::screen_width()/1300.0, Gosu::screen_height()/866.0);
		
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
