    
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <EEPROM.h>
#include <avr/eeprom.h>
#define DEBUG
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

int HOUSE_LEN=9;
char floorRoom[5]="";//empty char arrays are declared
char firstFlrRoom[4]="";
char groundRoom[4]="";
char outsideRoom[3]="";
char typeSetting[4]="";
char gardenSetting[5]="";
char deviceName[2]="";
char actionSetting[4]="";
char* setting = (char*) malloc(sizeof(char)*5);
char* room = (char*) malloc(sizeof(char)*5);
int written=EEPROM.read(0);// reads the first value in the eeprom 
char* house[9]={floorRoom,firstFlrRoom,groundRoom,outsideRoom,typeSetting,gardenSetting,deviceName,actionSetting};
typedef enum state_e {floors=0,first,ground,outside,type,gardenType,names,actions,values,data} state_t;
static int address,sumTotal,choice,typeChoice,modValue,modProduct,startingAddress,addressAtType,nameChoice,sum,value,opt,setNum ;
static bool chosen;
 

  static state_t state=floors;
  static state_t backToFloor,backToType,backToName,backToAction,lastMenuSelected;
  static char actionChoice;

void writeEE(){//function to write house description into eeprom
  char floorRoom[6]={'F','G','O','S','-'}; 
  char firstFlrRoom[5]={'1','2','B','-'};
  char groundRoom[5]={'K','H','L','-'};
  char outsideRoom[4]={'G','R','-'};
  char typeSetting[5]={'L','A','H','-'};
  char gardenSetting[6]={'L','A','H','W','-'};
  char deviceName[3]={'M','-'};
  char actionSetting[5]={'1','0','L','x'};
  char* house[HOUSE_LEN]={floorRoom,firstFlrRoom,groundRoom,outsideRoom,typeSetting,gardenSetting,deviceName,actionSetting};//stores pointers to the different char arrays
  int totalLen=0;

  EEPROM.update(totalLen,1); //writes a 1 at the firstFlrRoom memory location to signify data has been written to
  for(int i=0;i<HOUSE_LEN-1;i++){
    for(int j=0;j<strlen(house[i]);j++){
      totalLen++;
      EEPROM.update(totalLen,house[i][j]);
      #ifdef DEBUG
      Serial.print(totalLen);
      Serial.print("     ");
      Serial.println(house[i][j]);
      #endif
    }
  }
}

void readEE(char** house){//reads the house desc from the eeprom

    int count=1;
    for(int i=0;i<HOUSE_LEN-1;i++){//loop based on the amount of different house descriptors
      int j=0;

      while((char) EEPROM.read(count)!='-'){//keep looping until it sees actionSetting '-' which signifies that it should move to the next descriptor
        
        if((char) EEPROM.read(count)=='x'){break;}//breaks when it sees actionSetting 'x' which signifies the end of all the data being reae
   
        house[i][j]=(char) EEPROM.read(count);
        j++;
        count++;
        
      }
      count++;
       
    }
       
}


