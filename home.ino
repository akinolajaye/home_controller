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

        case 1://Floors
      
          if (setting[option]=='F'){
            lcd.print("First Floor");
            
          }else if (setting[option]=='G'){
            lcd.print("Ground Floor");
            
          }else if(setting[option]=='O'){
              lcd.print("Outside");        
            
          }
          break;


        case 2://first floor
      
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

        case 3://grounf floor
      
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

        case 4://outside
      
          if (setting[option]=='G'){
            lcd.print("Garage");
            
          }else if (setting[option]=='R'){
            lcd.print("Garden");
            
          }
          break;

        case 5://Type
      
          if (setting[option]=='L'){
            lcd.print("Light");
            
          }else if (setting[option]=='A'){
            lcd.print("Lamp");
            
          }else if(setting[option]=='B'){
              lcd.print("Bathroom");        
            
          }else if(setting[option]=='P'){
              lcd.print("Play Room");        
            
          }
          break;

        
      }
  
  }


int selectFloor(char* setting){
    int select=0;
   
    scrollSettings(setting,select,1);
    bool chosen =false;

    while(!chosen){

  
      static int old_buttons=lcd.readButtons(); //reads number pressed only runs this code once because it is static and so will remember its value
      int buttons =lcd.readButtons();//reads buttons currently pressed
      
      int changes = old_buttons & ~buttons; // uses ~ which means complement and is basicall not but for ints, check if the button pressed has changed from previous
  
    
      if (changes){
        if(old_buttons&BUTTON_RIGHT&&select<2){
          lcd.clear();
          select+=1;
          scrollSettings(setting,select,1);
          
        }else if(old_buttons&BUTTON_LEFT&&select>0){
          lcd.clear();
          select-=1;
          scrollSettings(setting,select,1);
          
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

  char* house[HOUSE_LEN]={floors,first,ground,outside,type,names,qualifier,action,val};
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

  selectFloor(floors);
  lcd.clear();
  delay(10000);

}
