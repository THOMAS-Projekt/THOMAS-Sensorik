// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Port für die Datenleitung des IR-Sensors
#define IR_rec_port 44

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Infrarot-Empfänger
IRrecv irrecv(IR_rec_port);

// Decoder
decode_results results;

// ++++++++++++++++++++<[ FUNKTIONEN ]>++++++++++++++++++++
// Initialisierung
void IR_init()
{
	// Empfänger starten
	irrecv.enableIRIn();
}

// Daten empfangen
int IR_read()
{
	// Daten überprüfen
	if(irrecv.decode(&results))
	{
		// Daten vorhanden => Daten zwischenspeichern
		int IR_data = results.value;

		// Einen Moment warten
		delay (300);

		// Auf nächsten Wert vorbereiten
		irrecv.resume();

		// Daten zurückgeben
		return IR_data;
	}
	else
	{
		// Keine Daten vorhanden => NULL zurückgeben
		return 0;
	}
}

// Liest evtl. eingegangene IR-Signale aus, verarbeitet diese und gibt falls das Kommando verstanden wurde true zurück.
bool IR_process()
{
	// Daten abrufen und in einen hexadezimalen String konvertieren
	String data = String(IR_read(), HEX);

	// Daten empfangen => Inhalt verarbeiten
	if(data == "c2a6" || data == "5830" || data == "285b" || data == "1285b" || data == "275b" || data == "1275b")
	{
		// Nächste Menüseite
		MU_next_page();
	}
	else if(data == "c439" || data == "59c3" || data == "285a" || data == "1285a"|| data == "275a" || data == "1275a")
	{
		// Vorherige Menüseite
		MU_previous_page();
	}
	else if(data == "c74a" || data == "8adc" || data == "280c" || data == "1280c" || data == "270c" || data == "1270c")
	{
		// Motorsteuerung umschalten
		RL_toggle_motor_control();
	}
	else if(data == "87fe" || data == "fab0" || data == "2820" || data == "12820" || data == "2720" || data == "12720")
	{
		// Servo nach rechts drehen
		SV_cam_increase_degree(10);
	}
	else if(data == "4d8f" || data == "606d" || data == "2821" || data == "12821" || data == "2721" || data == "12721")
	{
		// Servo nach links drehen
		SV_cam_decrease_degree(10);
	}
	else
	{
		// Kommando nicht erkannt => False zurückgeben
		return false;
	}

	return true;
}
