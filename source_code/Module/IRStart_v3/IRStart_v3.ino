#include <IRremote.h>
#include <EEPROM.h>

const int RECV_PIN = A1;

IRrecv irrecv(RECV_PIN);
decode_results results;

#define BOTON A0
#define LED_ON A4
#define LED_OFF A5

bool lectura_boton = 0;
bool modulo_ready = 0;
bool salida = 0;
byte contador = 0;
unsigned long comando_ready = 0;
unsigned long comando_on = 0;
unsigned long comando_off = 0;
unsigned long recivido = 0;
long millisAnterior = 0;
long millisAnterior2 = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(BOTON, INPUT_PULLUP);
  pinMode(LED_ON, OUTPUT);
  pinMode(LED_OFF, OUTPUT);
  irrecv.enableIRIn();

  EEPROM.get(100, comando_ready); 
  EEPROM.get(200, comando_on); 
  EEPROM.get(300, comando_off); 
  millisAnterior = millis();
}

void loop() {
  lectura_boton = !digitalRead(BOTON);
  if (irrecv.decode(&results)) {
    recivido = (results.value);
    if (recivido == 4294967295) {
      recivido = 0;
    }
    irrecv.resume();
  }

  if (lectura_boton == true) {
    if ((millis() - millisAnterior) > 1000) {
      contador = 1;
    }
  } else {
    millisAnterior = millis();
  }



  switch (contador) {

    case 0:  //Codigo de funcionamiento encender y apagar con el mando normalmente

      if (modulo_ready == false) {
        digitalWrite(LED_ON, HIGH);
        digitalWrite(LED_OFF, HIGH);
      } else if (salida == true) {
        digitalWrite(LED_ON, HIGH);
        digitalWrite(LED_OFF, LOW);
      } else if (salida == false) {
        digitalWrite(LED_ON, LOW);
        digitalWrite(LED_OFF, HIGH);
      }

      if (recivido > 0) {
        if (comando_ready == recivido) {
          modulo_ready = true;
          salida = false;
        } else if (modulo_ready == true && comando_off == recivido) {
          salida = false;
        } else if (modulo_ready == true && comando_on == recivido) {
          salida = true;
        }
      }
      break;
    case 1:  //Del case 1 en adelante codigo de grabado de señal de mando
      digitalWrite(LED_ON, LOW);
      digitalWrite(LED_OFF, LOW);
      delay(600);
      digitalWrite(LED_ON, HIGH);
      digitalWrite(LED_OFF, HIGH);
      delay(600);
      digitalWrite(LED_ON, LOW);
      digitalWrite(LED_OFF, LOW);
      delay(600);
      digitalWrite(LED_ON, HIGH);
      digitalWrite(LED_OFF, HIGH);
      delay(600);
      digitalWrite(LED_ON, LOW);
      digitalWrite(LED_OFF, LOW);
      delay(600);
      digitalWrite(LED_ON, HIGH);
      digitalWrite(LED_OFF, HIGH);
      delay(600);
      digitalWrite(LED_ON, LOW);
      digitalWrite(LED_OFF, LOW);
      delay(600);
      contador = 2;
      break;

    case 2:
      if (recivido > 0 ) {
        comando_ready = recivido;
        Serial.print("comando_ready: ");
        Serial.println(comando_ready);
        EEPROM.put( 100, recivido );
        recivido = 0;
        contador = 3;
      }
      break;
    case 3:
      digitalWrite(LED_ON, HIGH);
      digitalWrite(LED_OFF, HIGH);
      delay(1250);
      digitalWrite(LED_ON, LOW);
      digitalWrite(LED_OFF, LOW);
      contador = 4;
      break;
    case 4:
      if (recivido > 0 && recivido != comando_ready) {
        comando_on = recivido;
        Serial.print("comando_on: ");
        Serial.println(comando_on);
        EEPROM.put( 200, recivido );
        recivido = 0;
        contador = 5;
      }
      break;
    case 5:
      digitalWrite(LED_ON, HIGH);
      delay(1250);
      digitalWrite(LED_ON, LOW);
      contador = 6;
      break;
    case 6:
      if (recivido > 0 && recivido != comando_ready && recivido != comando_on) {
        comando_off = recivido;
        Serial.print("comando_off: ");
        Serial.println(comando_off);
        EEPROM.put( 300, recivido );
        recivido = 0;
        contador = 7;
      }
      break;
    case 7:
      digitalWrite(LED_OFF, HIGH);
      delay(1250);
      digitalWrite(LED_OFF, LOW);
      contador = 8;
      break;
    case 8:
      digitalWrite(LED_ON, HIGH);
      digitalWrite(LED_OFF, HIGH);
      delay(250);
      digitalWrite(LED_ON, LOW);
      digitalWrite(LED_OFF, LOW);
      delay(250);
      digitalWrite(LED_ON, HIGH);
      digitalWrite(LED_OFF, HIGH);
      delay(250);
      digitalWrite(LED_ON, LOW);
      digitalWrite(LED_OFF, LOW);
      delay(250);
      digitalWrite(LED_ON, HIGH);
      digitalWrite(LED_OFF, HIGH);
      delay(250);
      digitalWrite(LED_ON, LOW);
      digitalWrite(LED_OFF, LOW);
      delay(250);
      digitalWrite(LED_ON, HIGH);
      digitalWrite(LED_OFF, HIGH);
      delay(250);
      digitalWrite(LED_ON, LOW);
      digitalWrite(LED_OFF, LOW);
      delay(250);
      contador = 0;
      break;
  }

  recivido = 0;
}

