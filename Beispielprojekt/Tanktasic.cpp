#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
using namespace std; 


class Welt {
	public: 
		 

};

class Objekte {
public: 
	// Positionen vom Objekt
	float x_pos, vel_x; // Position + Geschwindigkeit 
	float y_pos, vel_y; // Position + Geschwindigkeit 
	float angle; 
	int breite;
	int hoehe;

	
};


class Spieler : public Objekte{
public: 
	unsigned score; // Gibt den Score an 
private:
	
	

};
class Hindernis : public Objekte {// Steht nur im Weg 
	
};
class Gegner : public Objekte{ // Kann schießen (evtl. Nur am Bildrand) 

};




class GameWindow : public Gosu::Window
{
	

public:
//
	Gosu::Image Tank1;
	Gosu::Image Bild; 
	Spieler spieler_1;


	double y; // Bild Startpunkt 
	double speed_Hintergrund = 5; // Bild bewegung 
	double screen_dehner_width = Gosu::screen_width() / 899.0; // Dehnungsfaktor Bild pixel
	double screen_dehner_hight = (Gosu::screen_height() / 602.0);
	
	

	GameWindow()
		: Window(Gosu::screen_width(), Gosu::screen_height(), true),
		Bild("road.png"),y(0.0),Tank1("tank.png")
	{
		set_caption("Tanktastic");
	
		spieler_1.vel_x = 5;
	
	
	}
	
	
	// Wird bis zu 60x pro Sekunde aufgerufen.Gosu::screen_height()
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt


	void draw() override
	{
		 
		// Bild passt sich an Monitor an 
		
		Bild.draw(0.0 , y , 0.0,  screen_dehner_width , screen_dehner_hight);
		Bild.draw(0.0, y- Gosu::screen_height()+5, 0.0, screen_dehner_width, screen_dehner_hight);
		Tank1.draw(spieler_1.x_pos, Gosu::screen_height()- (694*0.3), 0.0,0.3,0.3);
		
	
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if (Gosu::Input::down(Gosu::KB_LEFT)) {
			spieler_1.x_pos= spieler_1.x_pos-spieler_1.vel_x;
		}
		if (Gosu::Input::down(Gosu::KB_RIGHT)) {
			spieler_1.x_pos= spieler_1.x_pos+spieler_1.vel_x;
		}
	
		

		y += speed_Hintergrund; 
		if (y >= Gosu::screen_height()) {
			y = 0.0;
		}
		

	}
};



// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
	
}
