// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Port des Kamera-Servos
#define SV_cam_port 2

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Kamera-Servo
Servo SV_cam;

// Position des Kamera-Servos in Grad
int SV_cam_degree = 105;

// ++++++++++++++++++++<[ FUNKTIONEN ]>++++++++++++++++++++
// Initialisierung
void SV_init()
{
	// Kamera-Servo konfigurieren
	SV_cam.attach(SV_cam_port);

	// Kamera-Servo auf Standardwert drehen
	SV_cam_set_degree(SV_cam_degree);
}

// Servo-position setzen
int SV_cam_set_degree(int value)
{
	// Wert korrigieren
	value = SV_validate_degree(value);

	// Position setzen
	SV_cam.write(value);

	// Variable aktualisieren
	SV_cam_degree = value;

	// Wert zurückgeben
	return SV_cam_degree;
}

// Servo-position erhöhen
int SV_cam_increase_degree(int value)
{
	// Wert draufaddieren und Ergebnis korrigieren
	value = SV_validate_degree(SV_cam_degree + value);

	// Position setzen
	SV_cam.write(value);

	// Variable aktualisieren
	SV_cam_degree = value;

	// Wert zurückgeben
	return SV_cam_degree;
}

// Servo-position verkleinern
int SV_cam_decrease_degree(int value)
{
	// Wert abziehen und Ergebnis korrigieren
	value = SV_validate_degree(SV_cam_degree - value);

	// Position setzen
	SV_cam.write(value);

	// Variable aktualisieren
	SV_cam_degree = value;

	// Wert zurückgeben
	return SV_cam_degree;
}

// Korrigiert den Grad-Wert
int SV_validate_degree(int value)
{
	// Wert korrigieren
	value = value < 5 ? 5 : value;
	value = value > 180 ? 180 : value;

	// Wert zurückgeben
	return value;
}
