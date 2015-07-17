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
	if(data == "cd42" || data == "48b4")
	{
		// Sensorzustände aktualisieren
		US_reload_all();
	}
	else if(data == "c2a6" || data == "5830")
	{
		// Nächste Menüseite
		MU_next_page();
	}
	else if(data == "c439" || data == "59c3")
	{
		// Vorherige Menüseite
		MU_previous_page();
	}
	else if(data == "c74a" || data == "8adc")
	{
		// Motorsteuerung umschalten
		RL_toggle_motor_control();
	}
	else if(data == "87fe" || data == "fab0")
	{
		// Servo nach rechts drehen
		SV_cam_increase_degree(10);
	}
	else if(data == "4d8f" || data == "606d")
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
