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
	double Tank1_height_faktor = 0.3; //Panzer Skalierungsfaktor Höhe
	double Tank1_height = 694; // Panzer höhe
	double Tank1_width_faktor = 0.3; // Panzer Skalierungsfaktor Breite
	double Tank1_width = 281;
	Gosu::Image Bild; 
	Spieler spieler_1;


	double y; // Bild Startpunkt 
	double speed_Hintergrund = 5; // Bild bewegung 
	double screen_dehner_width = Gosu::screen_width() / 899.0; // Dehnungsfaktor Bild pixel
	double screen_dehner_hight = (Gosu::screen_height() / 602.0);

	//Bildschirm Grenzen 

	double screen_grenze_links = 0; 
	double screen_grenze_rechts = Gosu::screen_width() - Tank1_width * Tank1_width_faktor;
	
	Gosu::Font myfont;

	GameWindow()
		: Window(Gosu::screen_width(), Gosu::screen_height(), true),
		Bild("media/road.png"),y(0.0),Tank1("media/stein.png")
		
	{
		set_caption("Tanktastic");
	
		spieler_1.vel_x = 5; // Panzer Geschwindigkeit
	
	
	
	}
	
	
	// Wird bis zu 60x pro Sekunde aufgerufen.Gosu::screen_height()
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt

	

	void draw() override
	{
		 
		// Bild passt sich an Monitor an 
		Bild.draw(0.0 , y , 0.0,  screen_dehner_width , screen_dehner_hight);
		Bild.draw(0.0, y- Gosu::screen_height()+5, 0.0, screen_dehner_width, screen_dehner_hight);

		//	position Panzer   //damit Panzer auf X-Achse ganz zu sehen ist 
		Tank1.draw(spieler_1.x_pos, Gosu::screen_height()- (Tank1_height * Tank1_height_faktor), 0.0, Tank1_width_faktor, Tank1_height_faktor);


		//Score
		//Gosu::Image textImage("Hallo", 32);
		//textImage.draw(10, 10, 10); 
		
		myfont.draw_text("Score", 20, 30, 0 );
	
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if (Gosu::Input::down(Gosu::KB_LEFT) && !(spieler_1.x_pos <= screen_grenze_links)) {
			spieler_1.x_pos= spieler_1.x_pos-spieler_1.vel_x;
		}
		if (Gosu::Input::down(Gosu::KB_RIGHT) && !(spieler_1.x_pos >= screen_grenze_rechts)) {
			spieler_1.x_pos= spieler_1.x_pos+spieler_1.vel_x;
		}
	
		

		y += speed_Hintergrund; 
		if (y >= Gosu::screen_height()) {
			y = 0.0;
		}
		
		//Score
		//spieler_1.score = spieler_1.score  + 0.1; 
	}
};



// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
	
}
