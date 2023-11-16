#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <string.h>
#include <iostream>
#include <fstream>
#include <format>
#include <thread>
#include <chrono>
#include <Gosu/Audio.hpp>

using namespace std; 


//Start Variablen Global (Performance)
 
int screen_width = Gosu::screen_width();
int screen_height = Gosu::screen_height();

// Textdatei und Variable für den Highscore
string score_file = "media/Score"; 
int Highscore; 

class Welt {
	public: 
		bool GameOver;
		double y;
		bool Start;
		bool Motor; 
		
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
		Welt() {
			speed = 5;
			bild_h = 602.0;
			bild_w = 899.0;
			GameOver = false;
			Start = false;
			Motor = false; 
			
		}
		Welt(int speed,double hoehe,double breite) {
			this->speed = speed;
			bild_h = hoehe;
			bild_w = breite;
			GameOver = false;
			Start = false; 
			Motor = false;
		}
		void Set_Welt_Speed(int speed) {
			this->speed = speed;
		}
		int Get_Speed() {
			return this->speed;
		}
	private:
		double speed;
		double bild_h;
		double bild_w;
};

class Objekte {
public: 
	// Positionen vom Objekt
	int x_pos , vel_x; // Position + Geschwindigkeit 
	int y_pos, vel_y; // Position + Geschwindigkeit 
	int breite;
	int hoehe;
	double w_faktor;
	double h_faktor;
	

	double w_scale() {
		return (screen_width / breite)* w_faktor;
	}
	double h_scale() {
		return (screen_height / hoehe)* h_faktor;
	}

	double r_breite() {
		return breite * w_scale();
	}  
	double r_hoehe() {
		return hoehe * h_scale();
	} 

	bool is_hit(Objekte& stein) {
		//int Stein_X_M = stein.x_pos + stein.r_breite()/2;
		//int Spieler_X_M = x_pos + r_breite()/2;
	if (((abs(x_pos - stein.x_pos) - (r_breite() / 2) - (stein.r_breite() / 2)) <= 0) && ((stein.y_pos + stein.r_hoehe()) >= (screen_height - r_hoehe()))) {
		//x_pos < stein.x_pos + stein.r_breite() && x_pos + r_breite() > stein.x_pos && y_pos < stein.y_pos + stein.r_hoehe() && y_pos + r_hoehe() > stein.y_pos
		//x < other.x + other.width && x + width > other.x && y < other.y + other.height && y + height > other.y
		//((abs(x_pos - stein.x_pos) - (r_breite() / 2) - (stein.r_breite() / 2)) <= 0)&& ((stein.y_pos+ stein.r_hoehe()) >= (screen_height - r_hoehe()))
			stein.x_pos = rand() % (screen_width - (this->breite / 2));
			stein.y_pos = 0;
			return true;
		}
		return false;
	}
private:
float angle; 
	
};


class Spieler : public Objekte{
public: 
	

	void move() {
		if( ( Gosu::Input::down(Gosu::KB_A) || Gosu::Input::down(Gosu::KB_LEFT) )  && !(x_pos <= 0) ){
			x_pos = x_pos - vel_x;
		}
		if( ( Gosu::Input::down(Gosu::KB_D) || Gosu::Input::down(Gosu::KB_RIGHT) ) && !(x_pos >= screen_width - breite * w_scale()) ){
			x_pos = x_pos + vel_x;
		}
	}
	Spieler() {
		score = 0;
		leben = 3;
		this->x_pos = screen_width / 2 - (this->breite * this->w_scale()) / 2; // Panzer startet in der Mitte des Screen
	}
	Spieler(int StartLeben,int breite,int hoehe,double w_faktor ,double h_faktor,int vel_x) {
		score = 0;
		leben = StartLeben;
		this->breite = breite;
		this->hoehe = hoehe;
		this->h_faktor = h_faktor;
		this->w_faktor = w_faktor;
		this->vel_x = vel_x; 
		this->x_pos = screen_width / 2 - (this->breite * this->w_scale()) / 2; // Panzer startet in der Mitte des Screen
	}
	int Get_Score() {
		return score;
	}
	int Get_Leben() {
		return leben;
	}
	void Score_Hoch(int WeltSpeed) {
		score = score + WeltSpeed;
	}
	void Ein_Leben_Weg() {
		leben--;
	}
	void Max_Leben() {
		leben = 3;
	}
	void Set_Score_to_Zero() {
		score = 0;
	}