void printSettings(char* setting,int option,int num_setting){

      switch (num_setting){

        case 0://Floors
      
          if (setting[option]=='F'){
            Serial.print(F("First Floor"));
            
          }else if (setting[option]=='G'){
            Serial.print(F("Ground Floor"));
            
          }else if(setting[option]=='O'){
              Serial.print(F("Outside"));        
            
          }
          break;


        case 1://firstFlrRoom floor
      
          if (setting[option]=='1'){
            Serial.print(F("1st Bedroom"));
            
          }else if (setting[option]=='2'){
            Serial.print(F("2nd Bedroom"));
            
          }else if(setting[option]=='B'){
              Serial.print(F("Bathroom"));        
            
            
          }
          break;

        case 2://grounf floor
      
          if (setting[option]=='K'){
            Serial.print(F("Kitchen"));
            
          }else if (setting[option]=='H'){
            Serial.print(F("Hall"));
            
          }else if(setting[option]=='L'){
              Serial.print(F("Living Room"));        
            
          }     
            
          
          break;

        case 3://outsideRoom
      
          if (setting[option]=='G'){
            Serial.print(F("Garage"));
            
          }else if (setting[option]=='R'){
            Serial.print(F("Garden"));
            
          }
          break;

        case 4://Type
      
          if (setting[option]=='L'){
            Serial.print(F("Light"));
            
          }else if (setting[option]=='A'){
            Serial.print(F("Lamp"));
            
          }else if(setting[option]=='H'){
              Serial.print(F("Heat"));        
            
          }
          break;

        case 5://gardenType
      
          if (setting[option]=='L'){
            Serial.print(F("Light"));
            
          }else if (setting[option]=='A'){
            Serial.print(F("Lamp"));
            
          }else if(setting[option]=='H'){
              Serial.print(F("Heat"));        
            
          }else if(setting[option]=='W'){
              Serial.print(F("Water"));        
            
          }
          break;
          
        case 6://names
      
          if (setting[option]=='M'){
            Serial.print(F("Main"));
            
          }
          break;

        case 7: //Action

          if (setting[option]=='1'){
            Serial.print(F("On Time"));
            
          }else if (setting[option]=='0'){
            Serial.print(F("0ff Time"));
            
          }else if(setting[option]=='L'){
              Serial.print(F("Level"));        
            
          }
          break;

      }
  
  }
  
void printLcdSettings(char* setting,int option,int num_setting){


      lcd.clear();

      switch (num_setting){

        case 0://Floors
      
          if (setting[option]=='F'){
            lcd.print(F("First Floor >"));
            
          }else if (setting[option]=='G'){
            lcd.print(F("< Ground Floor >"));
            
          }else if(setting[option]=='O'){
              lcd.print(F("< Outside >"));        
            
          }else if(setting[option]=='S'){
              lcd.print(F("< Show Data"));        
            
          }
          break;


        case 1://firstFlrRoom floor
      
          if (setting[option]=='1'){
            lcd.print(F("1st Bedroom >"));
            
          }else if (setting[option]=='2'){
            lcd.print(F("< 2nd Bedroom >"));
            
          }else if(setting[option]=='B'){
              lcd.print(F("< Bathroom"));        
            
            
          }
          break;

        case 2://grounf floor
      
          if (setting[option]=='K'){
            lcd.print(F("Kitchen >"));
            
          }else if (setting[option]=='H'){
            lcd.print(F("< Hall >"));
            
          }else if(setting[option]=='L'){
              lcd.print(F("< Living Room"));        
            
          }     
            
          
          break;

        case 3://outsideRoom
      
          if (setting[option]=='G'){
            lcd.print(F("Garage >"));
            
          }else if (setting[option]=='R'){
            lcd.print(F("< Garden"));
            
          }
          break;

        case 4://Type
      
          if (setting[option]=='L'){
            lcd.print(F("Light >"));
            
          }else if (setting[option]=='A'){
            lcd.print(F("< Lamp >"));
            
          }else if(setting[option]=='H'){
              lcd.print(F("< Heat"));        
            
          }
          break;

        case 5://gardenType
      
          if (setting[option]=='L'){
            lcd.print(F("Light >"));
            
          }else if (setting[option]=='A'){
            lcd.print(F("< Lamp >"));
            
          }else if(setting[option]=='H'){
              lcd.print(F("< Heat >"));        
            
          }else if(setting[option]=='W'){
              lcd.print(F("< Water"));        
            
          }
          break;
          
        case 6://names
      
          if (setting[option]=='M'){
            lcd.print(F("Main"));
            
          }
          break;

        case 7: //Action

          if (setting[option]=='1'){
            lcd.print(F("On Time >"));
            
          }else if (setting[option]=='0'){
            lcd.print(F("< 0ff Time >"));
            
          }else if(setting[option]=='L'){
              lcd.print(F("< Level"));        
            
          }
          break;
          

  

      }
  
  }


