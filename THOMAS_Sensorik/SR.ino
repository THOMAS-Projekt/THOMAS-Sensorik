// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Baudrate für serielle Schnittstelle
#define SR_baud 9600

// Maximale Zeit für eine Paketübertragung
#define SR_timeout 1000

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Die aktuelle SSID
String status_ssid = "?";

// Die aktuelle Signalstärke
String status_strength = "?";

// Zeitpunkt des letzten Heartbeats
unsigned long heartbeat_time = 0;

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
	SR_reply(0);
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
			warning("Empfangen der Daten fehlgeschlagen!");
		}
	}
	else
	{
		// Nein => Fehlermeldung
		warning("Ungueltige Paketlaenge");
	}
}

// Ein Paket an den PC senden
void SR_send(char package[], unsigned int package_length)
{
	// Paketlänge gültig?
	if(package_length > 0 && package_length < 256)
	{
		// Paketlänge senden
		Serial.write(package_length);

		// Package Array senden
		for(int i = 0; i < (int) package_length; i++){
			Serial.write(package[i]);
		}
	}
	else
	{
		// Nein => Fehlermeldung
		warning("Ungueltige Paketlaenge");
	}
}

// Funktion zum Senden eines Integers (0-255)
void SR_reply(int data)
{
	// Das Array für die Antwort
	char resp[1];

	// Eintrag prüfen
	if (data > 255)
	{
		// Fehlermeldung
		error("Fehler #001");
	}

	// Eintrag übernehmen
	resp[0] = (char) data;

	// Daten senden
	SR_send(resp, 1);
}

