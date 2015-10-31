// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Port des kleinen Entfernungsmessers
#define IRD_sensor_small_port 1

// Port des großen Entfernungsmessers
#define IRD_sensor_large_port 0

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++


// ++++++++++++++++++++<[ FUNKTIONEN ]>++++++++++++++++++++
// Messwert abrufen (0 => Kleiner Sensor, 1 => Großer sensor)
int IRD_get_distance(int sensor)
{
	// Messung durchführen
	double value = analogRead (sensor == 0 ? IRD_sensor_small_port : IRD_sensor_large_port);

	// Spannung am Port bestimmen
	double voltage = value * (5.0 / 1023.0);

	// Zwischen Sensoren unterscheiden
	if (sensor == 0)
	{
		// Distanz errechnen und zurückgeben
		return (1 / ((0.003804 * pow (voltage, 2)) + (0.007976 * voltage) + 0.00351));
	}
	else
	{
		// Distanz errechnen und zurückgeben
		return (1 / ((-0.001393 * pow (voltage, 2)) + (0.013334 * voltage) -0.014978));
	}
}
