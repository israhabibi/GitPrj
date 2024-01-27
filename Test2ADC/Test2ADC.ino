int readD1;
int readD2;

int Pin_D1 = 5;
int Pin_D2 = 4;

void setup() 
{ 
  Serial.begin(9600);
  pinMode(Pin_D1,OUTPUT);
  pinMode(Pin_D2,OUTPUT);
  pinMode(A0,INPUT);
}

void loop() 
{
  //for first sensor
  digitalWrite(Pin_D1, HIGH);     //Turn D1 On
  digitalWrite(Pin_D2, LOW);
  delay(1000);                     
  readD1 = analogRead(0);       //Read Analog value of first sensor
  delay(1000);   
 
  digitalWrite(Pin_D1, LOW); 
  digitalWrite(Pin_D2, HIGH);     //Turn D2 On
  delay(1000);                     
  readD2 = analogRead(0); //Read Analog value of second sensor
  delay(1000); 


  Serial.print("Voltase Baterai : ");
  Serial.println(readD1);
  Serial.print("Voltase LDR : ");
  Serial.println(readD2);
  Serial.println("===================================");


  delay(1000); 

  
}