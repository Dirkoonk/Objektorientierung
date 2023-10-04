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

};


class Spieler : public Objekte{
public: 
	unsigned score; // Gibt den Score an 


};
class Hindernis : public Objekte {// Steht nur im Weg 
	
};
class Gegner : public Objekte{ // Kann schießen (evtl. Nur am Bildrand) 

};

class GameWindow : public Gosu::Window
{
	Gosu::Image background;

public:
//

	Gosu::Image Bild; 
	double x = 0 , y = -899; // Bild Startpunkt 
	double speed_Hintergrund = 1; // Bild bewegung 

	GameWindow()
		: Window(Gosu::screen_width(), Gosu::screen_height(), true),
		Bild("road.png")
	{
		set_caption("Tanktastic");
	
		//Bild.reset(new Gosu::Image("rakete.png"));
		//background.new = Gosu::Image("hintergrund.png"); // Bildpfad anpassen
		

		//backgrund load = Gosu::Image(Pfad)
		//std::string filename =  "Beispielprojekt/Strasse.png.png";
		//background_image.reset(new Gosu::Image("/rakete.png", Gosu::IF_TILEABLE));

			

		//std::string filename = Gosu::resource_prefix() + "media/Stasse.png"
	}

	// Wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		 
		// Bild passt sich an Monitor an 
		
		Bild.draw(x , y , 0.0, 1+ (Gosu::screen_width() / 899) , 1+ (Gosu::screen_height() / 602));
		
	
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if (y >= 0)
		{
			y = -899; 
		}
		y += speed_Hintergrund; 
		

	}
};



// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
