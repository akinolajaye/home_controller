#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <EEPROM.h>
#include <avr/eeprom.h>
#define DEBUG
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

int HOUSE_LEN=10;
char floors[4]="";
char first[5]="";
char ground[6]="";
char outside[3]="";
char type[5]="";
char names[7]="";
char qualifier[6]="";
char action[4]="";
char  val[3]="";
int written=EEPROM.read(0);// reads the first value in the eeprom 
char* house[10]={floors,first,ground,outside,type,names,qualifier,action,val};
typedef enum state_e {f=0,fl,g,o,t,n,q,a,v} state_t;


void writeEE(){//function to write house description into eeprom
  char floors[5]={'F','G','O','-'};
  char first[6]={'1','2','B','P','-'};
  char ground[7]={'K','H','L','3','4','-'};
  char outside[4]={'G','R','-'};
  char type[6]={'L','A','H','W','-'};
  char names[8]={'M','C','D','B','P','W','-'};
  char qualifier[7]={'1','2','3','L','R','-'};
  char action[5]={'1','0','L','-'};
  char  val[4]={'T','R','x'};
  char* house[HOUSE_LEN]={floors,first,ground,outside,type,names,qualifier,action,val};//stores pointers to the different char arrays
  int totalLen=0;

  EEPROM.write(totalLen,1); //writes a 1 at the first memory location to signify data has been written to
  for(int i=0;i<HOUSE_LEN-1;i++){
    for(int j=0;j<strlen(house[i]);j++){
      totalLen++;
      EEPROM.write(totalLen,house[i][j]);
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

      while((char) EEPROM.read(count)!='-'){//keep looping until it sees a '-' which signifies that it should move to the next descriptor
        
        if((char) EEPROM.read(count)=='x'){break;}//breaks when it sees a 'x' which signifies the end of all the data being reae
   
        house[i][j]=(char) EEPROM.read(count);
        j++;
        count++;
        
      }
      count++;
       
    }
       
}


void scrollSettings(char* setting,int option,int num_setting){

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


        case 1://first floor
      
          if (setting[option]=='1'){
            lcd.print("1st Bedroom");
            
          }else if (setting[option]=='2'){
            lcd.print("2nd Bedroom");
            
          }else if(setting[option]=='B'){
              lcd.print("Bathroom");        
            
          }else if(setting[option]=='P'){
              lcd.print("Play Room");        
            
          }
          break;

        case 2://grounf floor
      
          if (setting[option]=='K'){
            lcd.print("Kitchen");
            
          }else if (setting[option]=='H'){
            lcd.print("Hall");
            
          }else if(setting[option]=='L'){
              lcd.print("Lounge");        
            
          }else if(setting[option]=='3'){
              lcd.print("3rd Bedroom");        
            
          }else if(setting[option]=='4'){
              lcd.print("4th Bedroom");        
            
          }
          break;

        case 3://outside
      
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
            
          }else if(setting[option]=='W'){
              lcd.print("Water");        
            
          }
          break;

        case 5://names
      
          if (setting[option]=='M'){
            lcd.print("Main");
            
          }else if (setting[option]=='C'){
            lcd.print("Ceiling");
            
          }else if(setting[option]=='D'){
              lcd.print("Desk");        
            
          }else if(setting[option]=='B'){
              lcd.print("Bed");        
            
          }else if(setting[option]=='P'){
              lcd.print("Cupboard");        
            
          }else if(setting[option]=='W'){
              lcd.print("Wall");        
            
          }
          break;
          
        case 6://qualifiers
      
          if (setting[option]=='1'){
            lcd.print("One");
            
          }else if (setting[option]=='2'){
            lcd.print("Two");
            
          }else if(setting[option]=='3'){
              lcd.print("Three");        
            
          }else if(setting[option]=='L'){
              lcd.print("Left");        
            
          }else if(setting[option]=='R'){
              lcd.print("Right");        
            
          }
          break;

        case 7://Action
      
          if (setting[option]=='1'){
            lcd.print("On");
            
          }else if (setting[option]=='0'){
            lcd.print("Off");
            
          }else if(setting[option]=='L'){
              lcd.print("Level");        
            
          }
          break;

        case 8://Values
      
          if (setting[option]=='T'){
            lcd.print("Time: ");
            
          }else if (setting[option]=='R'){
            lcd.print("Range: ");
            
          }
          break;
   
      }
  
  }


int chooseSettings(char* setting,int num_setting){
    int select=0;
   
    scrollSettings(setting,select,num_setting);
    bool chosen =false;

    while(!chosen){
  
      static int old_buttons=lcd.readButtons(); //reads number pressed only runs this code once because it is static and so will remember its value
      int buttons =lcd.readButtons();//reads buttons currently pressed
      
      int changes = old_buttons & ~buttons; // uses ~ which means complement and is basicall not but for ints, check if the button pressed has changed from previous
  
    
      if (changes){
        if(old_buttons&BUTTON_RIGHT&&select<strlen(setting)-1){
          lcd.clear();
          select+=1;
          scrollSettings(setting,select,num_setting);
          
        }else if(old_buttons&BUTTON_LEFT&&select>0){
          lcd.clear();
          select-=1;
          scrollSettings(setting,select,num_setting);
          
        }else if(old_buttons&BUTTON_SELECT){

          chosen=true;
        }  
        
    }
  
      old_buttons=buttons;
  }

  return select;
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
  state_t setting=g;

  chooseSettings(house[g],g);
  lcd.clear();
  delay(10000);

}
