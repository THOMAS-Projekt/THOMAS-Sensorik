// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// ID des ersten IIC-Modules
#define ST_IIC_1 0x3E

// ID des zweiten IIC-Modules
#define ST_IIC_2 0x3C

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Zustände der Ports
bool ST_ports[] = {IIC_OFF, IIC_OFF, IIC_OFF, IIC_OFF, IIC_OFF, IIC_OFF, IIC_OFF, IIC_OFF,
		IIC_OFF, IIC_OFF, IIC_OFF, IIC_OFF, IIC_OFF, IIC_OFF, IIC_OFF, IIC_OFF};

// ++++++++++++++++++++<[ FUNKTIONEN ]>++++++++++++++++++++
// Initialisierung
void ST_init()
{
	// Bibliothek initailisieren
	Wire.begin();

	// Standardwerte setzen
	ST_apply();
}

// Funktion zum setzen des Wertes der Signalstärkenanzeige
void ST_set_strength_bar(int percent) {
	// Alle LEDs des Balkens durchlaufen
	for(int i = 0; i < 10; i++)
	{
		// Zustand setzen
		ST_ports[i] = percent > i * 10 ? IIC_ON : IIC_OFF;
	}

	// Zustände übernehmen
	ST_apply();
}

// Funktion zum Setzen eines Portzustandes
void ST_set_port(int port, bool value)
{
	// Port setzen
	ST_ports[port] = value;
}

// Setzt die Ausgänge der ICs auf die angegebenen Zustände
void ST_apply()
{
	// Ports binär umrechnen und an das erste IIC-Modul senden
	ST_set_pins(ST_IIC_1,
			ST_ports[0] * 1+
			ST_ports[1] * 2 +
			ST_ports[2] * 4 +
			ST_ports[3] * 8 +
			ST_ports[4] * 16 +
			ST_ports[5] * 32 +
			ST_ports[6] * 64 +
			ST_ports[7] * 128);

	// Ports binär umrechnen und an das zweite IIC-Modul senden
	ST_set_pins(ST_IIC_2,
			ST_ports[8] * 1+
			ST_ports[9] * 2 +
			ST_ports[10] * 4 +
			ST_ports[11] * 8 +
			ST_ports[12] * 16 +
			ST_ports[13] * 32 +
			ST_ports[14] * 64 +
			ST_ports[15] * 128);
}

// Funktion zum setzen aller Zustände eines IIC-Modules.
void ST_set_pins(byte id, byte pins)
{
	// Neue Übertragung starten
	Wire.beginTransmission (id);

	// Port-Zustände setzen
	Wire.write (pins);

	// Ende der Übertragung
	Wire.endTransmission ();
}
