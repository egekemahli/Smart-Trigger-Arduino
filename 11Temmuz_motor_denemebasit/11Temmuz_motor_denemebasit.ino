//Author Ege Kemahlıoğlu 
//Updated: 07/07/2019
//before the implementation please change the treshold value 

//Digital Pins Assigments
const  short int bra = 2; // brake pin
const  short int pwm = 3; // esc sending 
const  short int motor_direction = 4; // F/R pin
//Analog Pins Assigments
const unsigned short int sensorPin1 = A3;    // mic sensor pin assigment  
const unsigned short int sensorPin2 = A4;
const unsigned short int sensorPin3 = A5;

unsigned short int mic1;  // microphone values 
unsigned short int mic2;
unsigned short int mic3;
unsigned short int value = 2; // it present which mic measure high voltage data
unsigned short int prevalue = 2; //previous value


unsigned short int pre1=150;  
unsigned short int pre2=150;
unsigned short int pre3=150;

unsigned short int dif1=0;  
unsigned short int dif2=0;
unsigned short int dif3=0;

unsigned long int time1 = 0; 
unsigned long int time2 = 101; 
unsigned long int dura = 0;
unsigned short int num1=0;  // number of mic. values measured for higher values
unsigned short int num2=0;
unsigned short int num3=0;

boolean listening = true ;
boolean pre_directionLR = true; // Right true Left false 
boolean next_directionLR = true; // Right true Left false 
 unsigned short int maxvalue =150;


void setup() {
  Serial.begin(9600);
  pinMode(pwm,OUTPUT);
  pinMode(motor_direction ,OUTPUT);    
  pinMode(bra,OUTPUT);
  pinMode(mic1, INPUT);
  pinMode(mic2, INPUT);
  pinMode(mic3, INPUT);
 
  digitalWrite(motor_direction , LOW); //CCW (counter clock wise)
  digitalWrite(bra , LOW); //motor stopped at low 
  analogWrite(pwm,50); 
}

void loop() {
    delay(100);
    measure(); 
    delay(100);
    measure(); 
    delay(100);
    measure(); 
    delay(100);
    measure(); 
    delay(100);
    measure(); 
    delay(100);
}

void measure(){
  mic1 = analogRead(sensorPin1);  
  mic2 = analogRead(sensorPin2); 
  mic3 = analogRead(sensorPin3);
  time2= millis();
   if(time2-time1 > 100){
      listening = true;
   }
   else {
      listening = false;
   }
  if(listening == true){
     int treshold = 158; 
     if(mic1 > treshold || mic2 > treshold || mic3 > treshold  ){

     if(maxvalue< mic1){
            maxvalue=mic1;
            num1++;
     }
     else if(maxvalue< mic2){
            maxvalue=mic2;
            num2++;
     }
     else if (maxvalue< mic3){
            maxvalue=mic3;
            num3++;
     }
     
        listening = false;
        time1 = millis();        
        printvalue(); 
        maxvalue= 160;      
    }
  }
}


void printvalue(){
  if      (num1> num2 && num1 > num3){
     value = 1;
  }
  else if (num2> num1 && num2 > num3){
     value = 2;
  }
  else  {
    // (num3> num1 && num3 > num2) condition  and equalities included
     value = 3;
  }
   Serial.print("value: "); 
   Serial.println(value);
   checkDirection();
  num1=0;
  num2=0;
  num3=0;
}

void  checkDirection(){
   if(prevalue==1){
      if(value == 3){
         digitalWrite(motor_direction , LOW); 
         Serial.println("1den3");
         playMotor();
      }
      else if (value == 2){
         digitalWrite(motor_direction , HIGH); 
         Serial.println("1den2");
         playMotor();
      }
      else {
         Serial.println("1de KAL");
      }
    }
   else if(prevalue==3){
      if(value == 2){
         digitalWrite(motor_direction , LOW); 
         Serial.println("3den2");
         playMotor();
      }
      else if (value == 1){
         digitalWrite(motor_direction , HIGH); 
         Serial.println("3den1");
         playMotor();
      }
      else {
         Serial.println("3de KAL");
      }
   }
   //prevalue==2 condition 
   else{
    if     (value == 3){
          digitalWrite(motor_direction , HIGH); 
          Serial.println("2den3");
          playMotor();
      }
    else if(value ==1){
         digitalWrite(motor_direction , LOW);
         Serial.println("2den1"); 
         playMotor();
    }
    else{
         Serial.println("2 de KAL");
    }
   }
   prevalue = value;

}
void playMotor(){
    time1= millis();
    digitalWrite(bra , HIGH);
    while(dura < 730 ){
          time2 = millis();
          dura = time2-time1;
    }
    
    digitalWrite(bra , LOW);
    dura=0;
}
