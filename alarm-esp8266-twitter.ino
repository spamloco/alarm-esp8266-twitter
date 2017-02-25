// Programa ESP8266 con sensor de movimiento
// Envía alertas por Twitter

#include <ESP8266WiFi.h>
#include <Twitter.h>

// Conexión WiFi
const char* ssid = "tu-red-wifi";
const char* password = "password-de-tu-wifi";

byte sensorPin = 2; // Pin GPIO2 para el sensor de movimiento
byte movimiento = 0; // Variable que activa alarma según cantidad de movimientos detectados
long Numerorandom; // Número ramdom para enviar por Twitter
char msg[140]; // Mensaje para Twitter
Twitter twitter("xxxxxxxxxxxxxxx"); // Token de Twitter que se obtiene aquí arduino-tweet.appspot.com

void setup()
{
  pinMode(sensorPin,INPUT);
  Serial.begin(115200);

  // Conectando a la WiFi
  Serial.println();
  Serial.print("Contectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado de forma correca a ");
  Serial.println(ssid);

}

void loop()
{
  byte state = digitalRead(sensorPin);

  if(state == 1) {
    Serial.println("Se ha detectado movimiento!");
    movimiento++;
    delay(1000);
    Serial.println(movimiento);
  }
  else if (state == 0) {
    delay(1500);
    Serial.println("Sin movimiento!");
  }

// Se activa la alarma y se envía el mensaje a Twitter

  if (movimiento == 5) { // Se activa cuando se detectan 5 movimientos

    Numerorandom = random(101,1565);
    Serial.println(Numerorandom);

// Envía el Tweet junto a un número random para que los tweets sean diferentes
// Si son siempre iguales Twitter los bloquea

  sprintf(msg, "Alarma spamloco activada!: %d.", Numerorandom);

// Muestra el mensaje en el Puerto Serie
   Serial.println(msg);
   Serial.println("Conectando a Twitter…");

      if (twitter.post(msg)) {
        int status = twitter.wait(&Serial);
        if (status == 200) {
        Serial.println("200 OK");
        delay(60001); // Si el mensaje se envía se espera 1 minuto para evitar spam en arduino-tweet.appspot.com
      } 
      else {
        Serial.print("Error : code ");
        Serial.println(status);
      }
  } else {
    Serial.println("Fallo en la conexión.");
  }

movimiento = 0; // Variable de movimientos detectados vuelve a cero

  }
}
