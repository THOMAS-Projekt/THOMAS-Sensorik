// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Baudrate für serielle Schnittstelle
#define SR_baud 9600

// Maximale Zeit für eine Paketübertragung
#define SR_timeout 1000

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// TODO

// ++++++++++++++++++++<[ FUNKTIONEN ]>++++++++++++++++++++
// Initialisierung
void SR_init()
{
	// Serielle Schnittstelle initialisieren
	Serial.begin(SR_baud);

	// Timeout für das Warten auf Daten setzen
	Serial.setTimeout(SR_timeout);
}

// Start des Arduinos abgeschlossen
void SR_ready()
{
	// PC informieren
	SR_send({0}, 1);
}

// Daten vorhanden
void serialEvent()
{
	// Paketlänge abrufen
	unsigned int package_length = Serial.read();

	// Paketlänge gültig?
	if(package_length > 0)
	{
		// Gültiges Paket => Puffer für das Paket erstellen
		char package[package_length];

		// Daten empfangen und in den Puffer schreiben (wartet maximal SR_timeout millisekunden auf Daten)
		if(Serial.readBytes(package, package_length) == package_length)
		{
			// Alles empfangen => Daten verarbeiten
			SR_parse(package, package_length);
		}
		else
		{
			// Nicht gut..
			// TODO: Warnung ausgeben
		}
	}
}

// Ein Paket an den PC senden
void SR_send(byte package[], unsigned int package_length)
{
	// Paketlänge gültig?
	if(package_length > 0 && package_length < 256)
	{
		// Paketlänge senden
		Serial.write(package_length);

		// Paket senden
		if(Serial.write(package, package_length) != package_length) {
			// Da ist was schiefgelaufen
			// TODO: Fehler ausgeben
		}
	}
	else
	{
		// Nein
		// TODO: Meldung ausgeben
	}
}

// Erhaltene Daten verarbeiten
void SR_parse(char package[], unsigned int package_length)
{
	// Daten verarbeiten
	switch(package[0])
	{
		// Heartbeat
		case 0:
			SR_send({1}, 1);
			break;

		// Meldung ausgeben
		case 1:
			// TODO
			break;

		// Sensoren ansprechen
		case 2:
			// TODO
			break;

		// Aktoren ansprechen
		case 3:
			// TODO
			break;

		// Ungültig
		default:
			// TODO: Fehler ausgeben
			break;
	}
}
