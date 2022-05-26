#include <math.h>
#include <Adafruit_PCD8544.h>
#include <util/delay.h>
#include <PID_v1.h>

const int Rc = 10000; //valor de la resistencia
const int Vcc = 5;
const int temp_sensor = A0;
const int hum_sensor = A1;
const int selector = A2;

float A = 0.0006973550913078027;
float B = 0.00028800736970464863;
float C = 5.400097451986799e-9;
float temp_set = 0;
float hum = 0;
float celsius = 0;
double setpoint, input, output;

PID thePID(&input, &output, &setpoint, 0, 10, 100, DIRECT);

float K = 2.5; //factor de disipacion en mW/C

Adafruit_PCD8544 display = Adafruit_PCD8544(7,5, 6, 4, 8);

void display_screen(){
  display.setCursor(0,0);
  display.print("T.set: ");
  display.println(temp_set);
  display.print("Temp: ");
  display.println(celsius);
  display.print("Humid %: ");
  display.println(hum);
  display.display();
  display.clearDisplay();
  }

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  display.begin();
  display.setContrast(75);
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  thePID.SetMode(AUTOMATIC);
}

void loop() 
{
  float raw_temp = analogRead(temp_sensor);
  float raw_hum = analogRead(hum_sensor);
  float raw_temp_set = analogRead(selector);
  hum = map(raw_hum, 0, 1023, 0, 200);
  temp_set = map(raw_temp_set, 0, 1023, 0, 120);
  // Config PID meta value
  if(temp_set < 30){
    temp_set = 30;
    } 
  if(temp_set > 42){
    temp_set = 42;
    }
  setpoint = temp_set;
  Serial.print("Temperatura Meta: ");
  Serial.print(temp_set);
  Serial.print("\n");
  Serial.print("Humedad %: ");
  Serial.println(hum);
  // Calculate the temp value
  float V =  raw_temp / 1024 * Vcc;
  float R = (Rc * V ) / (Vcc - V);
  float logR  = log(R);
  float R_th = 1.0 / (A + B * logR + C * logR * logR * logR );
  float kelvin = R_th - V*V/(K * R)*1000;
  celsius = kelvin - 273.15;
  input = celsius;
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
  thePID.Compute();
  analogWrite(A3, output);
  Serial.print("T = ");
  Serial.print(celsius);
  Serial.print("C\n");
  display_screen();
  delay(4000);
}
