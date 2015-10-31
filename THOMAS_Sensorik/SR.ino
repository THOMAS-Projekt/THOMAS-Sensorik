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
int heartbeat_time = 0;

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
	// Erstes Lebenszeichen?
	if(heartbeat_time != 0)
	{
		// Nein => Lebenszeichen merken
		SR_heartbeat_received();
	}

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
	if (data > 255 || data < 0)
	{
		// Fehlermeldung
		critical("Fehler #001");
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
			// Ein Lebenszeichen!
			SR_heartbeat_received();

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
				critical("Fehler #002");
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
				critical("Fehler #003");
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
				critical("Fehler #004");
			}

			// Zwischen Sensortypen unterscheiden
			switch(package[1])
			{
				// Ultraschallsensor
				case 0:
				{
					// Paketlänge prüfen
					if (package_length < 3)
					{
						// Fehlermeldung
						critical("Fehler #005");
					}

					// Welcher Befehl?
					switch(package[2])
					{
						// Messwert abrufen
						case 0:
						{
							// Byte-Array für die Rückgabe
							char resp[US_get_count()];

							// Sensoren durchlaufen
							for (int index = 0; index < US_get_count(); index++) {
								// Messung durchführen (Eine Einheit entspricht 2cm.)
								int val = US_get_last_cm(index) / 2;

								// Antwort ergänzen
								resp[index] = val > 255 ? 255 : val < 0 ? 0 : val;
							}

							// Antworten
							SR_send(resp, US_get_count());

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
			// Paketlänge prüfen
			if (package_length < 2)
			{
				// Fehlermeldung
				critical("Fehler #010");
			}

			// Zwischen Aktoren unterscheiden
			switch(package[1])
			{
				// Kameraservo(s)
				case 0:
				{
					// Paketlänge prüfen
					if (package_length < 4)
					{
						// Fehlermeldung
						critical("Fehler #011");
					}

					// Kamera-ID abrufen
					int cam_id = package[2];

					// Welcher Befehl?
					switch(package[3])
					{
						// Drehe auf Wert
						case 0:
						{
							// Paketlänge prüfen
							if (package_length < 5)
							{
								// Fehlermeldung
								critical("Fehler #012");
							}

							// Position abrufen
							int degree = package[4];

							// Position ändern
							int new_deg = SV_cam_set_degree(degree);

							// Neue Position korrigieren zurückgeben
							SR_reply(degree < 0 ? 0 : degree > 255 ? 255 : degree);

							// Fertig!
							break;
						}

							// Drehe um Wert
						case 1:
						{
							// Paketlänge prüfen
							if (package_length < 6)
							{
								// Fehlermeldung
								critical("Fehler #013");
							}

							// Richtung abrufen
							int direction = package[4];

							// Position abrufen
							int degree = package[5];

							// Die neue Position
							int new_deg = 0;

							// Richtung?
							switch(direction)
							{
								// Nach links
								case 0:
								{
									// Position ändern
									new_deg = SV_cam_decrease_degree(degree);

									// Fertig!
									break;
								}

								// Nach rechts
								case 1:
								{
									// Position ändern
									new_deg = SV_cam_increase_degree(degree);

									// Fertig!
									break;
								}

								// Ungültig
								default:
								{
									// Fehlermeldung
									warning("Ungueltige Richtungsangabe!");

									// Fertig!
									break;
								}
							}

							// Neue Position korrigieren zurückgeben
							SR_reply(new_deg < 0 ? 0 : new_deg > 255 ? 255 : new_deg);

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

		// Status setzen
		case 4:
		{
			// Paketlänge prüfen
			if (package_length < 2)
			{
				// Fehlermeldung
				critical("Fehler #006");
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
						critical("Fehler #007");
					}

					// Länge der SSId abrufen
					int ssid_length = package[2];

					// Char-Array für die SSID
					char ssid[ssid_length];

					// Paketlänge prüfen
					if ((int) package_length != ssid_length + 3)
					{
						// Fehlermeldung
						critical("Fehler #008");
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
						critical("Fehler #009");
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

		// Minimalmodus
		case 5:
		{
			// Paketlänge prüfen
			if (package_length < 2)
			{
				// Fehlermeldung
				critical("Fehler #010");
			}

			// Minimalmodus setzen
			minimal_mode = package[1];

			// Antworten
			SR_reply(1);

			// Infomeldung
			info ("Minimalmodus aktiviert.");

			// Fertig!
			break;
		}

		// Entfernungsmessung
		case 6:
		{
			// Paketlänge prüfen
			if (package_length < 4)
			{
				// Fehlermeldung
				critical("Fehler #011");
			}

			// Winkel abrufen
			int angle = package[1];

			// Anzahl Messungen des kleinen Sensors abrufen
			int small_measurement_count = package[2];

			// Anzahl Messungen des großen Sensors abrufen
			int large_measurement_count = package[3];

			// Gesamtanzahl der Messwerte bestimmen.
			int measurement_count = small_measurement_count + large_measurement_count;

			// Das Array für die Antwort
			char resp[measurement_count];

			// Servo drehen
			SV_cam_set_degree (angle);

			// Messungen durchzählen
			for (int i = 0; i < measurement_count; i++)
			{
				// Messung durchführen
				int distance = IRD_get_distance (i < small_measurement_count ? 0 : 1);
				
				// Messwert validieren und in Antwort speichern
				resp[i] = distance < 0 ? 0 : distance > 255 ? 255 : distance;
			}

			// Antwort senden
			SR_send (resp, measurement_count);

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

// Heartbeat verarbeiten
void SR_heartbeat_received()
{
	// Erster Heartbeat?
	if(heartbeat_time == 0)
	{
		// Ja => Motorsteuerung einschalten
		RL_set_motor_control(RELAY_ON);
	}

	// Zeitpunkt merken
	heartbeat_time = now();
}

// Heartbeat prüfen
void SR_check_heartbeat()
{
	// Heartbeat schon gestartet?
	if(heartbeat_time != 0)
	{
		// Ja => Wie lange liegt der Heartbeat zurück?
		if(heartbeat_time > -1 && now() > heartbeat_time + 2)
		{
			// Zu lange => Fehler
			error("Der Herzschlag setzt aus, ich verblute... :(");

			// Motorsteuerung deaktivieren
			RL_set_motor_control(RELAY_OFF);

			// Signalstärkenanzeige ausschalten
			ST_set_strength_bar (0);

			// Heartbeat-Überprüfung deaktivieren
			heartbeat_time = -1;
		}
	}
}
