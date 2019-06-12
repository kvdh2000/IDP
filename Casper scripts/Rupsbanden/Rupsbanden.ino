#define TRIGGER_PIN 7
#define ECHO_PIN 6

int pinI1=8;//define I1 interface
int pinI2=11;//define I2 interface 
int speedpinA=9;//enable motor A
int pinI3=12;//define I3 interface 
int pinI4=13;//define I4 interface 
int speedpinB=10;//enable motor B

void setup() 
{
  Serial.begin (115200);

  pinMode(pinI1,OUTPUT);
  pinMode(pinI2,OUTPUT);
  pinMode(speedpinA,OUTPUT);
  pinMode(pinI3,OUTPUT);
  pinMode(pinI4,OUTPUT);
  pinMode(speedpinB,OUTPUT);
}

void loop()
{
  delay(200);
  int distance;
  Serial.print(distance);
      if (distance <10)//reverse
      {
      analogWrite(speedpinA,150);//Slows speed of motors to zero
      analogWrite(speedpinB,150);//Slows speed of motors to zero
      digitalWrite(pinI4,HIGH);//turn DC Motor B move clockwise
      digitalWrite(pinI3,LOW);
      digitalWrite(pinI2,HIGH);//turn DC Motor A move anticlockwise
      digitalWrite(pinI1,LOW);
      Serial.println("Reversing");
      delay(500);
      }
      
      else if (distance < 10) //stop motor
      { 
      analogWrite(speedpinA,220);//Slows speed of motors to zero
      analogWrite(speedpinB,220);//Slows speed of motors to zero
      digitalWrite(pinI4,HIGH);//turn DC Motor B move clockwise
      digitalWrite(pinI3,LOW);
      digitalWrite(pinI2,LOW);//turn DC Motor A move anticlockwise
      digitalWrite(pinI1,HIGH);
      Serial.println("Stopping");
      }
      
      else if (distance>10, distance <30) //slow and steady
      {
      analogWrite(speedpinA,200);//Drive Forward
      analogWrite(speedpinB,200);//Drive Forward
      digitalWrite(pinI4,LOW);//turn DC Motor B move clockwise
      digitalWrite(pinI3,HIGH);
      digitalWrite(pinI2,LOW);//turn DC Motor A move anticlockwise
      digitalWrite(pinI1,HIGH);
      Serial.println("Ahead Slow");
      }
      
      else  //full speed ahead
      {
      analogWrite(speedpinA,255);//Drive Forward
      analogWrite(speedpinB,255);//Drive Forward
      digitalWrite(pinI4,LOW);//turn DC Motor B move clockwise
      digitalWrite(pinI3,HIGH);
      digitalWrite(pinI2,LOW);//turn DC Motor A move anticlockwise
      digitalWrite(pinI1,HIGH);
      Serial.println("Ahead Full");
   
     }
}
