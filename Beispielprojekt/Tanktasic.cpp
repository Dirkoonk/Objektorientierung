#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <string.h>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std; 


//Start Variablen Global (Performance)
bool GameOver = false; 
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
	int x_pos , vel_x; // Position + Geschwindigkeit 
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

	double r_breite() {
		return breite * w_scale();
	}  
	double r_hoehe() {
		return hoehe * h_scale();
	} 

	bool is_hit(Objekte stein) {

	if ((abs(x_pos - stein.x_pos) - (r_breite() / 2) - (stein.r_breite() / 2) <= 0) && (stein.y_pos>=(screen_height-r_hoehe()))) {
			return true;
		}
		return false;
	}
	
};


class Spieler : public Objekte{
public: 
	int score; // Gibt den Score an 
	int leben = 0; 

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
			this->x_pos = Gosu::random(0, screen_width-breite);
			this->y_pos = 0.0;
		}

	}
	int random_xpos() {
		this->x_pos = int(Gosu::random(0,screen_width));
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
	Gosu::Image Hud_MaxHP_Paused;
	Gosu::Image Hud_MaxHP_NON_Paused;
	Gosu::Image Hud_V7HP_Paused;
	Gosu::Image Hud_V7HP_NON_Paused;
	Gosu::Image Hud_V3HP_Paused;
	Gosu::Image Hud_V3HP_NON_Paused;
	Gosu::Image HUD_GameOver;

	//Skalierung des Scores 
	double Score_x_scale = screen_width / 600; // 200 = gewünschte Score Größe x Wert 
	double Score_y_scale = screen_height / 600;  // 50 gewünschte Score Höhe y Wert 
 

	GameWindow()
		: Window(screen_width, screen_height, true),
		//Hintergrund
		Bild("media/road.png"), y(0.0),
		//Spieler
		Tank1("media/tank.png"), myfont(20),
		//Gegenstände
			Stein("media/stein.png"),
			//Kugel("media/kugel.png"),
		//HUD
		Hud_MaxHP_Paused("media/HUD/Hud_MaxHP_Paused.png"),
		Hud_MaxHP_NON_Paused("media/HUD/Hud_MaxHP_NON_Paused.png"),
		Hud_V7HP_Paused("media/HUD/Hud_V7HP_Paused.png"),
		Hud_V7HP_NON_Paused("media/HUD/Hud_V7HP_NON_Paused.png"),
		Hud_V3HP_Paused("media/HUD/Hud_V3HP_Paused.png"),
		Hud_V3HP_NON_Paused("media/HUD/Hud_V3HP_NON_Paused.png"),
		HUD_GameOver("media/HUD/GameOver.png")
		
		
	{
		set_caption("Tanktastic");
		//Spieler daten werden festgelegt
		spieler_1.breite = 281;//breie Bild
		spieler_1.hoehe = 694;//höhe Bild
		spieler_1.h_faktor = 0.37;
		spieler_1.w_faktor = 0.1;
		spieler_1.vel_x = 5; // Panzer Geschwindigkeit
		spieler_1.x_pos = screen_width / 2 - (spieler_1.breite * spieler_1.w_scale())/2; // Panzer startet in der Mitte des Screen
		spieler_1.leben = 3; // Anzahl Leben (Maximal 3 wegen HUD) 
		
		//versuch objekte zu verwenden
		stein_1.breite = 281;//breite Bild
		stein_1.hoehe = 694;//höhe Bild
		stein_1.w_faktor = 0.025;
		stein_1.h_faktor = 0.05;
		srand(0);
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
		// Bild passt sich an Monitor an 
		Bild.draw(0.0, welt.y, 0.0, welt.scale_w(), welt.scale_h());
		Bild.draw(0.0, welt.y - screen_height + 5, 0.0, welt.scale_w(), welt.scale_h());

		//	position Panzer   //damit Panzer auf X-Achse ganz zu sehen ist 
		Tank1.draw(spieler_1.x_pos, screen_height - (spieler_1.hoehe * spieler_1.h_scale()), 1, spieler_1.w_scale(), spieler_1.h_scale());

		//Gegenstände
		Stein.draw(stein_1.x_pos, stein_1.y_pos, 2, stein_1.w_scale(), stein_1.h_scale());

		// Score
		myfont.draw_text("Score:" + to_string(spieler_1.score), 0, 20, 4, Score_x_scale, Score_y_scale, Gosu::Color::BLACK);

		//HUD Not_Paused 

		if (!isPaused && !GameOver && spieler_1.leben >= 3){ // Hud Max LP Non Paused
			Hud_MaxHP_NON_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h());}
		else if (!isPaused && !GameOver && spieler_1.leben == 2) { // Hud V7 LP Non Paused
			Hud_V7HP_NON_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h());}
		else if (!isPaused && !GameOver && spieler_1.leben == 1) { // Hud V3 LP Non Paused
			Hud_V3HP_NON_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h());}

		//HUD Paused 

		else if (isPaused && !GameOver && spieler_1.leben >= 3) { // Hud Max LP Non Paused
			Hud_MaxHP_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h());}
		else if (isPaused && !GameOver && spieler_1.leben == 2) { //Hud V7 LP Paused
			Hud_V7HP_Paused.draw(0.0, 0.0, 0.0, welt.scale_w(), welt.scale_h());}
		else if (isPaused && !GameOver && spieler_1.leben == 1) { // Hud V3 LP Non Paused
			Hud_V3HP_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h());}

		//HUD Game-Over
		else if (GameOver) { //Game Over HUD
			HUD_GameOver.draw(0.0, 0.0, 0.0, welt.scale_w(), welt.scale_h());}
		
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{


			if (!isPaused && !GameOver) {
				// Führen Sie das Spiel-Update nur aus, wenn es nicht pausiert ist.
				// Bewegung Spieler
				welt.speed = 5;
				spieler_1.move();
				stein_1.move(welt.speed);
				welt.move();


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






				if (spieler_1.is_hit(stein_1)) {
					// Leben abziehen 
					spieler_1.leben = spieler_1.leben -1; 

					//Gegenstand weg Porten
					stein_1.x_pos = Gosu::random(0, screen_width);
					stein_1.y_pos = 0;

					// Wenn leben =0 Game Over
					if (spieler_1.leben <= 0) {
						GameOver = true;
					}
				}

			}
			else if (!GameOver)
			{
				welt.speed = 0;
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
