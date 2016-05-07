#include <SoftwareSerial.h>
#include <TinyGPS.h>

int gpspin=8;
int gpsstate=0;

SoftwareSerial mySerial(10, 11); // RX, TX
TinyGPS gps;

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);



void setup()  
{
  Serial.begin(9600);
  
  

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  
  delay(1000);
  Serial.print("Testing TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println();
  Serial.print("Sizeof(gpsobject) = "); Serial.println(sizeof(TinyGPS));
  Serial.println();
  pinMode(gpspin,INPUT);
}

void loop() // run over and over
{
  bool newdata = false;
  unsigned long start = millis();
  gpsstate=digitalRead(gpspin);

  // Every 5 seconds we print an update
  while (millis() - start < 2000) {
    if (mySerial.available()) {
      char c = mySerial.read();
      // Serial.print(c);  // uncomment to see raw GPS data
      if (gps.encode(c)) {
        newdata = true;
        // break;  // uncomment to print new data immediately!
      }
    }
  }
  
  if (newdata) {
    Serial.println("Acquired Data");
    Serial.println("-------------");
    gpsdump(gps);
    Serial.println();
    Serial.println(gpsstate);    
  }
 
}

void gpsdump(TinyGPS &gps)
{
  long lat, lon;
  float flat, flon;
  unsigned long age, date, time, chars;
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned short sentences, failed;

if(gpsstate==HIGH){
  Serial.println("Emergency Situation!");
  gps.get_position(&lat, &lon, &age);
  Serial.print("Lat/Long(10^-5 deg): "); Serial.print(lat); Serial.print(", "); Serial.print(lon); 
  Serial.print(" Fix age: "); 


  gps.f_get_position(&flat, &flon, &age);
  Serial.print("Lat/Long(float): "); printFloat(flat, 5); Serial.print(", "); printFloat(flon, 5);

  gps.get_datetime(&date, &time, &age);
  
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);


  gps.stats(&chars, &sentences, &failed);
}
else{
  }
}

void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0) {
     Serial.print('-');
     number = -number;
  }

  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);

  if (digits > 0)
    Serial.print("."); 

  while (digits-- > 0) {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint;
  }
}