	//std::vector<Kugel> kugelList; //Liste von Kugeln

private:
	int score; // Gibt den Score an 
	int leben; 
	

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

	
	Hindernis() {
		this->breite = 100;
		this->hoehe = 100;
		this->w_faktor = 0.05;
		this->h_faktor = 0.05;
		srand(0);
		this->x_pos = rand() % (screen_width - (int(this->r_breite())));
		this->y_pos = 0;
	}
	Hindernis(int breite, int hoehe, double w_faktor, double h_faktor) {
		this->breite = breite;
		this->hoehe = hoehe;
		this->w_faktor = w_faktor;
		this->h_faktor = h_faktor;
		srand(time(0));
		this->x_pos = rand() % (screen_width-(int(this->r_breite())));
		this->y_pos = 0;
	}




private:
	
};

//class Explosion : public Objekte{
//private:

//public:

//};

class Gegner : public Objekte{ // Kann schießen (evtl. Nur am Bildrand) 

};



class GameWindow : public Gosu::Window
{


	

	
public:
	int Time = Gosu::milliseconds();
	//Spieler erstellen
    Spieler spieler_1;
	//Steine erstellen
	Hindernis stein_1;
	Hindernis stein_2;
	Hindernis stein_3;
	Hindernis stein_4;
	Hindernis stein_5;
	vector<Hindernis> SteinListe;
	Gosu::Image Tank1;
	//ein Welt Objekt wird erzeugt
	Welt welt;

	//Explosion ex_klein1;
	//Explosion ex_mittel1;
	//Explosion ex_gross1;

	//Hintergrund bild
	Gosu::Image Bild; 
	
	double y; // Bild Startpunkt 
	
	//Bildschirm Grenzen 

	Gosu::Font myfont; //Schrift für den Score

	// Gegenstände
	//Stein
	
	Gosu::Image Stein1; 
	Gosu::Image Stein2;
	Gosu::Image Stein3;
	Gosu::Image Stein4;
	Gosu::Image Stein5;

	//Explosion Bilder

	Gosu::Image ex_klein;
	Gosu::Image ex_mittel;
	Gosu::Image ex_gross;

	//Sounds
	Gosu::Sample Panzer_Engine_Startup;
	Gosu::Sample Panzer_Engine_Drive;

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
	Gosu::Image HUD_Paused;
	Gosu::Image HUD_Score_Erweiterung;

	//Skalierung des Scores 
	double Score_x_scale = screen_width / 600; // 200 = gewünschte Score Größe x Wert 
	double Score_y_scale = screen_height / 600;  // 50 gewünschte Score Höhe y Wert 

	GameWindow()
		: Window(screen_width, screen_height,true),
		//Welt
		welt(5, 937.0, 899.0),
		//Spieler  ,	
		spieler_1(3,281,694,0.1,0.5,5),
		//Hintergrund
		Bild("media/road_V2.png"), y(0.0),
		//Spieler
		Tank1("media/Panzer_Neu_resized.png"), myfont(20),
		//Gegenstände
		Stein1("media/Barrel-Stein.png"),
		Stein2("media/Barrel-Wasser-Stein.png"),
		Stein3("media/Barrel-Stein.png"),
		Stein4("media/Barrel-Wasser-Stein.png"),
		Stein5("media/Barrel-Stein.png"),
		stein_1(281, 694, 0.025, 0.15),
		stein_2(281, 694, 0.025, 0.15),
		stein_3(281, 694, 0.025, 0.15),
		stein_4(281, 694, 0.025, 0.15),
		stein_5(281, 694, 0.025, 0.15),
		//Kugel("media/kugel.png"),
	//HUD
		Hud_MaxHP_Paused("media/HUD/Hud_MaxHP_Paused.png"),
		Hud_MaxHP_NON_Paused("media/HUD/Hud_MaxHP_NON_Paused.png"),
		Hud_V7HP_Paused("media/HUD/Hud_V7HP_Paused.png"),
		Hud_V7HP_NON_Paused("media/HUD/Hud_V7HP_NON_Paused.png"),
		Hud_V3HP_Paused("media/HUD/Hud_V3HP_Paused.png"),
		Hud_V3HP_NON_Paused("media/HUD/Hud_V3HP_NON_Paused.png"),
		HUD_GameOver("media/HUD/GameOver_V2.png"),
		HUD_Paused("media/HUD/HUD_Paused_Screen.png"),
		HUD_Score_Erweiterung("media/HUD/HUD_Score_Erweiterung.png"),


