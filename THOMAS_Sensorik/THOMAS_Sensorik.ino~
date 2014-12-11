// ++++++++++++++++++++<[ BIBLIOTHEKEN ]>++++++++++++++++++++
// Bibliothek für den Infrarotsensoren einbinden
#include <IRremote.h>

// Biblothek für die IIC-Kommunikation
#include <Wire.h>

// Bibliothek für die LCD-Ansteuerung per IIC
#include <LiquidCrystal_I2C.h>

// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// LED-Zustände
#define LED_OFF 0
#define LED_ON 1

// Hardware-Zustände
#define HW_DEFEKT 0
#define HW_OK 1

// Baudrate für serielle Schnittstelle
#define SR_baud 9600

// Infrarot-Befehle
#define IR_mute 0x270D
#define IR_power 0x270C
#define IR_red 0x276D
#define IR_green 0x276E
#define IR_yellow 0x276F
#define IR_textup 0x2770
#define IR_fav 0x2784
#define IR_window 0x274B  // Funktion: Sensorstati aktualisieren
#define IR_onetwo 0x274E
#define IR_textdown 0x273C
#define IR_guide 0x27CC
#define IR_info 0x27CB
#define IR_timer 0x2775
#define IR_menu 0x2754
#define IR_top 0x2758
#define IR_back 0x2783
#define IR_left 0x275A
#define IR_OK 0x275C
#define IR_right 0x275B
#define IR_volumeup 0x2710
#define IR_bottom 0x2759
#define IR_channelup 0x2720
#define IR_volumedown 0x2711
#define IR_channeldown 0x2721
#define IR_one 0x2701
#define IR_two 0x2702
#define IR_three 0x2703
#define IR_four 0x2704
#define IR_five 0x2705
#define IR_six 0x2706
#define IR_seven 0x2707
#define IR_eight 0x2708
#define IR_nine 0x2709
#define IR_tvstb 0x2743
#define IR_zero 0x2700
#define IR_radio 0x27B0

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++

// ++++++++++++++++++++<[ HAUPTPROGRAMM ]>++++++++++++++++++++
// Systemininitalisierung
void setup()
{
  // Serielle Schnittstelle initialisieren
  Serial.begin(SR_baud);
  
  // LCD initialisieren
  LCD_init();
  
  // Infrarot-Sensor initialisieren
  IR_init();
  
  // Ultraschall-Sensoren initialisieren
  US_init();
  
  // Ultraschallsensoren auf Funktionsfähigkeit prüfen
  US_reload_stat();
}

// Main-Schleife
void loop()
{
  // Sensordaten verarbeiten und senden
  process_US();
  
  // Infrarotsignale verarbeiten
  process_IR();
  
  // TODO: Was uns sonst noch so einfällt ;)
}

// Liest alle Sensordaten aus und sendet diese
void process_US()
{
  // Alle Ultraschallsensoren durchlaufen
  for(int i = 0; i < US_get_count(); i++)
  {
    // Funktionsfähig?
    if(US_get_stat(i) == HW_OK)
    {
      // Sollte funktionieren => Sensordaten ankündigen
      Serial.write(1);
      
      // Sensortypen senden
      Serial.write(0);
      
      // Sensorzahl senden
      Serial.write(i);
      
      // Sensorwert abrufen und senden
      Serial.println(US_get_cm(i));
    }
  }
}

void check_US()
{
  // Ultraschallsensoren erneut auf Funktionsfähigkeit prüfen
  US_reload_stat();
  
  // Gibt an, ob alle funktionsfähig sind
  bool all_OK = true;
  
  // Alle Ultraschallsensoren durchlaufen
  for(int i = 0; i < US_get_count(); i++)
  {
    // Sensor defekt?
    if(US_get_stat(i) == HW_DEFEKT)
    {
      // Sensor nicht funktionsfähig => Mindestens einer defekt
      all_OK = false;
      
      // TODO: Warnung auf Display schreiben
    }
  }
  
  // Defekte Sensoren gefunden?
  if(!all_OK)
  {
    // Mindestens einer defekt => Fehlermeldung ankündigen
    Serial.write(0);
    
    // Meldungstyp: Fehler
    Serial.write(2);
    
    // Fehler: Mindestens ein Sensor defekt
    Serial.write(0);
  }
}

// Prüft auf Infrarot-Signale und verarbeitet diese
void proccess_IR()
{
  // Daten abrufen und in einen hexadezimalen String konvertieren
  String data = String(IR_read(), HEX);
  
  // Daten vorhanden?
  if(!data == 0)
  {
    // Daten empfangen => Inhalt verarbeiten
    switch(data)
    {
      // Sensorstatus aktualisieren
      case IR_window:
        // Ultraschallsensoren prüfen und evtl. Fehler ausgeben
        check_US();
        
        // Ende
        break;
    }
  }
}
