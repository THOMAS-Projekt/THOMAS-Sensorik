// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Port für die Datenleitung des IR-Sensors
#define IR_rec_port 11

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Infrarot-Empfänger
IRrecv irrecv(IR_rec_port);

// Decoder
decode_results results;

// ++++++++++++++++++++<[ FUNKTIONEN ]>++++++++++++++++++++
// Initialisierung
void IR_init()
{
  // Empfänger starten
  irrecv.enableIRIn();
}

// Daten empfangen
int IR_read()
{
  // Daten überprüfen
  if(irrecv.decode(&results))
  {
    // Daten vorhanden => Daten zwischenspeichern
    int IR_data = results.value;
    
    // Nächsten Wert empfangen
    irrecv.resume();
    
    // Daten zurückgeben
    return IR_data;
  }
  else
  {
    // Keine Daten vorhanden => NULL zurückgeben
    return 0; 
  }
}
