#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

SoftwareSerial Sim(5,6);//GSM txd and rxd pins
#define SS_PIN 10
#define RST_PIN 9
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
#define buzzer A5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  Sim.begin(9600);
  Sim.println("AT");
  delay(1000);
  Sim.println("AT+CMGF=1"); // Set message format to text mode
  delay(1000);
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(buzzer,OUTPUT);
  Serial.println("Put your card to the reader...");
  Serial.println();
  delay(500);

}
void loop() 
{
  int b=0;    // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
Serial.print("UID tag :");
String content= "";
byte letter;
for (byte i = 0; i < mfrc522.uid.size; i++) 
{
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
}
Serial.println();
Serial.print("Message : ");
content.toUpperCase();

if (content.substring(1) == "99 8D 5D 6D") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
  }
  int x= bool(content.substring(1) != "99 8D 5D 6D");
  if(x){
  Serial.println("Access Denied");
  digitalWrite(buzzer,HIGH);
  delay(5000);
  digitalWrite(buzzer,LOW);
  delay(1000);
  }
 if(x){
    Serial.println("Sending messege....");
    Sim.print("AT+CMGS=\"+919705490493\"\r"); // change to the phone number you using 
    delay(1000);//delay of one second
    Sim.println("Hey Sai\nAlert! Somebody entered");
    Sim.write(26);//the content of the message
}
}
