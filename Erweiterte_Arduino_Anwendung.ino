void setup() {
  Serial.begin(9600);
  while (!Serial);
}

// Unterstützung für Konstanten und Fließkommazahlen
float parseOperand(const String& str, bool& ok) {
  ok = true;
  if (str == "pi") return 3.14159;        // Einführung mathematischer Konstanten    
  if (str == "e") return 2.71828;
  if (str == "mu") return 1.25664e-6;
  if (str == "eps") return 8.85419e-12;

  float val = str.toFloat();            
  if (val < 0 || val > 1000) {
    ok = false;
  }
  return val;
}

void loop() {
  if (!Serial.available()) return;

  String input = Serial.readStringUntil('\n');
  input.trim();

  int index = -1;
  char op = 0;
  for (char c : "+-*/") {
    index = input.indexOf(c);
    if (index != -1) {
      op = c;
      break;
    }
  }

  if (index == -1) {
    Serial.println("Kein gültiger Operator (+, -, *, /)");
    return;
  }

  String teil1 = input.substring(0, index);
  String teil2 = input.substring(index + 1);
  teil1.trim();
  teil2.trim();

  if (teil1.length() == 0 || teil2.length() == 0) {
    Serial.println("Einer oder beide Operanden fehlen");
    return;
  }
  
  bool ok1, ok2;                        
  float zahl1 = parseOperand(teil1, ok1); // parseOperand liefert float
  float zahl2 = parseOperand(teil2, ok2);

  if (!ok1 || !ok2) {
    Serial.println("Operanden müssen zwischen 0 und 1000 liegen oder gültige Konstanten (pi, e, mu, eps) sein");
    return;
  }

  float ergebnis = 0;
  bool fehler = false;

  switch (op) {
    case '+': ergebnis = zahl1 + zahl2; break;
    case '-': ergebnis = zahl1 - zahl2; break;
    case '*': ergebnis = zahl1 * zahl2; break;
    case '/':
      if (zahl2 == 0) {
        Serial.println("Division durch 0 ist nicht erlaubt");
        fehler = true;
      } else {
        ergebnis = zahl1 / zahl2;
      }
      break;
    default:
      Serial.println("Unbekannter Rechenoperator");
      fehler = true;
      break;
  }

  if (!fehler) {
    Serial.print(input + "=");
    Serial.println(ergebnis, 5);     // Mehr Nachkommastellen bei Ausgabe

  }
}
