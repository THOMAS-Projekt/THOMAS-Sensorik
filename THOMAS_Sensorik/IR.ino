// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Port für die Datenleitung des IR-Sensors
#define IR_rec_port 44

// Infrarot-Befehle
#define IR_mute "270d"
#define IR_power "270c"
#define IR_red "276d"
#define IR_green "276e"
#define IR_yellow "276f"
#define IR_textup "2770"
#define IR_fav "2784"
#define IR_window "274b" // Funktion: Zustände aktualisieren
#define IR_onetwo "274e"
#define IR_textdown "273c"
#define IR_guide "27cc"
#define IR_info "27cb"
#define IR_timer "2775"
#define IR_menu "2754"
#define IR_top "2758"
#define IR_back "2783"
#define IR_left "275a"
#define IR_OK "275c"
#define IR_right "275b"
#define IR_volumeup "2710"
#define IR_bottom "2759"
#define IR_channelup "2720"
#define IR_volumedown "2711"
#define IR_channeldown "2721"
#define IR_one "2701"
#define IR_two "2702"
#define IR_three "2703"
#define IR_four "2704"
#define IR_five "2705"
#define IR_six "2706"
#define IR_seven "2707"
#define IR_eight "2708"
#define IR_nine "2709"
#define IR_tvstb "2743"
#define IR_zero "2700"
#define IR_radio "27b0"

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
	if(data == IR_window)
	{
		// Sensorzustände aktualisieren
		US_reload_all();
	}
	else if(data == IR_right)
	{
		// Nächste Menüseite
		MU_next_page();
	}
	else if(data == IR_left)
	{
		// Vorherige Menüseite
		MU_previous_page();
	}
	else if(data == IR_power)
	{
		// Motorsteuerung umschalten
		RL_toggle_motor_control();
	}
	else
	{
		// Kommando nicht erkannt => False zurückgeben
		return false;
	}

	return true;
}
