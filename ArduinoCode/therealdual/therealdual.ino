#include <Servo.h> 

Servo horizontal; // horizontal servo
int servoh = 90; 
int servohLimitHigh = 175;
int servohLimitLow = 5;
// 65 degrees MAX

Servo vertical; // vertical servo
int servov = 45; 
int servovLimitHigh = 60;
int servovLimitLow = 1;

// LDR pin connections
// name = analogpin;
int ldrlt = A0; //LDR top left - BOTTOM LEFT <--- BDG
int ldrrt = A2; //LDR top rigt - BOTTOM RIGHT
int ldrld = A1; //LDR down left - TOP LEFT
int ldrrd = A3; //ldr down rigt - TOP RIGHT
int onstate1 = 0; // onstate for change from automatic to manual
int prevbutton1 = 0;//previousbutton for pB 1
int recbutton1; // for new state of pB 1
int button1 = 11;//button connected to pin 11 
int onstate2 = 0; // onstate for change from horizontal rotation to vertical rotation
int prevbutton2 = 0;//previousbutton for pB 2
int recbutton2; // for new state of pB 2
int button2 = 12;//button connected to pin 12 

int pot = A4;//potentiometer
int potvalue;


void setup(){
  Serial.begin(9600);
horizontal.attach(6);
vertical.attach(5);
horizontal.write(180);
vertical.write(45);
delay(2500);
pinMode(button1, INPUT);
pinMode(button2, INPUT);
pinMode(pot ,INPUT);              //Potentiometer for right-left movement and for up-down movement

}
void loop(){
  
recbutton1 = digitalRead(button1);
Serial.println(recbutton1);
if(prevbutton1 == 0 && recbutton1 == 1 ){
  if(onstate1 == 0){
    onstate1 = 1;
    }
    else {
      onstate1 = 0;
      }
  }
prevbutton1 = recbutton1;
delay(50);
if(onstate1 == 1){
  manual();
  }
  else{
    automatic();
    }
}

void manual(){
potvalue=analogRead(pot);
potvalue=map(potvalue, 0, 1023, 0, 180);
   recbutton2 = digitalRead(button2);
   Serial.println(recbutton2);
   Serial.println(potvalue);
  if (prevbutton2 == 0 && recbutton2 == 1) {
    if (onstate2 == 0) {
      onstate2 = 1;
      }
      else{
         //control up-down movement
    onstate2 = 0;
        }
   }
     prevbutton2 = recbutton2;
     delay(50); // Wait for 50  millisecond(s)
     if(onstate2 == 1){
       horizontal.write(potvalue);//control right-left movement
      }else{
         vertical.write(potvalue);
        }
}
 
 
void automatic(){
int lt = analogRead(ldrlt); // top left
int rt = analogRead(ldrrt); // top right
int ld = analogRead(ldrld); // down left
int rd = analogRead(ldrrd); // down right
int dtime = 10; int tol = 90; // dtime=diffirence time, tol=toleransi
int avt = (lt + rt) / 2; // average value top
int avd = (ld + rd) / 2; // average value down
int avl = (lt + ld) / 2; // average value left
int avr = (rt + rd) / 2; // average value right
int dvert = avt - avd; // check the diffirence of up and down
int dhoriz = avl - avr;// check the diffirence og left and rigt

if (-1*tol > dvert || dvert > tol) 
 {
 if (avt > avd)
 {
 servov = ++servov;
 if (servov > servovLimitHigh)
 {servov = servovLimitHigh;}
 }
 else if (avt < avd)
 {servov= --servov;
 if (servov < servovLimitLow)
 { servov = servovLimitLow;}
 }
 vertical.write(servov);
 }
if (-1*tol > dhoriz || dhoriz > tol) // check if the diffirence is in the tolerance else change horizontal angle
 {
 if (avl > avr)
 {
 servoh = --servoh;
 if (servoh < servohLimitLow)
 {
 servoh = servohLimitLow;
 }
 }
 else if (avl < avr)
 {
 servoh = ++servoh;
 if (servoh > servohLimitHigh)
 {
 servoh = servohLimitHigh;
 }
 }
 else if (avl = avr)
 {
 delay(5000);
 }
 horizontal.write(servoh);
 }

 
 delay(dtime);
 }
