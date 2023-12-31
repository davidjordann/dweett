#include <Arduino.h>
    #include <DHT.h>
    #include <WiFi.h>
    
    const char *ssid = "david12345";
    const char *pass = "juan12345";
    const char *host = "dweet.io";
    const int port = 80;
    
    DHT dht(DHTPIN, DHTTYPE);
    
    void setup() {
      Serial.begin(115200);
      dht.begin();
      WiFi.begin(ssid, pass);
      Serial.print("Intentando Conexion");
      while (WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(1000);
      }
      Serial.println("Conectado");
      Serial.print("Direccion IP: ");
      Serial.println(WiFi.localIP());
    }
    
    void loop() {
      delay(2000);
      float humedad = dht.readHumidity();
      float temperatura = dht.readTemperature();
      WiFiClient cliente;
    
      if (!cliente.connect(host, port))
      {
        Serial.println("Conexion Fallida al servidor");
        delay(1000);
        return;
      }
    
      if(isnan(humedad) || isnan(temperatura)) {
        Serial.println("Error al leer el sensor DHT11");
        return;
      }
    
      String request = String("GET ") + "/dweet/for/david?" + "Humedad=" + String(humedad) + "&" + "Temperature=" + String(temperatura) + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";
    
      cliente.print(request);
      unsigned long inicio = millis();
      while (cliente.available() == 0)
      {
        if (millis() - inicio > 8000)
        {
          Serial.println("Tiempo de espera del servidor agotado");
          cliente.stop();
          return;
        }
      }
    
      while (cliente.available()){
        String linea = cliente.readStringUntil('\r');
        Serial.println(linea);
      }
    
      Serial.println("Fin de conexion al servidor");
      cliente.stop();
      delay(5000);
    }
