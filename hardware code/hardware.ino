 int motionDetector=13; //input from motion sensor
 int gshutsensor=12;  //input from gshutsensor switch
 int obsSensor=11; //input from obsSensor
 int temper=10;  // as a output temper_valueper goes to controlNode
 int latchSensor=9; // input from o_lock
 int snibSensor=8; // input from snibSensor
 int relay=7; //output to board
 int ac =6;//output to ac
 int controlNode=5;//as a input recieve from controlNode
 int emergency=4;//as a input recieve from controlNode
 long int timer = 10000,ir_timer=0;
 unsigned int millis_v= 0,past = 0,loop_time = 0;
 int time_set_value = 0 , temper_value = 0,motion_detect = 0 , switch_status = 2 , emergency_v = 0 ;
 int readPins[7];
 void setup()//defines all the in modes
  {
    pinMode(motionDetector,INPUT);
    pinMode(emergency,INPUT);
    pinMode(controlNode,INPUT);
    pinMode(gshutsensor,INPUT);
    pinMode(obsSensor,INPUT);
    pinMode(latchSensor,INPUT);
    pinMode(snibSensor,INPUT);
    pinMode(relay,OUTPUT);
    pinMode(ac,OUTPUT);
    pinMode(temper,OUTPUT);
    Serial.begin(9600);
  }

void all_off()//function for closing of appliances
 {
  digitalWrite(relay,1);
  digitalWrite(ac,1);
 }
void all_on()//function for open of appliances
 {
  digitalWrite(relay,0);
  digitalWrite(ac,0);
 }
 
 
int personsleep(int status)// function for night sleeping
{
  if(status != 1)
  {
    timer = 21600000;
  }
  return 1;
}


int personin() function for person in the room
{ 
  timer = 1800000;
  return 0;
}


int personout(int status)// function for person is out of the room
{
  if(status != 2)
  {
     timer =  60000;
  }
  return 2;
}


void loop()// main body of the program
{

   //--------------------------------------------------------------------------
   // this part can manege timer variable
   //--------------------------------------------------------------------------
   readPins[0] =digitalRead(controlNode);
   readPins[1] =digitalRead(gshutsensor);
   readPins[2] =digitalRead(latchSensor);
   readPins[3] =digitalRead(snibSensor);
   readPins[4] =digitalRead(obsSensor);
   readPins[5] =digitalRead(motionDetector);
   readPins[6] =digitalRead(emergency);
   for(int i =0;i<7;i++)
   {
   Serial.print(readPins[i]);
    Serial.print(",");
   }
   Serial.println();
   delay(1000);
   if(readPins[0] == 0)
   {
    if(switch_status != 0)
    {
      all_off();
      switch_status = 0;
    }
   }
   else
   {
     //--------------------------------------------------------------------------
     // this part can manege timer variable
     //--------------------------------------------------------------------------
   
     //--------------------------------------------------------------------------
     // this part can manege timer variable
     //--------------------------------------------------------------------------
    
        millis_v = millis();
        loop_time = millis_v - past;
        past = millis_v;
        if (timer>loop_time || ir_timer>loop_time)
        {
          timer = timer - loop_time;
          ir_timer = ir_timer - loop_time;
        }
        else
        {
           timer = 0;
           motion_detect = 0;
           ir_timer = 0;
        }

     //--------------------------------------------------------------------------
     // end of the timer part 
     //--------------------------------------------------------------------------

   


     //--------------------------------------------------------------------------
     // this part can manege the calculation
     //--------------------------------------------------------------------------
        if((readPins[1] == 0 && readPins[2] == 1)||(readPins[1] == 0 && readPins[2] == 0 && readPins[3] == 1))//for tempring if any 
        {
           temper_value = 1; // set the temper value for controlling node
           Serial.print("tempering");
        }
        //this means that person is in emergency
        else if (readPins[1] == 1 && readPins[2] == 1 &&readPins[6] == 1) 
        {
          emergency_v = 1;
          temper_value =0; 
        }
        // this is for the person is sliping or not 
        else if (readPins[1]==1&&readPins[2] == 0 && readPins[3] == 1)
        {
          temper_value = 0;
          time_set_value = personsleep(time_set_value);
        // delay(1000);
        }
        // person is out side of the room 
        else if (readPins[1] == 1 && readPins[2] == 1 && readPins[6] == 0)
        {
          time_set_value = personout(time_set_value);
          temper_value = 0;
        }
        else if (readPins[1] == 0 && readPins[2] == 0 && readPins[3] == 0 && ir_timer > 0 && readPins[5] == 0)
        {
          time_set_value = personout(time_set_value);
          temper_value = 0;
        }
        // this is for person is in side the room
        else if (readPins[5] == 1||(readPins[1] == 1 && readPins[2] == 0 && readPins[3] == 0 && readPins[5] == 1)||(readPins[1] == 0 && readPins[2] == 0 && readPins[3] == 0 && readPins[4] == 0 && readPins[5] == 1))
        {
          time_set_value = personin();
          motion_detect = 1;
          
          temper_value = 0;
        }
        //some other condition is then 
        else
        {
          //all_off();
          temper_value = 0;
        }
     //--------------------------------------------------------------------------
     // end of the calculation part
     //--------------------------------------------------------------------------
     //--------------------------------------------------------------------------
     // this part for the on/off the devices
     //-------------------------------------------------------------------------- 
        Serial.print("values:");
        Serial.print(temper_value);
        Serial.print(",");
        Serial.print(emergency_v);
        Serial.print(",");
        Serial.println(timer);
        //delay(1000);
        if(temper_value == 0){
             if (emergency_v == 1)
              {
                
                if(switch_status != 1)
                {
                  all_on();
                  switch_status = 1;
                }
              }
              else{
                  if (timer>0)
                   {
                     if(switch_status != 1)
                      {
                        all_on();
                        switch_status = 1;
                      }
                   }
                  else
                   {
                     if (switch_status != 0)
                     {
                       all_off();
                       switch_status = 0;
                     }
                   }
               }
         }
        else{
          digitalWrite(temper,1);
          if(switch_status != 0)
          {
            all_off();
            switch_status = 0;
          }
        }

     //--------------------------------------------------------------------------
     // end of the on/off part
     //--------------------------------------------------------------------------

   }
}
