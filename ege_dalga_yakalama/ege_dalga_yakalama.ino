//Author Ege Kemahlıoğlu 
//Updated: 07/07/2019
int sensorPin = A1;    //analog pin for sensor 
unsigned short int limit = 50; // the similarity limit in the process at last to stop the time 
unsigned long starttime = 0; // the peak starting time
unsigned long finishtime = 0; //the time of the stop time of flashing
unsigned long maxtime = 0; //(not used)max time is the peak time at the calculation
unsigned short int maxvalue = 0;//(not used)
unsigned short int duration; // the process time
unsigned short int constantvalue = 0;//the constant value of the room after the flash (not used)
unsigned short int pre = 500;//pre value (assignment help to avoid a false triggering at the first comparison)
unsigned short int current = 320; //current value
unsigned short int samedata = 0; //same data number at comparison process when the system return stable position
unsigned short int peak = 1;// while loop condition assigment
unsigned short int senstivity = 3;// help to arrange the sensitivity interval of system
boolean start = false;
boolean printtime = false;
unsigned short int measureint= 1200 ; //measurement interval 
unsigned int numb = 0 ; //data number

//the flash detection function with printing process
void detector(){
  int  value       = analogRead(sensorPin);
  bool started     = false;
  int  sensitivity = 50;
  unsigned long  milis       = 0;
  unsigned long  last_same_value_time = 0;
  int  last_readed_value    = 0;

  while(1) {
    int temp              = analogRead(sensorPin);
              
    if ( last_readed_value > temp - 25 && last_readed_value < temp + 25 ) {
      if ( last_same_value_time == 0 )
        last_same_value_time = millis();
      else if ( millis() - last_same_value_time > 3000 ) {
        value = temp;
        if ( started ) {
          Serial.print("\nUzunluk: ");
          Serial.print(millis() - milis);
          started = false;             
        }
        last_same_value_time = 0;
      }
    }
    
    last_readed_value = temp;

 
    if ( last_readed_value > value + sensitivity ) {
      if ( started == false ) {
        started = true;
        milis = millis();
        last_same_value_time = 0;        
      }
    }

    if ( started && last_readed_value < value + sensitivity ) {
      Serial.print("\nUzunluk: ");
      Serial.print(millis() - milis);
              last_same_value_time = 0;
      started = false;
    }    
 }

  
  while(peak == 1)
  {   
      pre = current;
      current = analogRead(sensorPin); 
      
      if(pre  < current - 15 ){
      //the increasing is detected so we need to save starting time

         if(start == false){
            
            starttime = millis();
            start = true;
         }
      }
      //system enter that condition to compare the current data with the previous to see system at the stable position
      if(start == true){
      numb++;
      if(numb % measureint == 0 ){
         if ( pre- senstivity < current  && pre + senstivity > current   ){
            //finish time for stable position saved
            if(samedata == 1 ){
               finishtime= millis();
               constantvalue = current;
            }
          samedata++;
          }
          else{
            samedata=0;
          }
          if(samedata > limit){
            printtime = true;
            samedata= 0;
            start= false;
            peak= 0;
          }
        }
      }
   }
   if(printtime == true){
     duration = finishtime - starttime;
    
     if (duration < 10000){
        Serial.print("The process duration is ");
        Serial.print(duration);
        Serial.println(" miliseconds");
     }
     else {
        Serial.print("too long process. False trigger occured. ");
     }
    
     peak = 1; // function enter while for the next loop
     start = false;
     printtime=false;
     
     
   }
}

void setup() {
    Serial.begin(9600);
}
void loop() {
    detector();
}
