// ++++++++++++++++++++<[ DEFINITIONEN ]>++++++++++++++++++++
// Adresse des Porterweiterungsbausteins
#define LCD_IIC_addr 0x20

// Port für die Hintergrundbeleuchtung
#define LCD_backlight_pin 7

// Ports für die Ansteuerung des LCDs
#define LCD_En_pin 4
#define LCD_Rw_pin 5
#define LCD_Rs_pin 6
#define LCD_D4_pin 0
#define LCD_D5_pin 1
#define LCD_D6_pin 2
#define LCD_D7_pin 3

// ++++++++++++++++++++<[ VARIABELN ]>++++++++++++++++++++
// Objekt für Kommunikation mit LCD erstellen
LiquidCrystal_I2C lcd(LCD_IIC_addr, LCD_En_pin, LCD_Rw_pin, LCD_Rs_pin, LCD_D4_pin, LCD_D5_pin, LCD_D6_pin, LCD_D7_pin);

// ++++++++++++++++++++<[ FUNKTIONEN ]>++++++++++++++++++++
// Initialisierung
void LCD_init()
{
  // LCD initialisieren (Breite: 20, Höhe: 4)
  lcd.begin(20, 4);
  
  // Hintergrundpin setzen
  lcd.setBacklightPin(LCD_backlight_pin, NEGATIVE);
  
  // Hintergrundbeleuchtung einschalten
  lcd.setBacklight(LED_ON);
}

// LCD leeren
void LCD_clear()
{
  // Anzeige löschen
  lcd.clear();
  lcd.home();
}

// Auf Display zeichnen
void LCD_print_float(int x, int y, float value)
{
 // Cursor positionieren
 lcd.setCursor(x, y);
 
 // Auf Display schreiben
 lcd.print(value, DEC);
}

// Auf Display zeichnen
void LCD_print_string(int x, int y, String value)
{
 // Cursor positionieren
 lcd.setCursor(x, y);
 
 // Auf Display schreiben
 lcd.print(value);
}
