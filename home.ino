#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <EEPROM.h>
#include <avr/eeprom.h>
#define DEBUG
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();



void writeEE(){//function to write house description into eeprom

  int HOUSE_LEN=10;
  char floors[4]={'F','G','O'};
  char first[5]={'1','2','B','P'};
  char ground[6]={'K','H','L','3','4'};
  char outside[3]={'G','R'};
  char type[5]={'L','A','H','W'};
  char names[7]={'M','C','D','B','P','W'};
  char qualifier[6]={'1','2','3','L','R'};
  char action[4]={'1','0','L'};
  char  val[3]={'T','R'};
  char* house[HOUSE_LEN]={floors,first,ground,outside,type,names,qualifier,action,val};
  int totalLen=0;
  int written=EEPROM.read(0);

  if (written ==0){
    EEPROM.write(totalLen,1);
    for(int i=0;i<HOUSE_LEN-1;i++){
      for(int j=0;j<strlen(house[i]);j++){
        totalLen++;
        EEPROM.write(totalLen,house[i][j]);
      }
    }

    #ifdef DEBUG
    Serial.println("done"); 
    #endif
   
  }else{
    
    #ifdef DEBUG
    Serial.println("data already written");
    #endif
    
  }
  

  

  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  writeEE();


}

void loop() {
  // put your main code here, to run repeatedly:

}