// Erhaltene Daten verarbeiten
void SR_parse(char package[], unsigned int package_length)
{
	// Daten verarbeiten
	switch(package[0])
	{
		// Heartbeat
		case 0:
		{
			// Erster Heartbeat?
			if(heartbeat_time == 0)
			{
				// Ja => Motorsteuerung einschalten
				RL_set_motor_control(RELAY_ON);
			}

			// Zeitpunkt merken
			heartbeat_time = millis();

			// Antworten
			SR_reply(1);

			// Fertig!
			break;
		}

		// Meldung ausgeben
		case 1:
		{
			// Paketlänge prüfen
			if (package_length < 3)
			{
				// Fehlermeldung
				error("Fehler #002");
			}

			// Priorität abrufen
			int priority = package[1];

			// Textlänge abrufen
			int text_length = package[2];

			// Text abrufen
			char text[text_length];

			// Paketlänge prüfen
			if ((int)package_length != text_length + 3)
			{
				// Fehlermeldung
				error("Fehler #003");
			}

			// Textangabe durchlaufen
			for(int i = 0; i< text_length; i++)
			{
				// Zeichen übernehmen
				text[i] = package[i + 3];
			}

			// Zwischen Prioritäten wählen
			switch(priority)
			{
				// Information
				case 0:
				{
					// Ausgeben
					info(text);

					// Fertig!
					break;
				}

				// Warnung
				case 1:
				{
					// Ausgeben
					warning(text);

					// Fertig!
					break;
				}

				// Fehler
				case 2:
				{
					// Ausgeben
					error(text);

					// Fertig!
					break;
				}

				// Ungültig
				default:
				{
					// Fehlermeldung
					warning("Ungueltige Prioritaet!");

					// Fertig
					break;
				}
			}

			// Antworten
			SR_reply (text_length);

			// Fertig!
			break;
		}

		// Sensoren ansprechen
		case 2:
		{
			// Paketlänge prüfen
			if (package_length < 2)
			{
				// Fehlermeldung
				error("Fehler #004");
			}

			// Zwischen Sensortypen unterscheiden
			switch(package[1])
			{
				// Ultraschallsensor
				case 0:
				{
					// Paketlänge prüfen
					if (package_length < 4)
					{
						// Fehlermeldung
						error("Fehler #005");
					}

					// Sensor-ID abrufen
					int us_id = package[2];

					// Welcher Befehl?
					switch(package[3])
					{
						// Status aktualisieren
						case 0:
						{
							// Status aktualisieren
							US_reload_stat(us_id);

							// Status senden
							SR_reply(US_get_stat(us_id) ? 1 : 0);

							// Fertig!
							break;
						}

						// Status abrufen
						case 1:
						{
							// Status senden
							SR_reply(US_get_stat(us_id) ? 1 : 0);

							// Fertig!
							break;
						}

						// Messwert abrufen
						case 2:
						{
							// Messung durchführen (Eine Einheit entspricht 2cm.)
							int val = US_get_cm(us_id) / 2;

							// Wert zu groß?
							if(val / 2 > 255)
							{
								// Korrigieren
								val = 255;
							}

							// Antworten
							SR_reply(val);

							// Fertig!
							break;
						}

						// Ungültig
						default:
						{
							// Fehlermeldung
							warning("Ungueltiger Befehl!");

							// Fertig!
							break;
						}
					}

					// Fertig!
					break;
				}

				// Ungültig
				default:
				{
					// Fehlermeldung
					warning("Ungueltiger Sensortyp!");

					// Fertig
					break;
				}
			}

			// Fertig!
			break;
		}

		// Aktoren ansprechen
		case 3:
		{
			// TODO

			// Fertig!
			break;
		}

		// Status setzen
		case 4:
		{
			// Paketlänge prüfen
			if (package_length < 2)
			{
				// Fehlermeldung
				error("Fehler #006");
			}

			// Eigenschaft wählen
			switch(package[1])
			{
				// Wlan-SSID
				case 0:
				{
					// Paketlänge prüfen
					if (package_length < 3)
					{
						// Fehlermeldung
						error("Fehler #007");
					}

					// Länge der SSId abrufen
					int ssid_length = package[2];

					// Char-Array für die SSID
					char ssid[ssid_length];

					// Paketlänge prüfen
					if ((int) package_length != ssid_length + 3)
					{
						// Fehlermeldung
						error("Fehler #008");
					}

					// Zeichen durchlaufen
					for(int i = 0; i < ssid_length; i++)
					{
						// Zeichen übernehmen
						ssid[i] = package[i + 3];
					}

					// Status setzen
					status_ssid = ssid;

					// Antworten
					SR_reply(1);

					// Fertig!
					break;
				}

				// Wlan-Stärke
				case 1:
				{
					// Paketlänge prüfen
					if (package_length < 3)
					{
						// Fehlermeldung
						error("Fehler #009");
					}

					// Signal-Stärke abrufen
					int wlan_strength = package[2];

					// Status setzen
					status_strength = String(wlan_strength) += "%";

					// Wert der Statusanzeige setzen
					ST_set_strength_bar(wlan_strength);

					// Antworten
					SR_reply(1);

					// Fertig!
					break;
				}

				// Ungültig
				default:
				{
					// Fehlermeldung
					warning("Ungueltige Status-Eigenschaft!");

					// Fertig
					break;
				}
			}

			// Fertig!
			break;
		}

		// Ungültig
		default:
		{
			// Fehlermeldung
			warning("Ungueltige Anfrage!");

			// Fertig!
			break;
		}
	}
}

// SSID abrufen
String SR_get_ssid()
{
	// SSID zurückgeben
	return status_ssid;
}

// Signalstärke abrufen
String SR_get_strength()
{
	// Signalstärke zurückgeben
	return status_strength;
}

// Heartbeat prüfen
void SR_check_heartbeat()
{
	// Heartbeat schon gestartet?
	if(heartbeat_time != 0)
	{
		// Ja => Wie lange liegt der Heartbeat zurück?
		if(millis() - 2000 > heartbeat_time)
		{
			// Zu lange => Fehler
			error("Ich verblute...");

			// Motorsteuerung deaktivieren
			RL_set_motor_control(RELAY_OFF);

			// Signalstärkenanzeige ausschalten
			ST_set_strength_bar (0);
		}
	}
}
