// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Port des ersten Relais
#define RL_relay_1 23

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Status der Motorsteuerung
bool RL_motor_control_status = RELAY_OFF;

// ++++++++++++++++++++<[ FUNKTIONEN ]>++++++++++++++++++++
// Initialisierung
void RL_init()
{
	// Port als Ausgang setzen
	pinMode(RL_relay_1, OUTPUT);

	// Standardwert setzen
	RL_set_motor_control(RL_motor_control_status);
}

// Motorsteuerung ein-/ausschalten
void RL_set_motor_control(bool status)
{
	// Relay schalten
	digitalWrite(RL_relay_1, status);

	// Status merken
	RL_motor_control_status = status;
}

// Motorsteuerung umschalten
void RL_toggle_motor_control()
{
	// Status invertieren
	RL_set_motor_control(!RL_motor_control_status);
}

// Status der Motorsteuerung abrufen
bool RL_get_motor_control_status()
{
	// Status zurückgeben
	return RL_motor_control_status;
}
