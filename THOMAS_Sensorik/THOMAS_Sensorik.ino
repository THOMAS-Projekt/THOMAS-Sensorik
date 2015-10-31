// ++++++++++++++++++++<[ BIBLIOTHEKEN ]>++++++++++++++++++++
// Bibliothek für Zeitoperationen
#include <Time.h>

// Bibliothek für den Infrarotsensoren einbinden
#include <IRremote.h>

// Biblothek für die IIC-Kommunikation
#include <Wire.h>

// Bibliothek für die LCD-Ansteuerung per IIC
#include <LiquidCrystal_I2C.h>

// Bibliothek zur Ansteuerung von Servo-Motoren
#include <Servo.h> 

// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// LED-Zustände
#define LED_OFF 0
#define LED_ON 1

// Relay-Zustände
#define RELAY_OFF 1
#define RELAY_ON 0

// Port-Expander-Zustände
#define IIC_OFF 1
#define IIC_ON 0

// Ultraschallsensoren
#define US_FRONT_RIGHT 0
#define US_FRONT_MIDDLE 1
#define US_FRONT_LEFT 2
#define US_BACK_RIGHT 3
#define US_BACK_LEFT 4

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Momentan geprüfter Sensor
int us_index = 0;

// Gibt an ob der Minimalmodus aktiviert ist.
bool minimal_mode = false;

// ++++++++++++++++++++<[ HAUPTPROGRAMM ]>++++++++++++++++++++
// Systemininitalisierung
void setup()
{
	// LCD initialisieren
	LCD_init();

	// Statusanzeige initialisieren
	ST_init();

	// Relais initialisieren
	RL_init();

	// Servos initialisieren
	SV_init();

	// Infrarot-Sensor initialisieren
	IR_init();

	// Ultraschall-Sensoren initialisieren
	US_init();

	// Menü initialisieren
	MU_init();

	// Serielle Schnittstelle initialisieren
	SR_init();

	// Bereit!
	SR_ready();
}

// Main-Schleife
void loop()
{
	// Infrarotsignale verarbeiten
	bool changed = IR_process();

	// Menü neu zeichnen
	MU_update(changed);

	// Heartbeat prüfen
	SR_check_heartbeat();

	// Prüfen ob der Minimalmodus gewählt ist
	if (!minimal_mode)
	{
		// Wert des aktuellen Ulatraschallsensors aktualisieren
		US_get_cm(us_index++ < US_get_count() ? us_index : us_index = 0);
	}
}

// Info
void info(String message)
{
	// Präfix
	String prfx = "INF: ";

	// Ausgabe
	MU_message(prfx + message);
}

// Warnung
void warning(String message)
{
	// Präfix
	String prfx = "WARN: ";

	// Ausgabe
	MU_message(prfx + message);
}

// Fehler
void error(String message)
{
	// Präfix
	String prfx = "ERR: ";

	// Ausgabe
	MU_message(prfx + message);
}

// Kritischer Fehler
void critical(String message)
{
	// Display leeren
	LCD_clear();

	// Titel zeichnen
	LCD_print_string (4, 0, "SYSTEMFEHLER");

	// Fehlermeldung zeichnen
	LCD_print_string (0, 2, message);

	// Programm anhalten
	while(true);
}
