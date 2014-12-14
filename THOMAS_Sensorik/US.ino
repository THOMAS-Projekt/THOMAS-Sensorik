// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Trigger-Port
#define US_trig_pin 36

// Sensor-Timeout in µs
#define US_timeout 250000

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Echo-Ports
int US_echo_pins[] = {44, 3, 4, 5, 6};

// Anzahl der Sensoren (Darf 256 nicht übersteigen, sonst Protokollanpassung erforderlich!)
int US_count = 5;

// Bezeichnungen
String US_names[] = {"Sensor 1", "Sensor 2", "Sensor 3", "Sensor 4", "Sensor 5"};

// Definiert den Status jedes Sensors (Standardwert: HW_DEFEKT)
boolean US_stat[] = {HW_DEFEKT, HW_DEFEKT, HW_DEFEKT, HW_DEFEKT, HW_DEFEKT};

// Zwischenspeicher für die Zeitmessung
long US_duration;

// ++++++++++++++++++++<[ FUNKTIONEN ]>++++++++++++++++++++
// Sensoren initialisieren
void US_init()
{
	// Den Trigger-Pin als Ausgang setzen
	pinMode(US_trig_pin, OUTPUT);

	// Alle Sensoren durchlaufen
	for(int i = 0; i < US_count; i++)
	{
		// Echo-Pin als Eingang setzen
		pinMode(US_echo_pins[i], INPUT);
	}
}

// Entfernung (in cm) des angegebenen Sensors zum gegenüberliegendem Objekt abrufen
long US_get_cm(int index)
{
	// Index überprüfen
	if(index < US_count)
	{
		// Existiert => Digitales Signal an alle Sensoren senden
		digitalWrite(US_trig_pin, LOW);
		delayMicroseconds(2);
		digitalWrite(US_trig_pin, HIGH);
		delayMicroseconds(10);
		digitalWrite(US_trig_pin, LOW);

		// Die länge des Rückgabe-Impulses messen
		US_duration = pulseIn(US_echo_pins[index], HIGH, US_timeout);

		// Impulslänge in eine Distanz umrechnen und diese zurückgeben (Schallgeschwindigkeit: 340 m/s, 29 µs/cm)
		return US_duration / 58;
	}
	else
	{
		// Sensor nicht definiert => Fehlerhaften Wert zurückgeben
		return 0;
	}
}

// Prüft welche Sensoren funktionieren und welche Defekt scheinen
void US_reload_all()
{
	// Alle Sensoren durchlaufen
	for(int i = 0; i < US_count; i++)
	{
		// Sensor prüfen
		US_reload_stat(i);
	}
}

// Prüft den Sensor auf Funktionalität
void US_reload_stat(int index)
{
	// Prüfen ob der Sensor auf Anfragen reagiert
	if(US_get_cm(index) == 0)
	{
		// Sensor antwortet nicht => Defekt
		US_stat[index] = HW_DEFEKT;
	}
	else
	{
		// Sensor sendet brauchbare Antwort => alles OK
		US_stat[index] = HW_OK;
	}
}

// Gibt den Status des angegebenen Sensors zurück
boolean US_get_stat(int index)
{
	// Index überprüfen
	if(index < US_count)
	{
		// Status zurückgeben
		return US_stat[index] == HW_OK;
	}
	else
	{
		return false;
	}
}

// Ruft die Anzahl an Sensoren ab
int US_get_count()
{
	// Sensoranzahl zurückgeben
	return US_count;
}
