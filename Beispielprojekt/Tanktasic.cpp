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

	
};


class Spieler : public Objekte{
public: 
	int score; // Gibt den Score an 

private:
	
	

};
class Hindernis : public Objekte {// Steht nur im Weg 
	
};
class Gegner : public Objekte{ // Kann schießen (evtl. Nur am Bildrand) 

};




class GameWindow : public Gosu::Window
{
	

public:
    Spieler spieler_1;
	Hindernis stein_1;
	Gosu::Image Tank1;

	double Tank1_height = 694; // Panzer höhe
	double Tank1_height_faktor = screen_height / Tank1_height  * 0.37 ; //Panzer Skalierungsfaktor Höhe
	
	
	//double Tank1_real_height = 

	double Tank1_width = 281;
	double Tank1_width_faktor = screen_width / Tank1_width  * 0.1 ; // Panzer Skalierungsfaktor Breite
	

	//Hintergrund bild
	Gosu::Image Bild; 
	


	double y; // Bild Startpunkt 
	double speed_Hintergrund = 5; // Bild bewegung 
	double screen_dehner_width = screen_width / 899.0 ; // Dehnungsfaktor Bild pixel
	double screen_dehner_hight = screen_height / 602.0;

	//Bildschirm Grenzen 

	double screen_grenze_links = 0; 
	double screen_grenze_rechts = screen_width - Tank1_width * Tank1_width_faktor;
	
	Gosu::Font myfont; //Schrift für den Score

	// Gegenstände
	//Stein
	
	Gosu::Image Stein; 


	double Stein1_height_faktor = 0.3; //Panzer Skalierungsfaktor Höhe
	double Stein1_height = 694; // Panzer höhe

	//double Tank1_real_height = 

	double Stein1_width_faktor = 0.3; // Panzer Skalierungsfaktor Breite
	double Stein1_width = 281;
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
	
		spieler_1.vel_x = 5; // Panzer Geschwindigkeit
		spieler_1.x_pos = screen_width / 2 - (Tank1_width * Tank1_width_faktor)/2; // Panzer startet in der Mitte des Screen
	
	
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
			Tank1.draw(spieler_1.x_pos, screen_height - (Tank1_height * Tank1_height_faktor), 0.0, Tank1_width_faktor, Tank1_height_faktor);


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
			if (Gosu::Input::down(Gosu::KB_LEFT) && !(spieler_1.x_pos <= screen_grenze_links)) {
				spieler_1.x_pos = spieler_1.x_pos - spieler_1.vel_x;
			}
			if (Gosu::Input::down(Gosu::KB_RIGHT) && !(spieler_1.x_pos >= screen_grenze_rechts)) {
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
