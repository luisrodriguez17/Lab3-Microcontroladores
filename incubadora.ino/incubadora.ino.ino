#include <math.h>

const int Rc = 10000; //valor de la resistencia
const int Vcc = 5;
const int temp_sensor = A0;
const int hum_sensor = A1;
const int selector = A2;

float A = 0.0006973550913078027;
float B = 0.00028800736970464863;
float C = 5.400097451986799e-9;

float K = 2.5; //factor de disipacion en mW/C

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop() 
{
  float raw_temp = analogRead(temp_sensor);
  float raw_hum = analogRead(hum_sensor);
  float hum = map(raw_hum, 0, 1023, 0, 2000);
  float raw_temp_set = analogRead(selector);
  float temp_set = map(raw_temp_set, 0, 1023, 0, 120);
  // Config PID meta value
  if(temp_set < 30){
    temp_set = 30;
    } 
  if(temp_set > 42){
    temp_set = 42;
    }
  Serial.print("Temperatura Meta: ");
  Serial.print(temp_set);
  Serial.print("\n");
  Serial.print("Humedad: ");
  Serial.println(hum);
  // Calculate the temp value
  float V =  raw_temp / 1024 * Vcc;

  float R = (Rc * V ) / (Vcc - V);
  

  float logR  = log(R);
  float R_th = 1.0 / (A + B * logR + C * logR * logR * logR );

  float kelvin = R_th - V*V/(K * R)*1000;
  float celsius = kelvin - 273.15;

  if(celsius >= 42){
    digitalWrite(13, HIGH);
    }
  else{
    digitalWrite(13, LOW);
    }

  if(celsius <= 30){
    digitalWrite(12, HIGH);
    }
  else{
    digitalWrite(12, LOW);
    }

  Serial.print("T = ");
  Serial.print(celsius);
  Serial.print("C\n");
  delay(2500);
}
