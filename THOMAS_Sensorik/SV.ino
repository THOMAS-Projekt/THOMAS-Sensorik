// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Port des Kamera-Servos
#define SV_cam_port 2

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Position des Kamera-Servos in Grad
int SV_cam_degree = 0;

// ++++++++++++++++++++<[ FUNKTIONEN ]>++++++++++++++++++++
// Initialisierung
void SV_init()
{
	// Servo auf standardposition fahren
	SV_cam_set_degree (105);
}

// Servo-position setzen
int SV_cam_set_degree(int value)
{
	// Servo-Objekt erstellen
	Servo servo;

	// Kamera-Servo initialisieren
	servo.attach(SV_cam_port);

	// Wert korrigieren
	value = SV_validate_degree(value);

	// Position setzen
	servo.write(value);

	// Einen Moment warten, damit der Servo die Position annehmen kann (Für den Workaround leider nötig...)
	delay (15 * abs(SV_cam_degree - value));

	// Variable aktualisieren
	SV_cam_degree = value;

	// Servo-Verbindung wieder trennen (Auf diese weise kann das ständige Ruckeln verhindert werden)
	servo.detach ();

	// Wert zurückgeben
	return SV_cam_degree;
}

// Servo-position erhöhen
int SV_cam_increase_degree(int value)
{
	// Servo ansteuern und neuen Wert zurückgeben
	return SV_cam_set_degree(SV_cam_degree + value);
}

// Servo-position verkleinern
int SV_cam_decrease_degree(int value)
{
	// Servo ansteuern und neuen Wert zurückgeben
	return SV_cam_set_degree(SV_cam_degree - value);
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
