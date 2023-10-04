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
class Gegner : public Objekte{ // Kann schieﬂen (evtl. Nur am Bildrand) 

};

class GameWindow : public Gosu::Window
{
	std::unique_ptr<Gosu::Image> background_image;

public:
//

	Gosu::Image Bild; 


	GameWindow()
		: Window(Gosu::screen_width(), Gosu::screen_height(), true),
		Bild("Strasse.png.png")
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
		double x = Gosu::screen_width() / 1300, y = Gosu::screen_height() / 866; 
		
		Bild.draw(0.0 , 0.0 , 0.0, 1+ (Gosu::screen_width() / 1300) , 1+ (Gosu::screen_height() / 866));
		
	
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
