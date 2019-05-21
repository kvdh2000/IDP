
const String teststring = "05120512";
String dcinput;
double angle;
int intensity;
const int x = 0;
const int y = 1;
int dcpins[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

void setup() {
  Serial.begin(9600);
  dcinput = teststring;
  convertxy();
  for(byte c = 0; c < 12; c++){
    pinMode(dcpins[c], OUTPUT);
  }
  drive();
}

void loop() {
  if(dcinput != NULL){
    convertxy();
    drive();
    dcinput = "";
  }
}

void drive(){
  //Links
  if(angle <= M_PI * 0.75 && angle >= M_PI * -0.25){
    digitalWrite(dcpins[1], HIGH);
    digitalWrite(dcpins[2], LOW);
    digitalWrite(dcpins[4], HIGH);
    digitalWrite(dcpins[5], LOW);
    if(angle <= M_PI * 0.5 && angle >= 0){
      analogWrite(dcpins[0], map(intensity, 0, 512, 0, 255));
      analogWrite(dcpins[3], map(intensity, 0, 512, 0, 255));
    }
    else{
      if(angle >= M_PI * 0.5){
        analogWrite(dcpins[0], intensity / 512 * map((M_PI * 0.75) - angle, 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[3], intensity / 512 * map((M_PI * 0.75) - angle, 0, 0.25 * M_PI, 0, 255));
      }
      else{
        analogWrite(dcpins[0], intensity / 512 * map(abs(angle), 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[3], intensity / 512 * map(abs(angle), 0, 0.25 * M_PI, 0, 255));
      }
    }
  }
  else{
    digitalWrite(dcpins[1], LOW);
    digitalWrite(dcpins[2], HIGH);
    digitalWrite(dcpins[4], LOW);
    digitalWrite(dcpins[5], HIGH);
    if(angle <= M_PI * -0.5){
      analogWrite(dcpins[0], map(intensity, 0, 512, 0, 255));
      analogWrite(dcpins[3], map(intensity, 0, 512, 0, 255));
    }
    else{
      if(angle >= M_PI * 0.75){
        analogWrite(dcpins[0], intensity / 512 * map(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[3], intensity / 512 * map(angle - M_PI * 0.75, 0, 0.25 * M_PI, 0, 255));
      }
      else{
        analogWrite(dcpins[0], intensity / 512 * map(abs(0.25 * M_PI - abs(angle)), 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[3], intensity / 512 * map(abs(0.25 * M_PI - abs(angle)), 0, 0.25 * M_PI, 0, 255));
      }
    }
  }
  //Rechts
  if(angle <= 0.25 * M_PI && angle >= M_PI * -0.75){
    digitalWrite(dcpins[7], LOW);
    digitalWrite(dcpins[8], HIGH);
    digitalWrite(dcpins[10], LOW);
    digitalWrite(dcpins[11], HIGH);
    if(angle >= M_PI * -0.5 && angle <= 0){
      analogWrite(dcpins[6], map(intensity, 0, 512, 0, 255));
      analogWrite(dcpins[9], map(intensity, 0, 512, 0, 255));
    }
    else{
      if(angle <= M_PI * -0.5){
        analogWrite(dcpins[6], intensity / 512 * map(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[9], intensity / 512 * map(abs((M_PI * -0.75) - angle), 0, 0.25 * M_PI, 0, 255));
      }
      else{
        analogWrite(dcpins[6], intensity / 512 * map(angle, 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[9], intensity / 512 * map(angle, 0, 0.25 * M_PI, 0, 255));
      }
    }
  }
  else{
    digitalWrite(dcpins[7], HIGH);
    digitalWrite(dcpins[8], LOW);
    digitalWrite(dcpins[10], HIGH);
    digitalWrite(dcpins[11], LOW);
    if(angle >= M_PI * 0.5){
      analogWrite(dcpins[6], map(intensity, 0, 512, 0, 255));
      analogWrite(dcpins[9], map(intensity, 0, 512, 0, 255));
    }
    else{
      if(angle <= M_PI * -0.75){
        analogWrite(dcpins[6], intensity / 512 * map(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[9], intensity / 512 * map(-0.75 * M_PI - angle, 0, 0.25 * M_PI, 0, 255));
      }
      else{
        analogWrite(dcpins[6], intensity / 512 * map(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
        analogWrite(dcpins[9], intensity / 512 * map(angle - (0.25 * M_PI), 0, 0.25 * M_PI, 0, 255));
      }
    }
  }
  
}

void convertxy(){
  int x = dcinput.substring(0,4).toInt() - 512;
  int y = dcinput.substring(4).toInt() - 512;
  angle = atan2(y, x);
  intensity = sqrt(pow(x,2) + pow(y,2));
}