int navSettings(char* setting,int num_setting){//num setting is the number based on the enum which is used to specify what case will be called
    int select=0;//select is actionSetting number that is used to call the index of actionSetting house descriptor in order to print out its value and return actionSetting number that can be used for cases
   
    printLcdSettings(setting,select,num_setting);
    bool chosen =false;

    while(!chosen){
  
      static int old_buttons=lcd.readButtons(); //reads number pressed only runs this code once because it is static and so will remember its value
      int buttons =lcd.readButtons();//reads buttons currently pressed
      
      int changes = old_buttons & ~buttons; // uses ~ which means complement and is basicall not but for ints, check if the button pressed has changed from previous
      
      if(Serial.available()){

          select= 20;
          chosen=true;
          
        }
    
      if (changes){
        if(old_buttons&BUTTON_RIGHT&&select<strlen(setting)-1){//once select is greater than the length the index will not be in range
          lcd.clear();
          select+=1;
          printLcdSettings(setting,select,num_setting);
          
        }else if(old_buttons&BUTTON_LEFT&&select>0){
          lcd.clear();
          select-=1;
          printLcdSettings(setting,select,num_setting);
          
        }else if(old_buttons&BUTTON_SELECT){

          chosen=true;//ends the loop once on is selected
          
        }else if(old_buttons&BUTTON_DOWN){
          
          select=50;//chosen actionSetting random number to represent the case for going back
          chosen=true;
          
        }
        
    }
  
      old_buttons=buttons;
  }

  return select;
}

int sumIterations(char* firstIter,char* nestedIter,char* secondNestedIter){//function to generate a value based on the addition of index actions in a loop

  int sumTotal=0;

  for (int i=1;i<=strlen(firstIter);i++){

    for(int j=1;j<=strlen(nestedIter);j++){

      for(int k=1;k<=strlen(secondNestedIter);k++){
        sumTotal+=i+j+k;
      }
        
    }
    
  }

  return sumTotal;

}



int calculateAddressValue(char* setting,int sumTotal,int typeChoice,int nameChoice,int actionChoice){
    
      modValue=sumTotal/strlen(setting);
    
      modProduct=(typeChoice*modValue);
    
    
      sum=(modProduct+(typeChoice+1)+(nameChoice+1)+actionChoice) % (modValue*(typeChoice+1)+1);
    
      return sum;
    
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  Serial.println("BASIC");


  if (written ==0){
    writeEE();

    #ifdef DEBUG
    Serial.println("done");
    #endif
    
  }else{
    
    
  }
  
  readEE(house);

  #ifdef DEBUG
  for (int i=0;i<HOUSE_LEN;i++){

    Serial.println(house[i]);
  }
  #endif

  
  
}



