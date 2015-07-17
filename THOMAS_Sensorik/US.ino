// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Trigger-Port
#define US_trig_pin 33

// Sensor-Timeout in µs
#define US_timeout 100000

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Echo-Ports
int US_echo_pins[] = {49, 35, 37, 53, 51}; // Sechster: 31

// Anzahl der Sensoren (Darf 256 nicht übersteigen, sonst Protokollanpassung erforderlich!)
int US_count = 5;

// Gibt die Ergebnisse der letzten Messungen an
long US_last_cm[] = {0, 0, 0, 0, 0};

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

		// Impulslänge in eine Distanz umrechnen (Schallgeschwindigkeit: 340 m/s, 29 µs/cm)
		long val = US_duration / 58;

		// Messung merken
		US_last_cm[index] = val;

		// Messwert zurückgeben
		return val;
	}
	else
	{
		// Sensor nicht definiert => Fehlerhaften Wert zurückgeben
		return 0;
	}
}

// Wert letzter Messung abrufen
long US_get_last_cm(int index)
{
	// Index überprüfen
	if(index < US_count)
	{
		// Letzten Messwert zurückgeben
		return US_last_cm[index];
	}
	else
	{
		// Sensor nicht definiert => Fehlerhaften Wert zurückgeben
		return 0;
	}
}

// Die Entfernung des angegebenen Sensors als String formatieren und zurückgeben
String US_get_str(int index)
{

	// Wert der letzten Messung abrufen
	long val = US_last_cm[index];

	// Formatierten String zurückgeben
	return String(val) + "cm";
}

// Ruft die Anzahl an Sensoren ab
int US_get_count()
{
	// Sensoranzahl zurückgeben
	return US_count;
}
