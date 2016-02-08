#THOMAS-Protokoll (Version 1.5)
Protokoll zur Kommunikation zwischen dem Arduino und dem THOMAS-Server auf Byteebene.
Dabei sendet der THOMAS-Server in bestimmten Abständen Anfragen entsprechend dem Protokoll an den Arduino und bekommt die dazugehörigen Antworten als Byte-Array zurück.

##THOMAS => Arduino

Aufbau einer Übermittlung:
```
<byte> Paketlänge
<byte[Paketlänge]> Paket
```

Zusammensetzung eines Paketes:
```
<byte> Anfrage
	0 => Heartbeat
	{
		Rückgabe: <byte> Bestätigung: 1
	}

	1 => Meldung ausgeben
	{
		<byte> Priorität (0 - 2)
		<byte> Textlänge
		<byte[Textlänge * Länge char]> Text
		Rückgabe: <byte> Textlänge
	}

	2 => Sensoren ansprechen
	{
		<byte> Sensortyp
			0 => Ultraschallsensoren
			{
				<byte> Befehl
					0 => Messwerte abrufen
					{
						Rückgabe: 5x <byte> Messwert in cm pro Sensor (Eine Einheit entspricht 2cm)
					}
			}
	}

	3 => Aktoren ansprechen
	{
		<byte> Gerät
			0 => Kameraservo(s)
			{
				<byte> Servo-ID (0 - /)
				<byte> Befehl
					0 => Drehe auf Wert
					{
						<byte> Position in grad
						Rückgabe: <byte> Neue Position in grad
					}

					1 => Drehe um Wert
					{
						<byte> Richtung (0 -> Nach links | 1 -> Nach rechts)
						<byte> Änderungswert in grad
						Rückgabe: <byte> Neue Position in grad
					}
			}
	}

	4 => Status setzten
	{
		<byte> Eigenschaft
			0 => Wlan-SSID
			{
				<byte> Textlänge
				<byte[Textlänge * Länge char]> Neue SSID
				Rückgabe: <byte> Bestätigung: 1
			}

			1 => Wlan-Stärke
			{
				<byte> Signalstärke (0-100%)
				Rückgabe: <byte> Bestätigung: 1
			}
	}

	5 => Minimalmodus
	{
		<byte> Aktiv
		Rückgabe: <byte> Bestätigung: 1
	}

	6 => Entfernungsmessung
	{
		<byte> Winkel (0-180)
		<byte> Anzahl Messungen des kleinen Sensors
		<byte> Anzahl Messungen des großen Sensors
		Rückgabe: <byte[Anzahl Messungen]> Messwerte
	}
}
```

##Arduino => THOMAS
Aufbau einer Übermittlung:
```
<byte> Paketlänge
<byte[Paketlänge]> Paket
```

Zusammensetzung eines Paketes:
```
<byte> Anfrage
	0 => Start erfolgreich
	{
		Server-Software auf THOMAS startet.
	}
}
```
