
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <EEPROM.h>
#include <avr/eeprom.h>
#define DEBUG
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

int HOUSE_LEN=9;
char floorRoom[4]="";
char firstFlrRoom[4]="";
char groundRoom[4]="";
char outsideRoom[3]="";
char typeSetting[4]="";
char gardenSetting[5]="";
char deviceName[2]="";
char actionSetting[4]="";
int written=EEPROM.read(0);// reads the first value in the eeprom 
char* house[9]={floorRoom,firstFlrRoom,groundRoom,outsideRoom,typeSetting,gardenSetting,deviceName,actionSetting};
typedef enum state_e {floors=0,first,ground,outside,type,gardenType,names,actions,values} state_t;

//int sumIterations(char*,char*);
void writeEE(){//function to write house description into eeprom
  char floorRoom[5]={'F','G','O','-'};
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


void navSettings(char* setting,int option,int num_setting){


      lcd.clear();

      switch (num_setting){

        case 0://Floors
      
          if (setting[option]=='F'){
            lcd.print("First Floor");
            
          }else if (setting[option]=='G'){
            lcd.print("Ground Floor");
            
          }else if(setting[option]=='O'){
              lcd.print("Outside");        
            
          }
          break;


        case 1://firstFlrRoom floor
      
          if (setting[option]=='1'){
            lcd.print("1st Bedroom");
            
          }else if (setting[option]=='2'){
            lcd.print("2nd Bedroom");
            
          }else if(setting[option]=='B'){
              lcd.print("Bathroom");        
            
            
          }
          break;

        case 2://grounf floor
      
          if (setting[option]=='K'){
            lcd.print("Kitchen");
            
          }else if (setting[option]=='H'){
            lcd.print("Hall");
            
          }else if(setting[option]=='L'){
              lcd.print("Living Room");        
            
          }     
            
          
          break;

        case 3://outsideRoom
      
          if (setting[option]=='G'){
            lcd.print("Garage");
            
          }else if (setting[option]=='R'){
            lcd.print("Garden");
            
          }
          break;

        case 4://Type
      
          if (setting[option]=='L'){
            lcd.print("Light");
            
          }else if (setting[option]=='A'){
            lcd.print("Lamp");
            
          }else if(setting[option]=='H'){
              lcd.print("Heat");        
            
          }
          break;

        case 5://gardenType
      
          if (setting[option]=='L'){
            lcd.print("Light");
            
          }else if (setting[option]=='A'){
            lcd.print("Lamp");
            
          }else if(setting[option]=='H'){
              lcd.print("Heat");        
            
          }else if(setting[option]=='W'){
              lcd.print("Water");        
            
          }
          break;
          
        case 6://names
      
          if (setting[option]=='M'){
            lcd.print("Main");
            
          }
          break;

        case 7: //Action

          if (setting[option]=='1'){
            lcd.print("On Time");
            
          }else if (setting[option]=='0'){
            lcd.print("0ff Time");
            
          }else if(setting[option]=='L'){
              lcd.print("Level");        
            
          }
          break;
          

  

      }
  
  }


int chooseSettings(char* setting,int num_setting){//num setting is the number based on the enum which is used to specify what case will be called
    int select=0;//select is actionSetting number that is used to call the index of actionSetting house descriptor in order to print out its value and return actionSetting number that can be used for cases
   
    navSettings(setting,select,num_setting);
    bool chosen =false;

    while(!chosen){
  
      static int old_buttons=lcd.readButtons(); //reads number pressed only runs this code once because it is static and so will remember its value
      int buttons =lcd.readButtons();//reads buttons currently pressed
      
      int changes = old_buttons & ~buttons; // uses ~ which means complement and is basicall not but for ints, check if the button pressed has changed from previous
  
    
      if (changes){
        if(old_buttons&BUTTON_RIGHT&&select<strlen(setting)-1){//once select is greater than the length the index will not be in range
          lcd.clear();
          select+=1;
          navSettings(setting,select,num_setting);
          
        }else if(old_buttons&BUTTON_LEFT&&select>0){
          lcd.clear();
          select-=1;
          navSettings(setting,select,num_setting);
          
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
 


  if (written ==0){
    writeEE();

    #ifdef DEBUG
    Serial.println("done");
    #endif
    
  }else{
    
    Serial.println("VALID");
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


    
  static int address,sumTotal,choice,typeChoice,modValue,modProduct,startingAddress,addressAtType,nameChoice,sum;

  //address will be a unique value that is generated as a memory address for each setting chosen

  static state_t state=floors;
  static state_t backToFloor,backToType,backToName,backToAction;


  switch (state){

      case floors:
        address=0;
          
  
        choice=chooseSettings(floorRoom,floors); //could have used house[floors] instead of floorRoom

        address=(choice+1)*200;
        startingAddress=address;//stores the address so that when going back in the menu it will remember the adress before the value was changed
        
        
  
        switch (choice){
  
          case 0:
  
            state= first;
            
            break;
  
          case 1:
  
            state= ground;
            
            break;
  
          case 2:
  
            state= outside;
        
            break;
  
        }
        break;

        
  
      case first:
        address=startingAddress;
  
        choice=chooseSettings(firstFlrRoom,first);

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
        
        choice=chooseSettings(groundRoom,ground);

        address+=choice*50;
        
        if (choice==50){
          state=floors;// goes back to thee previous state
        }else{
          state=type;
          backToFloor=ground;//stores sthe previous state so that when using buttons to go back the system remembers the menu last selected
        }
        break;

        
  
      case outside:

        address=startingAddress;
        choice=chooseSettings(outsideRoom,outside);
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

        

        addressAtType=address;

        choice=chooseSettings(typeSetting,type);
        typeChoice=choice;//saves the choice which will be used in the address generation using mod
        sumTotal=sumIterations(typeSetting,deviceName,actionSetting);

        if (choice==50){
          state=backToFloor;
        }else{
          state=names;
          backToType=type;
        }
        break;

      case gardenType:

        addressAtType=address;

        choice=chooseSettings(gardenSetting,gardenType);
        typeChoice=choice;//saves choice for mod
        sumTotal=sumIterations(gardenSetting,deviceName,actionSetting);
        
        if (choice==50){
          state=backToFloor;
        }else{
          state=names;
          backToType=gardenType;
        }
        break;



      case names:

        
        

        choice = chooseSettings(deviceName,names);
        nameChoice=choice;

        if (choice==50){
          state=backToType;
          
        }else{

          state = actions;
          backToName=names;

        


          
          
        }

        
        break;

      case actions:

        address=addressAtType;
      
         sum=0;
      
        choice=chooseSettings(actionSetting,actions);



        if (choice==50){
          state=backToName;
        }else{

          /*unique address generation*/
          modValue=sumTotal/strlen(typeSetting);

          modProduct=(typeChoice*modValue);

          sum=(modProduct+(typeChoice+1)+(nameChoice+1)+choice) % (modValue*(typeChoice+1)+1);

          address+=sum;//final unique address
          state=values;
          backToAction=actions;

          

          #ifdef DEBUG
          Serial.println(address);
          #endif
          delay(10);
          
     
        }
        break;



    case values:
       
        int value =(int) EEPROM.read(address);
        Serial.println(value);
        if(value>100){
          //EEPROM.update(address,0);
          value=0;
        }
        lcd.clear();
        lcd.print(value);


        bool chosen =false;

    
        while(!chosen){
      
          static int old_buttons=lcd.readButtons(); //reads number pressed only runs this code once because it is static and so will remember its value
          int buttons =lcd.readButtons();//reads buttons currently pressed
          
          int changes = old_buttons & ~buttons; // uses ~ which means complement and is basicall not but for ints, check if the button pressed has changed from previous

          
        
          if (changes){
            if(old_buttons&BUTTON_RIGHT&&value<100){//once select is greater than the length the index will not be in range
              value+=10;
              lcd.clear();
              
              lcd.print(value);
              
              
              
            }else if(old_buttons&BUTTON_LEFT&&value>0){
              value-=10;
              lcd.clear();
              lcd.print(value);
              
              
              
            }else if(old_buttons&BUTTON_SELECT){

              //EEPROM.update(address,value);
    
              chosen=true;//ends the loop once on is selected
              state=backToAction;
              
            }else if(old_buttons&BUTTON_DOWN){
              

              chosen=true;
              state=backToAction;
              
            }
            
        }
      
          old_buttons=buttons;
      }

     

  
      break;
      
      
   
  }

  



}
