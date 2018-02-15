#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <mcp_can.h>
#include <SPI.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define version 3.4
#define CAN0_INT 2 // Set INT to pin 2
MCP_CAN CAN0(0); // Set CS to pin 10

#define MACRO_1(n1, n2) (n1-n2)
#define MACRO_2 (output_max-output_min)
#define MACRO_3(n1, n2) (n1-n2)
#define FORMULA(n1, n2, n3, n4) (MACRO_1(n1, n2) * MACRO_2 / MACRO_3(n3, n4) + output_min)

const uint8_t ledPin = 15; // PWM pin for the backlight
const uint8_t brightness_pwm = 5; // PWM for backlight while car lights on

// Set the pins on the I2C chip used for LCD connections:
//                    addr,en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address

long unsigned int rxId;
uint8_t rxBuf[8];

String html1 = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\r\n<title>WebSchalter</title>\r\n";

String html2 = "</head>\r\n<body>\r\n</body>\r\n</html>";
 
ESP8266WebServer server(80);    // Server Port  hier einstellen
String Temp = "";
 
void Ereignis_Index()           // Wird ausgeuehrt wenn "http://<ip address>/" aufgerufen wurde
{
  Temp = html1 + String((rxId)) + String((rxBuf[0])) + String((rxBuf[1])) + String((rxBuf[2])) + String((rxBuf[3])) + String((rxBuf[4])) + String((rxBuf[5])) + String((rxBuf[6])) + String((rxBuf[7])) + html2;
  server.send(200, "text/html", Temp);
}

void web_interface_setup()
{
 
  Serial.begin(115200);         // Serielle schnittstelle initialisieren
  Serial.println("");           // Lehere Zeile ausgeben
  Serial.println("Starte WLAN-Hotspot \"BlackBox\"");
  WiFi.mode(WIFI_AP);           // access point modus
  WiFi.softAP("BlackBox", "12345678");    // Name des Wi-Fi netzes
  delay(500);                   //Abwarten 0,5s
  Serial.print("IP Adresse ");  //Ausgabe aktueller IP des Servers
  Serial.println(WiFi.softAPIP());
 
  //  Behandlung der Ereignissen anschliessen
  server.on("/", Ereignis_Index);
 
  server.begin();               // Starte den Server
  Serial.println("HTTP Server gestartet");
}
 
void web_interface_loop()
{
  server.handleClient();
}



const char custom[][8] PROGMEM = {                      // Custom character definitions
    { 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00 }, // char 1 
    { 0x18, 0x1C, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }, // char 2 
    { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x07, 0x03 }, // char 3 
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F }, // char 4 
    { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1E, 0x1C, 0x18 }, // char 5 
    { 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F }, // char 6 
    { 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F }, // char 7 
    { 0x03, 0x07, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }  // char 8 
};

// BIG FONT Character Set
// - Each character coded as 1-4 byte sets {top_row(0), top_row(1)... bot_row(0), bot_row(0)..."}
// - Number of bytes terminated with 0x00; Character is made from [number_of_bytes/2] wide, 2 high
// - Codes are: 0x01-0x08 => custom characters, 0x20 => space, 0xFF => black square

