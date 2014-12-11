// Empfangene Daten Array mit der größe des empfangenen Arrays
byte receivedBytes[255];

// TODO: Code in die vorhandene Methode einfügen
void setup()
{
  
  // Serial initialisieren mit Boundrate von 9600
  Serial.begin(9600);
  
  // on-Signal senden
  send_upstate();
  
  
}

// Sende dem PC ein Status, dass der Arduino gestartet
void send_upstate()
{
  // Sende PC on-Signal
  Serial.write(1);
  
}

void loop()
{
  // Rufe analyze Data auf und verwalte die empfangenen Daten
  if(readSerial(receivedBytes) != -1){
      analyzeData(receivedBytes);
  }
  
}

int readSerial(byte byteArray[])
{
  // Befinden sich neue Daten im Buffer
  if(Serial.available() <= 0){
     return -1;
  }
  
  // Daten dürfen nicht größer als 127 sein.
  if(Serial.available() > 127){
    return -1;
  }
  
  // Länge des Byte Arrays (Byte - 48)
  int byteLength = Serial.read() - '0';
  
  // Setze die Länge des Arrays
  byteArray[0] = byteLength;

  for(int i = 0; i < byteLength; i++)
  {
      // Achtung!: Es werden chars gespeichert: 0 = 48, 1 = 49 etc...
      byteArray[i] = Serial.read();
  }
  
  return 0;
  
}

void analyzeData(byte byteArray[])
{
  
  switch(byteArray[1])
  {
    case '0':
      Serial.write(heartbeat());
      break; 
 
    case '1':
      Serial.write(writeMessage(byteArray)); 
  }
}

// Ping - Methode
int heartbeat()
{
   return 1; 
}

// Schreibt Nachricht auf Display
int writeMessage(byte data[])
{
  // TODO: Implementation
  int priority = data[2] - '0';
  int textLength = data[3] - '0';
  
  // Text Array mit Länge des Textes
  char text[textLength];
  
  for(int i = 0; i < textLength; i++)
  {
     // Byte Array in Text Array schreiben
     text[i] = data[i+4]; 
  }
  
  // TODO: text Array auf LCD anzeigen
  
  return textLength;
  
}
