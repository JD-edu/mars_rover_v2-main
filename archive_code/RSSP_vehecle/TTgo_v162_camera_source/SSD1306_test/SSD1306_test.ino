/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <U8x8lib.h>
#include <SPI.h>



#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#define SCL 22
#define SDA 21

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display


void setup() {
    
    u8x8.begin();
    u8x8.setPowerSave(0);
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    //u8x8.setFont(u8x8_font_px437wyse700a_2x2_r);
    delay(50);
    u8x8.clear();
    u8x8.setFlipMode(1);
    u8x8.drawString(1,0,"ugaa");
    u8x8.drawString(1,10,"siva");
    u8x8.drawString(1,20,"joto");
    u8x8.drawString(1,30,"nimi");
}



void loop() {



}