const char bigChars[][8] PROGMEM = {
    { 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Space
    { 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // !
    { 0x05, 0x05, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00 }, // "
    { 0x04, 0xFF, 0x04, 0xFF, 0x04, 0x01, 0xFF, 0x01 }, // #
    { 0x08, 0xFF, 0x06, 0x07, 0xFF, 0x05, 0x00, 0x00 }, // $
    { 0x01, 0x20, 0x04, 0x01, 0x04, 0x01, 0x20, 0x04 }, // %
    { 0x08, 0x06, 0x02, 0x20, 0x03, 0x07, 0x02, 0x04 }, // &
    { 0x05, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // '
    { 0x08, 0x01, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00 }, // (
    { 0x01, 0x02, 0x04, 0x05, 0x00, 0x00, 0x00, 0x00 }, // )
    { 0x01, 0x04, 0x04, 0x01, 0x04, 0x01, 0x01, 0x04 }, // *
    { 0x04, 0xFF, 0x04, 0x01, 0xFF, 0x01, 0x00, 0x00 }, // +
    { 0x20, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 
    { 0x04, 0x04, 0x04, 0x20, 0x20, 0x20, 0x00, 0x00 }, // -
    { 0x20, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // .
    { 0x20, 0x20, 0x04, 0x01, 0x04, 0x01, 0x20, 0x20 }, // /
    { 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00 }, // 0
    { 0x01, 0x02, 0x20, 0x04, 0xFF, 0x04, 0x00, 0x00 }, // 1
    { 0x06, 0x06, 0x02, 0xFF, 0x07, 0x07, 0x00, 0x00 }, // 2
    { 0x01, 0x06, 0x02, 0x04, 0x07, 0x05, 0x00, 0x00 }, // 3
    { 0x03, 0x04, 0xFF, 0x20, 0x20, 0xFF, 0x00, 0x00 }, // 4
    { 0xFF, 0x06, 0x06, 0x07, 0x07, 0x05, 0x00, 0x00 }, // 5
    { 0x08, 0x06, 0x06, 0x03, 0x07, 0x05, 0x00, 0x00 }, // 6
    { 0x01, 0x01, 0x02, 0x20, 0x08, 0x20, 0x00, 0x00 }, // 7
    { 0x08, 0x06, 0x02, 0x03, 0x07, 0x05, 0x00, 0x00 }, // 8
    { 0x08, 0x06, 0x02, 0x07, 0x07, 0x05, 0x00, 0x00 }, // 9
    { 0xA5, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // :
    { 0x04, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ;
    { 0x20, 0x04, 0x01, 0x01, 0x01, 0x04, 0x00, 0x00 }, // <
    { 0x04, 0x04, 0x04, 0x01, 0x01, 0x01, 0x00, 0x00 }, // =
    { 0x01, 0x04, 0x20, 0x04, 0x01, 0x01, 0x00, 0x00 }, // >
    { 0x01, 0x06, 0x02, 0x20, 0x07, 0x20, 0x00, 0x00 }, // ?
    { 0x08, 0x06, 0x02, 0x03, 0x04, 0x04, 0x00, 0x00 }, // @
    { 0x08, 0x06, 0x02, 0xFF, 0x20, 0xFF, 0x00, 0x00 }, // A
    { 0xFF, 0x06, 0x05, 0xFF, 0x07, 0x02, 0x00, 0x00 }, // B
    { 0x08, 0x01, 0x01, 0x03, 0x04, 0x04, 0x00, 0x00 }, // C
    { 0xFF, 0x01, 0x02, 0xFF, 0x04, 0x05, 0x00, 0x00 }, // D
    { 0xFF, 0x06, 0x06, 0xFF, 0x07, 0x07, 0x00, 0x00 }, // E
    { 0xFF, 0x06, 0x06, 0xFF, 0x20, 0x20, 0x00, 0x00 }, // F
    { 0x08, 0x01, 0x01, 0x03, 0x04, 0x02, 0x00, 0x00 }, // G
    { 0xFF, 0x04, 0xFF, 0xFF, 0x20, 0xFF, 0x00, 0x00 }, // H
    { 0x01, 0xFF, 0x01, 0x04, 0xFF, 0x04, 0x00, 0x00 }, // I
    { 0x20, 0x20, 0xFF, 0x04, 0x04, 0x05, 0x00, 0x00 }, // J
    { 0xFF, 0x04, 0x05, 0xFF, 0x20, 0x02, 0x00, 0x00 }, // K
    { 0xFF, 0x20, 0x20, 0xFF, 0x04, 0x04, 0x00, 0x00 }, // L
    { 0x08, 0x03, 0x05, 0x02, 0xFF, 0x20, 0x20, 0xFF }, // M
    { 0xFF, 0x02, 0x20, 0xFF, 0xFF, 0x20, 0x03, 0xFF }, // N
    { 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00 }, // 0
    { 0x08, 0x06, 0x02, 0xFF, 0x20, 0x20, 0x00, 0x00 }, // P
    { 0x08, 0x01, 0x02, 0x20, 0x03, 0x04, 0xFF, 0x04 }, // Q
    { 0xFF, 0x06, 0x02, 0xFF, 0x20, 0x02, 0x00, 0x00 }, // R
    { 0x08, 0x06, 0x06, 0x07, 0x07, 0x05, 0x00, 0x00 }, // S
    { 0x01, 0xFF, 0x01, 0x20, 0xFF, 0x20, 0x00, 0x00 }, // T
    { 0xFF, 0x20, 0xFF, 0x03, 0x04, 0x05, 0x00, 0x00 }, // U
    { 0x03, 0x20, 0x20, 0x05, 0x20, 0x02, 0x08, 0x20 }, // V
    { 0xFF, 0x20, 0x20, 0xFF, 0x03, 0x08, 0x02, 0x05 }, // W
    { 0x03, 0x04, 0x05, 0x08, 0x20, 0x02, 0x00, 0x00 }, // X
    { 0x03, 0x04, 0x05, 0x20, 0xFF, 0x20, 0x00, 0x00 }, // Y
    { 0x01, 0x06, 0x05, 0x08, 0x07, 0x04, 0x00, 0x00 }, // Z
    { 0xFF, 0x01, 0xFF, 0x04, 0x00, 0x00, 0x00, 0x00 }, // [
    { 0x01, 0x04, 0x20, 0x20, 0x20, 0x20, 0x01, 0x04 }, // Backslash
    { 0x01, 0xFF, 0x04, 0xFF, 0x00, 0x00, 0x00, 0x00 }, // ]
    { 0x08, 0x02, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00 }, // ^
    { 0x20, 0x20, 0x20, 0x04, 0x04, 0x04, 0x00, 0x00 }  // _
};
byte col,row,nb=0,bc=0; // General
byte bb[8]; // Byte buffer for reading from PROGMEM

//long unsigned int rxId;
unsigned char len = 0;
//uint8_t rxBuf[8];

typedef void (*pkg_handler_t) (uint8_t rxBuf[8]);

typedef struct {
    long unsigned int id;
    pkg_handler_t handler;
} pkg_routine_t;

/*******************************************************************************
 * Handler routines                                                            *
 ******************************************************************************/

void accelerator_pedal_handler(uint8_t rxBuf[8]) {
    uint8_t accelerator_pedal_raw = rxBuf[5];

    const uint8_t pedal_input_min = 0; // The lowest number of the range input.
    const uint8_t pedal_input_max = 250; // The lowest number of the range input.
    const uint8_t output_min = 0; // The lowest number of the range output.
    const uint8_t output_max = 100; // The largest number of the range ouput.

    uint8_t accelerator_pedal_percent = FORMULA(accelerator_pedal_raw, pedal_input_min, pedal_input_max, pedal_input_min);

    char str_temp[4];

    dtostrf(accelerator_pedal_percent, 3, 0, str_temp);

    print_accelerator_pedal_percent(str_temp);
}

void engine_temp_handler(uint8_t rxBuf[8]) {
    uint8_t engtempraw = rxBuf[5];  

    const uint8_t input_min = 0; // The lowest number of the range input.
    const uint8_t input_max = 255; // The lowest number of the range input.
    const int8_t output_min = -40; // The lowest number of the range output.
    const uint8_t output_max = 215; // The largest number of the range ouput.

    int_fast16_t engtemp_degC = FORMULA(engtempraw, input_min, input_max, input_min);

    char str_temp[4];

    dtostrf(engtemp_degC, 3, 0, str_temp);

    print_engtemp_degC(str_temp);
}

void signal_handler(uint8_t rxBuf[8]) {
    static uint8_t oldbits = 0;
    const uint8_t newbits = rxBuf[0];

    if(oldbits==newbits)
    {
        return;
    }
    else
    {
        oldbits=newbits;

        if(rxBuf[0] & 0b00100000) // Turn signal left
        {
            print_turn_signal_left("<=");
        }
        else
        {
            print_turn_signal_left("  ");
        }
  
        if(rxBuf[0] & 0b00010000) // Turn signal right
        {
            print_turn_signal_right("=>");
        }
        else
        {
            print_turn_signal_right("  ");
        }

        /*if(rxBuf[0] & 0b01000000) // If headlight, dim the backlight
        {
            analogWrite(ledPin, brightness_pwm);
        }
        else
        {
            analogWrite(ledPin, 1023);
        }*/
    }
}

void mileage_handler(uint8_t rxBuf[8]) {
    static uint32_t oldkm1 = 0;
    static uint32_t oldkm2 = 0;
    static uint32_t oldkm3 = 0;

    const uint8_t newkm1 = rxBuf[2];
    const uint8_t newkm2 = rxBuf[3];
    const uint8_t newkm3 = rxBuf[4];

    if(oldkm1==newkm1 && oldkm2==newkm2 && oldkm3==newkm3)
    {
        return;
    }
    else
    {
        oldkm1=newkm1;
        oldkm2=newkm2;
        oldkm3=newkm3;
  
        uint32_t km5=(oldkm1<<16)|(oldkm2<<8)|(oldkm3);

        char str_temp[8];
  
        dtostrf(km5, 7, 0, str_temp);
  
        print_km(str_temp);
    }
}

void signal_and_mileage_handler(uint8_t rxBuf[8]) {
    signal_handler(rxBuf);
    mileage_handler(rxBuf);
}

void speed_handler(uint8_t rxBuf[8]) {
    uint8_t speed_kmh = rxBuf[1];

    char str_temp[4];

    dtostrf(speed_kmh, 3, 0, str_temp);

    print_kmh(str_temp);
}

void rpm_handler(uint8_t rxBuf[8]) {
    uint8_t rpm1 = rxBuf[1];
    uint8_t rpm2 = rxBuf[2];
    uint16_t rpm3=(rpm1<<8)|(rpm2);

    char str_temp[5];

    dtostrf(rpm3, 4, 0, str_temp);

    print_rpm(str_temp);
}

void gear_handler(uint8_t rxBuf[8]) {
    int gear = rxBuf[3];

    if(gear == 0)
    {
        writeBigString("N", 0, 2);
    }
    if(gear == 17)
    {
        writeBigString("1", 0, 2);
        writeBigString(" ", 3, 2);
    }
    if(gear == 34)
    {
        writeBigString("2", 0, 2);
        writeBigString(" ", 3, 2);
    }
    if(gear == 51)
    {
        writeBigString("3", 0, 2);
        writeBigString(" ", 3, 2);
    }
    if(gear == 68)
    {
        writeBigString("4", 0, 2);
        writeBigString(" ", 3, 2);
    }
    if(gear == 85)
    {
        writeBigString("5", 0, 2);
        writeBigString(" ", 3, 2);
    }
    if(gear == 102)
    {
        writeBigString("6", 0, 2);
        writeBigString(" ", 3, 2);
    }
    if(gear == 119)
    {
        writeBigString("R", 0, 2);
        writeBigString(" ", 3, 2);
    }
}

void rpm_and_gear_handler(uint8_t rxBuf[8]) {
    rpm_handler(rxBuf);
    gear_handler(rxBuf);
}

void fuel_tank_volume_handler(uint8_t rxBuf[8]) {
    uint8_t volumeraw = rxBuf[6];

    if(volumeraw == 255)
    {
        print_volume_l("  err");
    }
    else
    {
        float volume_l = volumeraw*0.5;
        char str_temp[6];

        dtostrf(volume_l, 5, 1, str_temp);

        print_volume_l(str_temp);
    }
}

pkg_routine_t pkg_routines[] = {
    {
        .id = 0x310,
        .handler = accelerator_pedal_handler,
    },
    {
        .id = 0x190,
        .handler = engine_temp_handler,
    },
    {
        .id = 0x220,
        .handler = signal_and_mileage_handler,
    },
    {
        .id = 0x23a,
        .handler = speed_handler,
    },
    {
        .id = 0x300,
        .handler = rpm_and_gear_handler,
    },
    {
        .id = 0x515,
        .handler = fuel_tank_volume_handler,
    },{.id = 0, .handler = NULL}
};

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/

void setup() {
    Serial.begin(9600); // Used to type in characters

    lcd.begin(20,4);
    for (nb=0; nb<8; nb++ ) // Create 8 custom characters
    {
        for (bc=0; bc<8; bc++) bb[bc]= pgm_read_byte( &custom[nb][bc] );
        lcd.createChar ( nb+1, bb );
    }

    bootscreen();
    init_mcp();

    lcd.clear();
    print_emptyValues();
    web_interface_setup();
}

void loop() {

    web_interface_loop();
    
    if(!digitalRead(CAN0_INT)) // If CAN0_INT pin is low, read receive buffer
    {
        CAN0.readMsgBuf(&rxId, &len, rxBuf); // Read data: len = data length, buf = data byte(s)

        const unsigned command_cnt = sizeof(pkg_routines)/sizeof(pkg_routine_t);

        for(size_t i = 0; i < command_cnt; i++)
        {
            if(rxId == pkg_routines[i].id)
            {
                if(pkg_routines[i].handler)
                {
                    pkg_routines[i].handler(rxBuf);
                }
            }
        }
    }
}

/*******************************************************************************
 * BigFont routines                                                            *
 ******************************************************************************/

// writeBigChar: writes big character 'ch' to column x, row y; returns number of columns used by 'ch'
int writeBigChar(char ch, byte x, byte y) {
    if (ch < ' ' || ch > '_') return 0; // If outside table range, do nothing

    nb=0; // character byte counter 
    for (bc=0; bc<8; bc++) 
    {
        bb[bc] = pgm_read_byte( &bigChars[ch-' '][bc] ); // Read 8 bytes from PROGMEM
        if(bb[bc] != 0) nb++;
    }

    bc=0;
    for (row = y; row < y+2; row++)
    {
        for (col = x; col < x+nb/2; col++ ) {
        lcd.setCursor(col, row); // Move to position
        lcd.write(bb[bc++]); // Write byte and increment to next
    }
        //lcd.setCursor(col, row);
        //lcd.write(' '); // Write ' ' between letters
    }
    return nb/2-1; // Returns number of columns used by char
}

// writeBigString: writes out each letter of string
void writeBigString(char *str, byte x, byte y) {
    char c;
    while ((c = *str++))
    x += writeBigChar(c, x, y) + 1;
}

/*******************************************************************************
 * Setup routines                                                              *
 ******************************************************************************/

void bootscreen() {
    lcd.backlight();
    for (int fadeValue = 0 ; fadeValue <= 1023; fadeValue += 8)
    {
        // Sets the value (range from 0 to 1023):
        analogWrite(ledPin, fadeValue);
        // Wait for 30 milliseconds to see the dimming effect
        delay(30);
    }
    writeBigString("SMART",2,0);
    lcd.setCursor(0,2);
    lcd.print(":.:.:_Bl4ckB0x_:.:.:");
    lcd.setCursor(0,3);
    lcd.print(version);
    delay(3000);
}

void init_mcp() {
    // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
    if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    {
        Serial.println("MCP2515 Initialized Successfully!");
        lcd.setCursor(0,3);
        lcd.print("MCP2515 O.K.");
        delay(1000);
    }
    else
    {
        lcd.clear();
        Serial.println("Error Initializing MCP2515...");
        lcd.print("Error MCP2515...");
        delay(2000);
        lcd.setCursor(0,2);
        lcd.print("Restarting...");
        delay(2000);
        setup();
    }

    //CAN0.setMode(MCP_NORMAL);
    CAN0.setMode(MCP_LISTENONLY); // Set operation mode to MCP_NORMAL so the MCP2515 sends acks to received data.

    pinMode(CAN0_INT, INPUT); // Configuring pin for /INT input

    Serial.println("MCP2515 Listen-Only...");
}

/*******************************************************************************
 * Print routines                                                              *
 ******************************************************************************/

void print_accelerator_pedal_percent(char str[]) {
    lcd.setCursor(16,3);
    lcd.print(str);
    lcd.setCursor(19,3);
    lcd.print("%");
}

void print_turn_signal_left(char str[]) {
    lcd.setCursor(5,2);
    lcd.print(str);
}

void print_turn_signal_right(char str[]) {
    lcd.setCursor(13,2);
    lcd.print(str);
}

void print_engtemp_degC(char str[]) {
    lcd.setCursor(14,0);
    lcd.print(str);
    lcd.setCursor(17,0);
    lcd.print(" ");
    lcd.print((char)223);
    lcd.print("C");
}

void print_km(char str[]) {
    lcd.setCursor(5,3);
    lcd.print(str);
    lcd.setCursor(12,3);
    lcd.print(" km");
}

void print_kmh(char str[]) {
    lcd.setCursor(1,1);
    lcd.print(str);
    lcd.setCursor(4,1);
    lcd.print(" km/h");
}

void print_rpm(char str[]) {
    lcd.setCursor(0,0);
    lcd.print(str);
    lcd.setCursor(4,0);
    lcd.print(" RPM");
}

void print_volume_l(char str[]) {
    lcd.setCursor(12,1);
    lcd.print(str);
    lcd.setCursor(18,1);
    lcd.print(" L");
}

void print_emptyValues() {
    print_accelerator_pedal_percent("  -");
    print_engtemp_degC("  -");
    print_km("      -");
    print_kmh("  -");
    print_rpm("   -");
    print_volume_l("    -");
    writeBigString("-", 0, 2);
    writeBigString(" ", 3, 2);
}