	//Sound
		Panzer_Engine_Startup("media/Sound/sound_Panzer_Startup.wav"),
		Panzer_Engine_Drive("media/Sound/sound_Panzer_Drive.wav")


		//Explosion
	//	ex_klein("media/ex/ex_klein.png"),
	//	ex_mittel("media/ex/ex_mittel.png"),
	//	ex_gross("media/ex/ex_gross.png")
		
		
	{
		set_caption("Tanktastic");
		
		//versuch die Steine in eine Liste zu packen
		SteinListe.push_back(stein_1);
		SteinListe.push_back(stein_2);
		SteinListe.push_back(stein_3);
		SteinListe.push_back(stein_4);
		SteinListe.push_back(stein_5);

		
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
		Tank1.draw(spieler_1.x_pos, screen_height - (spieler_1.hoehe * spieler_1.h_scale()), 1, spieler_1.w_scale(),spieler_1.h_scale());

		//Gegenstände
		Stein1.draw(stein_1.x_pos, stein_1.y_pos, 2, stein_1.w_scale(), stein_1.h_scale());

		//Schwierigkeit erhöhen
		if (spieler_1.Get_Score() > 3000) {
			Stein2.draw(stein_2.x_pos, stein_2.y_pos, 2, stein_2.w_scale(), stein_2.h_scale());
		}
		if (spieler_1.Get_Score() > 5000) {
			Stein3.draw(stein_3.x_pos, stein_3.y_pos, 2, stein_3.w_scale(), stein_3.h_scale());
		}
		if (spieler_1.Get_Score() > 10000) {
			Stein4.draw(stein_4.x_pos, stein_4.y_pos, 2, stein_4.w_scale(), stein_4.h_scale());
		}
		if (spieler_1.Get_Score() > 15000) {
			Stein5.draw(stein_5.x_pos, stein_5.y_pos, 2, stein_5.w_scale(), stein_5.h_scale());
		}
		// Score
		myfont.draw_text("Score:" + to_string(spieler_1.Get_Score()), 0, 20, 5, Score_x_scale, Score_y_scale, Gosu::Color::BLACK);
		//Highscore
		myfont.draw_text("Topscore:" + to_string(Highscore), 0, 40, 5, Score_x_scale, Score_y_scale, Gosu::Color::BLACK);

		//HUD Not_Paused 

		if (!isPaused && !welt.GameOver && spieler_1.Get_Leben() >= 3) { // Hud Max LP Non Paused
			Hud_MaxHP_NON_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h()+0.5);
			HUD_Score_Erweiterung.draw(0.0, 0.0, 4, welt.scale_w(), welt.scale_h()+0.5);
		}
		else if (!isPaused && !welt.GameOver && spieler_1.Get_Leben() == 2) { // Hud V7 LP Non Paused
			Hud_V7HP_NON_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h()+0.5);
			HUD_Score_Erweiterung.draw(0.0, 0.0, 4, welt.scale_w(), welt.scale_h()+0.5);
		}
		else if (!isPaused && !welt.GameOver && spieler_1.Get_Leben() == 1) { // Hud V3 LP Non Paused
			Hud_V3HP_NON_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h()+0.5);
			HUD_Score_Erweiterung.draw(0.0, 0.0, 4, welt.scale_w(), welt.scale_h()+0.5);
		}

		//HUD Paused 

		else if (isPaused && !welt.GameOver && spieler_1.Get_Leben() >= 3) { // Hud Max LP  Paused
			Hud_MaxHP_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h()+0.5);
			HUD_Score_Erweiterung.draw(0.0, 0.0, 4, welt.scale_w(), welt.scale_h()+0.5);
			HUD_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h());}
		else if (isPaused && !welt.GameOver && spieler_1.Get_Leben() == 2) { //Hud V7 LP Paused
			Hud_V7HP_Paused.draw(0.0, 0.0, 0.0, welt.scale_w(), welt.scale_h()+0.5);
			HUD_Score_Erweiterung.draw(0.0, 0.0, 4, welt.scale_w(), welt.scale_h()+0.5);
			HUD_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h());}
		else if (isPaused && !welt.GameOver && spieler_1.Get_Leben() == 1) { // Hud V3 LP  Paused
			Hud_V3HP_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h()+0.5);
			HUD_Score_Erweiterung.draw(0.0, 0.0, 4, welt.scale_w(), welt.scale_h()+0.5);
			HUD_Paused.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h());}

		//HUD Game-Over
		else if (welt.GameOver) { //Game Over HUD
			HUD_Score_Erweiterung.draw(0.0, 0.0, 4, welt.scale_w(), welt.scale_h()+0.5);
			HUD_GameOver.draw(0.0, 0.0, 3, welt.scale_w(), welt.scale_h());}

		//Explosion
		//ex_klein.draw(ex_klein1.x_pos, ex_klein1.y_pos, 2, ex_klein1.w_scale(), ex_klein1.h_scale());
	

		

	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		while (!welt.Start) {
			Panzer_Engine_Startup.play();
			welt.Start = true; 
			break; 
		}


			if (!isPaused && !welt.GameOver && Gosu::milliseconds() >= 6000) {
				
				// Führen Sie das Spiel-Update nur aus, wenn es nicht pausiert ist.
				// Bewegung Spieler
				
				while (!welt.Motor) {
					Panzer_Engine_Drive.play(1.0, 1.0, true);
					welt.Motor = true;
					break;
				}

				welt.Set_Welt_Speed(5);

				spieler_1.move();

				stein_1.move(welt.Get_Speed());
				
				if (spieler_1.Get_Score() > 3000) {
					stein_2.move(welt.Get_Speed());
				}
				if (spieler_1.Get_Score() > 5000) {
					stein_3.move(welt.Get_Speed());
				}
				if (spieler_1.Get_Score() > 10000) {
					stein_4.move(welt.Get_Speed());
				}
				if (spieler_1.Get_Score() > 15000) {
					stein_5.move(welt.Get_Speed());
				}
				
				// Bewegung der Welt

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
				spieler_1.Score_Hoch(welt.Get_Speed());

				
				if (spieler_1.is_hit(stein_1)|| spieler_1.is_hit(stein_2)|| spieler_1.is_hit(stein_3)|| spieler_1.is_hit(stein_4)|| spieler_1.is_hit(stein_5)) {
					// Leben abziehen 
					spieler_1.Ein_Leben_Weg(); 

					//Gegenstand weg Porten
					stein_1.x_pos = Gosu::random(0, screen_width);
					stein_1.y_pos = 0;

					// Wenn leben =0 Game Over
					if (spieler_1.Get_Leben() <= 0) {
						welt.GameOver = true;
						
					}
				}

			}
			else if (!welt.GameOver)
			{
				welt.Set_Welt_Speed(0);
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
				ofstream outputFile(score_file); //File öffnen
				if (outputFile.is_open()) { // Hat das öffnen der Datei geklappt ? 


					//Abfrage ob neuer Score > Alter Highscore
					if(Highscore < spieler_1.Get_Score()){
					outputFile << spieler_1.Get_Score() << endl; //Schreiben in Datei 
					outputFile.close();// File schließen
					}
					else {	// Alten Highscore in Datei schreiben falls neuer Score zu wenig
						outputFile << Highscore << endl; 
						outputFile.close();// File schließen
					}
				}
				else {
					cout << "Fehler beim Öffnen der Datei zum Schreiben." << endl;
				}



				close(); // Beendet das Spiel.
			}
			//Neues Spiel kann mit "N" gestartet werden.
			if (Gosu::Input::down(Gosu::KB_N))
			{
				spieler_1.Set_Score_to_Zero();
				spieler_1.Max_Leben();
				welt.GameOver = false;
				welt.Start = false; 
				welt.Motor = false; 
			}


		}

	
	
};



// C++ Hauptprogramm
int main()
{
	

	// Score auslesen:

	ifstream inputFile(score_file); 

	if (!inputFile.is_open()) {
		cout << "Fehler beim init des Scores"; 
	}
	
	inputFile >> Highscore; // Daten auf Highscore speichern 
	inputFile.close();
	

	GameWindow window;
	
	window.show();
	
	
}
