// Definición de Pines
const int xPin = A0;
const int yPin = A1;
const int enterPin = 2;

const int ledVerde = 7;
const int ledAmarillo = 8;
const int ledRojo = 9;
const int buzzerPin = 10;

// Variables de lectura
int xVal;
int yVal;
int statusButton;

void setup() {
  Serial.begin(9600);
  
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(enterPin, INPUT_PULLUP);

  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Secuencia de inicio: arpegio de encendido
  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledAmarillo, HIGH);
  digitalWrite(ledRojo, HIGH);
  
  int introNotas[] = {523, 659, 784, 1047};
  for (int i = 0; i < 4; i++) {
    tone(buzzerPin, introNotas[i], 70);
    delay(85);
  }
  delay(200);
  clearEffects();
}

void loop() {
  // 1. Leer estado del Joystick y enviarlo a Python
  xVal = analogRead(xPin);
  yVal = analogRead(yPin);
  statusButton = digitalRead(enterPin);

  Serial.println(String(xVal) + ";" + String(yVal));

  // 2. Leer comandos enviados por Python
  if (Serial.available() > 0) {
    char comando = Serial.read();
    procesarComando(comando);
  }

  delay(100);
}

void procesarComando(char comando) {
  switch (comando) {
    case 'A': // Secuencia de inicio estilo arcade ascendente
      for (int i = 0; i < 3; i++) {
        int salto = i * 250; // Sube de tono en cada vuelta
        digitalWrite(ledVerde, HIGH);
        tone(buzzerPin, 600 + salto, 70);
        delay(85);
        digitalWrite(ledVerde, LOW);

        digitalWrite(ledAmarillo, HIGH);
        tone(buzzerPin, 800 + salto, 70);
        delay(85);
        digitalWrite(ledAmarillo, LOW);

        digitalWrite(ledRojo, HIGH);
        tone(buzzerPin, 1000 + salto, 70);
        delay(85);
        digitalWrite(ledRojo, LOW);
      }
      noTone(buzzerPin);
      break;

    case 'G': // LED Verde + Sonido clásico de Moneda / Éxito
      clearEffects();
      digitalWrite(ledVerde, HIGH);
      efectoMoneda();
      digitalWrite(ledVerde, LOW);
      break;

    case 'Y': // LED Amarillo + Sonido de Llave / Puerta (Arpegio rápido)
      clearEffects();
      digitalWrite(ledAmarillo, HIGH);
      tone(buzzerPin, 587, 60);  // Re
      delay(70);
      tone(buzzerPin, 880, 60);  // La
      delay(70);
      tone(buzzerPin, 1175, 120); // Re agudo
      delay(130);
      digitalWrite(ledAmarillo, LOW);
      break;

    case 'R': // LED Rojo + Sonido de Daño / Trampa (Caída de frecuencia)
      clearEffects();
      digitalWrite(ledRojo, HIGH);
      efectoDano();
      digitalWrite(ledRojo, LOW);
      break;

    case 'O': // Apagar todo
      clearEffects();
      break;

    case 'C': // Sonido corto de moneda (sin luces)
      efectoMoneda();
      break;

    case 'D': // Sonido de daño (sin luces)
      efectoDano();
      break;

    case 'V': // Melodía de victoria
      victoria();
      break;

    case 'L': // Melodía de derrota
      derrota();
      break;

    default:
      break;
  }
}

// --- EFECTOS DE SONIDO RETRO ---

void efectoMoneda() {
  // Salto clásico de Si (B5) a Mi (E6)
  tone(buzzerPin, 988, 65);
  delay(70);
  tone(buzzerPin, 1319, 180);
  delay(185);
  noTone(buzzerPin);
}

void efectoDano() {
  // Barrido descendente rápido tipo explosión o golpe
  for (int freq = 480; freq >= 110; freq -= 35) {
    tone(buzzerPin, freq, 15);
    delay(15);
  }
  noTone(buzzerPin);
}

void clearEffects() {
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledRojo, LOW);
  noTone(buzzerPin);
}

void victoria() {
  clearEffects();

  // Fanfarria triunfal de 6 notas con pausas marcadas
  int notas[] = {523, 659, 784, 1047, 784, 1047};
  int duraciones[] = {120, 120, 120, 240, 120, 480};

  for (int i = 0; i < 6; i++) {
    digitalWrite(ledVerde, (i % 2 == 0) ? HIGH : LOW);
    digitalWrite(ledAmarillo, (i % 2 != 0) ? HIGH : LOW);
    digitalWrite(ledRojo, (i == 5) ? HIGH : LOW);
    
    tone(buzzerPin, notas[i], duraciones[i]);
    delay(duraciones[i] * 1.25); // El 25% extra separa las notas
  }

  noTone(buzzerPin);
  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledAmarillo, HIGH);
  digitalWrite(ledRojo, HIGH);
  delay(600);
  clearEffects();
}

void derrota() {
  clearEffects();

  // Descenso dramático estilo "Game Over"
  int notas[] = {392, 370, 349, 330};
  int duraciones[] = {220, 220, 220, 600};

  for (int i = 0; i < 4; i++) {
    digitalWrite(ledRojo, HIGH);
    tone(buzzerPin, notas[i], duraciones[i]);
    delay(duraciones[i]);
    digitalWrite(ledRojo, LOW);
    delay(65); // Pausa breve entre notas
  }

  noTone(buzzerPin);
  clearEffects();
}
