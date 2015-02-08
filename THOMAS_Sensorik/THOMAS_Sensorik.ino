// ++++++++++++++++++++<[ BIBLIOTHEKEN ]>++++++++++++++++++++
// Bibliothek für den Infrarotsensoren einbinden
#include <IRremote.h>

// Biblothek für die IIC-Kommunikation
#include <Wire.h>

// Bibliothek für die LCD-Ansteuerung per IIC
#include <LiquidCrystal_I2C.h>

// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// LED-Zustände
#define LED_OFF 0
#define LED_ON 1

// Relay-Zustände
#define RELAY_OFF 1
#define RELAY_ON 0

// Hardware-Zustände
#define HW_DEFEKT 0
#define HW_OK 1

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

// ++++++++++++++++++++<[ HAUPTPROGRAMM ]>++++++++++++++++++++
// Systemininitalisierung
void setup()
{
	// Serielle Schnittstelle initialisieren
	SR_init();

	// LCD initialisieren
	LCD_init();

	// Statusanzeige initialisieren
	ST_init();

	// Relais initialisieren
	RL_init();

	// Infrarot-Sensor initialisieren
	IR_init();

	// Ultraschall-Sensoren initialisieren
	US_init();

	// Ultraschallsensoren auf Funktionsfähigkeit prüfen
	US_reload_all();

	// Menü initialisieren
	MU_init();

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

	// Kurz warten
	delay(50);
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

// TODO: fatal
