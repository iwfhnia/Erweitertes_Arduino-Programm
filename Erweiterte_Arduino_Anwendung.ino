void setup() {
  Serial.begin(9600);           // Starte serielle Kommunikation mit 9600 Baud
  while (!Serial);              // Warte, bis die serielle Schnittstelle bereit ist
}

bool parseOperand(const String& str, int& val) {  //Hilfsfunktion zur Umwandlung und Prüfung des Strings:
  val = str.toInt();            // Wandle String in int um (Wert ´0´ bei Fehler)
  return val >= 0 && val <= 1000; // Prüfe, ob val im Bereich 0–1000 liegt
}

void loop() {
  if (!Serial.available())      // Prüft, ob neue Daten eingetroffen sind. Wenn nicht...
    return;                     //...wird die loop()-Funktion beendet und startet von vorn

  String input = Serial.readStringUntil('\n'); // Lese Eingabe bis Zeilenumbruch
  input.trim();                 // Entferne Leerzeichen aus dem Eingabe-String

  int index = -1;               // Position des Operators (-1: anfangs nicht gefunden)
  char op = 0;                  // Gefundener Operator (0: anfangs kein Zeichen)

  for (char c : "+-*/") {       // Suche nach erstem Operator in Eingabe
    index = input.indexOf(c);   //Suche nach Operator:
    if (index != -1) {          // Operator gefunden
      op = c;                   // Speichere Operator
      break;                    // Beenden der Operatorsuche
    }
  }

  if (index == -1) {            // Kein Operator gefunden?
    Serial.println("Fehler: Kein gültiger Operator"); // Fehlermeldung ausgeben
    return;                     // loop neu starten
  }

  String teil1 = input.substring(0, index);    // extrahieren des ersten Operanden 
  String teil2 = input.substring(index + 1);   // extrahieren des zweiten Operanden
  teil1.trim();                                // Leerzeichen entfernen
  teil2.trim();                                // Leerzeichen entfernen

  if (teil1.length() == 0 || teil2.length() == 0) { // Prüfung auf vollständige Operanden
    Serial.println("Fehler: Operanden fehlen");     // Fehlermeldung bei fehlendem Operanden
    return;                                         // loop neu starten
  }

  int zahl1, zahl2;             // Variablen für Operanden als int
  if (!parseOperand(teil1, zahl1) || !parseOperand(teil2, zahl2)) { // Umwandlung von ´teil1,2´ in ´Zahl1,2´ und Prüfung des Wertebereichs
    Serial.println("Fehler: Operanden müssen zwischen 0 und 1000 liegen"); // Fehlermeldung, wenn Operanden außerhalb des gültigen Wertebereichs liegen
    return;                     //loop neu starten
  }

  float ergebnis = 0;           // Ergebnisvariable (float für Division)
  bool fehler = false;          // Flag für Kennzeichnung/ Feststellung von Fehlern

  switch (op) {                 // Auswahl der Rechenoperation:
    case '+':
      ergebnis = zahl1 + zahl2; // Addition
      break;
    case '-':
      ergebnis = zahl1 - zahl2; // Subtraktion
      break;
    case '*':
      ergebnis = zahl1 * zahl2; // Multiplikation
      break;
    case '/':
      if (zahl2 == 0) {         // Prüfung auf Nullteilung
        Serial.println("Fehler: Division durch 0"); // Fehlerausgabe bei Nullteilung
        fehler = true;          // Flag auf true: Fehler aufgetreten
      } else {                  // liegt keine Nullteilung vor:
        ergebnis = (float)zahl1 / zahl2; // Division ausführen
      }
      break;
    default:                    // liegt kein gültiger Operator vor:
      Serial.println("Fehler: Unbekannter Operator"); // Fehlerausgabe
      fehler = true;          // Flag auf true: Fehler aufgetreten
      break;
  }

  if (!fehler) {                // Ist kein Fehler aufgetreten:
    Serial.println(input + "=" + ergebnis); // Ausgabe der Eingabezeile (input) und des zugehörigen berechneten Ergebnisses
  }
}