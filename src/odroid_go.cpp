// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "odroid_go.h"

void ODROID_GO::begin(unsigned long baud) {

    // UART
    Serial.begin(baud);

    Serial.flush();
    Serial.print("ODROID_GO initializing...");

    // Speaker.begin();

    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    pinMode(BUTTON_B_PIN, INPUT_PULLUP);
    pinMode(BUTTON_MENU, INPUT_PULLUP);
    pinMode(BUTTON_SELECT, INPUT_PULLUP);
    pinMode(BUTTON_START, INPUT_PULLUP);
    pinMode(BUTTON_VOLUME, INPUT_PULLUP);
    pinMode(BUTTON_JOY_Y, INPUT_PULLDOWN);
    pinMode(BUTTON_JOY_X, INPUT_PULLDOWN);

    // ODROID_GO LCD INIT
    lcd.begin();
    lcd.setRotation(1);
    lcd.fillScreen(BLACK);
    lcd.setCursor(0, 0);
    lcd.setTextColor(WHITE);
    lcd.setTextSize(1);
    lcd.setBrightness(255);

    // Battery
    // battery.begin();

    Serial.println("OK");
}

void ODROID_GO::update() {

    //Button update
    BtnA.read();
    BtnB.read();
    BtnMenu.read();
    BtnVolume.read();
    BtnSelect.read();
    BtnStart.read();
    JOY_Y.readAxis();
    JOY_X.readAxis();

    //Speaker update
    // Speaker.update();
    // battery.update();
}

uint8_t ODROID_GO::checkButtons(){
    update();
    return ( BtnA.isChanged() || BtnB.isChanged() || BtnMenu.isChanged() || BtnVolume.isChanged() || 
    BtnSelect.isChanged() || BtnStart.isChanged() || JOY_Y.isChanged() || JOY_X.isChanged() );
}

void ODROID_GO::command (const uint8_t *data, size_t payloadLen)
{

    uint8_t cmd = data[0]; 
    if( cmd >= 32 && cmd <= 126){
        //string message
        // data++;
        for(int i = 0; i < payloadLen  ; i++)
            GO.lcd.write( *(data++) );
    }else{
        //byte command
        switch( cmd ){
            //

            case 0 ://setTextFont
                {
                GO.lcd.setTextFont( data[1] );
                } 
                break;
            case 1 ://setTextSize
                {
                GO.lcd.setTextSize( data[1] );
                } 
                break;
            case 2 ://setTextColor
                {
                uint16_t color =  ( (uint16_t)data[2] << 8 ) + data[1];
                uint16_t colorbg =  ( (uint16_t)data[4] << 8 ) + data[3];
                GO.lcd.setTextColor( color, colorbg);
                } 
                break;
            case 3 ://setCursor
                {
                //read i16 x ,i16 y
                int16_t x =  ( (uint16_t)data[2] << 8 ) + data[1];
                int16_t y =  ( (uint16_t)data[4] << 8 ) + data[3];
                GO.lcd.setCursor(x,y);
                } 
                break;
            case 4 ://fillRect
                {
                    if(payloadLen == 11){
                        int16_t x =  ( (uint16_t)data[2] << 8 ) + data[1];
                        int16_t y =  ( (uint16_t)data[4] << 8 ) + data[3];
                        int16_t w =  ( (uint16_t)data[6] << 8 ) + data[5];
                        int16_t h =  ( (uint16_t)data[8] << 8 ) + data[7];
                        uint16_t color =  ( (uint16_t)data[10] << 8 ) + data[9];
                        GO.lcd.fillRect( (int32_t)x, (int32_t)y, (int32_t)w, (int32_t)h, (uint32_t)color );
                        
                    }else{
                        GO.lcd.print("invalid fillRect Size");
                        GO.lcd.print( payloadLen);
                    }
                }
                break;

            case 5 ://fillBlack, position 0
                {
                  GO.lcd.setCursor(0,0);
                  GO.lcd.setTextColor(WHITE);
                  GO.lcd.clear();
                } 
                break;
            case 6 :// image (rgb565 format)
                {
                    if(payloadLen == 153601){
                      data++;
                      uint16_t *img = (uint16_t *)data;
                      GO.lcd.pushRect(0,0,320,240,img);
                    }else{
                      GO.lcd.print("invalid rgb565 Size");
                      GO.lcd.print( payloadLen);
                    }
                break;
                }

            default:
                GO.lcd.print("invalid CMD");
            break;
        }
    }
    

//    for( int d = 0; d < payloadLen ; d++) GO.lcd.write( *(data++));



}




ODROID_GO GO;
