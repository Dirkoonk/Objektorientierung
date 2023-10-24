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

	
};


class Spieler : public Objekte{
public: 
	int score; // Gibt den Score an 

private:
	
	

};
class Hindernis : public Objekte {// Steht nur im Weg 
public:

private:
	int random_xpos() {
		this->x_pos = Gosu::random(this->breite,Gosu::screen_height()-this->hoehe);
		return this->x_pos;
	}
};
class Gegner : public Objekte{ // Kann schießen (evtl. Nur am Bildrand) 

};



class GameWindow : public Gosu::Window
{
	

public:
    Spieler spieler_1;
	Hindernis stein_1;
	Gosu::Image Tank1;

	//Hintergrund bild
	Gosu::Image Bild; 
	
	double y; // Bild Startpunkt 
	double speed_Hintergrund = 5; // Bild bewegung 
	double screen_dehner_width = screen_width / 899.0 ; // Dehnungsfaktor Bild pixel
	double screen_dehner_hight = screen_height / 602.0;

	//Bildschirm Grenzen 

	Gosu::Font myfont; //Schrift für den Score

	// Gegenstände
	//Stein
	
	Gosu::Image Stein; 

	//double Tank1_real_height = 

	bool isPaused = false;
	bool wasPaused = false;

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
		
	{
		set_caption("Tanktastic");
		//Spiler daten werden festgelegt
		spieler_1.breite = 281;//breie Bild
		spieler_1.hoehe = 694;//höhe Bild
		spieler_1.h_faktor = 0.37;
		spieler_1.w_faktor = 0.1;
		spieler_1.vel_x = 5; // Panzer Geschwindigkeit
		spieler_1.x_pos = screen_width / 2 - (spieler_1.breite * spieler_1.w_scale())/2; // Panzer startet in der Mitte des Screen
		
		//versuch objekte zu verwenden
		stein_1.breite = 281;//breite Bild
		stein_1.hoehe = 694;//höhe Bild
		stein_1.w_faktor = 0.3;
		stein_1.h_faktor = 0.3;

	
	
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
			Bild.draw(0.0, y, 0.0, screen_dehner_width, screen_dehner_hight);
			Bild.draw(0.0, y - screen_height + 5, 0.0, screen_dehner_width, screen_dehner_hight);

					//	position Panzer   //damit Panzer auf X-Achse ganz zu sehen ist 
			Tank1.draw(spieler_1.x_pos, screen_height - (spieler_1.hoehe * spieler_1.h_scale()), 0.0, spieler_1.w_scale(), spieler_1.h_scale());


			// Score
			myfont.draw_text("Score:    " + to_string(spieler_1.score), 20, 20, 0, Score_x_scale, Score_y_scale, Gosu::Color::BLACK);


			//Gegenstände
			//Stein.draw(stein_1.x_pos, stein_1.y_pos, 0.0 )
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
			if (Gosu::Input::down(Gosu::KB_LEFT) && !(spieler_1.x_pos <= 0)) {
				spieler_1.x_pos = spieler_1.x_pos - spieler_1.vel_x;
			}
			if (Gosu::Input::down(Gosu::KB_RIGHT) && !(spieler_1.x_pos >= screen_width - spieler_1.breite * spieler_1.w_scale())) {
				spieler_1.x_pos = spieler_1.x_pos + spieler_1.vel_x;
			}

			if (Gosu::Input::down(Gosu::KB_P)) // Prüfen Sie die Taste "P", um das Spiel zu pausieren
			{
				isPaused = !isPaused;
				this_thread::sleep_for(chrono::milliseconds(200)); //delay um schnelles wechseln zu verhindern.
			}
			// Score hochzählen
			y += speed_Hintergrund;
			spieler_1.score = spieler_1.score + speed_Hintergrund;
			if (y >= screen_height) {
				y = 0.0;
			}
		}
		else {
		if (Gosu::Input::down(Gosu::KB_P)) // Prüfen Sie die Taste "P", um das Spiel fortzusetzen
			{
			isPaused = !isPaused;
			this_thread::sleep_for(chrono::milliseconds(200));

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