void loop() {
  // put your main code here, to run repeatedly:

  
  char command=' ';
  command=Serial.read();

  if (choice==20){
    if (command=='q' or command=='Q'){
      state=data;
      choice=0;
    }
  }

  switch (state){

      case floors:
        address=0;
        lastMenuSelected=floors;
          
  
        choice=navSettings(floorRoom,floors); //could have used house[floors] instead of floorRoom


        address=(choice+1)*200;
        startingAddress=address;//stores the address so that when going back in the menu it will remember the adress before the value was changed
        
        
      
        if (choice==0){
          
          state= first;
          
        }else if(choice==1){

          state=ground;

          
        }else if(choice==2){

          state=outside;
          
          
        }else if(choice==3){

          state=data;
          
        }
        
        break;

        
  
      case first:
        address=startingAddress;
        lastMenuSelected=first;
  
        choice=navSettings(firstFlrRoom,first);

        address+=choice*50;
        

        
        if (choice==50){
          state=floors;// goes back to thee previous state
        }else{
          state=type;
          backToFloor=first;
        }
        break;

        
  
      case ground:
        address=startingAddress;
        lastMenuSelected=ground;
        choice=navSettings(groundRoom,ground);

        address+=choice*50;
        
        
        if (choice==50){
          state=floors;// goes back to thee previous state
        }else{
          state=type;
          backToFloor=ground;//stores sthe previous state so that when using buttons to go back the system remembers the menu last selected
        }
        break;

        
  
      case outside:
        lastMenuSelected=outside;
        address=startingAddress;
        choice=navSettings(outsideRoom,outside);
        address+=choice*50;
        
        
        if (choice==50){
          state=floors;// goes back to thee previous state
        }else{
          
          if (outsideRoom[choice]=='R'){
              state=gardenType;
          }else{

            state=type;
          }
          
          backToFloor=outside;
        }
        break;




      case type:

        lastMenuSelected=type;

        addressAtType=address;

        choice=navSettings(typeSetting,type);
        typeChoice=choice;//saves the choice which will be used in the address generation using mod
        setting=typeSetting;//saves the setting
        sumTotal=sumIterations(typeSetting,deviceName,actionSetting);
        

        if (choice==50){
          state=backToFloor;
        }else{
          state=names;
          backToType=type;
        }
        break;

      case gardenType:
        lastMenuSelected=gardenType;
        addressAtType=address;

        choice=navSettings(gardenSetting,gardenType);
        typeChoice=choice;//saves choice for mod
        setting=gardenSetting;//saves the setting
        sumTotal=sumIterations(gardenSetting,deviceName,actionSetting);
        
        if (choice==50){
          state=backToFloor;
        }else{
          state=names;
          backToType=gardenType;
        }
        break;



      case names:
        lastMenuSelected=names;
        choice = navSettings(deviceName,names);
        nameChoice=choice;

        if (choice==50){
          state=backToType;
          
        }else{

          state = actions;
          backToName=names;
  
        }

        
        break;

      case actions:
        lastMenuSelected=actions;

        address=addressAtType;
      
         sum=0;
      
        choice=navSettings(actionSetting,actions);


        if (choice==50){
          state=backToName;
        }else{

          /*unique address generation*/


          sum=calculateAddressValue(setting,sumTotal,typeChoice,nameChoice,choice);
 
          address+=sum;//final unique address
          
          
          

          actionChoice=actionSetting[choice];
          state=values;
          backToAction=actions;

          

          #ifdef DEBUG
          Serial.println(address);
          Serial.println(setting);
          #endif
          delay(10);
          
     
        }
        break;



    case values:
       lastMenuSelected=values;
        value =(int) EEPROM.read(address);

        
        if (actionChoice=='L'){

          if(value>100){//the defaulty value of an eeprom is 255 which is outside the range for level (0 -100) thus gets changed to 0
            EEPROM.update(address,0);
            value=0;
          }

          lcd.clear();
          lcd.print(value);
          
        }else{

          if(value>23){
            EEPROM.update(address,0);
            value=0;
          }
          
          lcd.clear();

          if (value<10){

            lcd.print(F("0"));
          }
          
          lcd.print(value);
          lcd.print(F(":00"));
          
        }


        

        chosen =false;

    
        while(!chosen){
      
          static int old_buttons=lcd.readButtons(); //reads number pressed only runs this code once because it is static and so will remember its value
          int buttons =lcd.readButtons();//reads buttons currently pressed
          
          int changes = old_buttons & ~buttons; // uses ~ which means complement and is basicall not but for ints, check if the button pressed has changed from previous

          //if the action selected is level then values are incremented in 10s between 100 and 0
          if (actionChoice=='L'){
              if (changes){
                  if(old_buttons&BUTTON_RIGHT&&value<100){
                    value+=10;
                    lcd.clear();
                    
                    lcd.print(value);
                    
                    
                    
                  }else if(old_buttons&BUTTON_LEFT&&value>0){
                    value-=10;
                    lcd.clear();
                    lcd.print(value);
                    
                    
                    
                  }else if(old_buttons&BUTTON_SELECT){
      
                    EEPROM.update(address,value);
          
                    chosen=true;//ends the loop once on is selected
                    state=backToAction;
                    
                  }else if(old_buttons&BUTTON_DOWN){
                    
      
                    chosen=true;
                    state=backToAction;
                    
                  }
                  
              }
               
          }else{

              if (changes){
                  if(old_buttons&BUTTON_RIGHT&&value<23){
                    value+=1;
                    lcd.clear();

                    if (value<10){

                      lcd.print(F("0"));
                    }
                    
                    lcd.print(value);
                    lcd.print(F(":00"));
                    
                    
                    
                  }else if(old_buttons&BUTTON_LEFT&&value>0){
                    value-=1;
                    lcd.clear();
                    
                    if (value<10){

                      lcd.print(F("0"));
                    }
                    
                    lcd.print(value);
                    lcd.print(F(":00"));
                    
                    
                    
                  }else if(old_buttons&BUTTON_SELECT){
                    
      
                    EEPROM.update(address,value);
          
                    chosen=true;//ends the loop once on is selected
                    state=backToAction;
                    
                  }else if(old_buttons&BUTTON_DOWN){
                    
      
                    chosen=true;
                    state=backToAction;
                    
                  }
                  
              }
          
          }
  
          old_buttons=buttons;   
      }

      break; 

    case data:

      bool printData;
     
      for(int a=0;a<(strlen(floorRoom)-1);a++){
        
        
        if (floorRoom[a]=='F'){
            room=firstFlrRoom;
            opt=1;
        }else if(floorRoom[a]=='G'){

          room=groundRoom;
          opt=2;
          
          
        }else if(floorRoom[a]=='O'){
            room=outsideRoom;
            opt=3;
        }

        address=200*opt;
             
            
        for (int b=0;b<strlen(room);b++){

          address+=b*50;

          if(room[b]=='R'){
            setting=gardenSetting;
            setNum=5;
            sumTotal=sumIterations(gardenSetting,deviceName,actionSetting);
          }else{
            setting = typeSetting;
            setNum=4;
            sumTotal=sumIterations(typeSetting,deviceName,actionSetting);
          }
          

            for (int c=0; c<strlen(setting);c++){

                for (int d=0;d<strlen(deviceName);d++){

                    for(int e=0;e<strlen(actionSetting);e++){

                        sum=calculateAddressValue(setting,sumTotal,c,d,e);
                        

                        
                        //Serial.println(address+sum);

                        value = (int) EEPROM.read(address+sum);
                        if (actionSetting[e]=='L'){
                
                          if(value>100){//the defaulty value of an eeprom is 255 which is outside the range for level (0 -100) thus gets changed to 0
                            printData=false;
                          }else{
                            printData=true;
                          }

                          if(printData){
                            
                              printSettings(floorRoom,a,0);
                              Serial.print("/");
                              printSettings(room,b,opt);
                              Serial.print("/");
                              printSettings(setting,c,setNum);
                              Serial.print("/");
                              printSettings(deviceName,d,6);
                              Serial.print("/");
                              printSettings(actionSetting,e,7); 
                              Serial.print(": ");
                              Serial.println(value);
                            
                          }



                          

                        }else{
                
                          if(value>23){
                            
                            printData=false;
                            
                          }else{

                            printData=true;
                          }


                          if(printData){
                            
                              printSettings(floorRoom,a,0);
                              Serial.print("/");
                              printSettings(room,b,opt);
                              Serial.print("/");
                              printSettings(setting,c,setNum);
                              Serial.print("/");
                              printSettings(deviceName,d,6);
                              Serial.print("/");
                              printSettings(actionSetting,e,7); 
                              Serial.print(": ");
                            
                          
                          
                
                              if (value<10){
                    
                                Serial.print(F("0"));
                              }
                              
                              Serial.print(value);
                              Serial.println(F(":00"));


                          }
                          
                        }
                        
                      
                    }              
                }
              
            }
          
        }
    
      
    }

    state=lastMenuSelected;
    break;

      

      
   
  }


  



}
