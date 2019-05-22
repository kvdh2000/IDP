
//All methods
/*
 * setup
 * loop
 * convertxy
 * drive
 */

#define LED 13
#define X A5
#define Y A6

const String teststring = "05120512";
String dcinput;
double angle;
int intensity;
int dcpins[12] = {4, 26, 27, 5, 28, 29, 2, 22, 23, 3, 24, 25};
int jX = 512;
int jY = 512;

void setup()
{
  Serial.begin(9600);
  Serial.println("Arduino MEGA start");
  
  pinMode(LED, OUTPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);

  
  for(byte c = 0; c < 12; c++)
  {
    pinMode(dcpins[c], OUTPUT);
  }

  dcinput = teststring;
}

void loop()
{
  Serial.println("-------------------------------------------------");
  Serial.println();
  Serial.println("Restart loop");
  Serial.println();

  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);
  delay(50);

  readJoy();

  if(dcinput != "")
  {
    drive();
    dcinput = "";
  }

  //Serial.print(intensity);

  delay(500);
  Serial.println();
}

void convertxy()
{
  double x = dcinput.substring(0,4).toInt() - 512;
  double y = dcinput.substring(4).toInt() - 512;
  angle = atan2(y, x);
  int bigPI = 157;
  int otherthing = (100 * abs(angle));
  double itmpangle = (otherthing % bigPI);
  double tmpangle = itmpangle / 100;
  if(tmpangle >= M_PI * 0.25){
    tmpangle = (M_PI * 0.25) - (tmpangle - (M_PI * 0.25));
  }
  intensity = ((sqrt(pow(x,2) + pow(y,2))) * (512 / (512 / cos(tmpangle))));
}

void drive()
{
  convertxy();
  
  //Links
  if(angle <= M_PI * 0.75 && angle >= M_PI * -0.25)
  {
    digitalWrite(dcpins[1], HIGH);
    digitalWrite(dcpins[2], LOW);
    digitalWrite(dcpins[4], HIGH);
    digitalWrite(dcpins[5], LOW);
    if(angle <= M_PI * 0.5 && angle >= 0){
      analogWrite(dcpins[0], map(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[3], map(intensity, 0, 515, 0, 255));
    }
    else
    {
      if(angle >= M_PI * 0.5)
      {
        analogWrite(dcpins[0], intensity / 515 * map((M_PI * 0.75) - angle, 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[3], intensity / 515 * map((M_PI * 0.75) - angle, 0, 0.25 * M_PI, 0, 255));
      }
      else
      {
        analogWrite(dcpins[0], intensity / 515 * map(abs(angle), 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[3], intensity / 515 * map(abs(angle), 0, 0.25 * M_PI, 0, 255));
      }
    }
  }
  else
  {
    digitalWrite(dcpins[1], LOW);
    digitalWrite(dcpins[2], HIGH);
    digitalWrite(dcpins[4], LOW);
    digitalWrite(dcpins[5], HIGH);
    if(angle <= M_PI * -0.5)
    {
      analogWrite(dcpins[0], map(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[3], map(intensity, 0, 515, 0, 255));
    }
    else{
      
      if(angle >= M_PI * 0.75)
      {
        analogWrite(dcpins[0], intensity / 515 * map(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[3], intensity / 515 * map(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
      }
      else
      {
        analogWrite(dcpins[0], intensity / 515 * map(abs(0.25 * M_PI - abs(angle)), 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[3], intensity / 515 * map(abs(0.25 * M_PI - abs(angle)), 0, 0.25 * M_PI, 0, 255));
      }
    }
  }
  //Rechts
  if(angle <= 0.25 * M_PI && angle >= M_PI * -0.75)
  {
    digitalWrite(dcpins[7], LOW);
    digitalWrite(dcpins[8], HIGH);
    digitalWrite(dcpins[10], LOW);
    digitalWrite(dcpins[11], HIGH);
    if(angle >= M_PI * -0.5 && angle <= 0)
    {
      analogWrite(dcpins[6], map(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[9], map(intensity, 0, 515, 0, 255));
    }
    else
    {
      if(angle <= M_PI * -0.5)
      {
        analogWrite(dcpins[6], intensity / 515 * map(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[9], intensity / 515 * map(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
      }
      else
      {
        analogWrite(dcpins[6], intensity / 515 * map(angle, 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[9], intensity / 515 * map(angle, 0, 0.25 * M_PI, 0, 255));
      }
    }
  }
  else
  {
    digitalWrite(dcpins[7], HIGH);
    digitalWrite(dcpins[8], LOW);
    digitalWrite(dcpins[10], HIGH);
    digitalWrite(dcpins[11], LOW);
    if(angle >= M_PI * 0.5)
    {
      analogWrite(dcpins[6], map(intensity, 0, 515, 0, 255));
      analogWrite(dcpins[9], map(intensity, 0, 515, 0, 255));
    }
    else
    {
      if(angle <= M_PI * -0.75)
      {
        analogWrite(dcpins[6], intensity / 515 * map(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[9], intensity / 515 * map(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
      }
      else
      {
        analogWrite(dcpins[6], intensity / 515 * map(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[9], intensity / 515 * map(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
      }
    }
  }
}

void readJoy()
{
  jX = analogRead(X);
  jY = analogRead(Y);
  String Xx = String(jX);
  while(Xx.length() < 4)
  {
    Xx = 0 + Xx;
  }
  String Yy = String(jY);
  while(Yy.length() < 4)
  {
    Yy = 0 + Yy;
  }
  Serial.println(Xx);
  Serial.println(Yy);
  dcinput = Xx + Yy;
}
