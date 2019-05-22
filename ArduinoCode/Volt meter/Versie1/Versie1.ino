const int analogIn = A0;//potentiometer attach to A0
#define AANTAL_METINGEN  10
float input_volt = 0.0;
float temp=0.0;
float factor= 1.02;
float voltages[AANTAL_METINGEN];
uint8_t voltagesIndex;
float som; 
boolean arrayGevuld;
float gemiddeldeVoltage;

/****************************************************/

void setup()

{
for (uint8_t i = 0; i < 10; i++) 
{
   voltages[i] = 0;
}
som = 0;
voltagesIndex = 0;
arrayGevuld = false;
gemiddeldeVoltage = 0.0;
Serial.begin(9600);//Initialize the serial 

}

/****************************************************/

void loop()

{

    int analogvalue = analogRead(A0);
    temp = (analogvalue * 5.0) / 1024.0;       // FORMULA USED TO CONVERT THE VOLTAGE
    //input_volt = temp / (r2/(r1+r2));
    input_volt = temp / factor;

    // eerst de oudste waarde van de som aftrekken
    //
    som = som - voltages[voltagesIndex];  
    // 
    // voeg de laatste meting toe aan de tabel
    //
    voltages[voltagesIndex] = input_volt;

    som = som + voltages[voltagesIndex];

    if (arrayGevuld)
    {   
   //
   // bij een volledig gevulde tabel
   //
      gemiddeldeVoltage = som / AANTAL_METINGEN;
    }
    
    else 
    {
   // 
   // als de tabel nog niet vol is
   //
      gemiddeldeVoltage = som / (voltagesIndex + 1);
    }
    //
    // verhoog de metingen index
    //
    voltagesIndex = voltagesIndex + 1;
    //
    // test voor loop around
    //
    if (voltagesIndex == AANTAL_METINGEN)
    {
      voltagesIndex = 0;
      arrayGevuld = true;
    }

    Serial.print("v= ");                 // prints the voltage value in the serial monitor
    Serial.println(input_volt);
    Serial.print("average v= ");                 // prints the voltage value in the serial monitor
    Serial.println(gemiddeldeVoltage);
 
delay(500);

}
