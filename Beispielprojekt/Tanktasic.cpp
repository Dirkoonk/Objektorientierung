#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <string.h>
#include <iostream>
#include <thread>
#include <chrono>
using namespace std; 

//Start Variablen Global (Performance)

int  screen_width = Gosu::screen_width(); 
double  screen_height = Gosu::screen_height(); 

class Welt {
	public: 
		double speed;
		double bild_h;
		double bild_w;
		double y;

		double scale_h() {
			return screen_height / bild_h;
		}
		
		double scale_w() {
			return screen_width / bild_w;
		}
		void move() {
			y += speed;
			if (y >= screen_height) {
				y = 0.0;
			}
		}
		


};

class Objekte {
public: 
	// Positionen vom Objekt
	float x_pos , vel_x; // Position + Geschwindigkeit 
	float y_pos, vel_y; // Position + Geschwindigkeit 
	float angle; 
	int breite;
	int hoehe;
	double w_faktor;
	double h_faktor;

	double w_scale() {
		return screen_width / breite * w_faktor;
	}
	double h_scale() {
		return screen_height / hoehe * h_faktor;
	}

	bool is_hit(Hindernis stein) {

	if (((x_pos - stein.x_pos - (breite / 2) - (stein.breite / 2)) == 0) && ((y_pos - stein.y_pos - (hoehe / 2) - (stein.hoehe / 2)) == 0)) {
			return true;
		}
		return false;
	}
		

	
};


class Spieler : public Objekte{
public: 
	int score; // Gibt den Score an 

	void move() {
		if( ( Gosu::Input::down(Gosu::KB_A) || Gosu::Input::down(Gosu::KB_LEFT) )  && !(x_pos <= 0) ){
			x_pos = x_pos - vel_x;
		}
		if( ( Gosu::Input::down(Gosu::KB_D) || Gosu::Input::down(Gosu::KB_RIGHT) ) && !(x_pos >= screen_width - breite * w_scale()) ){
			x_pos = x_pos + vel_x;
		}
	}

	//std::vector<Kugel> kugelList; //Liste von Kugeln

private:
	
	

};
class Hindernis : public Objekte {// Steht nur im Weg 
public:
	void move(double welt_speed) {

		if (this->y_pos <= screen_height) {
			this->y_pos = y_pos + welt_speed;
		}
		
		if (this->y_pos > screen_height) {
			this->x_pos = Gosu::random(0, screen_width);
			this->y_pos = 0.0;
		}

	}
	int random_xpos() {
		this->x_pos = Gosu::random(0,screen_width);
		return this->x_pos;
	}
private:
	
};
class Gegner : public Objekte{ // Kann schießen (evtl. Nur am Bildrand) 

};



class GameWindow : public Gosu::Window
{
	

public:
    Spieler spieler_1;
	Hindernis stein_1;
	Gosu::Image Tank1;
	//ein Welt Objekt wird erzeugt
	Welt welt;

	//Hintergrund bild
	Gosu::Image Bild; 
	
	double y; // Bild Startpunkt 
	
	//Bildschirm Grenzen 

	Gosu::Font myfont; //Schrift für den Score

	// Gegenstände
	//Stein
	
	Gosu::Image Stein; 

	//Gosu::Image Kugel;

	//double Tank1_real_height = 

	bool isPaused;
	bool isPauseKeyDown;

	// GUI - HUD
	Gosu::Image ESC_Button; 
	Gosu::Image Pause_Button; 

	//Skalierung des Scores 
	double Score_x_scale = screen_width / 600; // 200 = gewünschte Score Größe x Wert 
	double Score_y_scale = screen_height / 600;  // 50 gewünschte Score Höhe y Wert 
 

	GameWindow()
		: Window(screen_width, screen_height, true),
		Bild("media/road.png"),y(0.0),
		Tank1("media/tank.png"), myfont(20),
		Stein("media/stein.png"),
		ESC_Button("media/ESC-Button.png"),
		Pause_Button("media/PauseButton.png")
		//Kugel("media/kugel.png")
		
	{
		set_caption("Tanktastic");
		//Spieler daten werden festgelegt
		spieler_1.breite = 281;//breie Bild
		spieler_1.hoehe = 694;//höhe Bild
		spieler_1.h_faktor = 0.37;
		spieler_1.w_faktor = 0.1;
		spieler_1.vel_x = 5; // Panzer Geschwindigkeit
		spieler_1.x_pos = screen_width / 2 - (spieler_1.breite * spieler_1.w_scale())/2; // Panzer startet in der Mitte des Screen
		
		//versuch objekte zu verwenden
		stein_1.breite = 281;//breite Bild
		stein_1.hoehe = 694;//höhe Bild
		stein_1.w_faktor = 0.025;
		stein_1.h_faktor = 0.05;
		stein_1.x_pos = Gosu::random(0,screen_width);
		stein_1.y_pos = 0;

		welt.speed = 5;
		welt.bild_h = 602.0;
		welt.bild_w = 899.0;

	
	
	}
	
	
	// Wird bis zu 60x pro Sekunde aufgerufen.screen_height
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt

	

	void draw() override
	{
		ESC_Button.draw(10, 10, 0.0); 
		Pause_Button.draw(20, 10, 0.0); 

		if (!isPaused)
		{
			// Bild passt sich an Monitor an 
			Bild.draw(0.0, welt.y, 0.0, welt.scale_w(), welt.scale_h());
			Bild.draw(0.0, welt.y - screen_height + 5, 0.0, welt.scale_w(), welt.scale_h());

					//	position Panzer   //damit Panzer auf X-Achse ganz zu sehen ist 
			Tank1.draw(spieler_1.x_pos, screen_height - (spieler_1.hoehe * spieler_1.h_scale()), 0.0, spieler_1.w_scale(), spieler_1.h_scale());


			// Score
			myfont.draw_text("Score:    " + to_string(spieler_1.score), 20, 20, 0, Score_x_scale, Score_y_scale, Gosu::Color::BLACK);


			//Gegenstände
			Stein.draw(stein_1.x_pos , stein_1.y_pos, 0.0, stein_1.w_scale(), stein_1.h_scale());
		}
		else
		{
			myfont.draw_text("Pause - Druecke \"P\"", screen_width/2, screen_height/2, 0, 1, 1, Gosu::Color::WHITE);
		}
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if (!isPaused) {
			// Führen Sie das Spiel-Update nur aus, wenn es nicht pausiert ist.
			// Bewegung Spieler
			spieler_1.move();

			if (Gosu::Input::down(Gosu::KB_P) && !isPauseKeyDown) // Prüfen Taste "P", um das Spiel zu pausieren
			{
				isPaused = true;
				isPauseKeyDown = true;
			}
			else if (!Gosu::Input::down(Gosu::KB_P))
			{
				isPauseKeyDown = false;
			}

			// Score hochzählen
			spieler_1.score = spieler_1.score + welt.speed;

			// Bewegung der Welt
			welt.move();

			stein_1.move(welt.speed);
		}
		else {
			if (Gosu::Input::down(Gosu::KB_P) && !isPauseKeyDown) // Prüfen Taste "P", um das Spiel fortzusetzen
			{
				isPaused = false;
				isPauseKeyDown = true;
			}
			else if (!Gosu::Input::down(Gosu::KB_P))
			{
				isPauseKeyDown = false;
			}
		}

		if (Gosu::Input::down(Gosu::KB_ESCAPE)) {
			close(); // Beendet das Spiel.
		}
		
		 
	}
};



// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
	
}
