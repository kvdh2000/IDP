const int analogIn = A0;//potentiometer attach to A0
#define AANTAL_METINGEN  10
float input_volt = 0.0;
float temp=0.0;
float factor= 1.02;
float voltages[AANTAL_METINGEN];
uint8_t voltagesIndex = 0;
float som = 0; 
boolean arrayGevuld = false;
float gemiddeldeVoltage = 0.0;

void setup()
{
for (uint8_t i = 0; i < 10; i++) 
{
  voltages[i] = 0;
}

Serial.begin(9600);
}

void loop()
{   
  voltMeter();
  delay(500);
}

void voltMeter()
{
  int analogvalue = analogRead(A0);
  temp = (analogvalue * 5.0) / 1024.0;
  input_volt = temp / factor;
  som = som - voltages[voltagesIndex];  
  voltages[voltagesIndex] = input_volt;

  som = som + voltages[voltagesIndex];

  if (arrayGevuld)
  {
    gemiddeldeVoltage = som / AANTAL_METINGEN;
  }
  
  else 
  {
    gemiddeldeVoltage = som / (voltagesIndex + 1);
  }
  
  voltagesIndex = voltagesIndex + 1;
  
  if (voltagesIndex == AANTAL_METINGEN)
  {
    voltagesIndex = 0;
    arrayGevuld = true;
  }

  Serial.print("U = ");
  Serial.println(input_volt);
  Serial.print("Average U = ");
  Serial.println(gemiddeldeVoltage);
}
