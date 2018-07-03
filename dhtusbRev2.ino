
#define BLYNK_PRINT SwSerial


  #include <SoftwareSerial.h>
  SoftwareSerial SwSerial(2, 3 ); // RX, TX
      
  #include <BlynkSimpleStream.h>
  #include <DHT.h>
  
  int pump=8;
  int sensorValue;
  float h;
  float t;
  int solenoid=9;
  int high;
  int low;
const int ldrPin = A1;
//int up=0;
//int down=0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "900ed2409d2e45d7ba76f6ea0971c174";

#define DHTPIN 5          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
   sensorValue = analogRead(A0);
   h = dht.readHumidity();
   t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
   int ldrStatus = analogRead(ldrPin);
   Blynk.virtualWrite(V9,ldrStatus);
   Blynk.virtualWrite(V7, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V8,sensorValue);
   
   

  if (isnan(h) || isnan(t)) {
    SwSerial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
 
  
}


void setup()
{
  // Debug console
  
  pinMode(pump,OUTPUT);
  pinMode(solenoid,OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(ldrPin, INPUT);
  SwSerial.begin(9600);
  
  

  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}


BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  //if(param.asInt == 1)
  sol();
  
}

void sol(){
  if(digitalRead(solenoid)==HIGH)
  {
  digitalWrite(solenoid,LOW);
  }
  else if(digitalRead(solenoid)==LOW)
  {
    digitalWrite(solenoid,HIGH);
  }
}




void loop()
{
  high = digitalRead(10); //initialization 
  low = digitalRead(11);
   int ldrStatus = analogRead(ldrPin);

  if (ldrStatus>500)
  {
       if(high == LOW && low == LOW)
      {
      digitalWrite(solenoid,HIGH);
      Blynk.virtualWrite(V3,"Water level Low");
      Blynk.virtualWrite(V2,"Valve On");
      
     }
      else if(high == HIGH && low==HIGH)
     {
      digitalWrite(solenoid,LOW);
      Blynk.virtualWrite(V3,"Water Level High");
      Blynk.virtualWrite(V2,"Valve Off");
      delay(100);
     }
    
    
      sensorValue = analogRead(A0);
      if(sensorValue>900 || h<40.00 )
      {
        digitalWrite(pump,HIGH);
        Blynk.virtualWrite(V4,"Pump On");
        delay(100);
      }
      else if(sensorValue<600 || h>60.00 )
      {
        digitalWrite(pump,LOW);
        Blynk.virtualWrite(V4,"Pump Off");
        delay(100);
      }
       
  }
  else
  {
    digitalWrite(pump,LOW);
   digitalWrite(solenoid,LOW); 
   delay(100);
  }
   Blynk.run();
   timer.run();
   delay(100);
  
}


