
int displayPins[] = {A1, 2, 3, 4, 5, 6, 7}; 
int signPin = A0; 


int btnA = 8;
int btnB = 9;
int btnOperacion1 = 10;
int btnOperacion2 = 11;
int btnOperacion3 = 12;
int btnOperacion4 = 13;

int A = 0;
int B = 0;
int operacion = 0;

byte numbers[3][7] = {
  {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH},
  {HIGH, LOW, HIGH, LOW, LOW, LOW, LOW},
  {LOW, HIGH, HIGH, HIGH, LOW, HIGH, HIGH}
};

byte undefinedPattern[7] = {LOW, HIGH, LOW, LOW, LOW, LOW, LOW};

void setup() {
  Serial.begin(9600);
  Serial.println("En línea, esperando instrucciones...");

  for (int i = 0; i < 7; i++) {
    pinMode(displayPins[i], OUTPUT);
  }
  pinMode(signPin, OUTPUT);
  pinMode(btnA, INPUT);
  pinMode(btnB, INPUT);
  pinMode(btnOperacion1, INPUT);
  pinMode(btnOperacion2, INPUT);
  pinMode(btnOperacion3, INPUT);
  pinMode(btnOperacion4, INPUT);
}

void loop() {
  // Leer entradas de los botones
  A = digitalRead(btnA);
  B = digitalRead(btnB);
  int estadoOperacion1 = digitalRead(btnOperacion1);
  int estadoOperacion2 = digitalRead(btnOperacion2);
  int estadoOperacion3 = digitalRead(btnOperacion3);
  int estadoOperacion4 = digitalRead(btnOperacion4);

  if (A == 0 && B == 0 && estadoOperacion1 == LOW && estadoOperacion2 == LOW && estadoOperacion3 == LOW && estadoOperacion4 == LOW) {
    apagarDisplays();
    Serial.println("Displays apagados. Ninguna entrada detectada.");
  } else {
    operacion = (estadoOperacion1 << 3) | (estadoOperacion2 << 2) | (estadoOperacion3 << 1) | estadoOperacion4;
    ejecutarOperacion(operacion, A, B);
  }

  // Modo de operación mediante comandos
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n'); // Leer el comando ingresado
    comando.trim(); // Eliminar espacios en blanco adicionales

    if (comando == "sum") {
      operacion = 1;
      Serial.println("Ejecutando operación: Suma");
    } else if (comando == "res") {
      operacion = 2;
      Serial.println("Ejecutando operación: Resta");
    } else if (comando == "mul") {
      operacion = 3;
      Serial.println("Ejecutando operación: Multiplicación");
    } else if (comando == "div") {
      operacion = 4;
      Serial.println("Ejecutando operación: División");
    } else if (comando == "and") {
      operacion = 5;
      Serial.println("Ejecutando operación: AND");
    } else if (comando == "or") {
      operacion = 6;
      Serial.println("Ejecutando operación: OR");
    } else if (comando == "not") {
      operacion = 7;
      Serial.println("Ejecutando operación: NOT");
    } else if (comando == "xor") {
      operacion = 8;
      Serial.println("Ejecutando operación: XOR");
    } else if (comando == "xnor") {
      operacion = 9;
      Serial.println("Ejecutando operación: XNOR");
    } else if (comando == "nand") {
      operacion = 10;
      Serial.println("Ejecutando operación: NAND");
    } else {
      Serial.println("Comando no reconocido");
      operacion = 0;
    }
    ejecutarOperacion(operacion, A, B);
  }

  delay(3000); 
}

void ejecutarOperacion(int operacion, int A, int B) {
  int resultado = 0;
  bool negativo = false;

  switch (operacion) {
    case 1:  // Suma
      resultado = A + B;
      break;
    case 2:  // Resta
      resultado = A - B;
      if (resultado < 0) {
        resultado = -resultado;
        negativo = true;
      }
      break;
    case 3:  // Multiplicación
      resultado = A * B;
      break;
    case 4:  // División
      if (A == 0 && B == 0) {
        mostrarIndefinido();
        Serial.println("Resultado: Indefinido (0/0)");
        return;
      } else if (A == 1 && B == 0) {
        mostrarInfinito();
        Serial.println("Resultado: Infinito (1/0)");
        return;
      } else if (B != 0) {
        resultado = A / B;
      }
      break;
    case 5:  // AND
      resultado = A & B;
      break;
    case 6:  // OR
      resultado = A | B;
      break;
    case 7:  // NOT (solo aplica a A)
      resultado = !A;
      break;
    case 8:  // XOR
      resultado = A ^ B;
      break;
    case 9:  // XNOR
      resultado = !(A ^ B);
      break;
    case 10:  // NAND
      resultado = !(A & B);
      break;
    default:
      resultado = 0;
      break;
  }

  Serial.print("Resultado: ");
  if (negativo) {
    Serial.print("-");
  }
  Serial.println(resultado);

  mostrarNumero(resultado, negativo);
}

void mostrarNumero(int num, bool negativo) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(displayPins[i], numbers[num][i]);
  }
  digitalWrite(signPin, negativo ? HIGH : LOW);
}

void mostrarIndefinido() {
  apagarDisplays();
  digitalWrite(displayPins[0], HIGH);
}

void mostrarInfinito() {
  apagarDisplays();
  for (int i = 0; i < 7; i++) {
    digitalWrite(displayPins[i], undefinedPattern[i]);
  }
  digitalWrite(signPin, HIGH);
}

void apagarDisplays() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(displayPins[i], LOW);
  }
  digitalWrite(signPin, LOW);
}
