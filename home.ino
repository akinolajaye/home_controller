
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <EEPROM.h>
#include <avr/eeprom.h>
#define DEBU
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

int HOUSE_LEN=8;
char floorRoom[4]="";
char firstFlrRoom[4]="";
char groundRoom[4]="";
char outsideRoom[3]="";
char typeSetting[4]="";
char gardenSetting[5]="";
char deviceName[2]="";
int written=EEPROM.read(0);// reads the first value in the eeprom 
char* house[8]={floorRoom,firstFlrRoom,groundRoom,outsideRoom,typeSetting,gardenSetting,deviceName};
typedef enum state_e {floors=0,first,ground,outside,type,gardenType,names} state_t;

//int sumIterations(char*,char*);
void writeEE(){//function to write house description into eeprom
  char floorRoom[5]={'F','G','O','-'};
  char firstFlrRoom[5]={'1','2','B','-'};
  char groundRoom[5]={'K','H','L','-'};
  char outsideRoom[4]={'G','R','-'};
  char typeSetting[5]={'L','A','H','-'};
  char gardenSetting[6]={'L','A','H','W','-'};
  char deviceName[3]={'M','x'};
  char* house[HOUSE_LEN]={floorRoom,firstFlrRoom,groundRoom,outsideRoom,typeSetting,gardenSetting,deviceName};//stores pointers to the different char arrays
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

      while((char) EEPROM.read(count)!='-'){//keep looping until it sees valueSetting '-' which signifies that it should move to the next descriptor
        
        if((char) EEPROM.read(count)=='x'){break;}//breaks when it sees valueSetting 'x' which signifies the end of all the data being reae
   
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

      }
  
  }


int chooseSettings(char* setting,int num_setting){//num setting is the number based on the enum which is used to specify what case will be called
    int select=0;//select is valueSetting number that is used to call the index of valueSetting house descriptor in order to print out its value and return valueSetting number that can be used for cases
   
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
          
          select=50;//chosen valueSetting random number to represent the case for going back
          chosen=true;
          
        }
        
    }
  
      old_buttons=buttons;
  }

  return select;
}

int sumIterations(char* firstIter,char* nestedIter){

  int sumTotal=0;

  for (int i=1;i<=strlen(firstIter);i++){

    for(int j=1;j<=strlen(nestedIter);j++){

      sumTotal+=i+j+1;
      
    }
    
  }

  return sumTotal;
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  /*
  char x[10] ="";
  String z="feejjt";
  String outsideRoom ="hii";
  z.toCharArray(x,10);
  outsideRoom.toCharArray(x,10);
  
  
  Serial.println(x);
  */

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


    
  static int add,sumTotal,choice,modChoice,modValue,modProduct,sum,startingAddress;

  static state_t state=floors;
  static state_t back,secondBack;


  switch (state){

      case floors:
        add=0;
          
  
        choice=chooseSettings(floorRoom,floors); //could have used house[floors] instead of floorRoom

        add=(choice+1)*200;
        startingAddress=add;
        
        
  
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
        add=startingAddress;
  
        choice=chooseSettings(firstFlrRoom,first);

        add+=choice*50;

        
        if (choice==50){
          state=floors;// goes back to thee previous state
        }else{
          state=type;
          back=first;
        }
        break;

        
  
      case ground:
        add=startingAddress;
        
        choice=chooseSettings(groundRoom,ground);

        add+=choice*50;
        
        if (choice==50){
          state=floors;// goes back to thee previous state
        }else{
          state=type;
          back=ground;//stores sthe previous state so that when using buttons to go back the system remembers the menu last selected
        }
        break;

        
  
      case outside:

        add=startingAddress;
        choice=chooseSettings(outsideRoom,outside);
        add+=choice*50;
        
        if (choice==50){
          state=floors;// goes back to thee previous state
        }else{
          
          if (outsideRoom[choice]=='R'){
              state=gardenType;
          }else{

            state=type;
          }
          
          back=outside;
        }
        break;




      case type:

        

        choice=chooseSettings(typeSetting,type);
        modChoice=choice;//saves choice for mod
        sumTotal=sumIterations(typeSetting,deviceName);

        if (choice==50){
          state=back;
        }else{
          state=names;
          secondBack=type;
        }
        break;

      case gardenType:

        choice=chooseSettings(gardenSetting,gardenType);
        modChoice=choice;//saves choice for mod
        sumTotal=sumIterations(gardenSetting,deviceName);
        
        if (choice==50){
          state=back;
        }else{
          state=names;
          secondBack=gardenType;
        }
        break;



      case names:

        choice = chooseSettings(deviceName,names);



        
        if (choice==50){
          state=secondBack;
          add-=sum;
        }else{

        

          modValue=sumTotal/strlen(typeSetting);
          modProduct=(modChoice*modValue);

          sum=(modProduct+(modChoice+1)+(choice+1)+1) % (modValue*(modChoice+1)+1);

          add+=sum;
          Serial.println(add);
          delay(100);
        }
        break;
      
   
  }

  



}
