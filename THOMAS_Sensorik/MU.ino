// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Anzahl Zeilen
#define MU_rows 4

// Anzahl Spalten
#define MU_cols 20

// ++++++++++++++++++++<[ STRUKTUREN ]>++++++++++++++++++++
// Ein Text-Element
struct MU_OBJECT {
	// Die Seite auf der das Objekt angezeigt wird
	int page;

	// Der Text des Objektes
	String title;

	// Die Anzahl an zu verwendenen Spalten (width <= MU_cols
	int width;

	// Die Spalte an der gestartet wird
	int x;

	// Die Zeile des Objektes
	int y;

	// Die momentane Scrollposition
	int scroll_x;
};

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Zählt die Anzahl durchläufe
float loops = 0;

// Anzahl an Seiten
int page_count = 3;

// Die aktuelle Menü-Seite
int current_page;
	// 0 => Hauptmenü
	// 1 => Status
	// 2 => Ultraschallsensoren

// Anzahl an Objekten
int object_count = 22;

// Die Objekte {Seite, Text, Breite, X, Y, 0}
MU_OBJECT objects[] = {
		{0, "++++ MELDUNGEN +++++", 20, 0, 0, 0},
		{0, "!%MELDUNG0%!", 20, 0, 1, 0},
		{0, "!%MELDUNG1%!", 20, 0, 2, 0},
		{0, "!%MELDUNG2%!", 20, 0, 3, 0},
		{1, "++++++ STATUS ++++++", 20, 0, 0, 0},
		{1, "SSID: ", 6, 0, 1, 0},
		{1, "!%SSID%!", 14, 6, 1, 0},
		{1, "Signalstaerke: ", 15, 0, 2, 0},
		{1, "!%STRENGTH%!", 5, 15, 2, 0},
		{1, "Motorsteuerung: ", 16, 0, 3, 0},
		{1, "!%MOTOREN%!", 4, 16, 3, 0},
		{2, "+++ US-Sensoren ++++", 20, 0, 0, 0},
		{2, "FL: ", 4, 0, 1, 0},
		{2, "!%US_FL%!", 6, 4, 1, 0},
		{2, "FM: ", 4, 10, 1, 0},
		{2, "!%US_FM%!", 6, 14, 1, 0},
		{2, "FR: ", 4, 0, 2, 0},
		{2, "!%US_FR%!", 6, 4, 2, 0},
		{2, "BL: ", 4, 10, 2, 0},
		{2, "!%US_BL%!", 6, 14, 2, 0},
		{2, "BR: ", 4, 0, 3, 0},
		{2, "!%US_BR%!", 6, 4, 3, 0}
};

// Die drei neuesten Meldungen
String message0;
String message1;
String message2;

// ++++++++++++++++++++<[ FUNKTIONEN ]>++++++++++++++++++++
// Initialisierung
void MU_init()
{
	// Startseite wählen
	MU_set_page(0);
}

// Aktuelles Menü zeichnen
void MU_update(bool ignore_timer)
{
	// Nächster Frame?
	if(loops >= 1 || ignore_timer)
	{
		// Ja => Alle Objekte durchlaufen
		for(int i = 0; i < object_count; i++)
		{
			// Objekt abrufen
			MU_OBJECT obj = objects[i];

			// Auf aktuellem Menü anzeigen?
			if(obj.page == current_page)
			{
				// Ja => Text parsen
				String obj_title = MU_parse(obj.title);

				// String-Länge mit der Breite vergleichen
				if((int) obj_title.length() <= obj.width)
				{
					// Kein Lauftext => Text zeichnen
					LCD_print_string(obj.x, obj.y, obj_title);

					// Freie Stellen durchlaufen
					for(int j = 0; j < obj.width - obj_title.length(); j++)
					{
						// Stelle mit einem Leerzeichen überschreiben
						LCD_print_string(obj.x + obj_title.length() + j, obj.y, " ");
					}
				}
				else
				{
					// Lauftext => Text kürzen
					String content = obj_title.substring(obj.scroll_x, obj.scroll_x + obj.width);

					// Text zeichnen
					LCD_print_string(obj.x, obj.y, content);

					// Scrollposition prüfen
					if(obj.scroll_x >= (int)obj_title.length() - obj.width)
					{
						// Am Ende => Zum Anfang
						obj.scroll_x = 0;
					}
					else
					{
						// Noch nicht am Ende => Weiterscrollen
						obj.scroll_x++;
					}

					// Änderungen übernehmen
					objects[i] = obj;
				}
			}
		}

		// Zähler zurücksetzen
		loops = 0;
	}
	else
	{
		// Nein => Weiterzählen (Alle 25 Durchläufe einen Weiter: 1/25=0.04)
		loops += 0.04;
	}
}

// Text parsen
String MU_parse(String txt)
{
	// Variablen ersetzen
	txt.replace("!%MELDUNG0%!", message0);
	txt.replace("!%MELDUNG1%!", message1);
	txt.replace("!%MELDUNG2%!", message2);
	txt.replace("!%SSID%!", SR_get_ssid());
	txt.replace("!%STRENGTH%!", SR_get_strength());
	txt.replace("!%MOTOREN%!", RL_get_motor_control_status() ? "AUS" : "AN");
	txt.replace("!%US_FL%!", US_get_str(US_FRONT_LEFT));
	txt.replace("!%US_FM%!", US_get_str(US_FRONT_MIDDLE));
	txt.replace("!%US_FR%!", US_get_str(US_FRONT_RIGHT));
	txt.replace("!%US_BL%!", US_get_str(US_BACK_LEFT));
	txt.replace("!%US_BR%!", US_get_str(US_BACK_RIGHT));

	// Text zurückgeben
	return txt;
}

// Eine Meldung ausgeben
void MU_message(String message)
{
	// Neuesten zwei Nachrichten nach unten verschieben
	message2 = String(message1);
	message1 = String(message0);

	// Neue Nachricht nach oben
	message0 = String(message);

	// Auf Meldungsseite springen
	current_page = 0;
}

// Seite aufrufen
void MU_set_page(int new_page)
{
	// Existiert diese Seite?
	if(new_page < page_count)
	{
		// Display leeren
		LCD_clear();

		// Seite setzen
		current_page = new_page;
	}
}

// Nächste Seite
void MU_next_page()
{
	// Display leeren
	LCD_clear();

	// Am Ende angelangt?
	if(current_page < page_count - 1)
	{
		// Nein => Nächste Seite
		current_page++;
	}
	else
	{
		// Ja => Zurück zum Anfang
		current_page = 0;
	}
}

// Vorherige Seite
void MU_previous_page()
{
	// Display leeren
	LCD_clear();

	// Am Anfang angelangt?
	if(current_page > 0)
	{
		// Nein => Vorherige Seite
		current_page--;
	}
	else
	{
		// Ja => Zurück zum Ende
		current_page = page_count - 1;
	}
}

// Prüfen ob die Sensordaten-Seite geöffnet ist
boolean MU_is_sensor_data()
{
	// Seite prüfen
	return current_page == 2;
}
