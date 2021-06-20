//***************************************************************************************************
//**************** WRITTEN by WILLIAM J. SANEK, BBA, INFORMATION SYSTEMS ****************
//********************************** UNPUBLISHED WORK (c) 2021 ********************************
//**************************************************************************************************

// Create Variables:
String Mac;                              //For reading Mac Address
String board = "";                       //For displaying NodeMCU board number
String Test;                             //For general variable display on a page
String BH1750_incl_Light_Sensor_HTML;    //For ex: of alternate method of using includes for HTML
int LEDPin = 16; // THE GPIO FOR LED     //For LED routines
float temp_raw = 000.00;                 //For DS18B20 Temp sensor
char temp_converted[15];                 //For DS18B20 Temp sensor
String temp_final = "";                  //For DS18B20 Temp sensor
float bright = 000.00;                   //For BH1750 Light sensor
String brightt = "";                     //For BH1750 Light sensor
char bright_converted[15];               //For BH1750 Light sensor
float lux = 000.00;                      //For BH1750 Light sensor
char lux_converted[15];                  //For BH1750 Light sensor
String luxx = "";                        //For BH1750 Light sensor
char humidity_converted[15];             //For DHT22 Humidity sensor
char temperature_converted[15];          //For DHT22 Humidity sensor
String temperaturee = "";                //For DHT22 Humidity sensor
String humidityy = "";                   //For DHT22 Humidity sensor
float humidity;                          //For BME280 Barometer sensor
float temperature;                       //For BME280 Barometer sensor
char bar_inches_converted[15];           //For BME280 Barometer sensor
String bar_inchess;                      //For BME280 Barometer sensor
char BME280_temperature_converted[15];   //For BME280 Barometer sensor
float BME280_temperature_raw = 000.00;   //For BME280 Barometer sensor
String BME280_temperature;               //For BME280 Barometer sensor
int BME280_temperature_rounded;          //For BME280 Barometer sensor
char BME280_humidity_converted[15];      //For BME280 Barometer sensor
float BME280_humidity_raw = 000.00;      //For BME280 Barometer sensor
String BME280_humidity;                  //For BME280 Barometer sensor
String BME280_Temp_Tab;                  //For BME280 Barometer sensor
String BME280_Humid_Tab;                 //For BME280 Barometer sensor
String BME280_Barom_Tab;                 //For BME280 Barometer sensor
String weather = "";                     //For BME280 Barometer sensor
float pressure, altitude, bar_inches;    //For BME280 Barometer sensor
String InputNum1 = "";                   //For APPEND input form
String InputNum2 = "";                   //For SEARCH input form
String InputNum3 = "";                   //For SET FILE NAME input form
String InputNum4 = "";                   //For UPDATE input form
String page;                             //For future tests
String Temp_Page_From_String = "";       //For future tests
String x = "";                           //General work use
String y = "";                           //General work use
String z = "";                           //General work use
int work;                                //A work variable
char pick;                               //Character read off of SD CARD
String pickk;                            //Assists in constructing a read record
String pickkk;                           //The completed read record
String trans;                            //Assembled record for printing to HTML page in APPEND routine
String result="";                           //Holds search result for HTML page display
int flag1=0;                             //Prevents deletion of a file with out a confirmation
String filename ="";                     //Stores entered file name
int has_fields=0;                        //Indicates record contains multiple fields
String file_list="";                     //Builds list of files on SD CARD
String record_list="";                   //Builds list of record entries from a search
int field_num=0;                         //Counts fields/enties found in a search
String field_num_string="";              //Converts int field_num to a string for printing in HTML
int field_num_index=0;                   //Counts main (index) entries found in a search
int append_field_count=0;                //Counts how many append entries were made during appends
int rec_num_when_listing=0;              //Counts records when listing them
int file_pos=0;                          //Record's last byte file position
int record_pos=0;                        //Record's length incl the two non-printing end chars 
int target_is_at_file_pos=0;             //Update target record's first char position in file

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h> // FOR OTA PROGRAMMING & UPLOADING
#include <WiFiUdp.h>     // FOR OTA PROGRAMMING & UPLOADING
#include <ArduinoOTA.h>  // FOR OTA PROGRAMMING & UPLOADING
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>           //FOR DS18B20 TEMP SENSOR on D4
#include <DallasTemperature.h> //FOR DS18B20 TEMP SENSOR on D4
#include <Wire.h>
#include <BH1750.h>            //FOR BH1750 LIGHT SENSOR
#include <DHTesp.h>            //FOR DHT22 HUMIDITY SENSOR
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>   //FOR BME280 BAROMETER SENSOR
#include <SPI.h>
#include <SD.h>
//#include <SPI.h>
//#include <SD.h>
//Our HTML webpages contents:
//#include "index.h"  (I Use Subroutine Now Instead)
//#include "gohigh.h" (I Use Subroutine Now Instead)
//#include "golow.h"  (I Use Subroutine Now Instead)
//#include "DS18B20_Temp_Sensor_Read.h"(I Use Subroutine Now Instead)

#define ONE_WIRE_BUS 0                 //FOR DS18B20 TEMP SENSOR on D3
#define SEALEVELPRESSURE_HPA (1013.25)
OneWire oneWire(ONE_WIRE_BUS);         //FOR DS18B20 TEMP SENSOR on D3
DallasTemperature sensors(&oneWire);   //FOR DS18B20 TEMP SENSOR on D3
BH1750 lightMeter;
DHTesp dht;
Adafruit_BME280 bme;
File myFile; //Refer to SD CARD COMMANDS 'FILE CLASS'. Here 'file' becomes 'myfile'. 
             //Used for file differentiation when using more than one file.
             
// Credentials if in STA WIFI MODE:
const char* ssid = "HAL9000";
const char* password = "Znildorkpad5+";

ESP8266WebServer server(80);
// Credentials if in AP WIFI MODE:
// const char* www_username = "admin";
// const char* www_password = "esp8266";

// void handleRoot() {
// String s = MAIN_page; //Read HTML contents
// server.send(200, "text/html", s); //Send web page
// }
// void ledhigh() {
// String s = HIGH_page; //Read HTML contents
// server.send(200, "text/html", s); //Send web page
// }
// void ledlow() {
// String s = LOW_page; //Read HTML contents
// server.send(200, "text/html", s); //Send web page
// }

void setup(void){

// Assign Variables With Some Value:
//page = "<h1>SWITCHBOARD</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
x = "Billy";

// Hardware Setups:
pinMode(LEDPin, OUTPUT);
digitalWrite(LEDPin, LOW);
delay(100);

// Start Up The Services:
Serial.begin(115200);
delay(100);
SD.begin(15); //GPIO FOR SD CARD ADAPTER CS PIN
bme.begin(0x76);
Wire.begin(D2, D1); //D2 (SDA) D1 (SCL)
lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
dht.setup(2, DHTesp::DHT22); // Connect DHT22 sensor to GPIO 2 on D3
sensors.begin();
WiFi.begin(ssid, password);
WiFi.mode(WIFI_STA);
// WiFi.mode(WIFI_AP);
// WiFi.mode(WIFI_AP_STA);

// Wait Until A Connection Is Accomplished:
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

//WAS USED IN ORIGINAL SAMPLE OTA DEMO SKETCH:
//while (WiFi.waitForConnectResult() != WL_CONNECTED) {
//    Serial.println("Connection Failed! Rebooting...");
//    delay(5000);
//    ESP.restart();
//  }

ArduinoOTA.begin(); // FOR OTA PROGRAMMING & UPLOADING

// WHAT TO DO WHEN A CERTAIN BUTTON/LINK IS CLICKED ON A PAGE:

// General Usage: server.on(path,subroutine_name)

// Used With Credentials When AP WIFI MODE IS USED:
// server.on("/", []() {
// if (!server.authenticate(www_username, www_password)) {
// return server.requestAuthentication();
// }
// server.send(200, "text/html", MAIN_page);
// });

//********BUTTONS TAG AND FORMS SUBMIT BUTTOM PROCESSING (are different)*********
// Used With Credentials When STA WIFI MODE IS USED:
server.on("/", [](){ //FOR MAIN HTML
Test = "Hello"; //EX: Variables In HTML Must Be Assigned Before HTML Is Sent
Mac = WiFi.macAddress(); //Displays Mac No. In Page
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
if (Mac == "BC:DD:C2:9D:2D:24") {board = "3";}; //Identify Board Number As Labeled On Board
if (Mac == "BC:DD:C2:9D:E4:F4") {board = "2";}; //Identify Board Number As Labeled On Board
if (Mac == "CC:50:E3:08:94:A7") {board = "1";}; //Identify Board Number As Labeled On Board
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
//subMain(); //Main HTML Page Subroutine
delay(2000);
server.send(200, "text/html", MainHTML());
delay(5000);
});

// i.e. What To Do When LEDon Button Is Clicked, etc:

//»»»»»»»»»»»»FORMS BUTTON PROCESSING (requires a function)»»»»»»»»»»»»»»»»»»»
//EX: FORM1 INPUT IS DIRECTED TO A PROCESSING FUNCTION:
server.on("/form1_process", funcForm1); 

//SD CARD DIRECTED TO IT'S APPEND PROCESSING FUNCTION:
server.on("/form_sd_card_append", funcForm2);

//SD CARD DIRECTED TO IT'S DELETE FILE PROCESSING FUNCTION:
server.on("/form_sd_card_file_delete", funcForm3);

//SD CARD DIRECTED TO IT'S PRINT DIRECTORY PROCESSING FUNCTION:
server.on("/form_sd_card_print_directory", funcForm4);

//SD CARD DIRECTED TO IT'S LIST RECORDS PROCESSING FUNCTION:
server.on("/form_list_sd_card_file_records", funcForm5);

//SD CARD DIRECTED TO IT'S SEARCH RECORDS PROCESSING FUNCTION:
server.on("/form_search_sd_card_file_records", funcForm6);

//SD CARD DIRECTED TO IT'S ENTER FILE NAME PROCESSING FUNCTION:
server.on("/form_sd_card_filename", funcForm7);

//SD CARD DIRECTED TO IT'S GO BACK TO MAIN PAGE PROCESSING FUNCTION:
server.on("/form_sd_card_go_main", funcForm8);

//SD CARD DIRECTED TO IT'S UPDATE RECORDS PROCESSING FUNCTION:
server.on("/form_sd_card_update", funcForm9);
//»»»»»»»»»»»»»»»END FORMS BUTTON PROCESSING (requires a function)»»»»»»»»»»»»»»»»»»»

server.on("/LEDOn/", [](){ //FOR PIN HIGH
Test = "Hello"; //EX: Variables In HTML Must Be Assigned Before HTML Is Sent
Mac = WiFi.macAddress(); //Displays Mac No. In Page
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
if (Mac == "BC:DD:C2:9D:2D:24") {board = "3";}; //Identify Board Number As Labeled On Board
if (Mac == "BC:DD:C2:9D:E4:F4") {board = "2";}; //Identify Board Number As Labeled On Board
if (Mac == "CC:50:E3:08:94:A7") {board = "1";}; //Identify Board Number As Labeled On Board
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
//subPinHigh(); //Pin High HTML Page Subroutine 
digitalWrite(LEDPin, HIGH);
delay(2000);
server.send(200, "text/html", PinHighHTML());
delay(5000);
});

server.on("/LEDOff/", [](){ //FOR PIN LOW
Test = "Hello"; //EX: Variables In HTML Must Be Assigned Before HTML Is Sent
Mac = WiFi.macAddress(); //Displays Mac No. In Page
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
if (Mac == "BC:DD:C2:9D:2D:24") {board = "3";}; //Identify Board Number As Labeled On Board
if (Mac == "BC:DD:C2:9D:E4:F4") {board = "2";}; //Identify Board Number As Labeled On Board
if (Mac == "CC:50:E3:08:94:A7") {board = "1";}; //Identify Board Number As Labeled On Board
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
//subPinLow();  //Pin Low HTML Page Subroutine
digitalWrite(LEDPin, LOW);
delay(2000);
server.send(200, "text/html", PinLowHTML());
delay(5000);
});

server.on("/TEMP_READ/", [](){ //FOR READING DS18B20 TEMP SENSOR
// Aquiring, Assembling and Converting Reading For Final Useage and-or Tab Display:
sensors.requestTemperatures();
temp_raw = sensors.getTempFByIndex(0);
dtostrf(temp_raw,6,2,temp_converted); // Assembling and Converting Raw Reading Variable(s) For Page Tab Display:
temp_final = temp_converted;
///////////////////////////////////////////////////////////
Test = "Hello"; //EX: Variables In HTML Must Be Assigned Before HTML Is Sent
Mac = WiFi.macAddress(); //Displays Mac No. In Page
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
if (Mac == "BC:DD:C2:9D:2D:24") {board = "3";}; //Identify Board Number As Labeled On Board
if (Mac == "BC:DD:C2:9D:E4:F4") {board = "2";}; //Identify Board Number As Labeled On Board
if (Mac == "CC:50:E3:08:94:A7") {board = "1";}; //Identify Board Number As Labeled On Board
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
//DS18B20_Temp_Sensor_Reader(); //Subroutine 'Temp Sensor Read' HTML Page
delay(2000);
server.send(200, "text/html", DS18B20_Temp_Sensor_HTML());
delay(5000);
});

server.on("/LUX_READ/", [](){ //FOR READING BH1750 LIGHT SENSOR
// Aquiring, Assembling and Converting Reading For Final Useage and-or Tab Display:
bright = lightMeter.readLightLevel(); //Must read lux sensor twice due to sensor quirk
delay(2000);                          //Must read lux sensor twice due to sensor quirk
bright = lightMeter.readLightLevel(); //Must read lux sensor twice due to sensor quirk
dtostrf(bright,6,2,bright_converted); // Assembling and Converting Raw Reading Variable(s) For Page Tab Display:
brightt = bright_converted;
///////////////////////////////////////////////////////////
Test = "Hello"; //EX: Variables In HTML Must Be Assigned Before HTML Is Sent
Mac = WiFi.macAddress(); //Gets Mac No. For Display In Page
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
if (Mac == "BC:DD:C2:9D:2D:24") {board = "3";}; //Identify Board Number As Labeled On Board
if (Mac == "BC:DD:C2:9D:E4:F4") {board = "2";}; //Identify Board Number As Labeled On Board
if (Mac == "CC:50:E3:08:94:A7") {board = "1";}; //Identify Board Number As Labeled On Board
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
//BH1750_Light_Sensor_Reader(); //Subroutine 'Temp Sensor Read' HTML Page
delay(2000);
server.send(200, "text/html", BH1750_Light_Sensor_HTML());
delay(5000);
//server.send(00, "text/html", BH1750_incl_Light_Sensor_HTML);
});

server.on("/HUMIDITY_TEMP_READ/", [](){ //FOR READING DHT22 HUMIDITY/TEMP SENSOR
// Aquiring, Assembling and Converting Reading For Final Useage and-or Tab Display:
humidity = dht.getHumidity();
temperature = dht.getTemperature();
temperature = temperature * 1.8; //Convert To Fahrenheit
temperature = temperature + 32;  //Convert To Fahrenheit
dtostrf(humidity,6,2,humidity_converted); // Assembling and Converting Raw Reading Variable(s) For Page Tab Display:
humidityy = humidity_converted;
dtostrf(temperature,6,2,temperature_converted); // Assembling and Converting variable 'humidity and temperature' For Page Tab Display:
temperaturee = temperature_converted;
///////////////////////////////////////////////////////////
Test = "Hello"; //EX: Variables In HTML Must Be Assigned Before HTML Is Sent
Mac = WiFi.macAddress(); //Gets Mac No. For Display In Page
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
if (Mac == "BC:DD:C2:9D:2D:24") {board = "3";}; //Identify Board Number As Labeled On Board
if (Mac == "BC:DD:C2:9D:E4:F4") {board = "2";}; //Identify Board Number As Labeled On Board
if (Mac == "CC:50:E3:08:94:A7") {board = "1";}; //Identify Board Number As Labeled On Board
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
//DHT22_Humidity_Sensor_Reader(); //Subroutine 'Humidity/Temp Sensor Read' HTML Page
delay(2000);
server.send(200, "text/html", DHT22_Humidity_Temp_Sensor_HTML());
delay(5000);
});

server.on("/BME280_BAROMETER_ETC/", [](){ //FOR READING BME280 BAROMETER ETC SENSOR
// Aquiring, Assembling and Converting Reading For Final Useage and-or Tab Display:
//FOR THERMOMETER SECTION:
BME280_temperature_raw = bme.readTemperature(); //Read temp from sensor
BME280_temperature_raw = BME280_temperature_raw * 1.8; //Converting from Celcius to F
BME280_temperature_raw = BME280_temperature_raw + 32;  //Converting from Celcius to F
dtostrf(BME280_temperature_raw,6,2,BME280_temperature_converted); //Converting from float to char
BME280_temperature = BME280_temperature_converted; //Non-rounded ready for regular display in page | converted from char to string
//Prepare temp in a special way for page tab display:
work = BME280_temperature_raw + .5;             //Rounded then float converted to an integer
dtostrf(work,6,2,BME280_temperature_converted); //Rounded integer converted to a char array
BME280_Temp_Tab = BME280_temperature_converted; //char array converted to a string for page tab display
//FOR BAROMETER SECTION:
pressure = bme.readPressure() / 100.0F; //Read Atmospheric pressure from sensor in hPa. The "F" is not a typo.
if (pressure > 1022.689) {weather = "Currently:<br><font style='color:#3bed4d'>HIGH AIR PRESSURE</font><br>(Fair Weather Unless Rapid Fall Then Means Cloudy.)";};
if (pressure <= 1022.689 && pressure >=1009.144) {weather = "Currently:<br><font style='color:yellow'>NORMAL AIR PRESSURE</font><br>(Weather Not Changing Unless Rapid Fall Then Means Rain.)";};
if (pressure < 1009.144) {weather = "Currently:<br><font style='color:red'>LOW AIR PRESSURE</font><br>(Rising or Steady Means Clearing and Cooler. Rain If Slowly Falling. Storm If Rapidly Falling.)";};
bar_inches = pressure * .0295301;                //Convert pressure to barometric inches
dtostrf(bar_inches,6,2,bar_inches_converted);    //Converting from float to char
bar_inchess = bar_inches_converted;              //Non-rounded ready for regular display in page | converted from char to string
//Prepare barometer reading in a special way for page tab display:
work = bar_inches + .5;                          //Rounded then float converted to an integer
dtostrf(work,6,2,bar_inches_converted);          //Rounded integer converted to a char array
BME280_Barom_Tab = bar_inches_converted;         //char array converted to a string for page tab display
//FOR HYGROMETER SECTION:
BME280_humidity_raw = bme.readHumidity();        //Read Amount Of Moisture In The Air From Sensor
dtostrf(BME280_humidity_raw,6,2,BME280_humidity_converted); //Converting from float to char
BME280_humidity = BME280_humidity_converted;     //Non-rounded ready for regular display in page | converted from char to string
//Prepare barometer reading in a special way for page tab display:
work = BME280_humidity_raw + .5;                 //Rounded then float converted to an integer
dtostrf(work,6,2,BME280_humidity_converted);     //Rounded integer converted to a char array
BME280_Humid_Tab = BME280_humidity_converted;    //char array converted to a string for page tab display
//ALTITUDE SECTION:
altitude = bme.readAltitude(SEALEVELPRESSURE_HPA); // (NOTE: Altitude Is Not Displayed In Page Tab)
altitude = altitude * 3.28084;                     //convert meters to feet
////////////////////////////////////////////////////////////
Test = "Hello"; //EX: Variables In HTML Must Be Assigned Before HTML Is Sent
Mac = WiFi.macAddress(); //Gets Mac No. For Display In Page
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
if (Mac == "BC:DD:C2:9D:2D:24") {board = "3";}; //Identify Board Number As Labeled On Board
if (Mac == "BC:DD:C2:9D:E4:F4") {board = "2";}; //Identify Board Number As Labeled On Board
if (Mac == "CC:50:E3:08:94:A7") {board = "1";}; //Identify Board Number As Labeled On Board
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
delay(2000);
server.send(200, "text/html", BME280_Barometer_Sensor_HTML()); 
//These were in the parentheses above: BME280_temperature,BME280_humidity,pressure,altitude
delay(5000);
});

server.on("/RESTART/", [](){ //RESTART ESP8266 NodeMCU
Test = "ESP8266 NodeMCU RESTARTING.."; //EX: Variables In HTML Must Be Assigned Before HTML Is Sent
Mac = WiFi.macAddress(); //Displays Mac No. In Page
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
if (Mac == "BC:DD:C2:9D:2D:24") {board = "3";}; //Identify Board Number As Labeled On Board
if (Mac == "BC:DD:C2:9D:E4:F4") {board = "2";}; //Identify Board Number As Labeled On Board
if (Mac == "CC:50:E3:08:94:A7") {board = "1";}; //Identify Board Number As Labeled On Board
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
delay(2000);
server.send(200, "text/html", RestartBoardHTML());
delay(5000);
ESP.restart();
delay(5000);
});

server.on("/SD_CARD/", [](){ //FOR SD CARD
Test = "Hello"; //EX: Variables In HTML Must Be Assigned Before HTML Is Sent
Mac = WiFi.macAddress(); //Displays Mac No. In Page
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
if (Mac == "BC:DD:C2:9D:2D:24") {board = "3";}; //Identify Board Number As Labeled On Board
if (Mac == "BC:DD:C2:9D:E4:F4") {board = "2";}; //Identify Board Number As Labeled On Board
if (Mac == "CC:50:E3:08:94:A7") {board = "1";}; //Identify Board Number As Labeled On Board
//••••••••••••••••••••••••••••••••••••••••••••••••••••••
delay(2000);
server.send(200, "text/html", SD_CARD_HTML());
delay(5000);
});
//*******END BUTTONS TAG AND FORMS SUBMIT BUTTOM PROCESSING (are different)*********

// Used To Display a WebPage HTML Contained In a Long One-Line String:
// Page and Temp Refreshes Every 15 Minutes (content='900').
// Temp_Page_From_String = "<HTML><HEAD><meta http-equiv='refresh' content='900'></HEAD><BODY><center><br><h1>WIRELESS CONTROL STATION</h1><h1>SWITCHBOARD</h1><h2>TURN D0 CIRCUIT <a href='http://192.168.1.77/LEDOn/'><button>ON</button></a><a href='http://192.168.1.77/LEDOff/'><button>OFF</button></a></h2><br><h3>DS18B20 TEMP SENSOR READ ON D2</h3><h2>TEMPERATURE IS: " + temp_final + "&nbspF</h2><br><img src='data:image/jpg;base64,/9j/4AAQSkZJRgABAQEAYABgAAD/2wBDABALDA4MChAODQ4SERATGCgaGBYWGDEjJR0oOjM9PDkzODdASFxOQERXRTc4UG1RV19iZ2hnPk1xeXBkeFxlZ2P/2wBDARESEhgVGC8aGi9jQjhCY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2NjY2P/wAARCAA3ACEDASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD0Cs7U9Yt9OIRgZJSM7F7fX0rRrk9NhS/8RTm5USBSzYPQ4OBTSuzCtOUUlHduxdtvFEEkoWeBoVP8QbcB9eBW8CGUMpBB5BHesbxBYW5015ViRHjwQVGO/SpfDcjSaRGGOdjFR9KejV0TCU4z5Ju5q0UUVJ0hXK6Eyprt2zkKoVySe3zCuqrirf8A4+tU/wCuUn/oQpx6nLiHZwfmbup3tte6Ldm2lEm0AHGRjketHhf/AJBI/wCujVzun/8AHlqH/XIf+hCui8L/APIJH/XRqq1kzKlUdSpGT7M2KKKKg7yjrN01ppk0qcPjap9CeK5SMi001pSN0t2GQZP3VB5P1Jrq9ZtWu9MmiQZfG5R6kc1ykWy8s0tXkSKeEnyy5wGB6jPY5qo9TgxV+demnr/wwzTJkWV4JVzFcARsR1Xng/nWr4blkttQnsHOV5P4is+CyFpKs97JGqRncEWQMzkdAMVoeG4pLnUJ79xheR/wIn/CrdjCipKUU97/AIdTp6KKKyPWCsnUNBtr2QyqTDKepUZB+ooooJlCM1aSKsHhWFXzNcvIvoq7f6mtyGGOCJYokCIowAKKKbbZMKUKfwokooopGh//2Q=='/></center></BODY></HTML>";
// server.send(200, "text/html", Temp_Page_From_String);

// Used For Outputing Text Like From A Variable or Littoral:
// server.send(200, "text/plain",x);
// server.send(200, "text/plain","is great.");

// How To Just Print To The Serial Monitor In Arduino:
server.begin();
Serial.println("Web server started!");
}
// *************SUBROUTINES******************
String MainHTML(){ //MAIN PAGE
  String ptr = "<!DOCTYPE html> <html>\n";
ptr +="<HEAD>";
ptr +="<TITLE>WIRELESS CONTROL STATION</TITLE>";
ptr +="</HEAD>";
ptr +="<BODY style='background-color:black;color:white;'>";
ptr +="<CENTER>";
ptr +="<br>";
ptr +=Test;
ptr +="<!-- Line Below Prints Mac No. & Board Label No. -->";
ptr +="<h5>MAC NO.&nbsp&nbsp&nbsp<font style='color:cyan'>";
ptr +=Mac;
ptr +="</font>&nbsp&nbsp&nbspOf&nbsp&nbsp&nbspNodeMCU&nbsp&nbspBOARD";
ptr +="#<font style='font-size:20px;color:cyan'>";
ptr += board;
ptr +="</font></h5>";
ptr +="<!-- ◘◘◘BELOW IS AN EXAMPLE OF AN INPUT FORM◘◘◘ -->";
ptr +="<form action='/form1_process'>";
ptr +="Input 1:";
ptr +="<input type='text' name='input1' value='' style=width:10%;background-color:#3449eb;color:white>";
ptr +="&nbsp&nbsp&nbsp";
ptr +="Input 2:";
ptr +="<input type='text' name='input2' value='' style=width:10%;background-color:#3449eb;color:white>";
ptr +="&nbsp&nbsp&nbsp";
ptr +="<input type='submit' value='Submit' style=width:100px;background-color:#3449eb;color:white>";
ptr +="</form>"; 
ptr +="<!-- ◘◘◘EXAMPLE OF AN INPUT FORM END◘◘◘ -->";
ptr +="<h1>WIRELESS CONTROL STATION</h1>";
ptr +="<h1>SWITCHBOARD</h1>";
ptr +="<p>";
ptr +="<table>";
ptr +="<tr>";
ptr +="<th style='text-align:left;color:#fcdf03'>OPERATION</th>";
ptr +="<th style='text-align:left;color:#fcdf03'>CONTROL</th>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOn/'><button style=width:100%;background-color:#3449eb;color:white>ON</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOff/'><button style=width:100%;background-color:#3449eb;color:white>OFF</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DS18B20 TEMP SENSOR <font size=1>ON D3</font></td>";
ptr +="<td><a href=\'/TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>TEMP &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BH1750 LIGHT SENSOR <font size=1>ON D2 (SDA) D1 (SCL)</font></td>";
ptr +="<td><a href=\'/LUX_READ/'><button style=width:100%;background-color:#3449eb;color:white>LUX &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DHT22 HUMIDITY/TEMP SENSOR <font size=1>ON D4</font></td>";
ptr +="<td><a href=\'/HUMIDITY_TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>Humid&nbsp/&nbspTemp&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BME280 PRESSURE/TEMP/HUMID SENSOR <font size=1>ON D2 (SDA)  D1  (SCL)</font></td>";
ptr +="<td><a href=\'/BME280_BAROMETER_ETC/'><button style=width:100%;background-color:#3449eb;color:white>Pres&nbsp/&nbspTemp&nbsp/&nbspHumid&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>SD CARD MANAGEMENT  <font size=1>ON D5 (SCK) D6 (MISO) D0 (MOSI) D8 (CS) VU (POS) G (GND)</font></td>";
ptr +="<td><a href=\'/SD_CARD/'><button style=width:100%;background-color:#3449eb;color:white>PROCEED</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>RESTART ESP8266 NodeMCU BOARD <font style='color:red'><b>CAUTION! CLEARS PROGRAM!</b></font></td>";
ptr +="<td><a href=\'/RESTART/'><button style=width:100%;background-color:#3449eb;color:white><b>RESTART MICROCONTROLLER!</b></button></a></td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="</CENTER>";  
ptr +="</BODY>";
ptr +="</HTML>";
return ptr;
}

String PinHighHTML(){ //TURN PIN D0 ON
  String ptr = "<!DOCTYPE html> <html>\n";
ptr +="<HEAD>";
ptr +="<TITLE>WIRELESS CONTROL STATION</TITLE>";
ptr +="</HEAD>";
ptr +="<BODY style='background-color:black;color:white;'>";
ptr +="<CENTER>";
ptr +="<br>";
ptr +=Test;
ptr +="<!-- Line Below Prints Mac No. & Board Label No. -->";
ptr +="<h5>MAC NO.&nbsp&nbsp&nbsp<font style='color:cyan'>";
ptr +=Mac;
ptr +="</font>&nbsp&nbsp&nbspOf&nbsp&nbsp&nbspNodeMCU&nbsp&nbspBOARD";
ptr +="#<font style='font-size:20px;color:cyan'>";
ptr += board;
ptr +="</font></h5><br>";
ptr +="<h1>WIRELESS CONTROL STATION</h1>";
ptr +="<h1>SWITCHBOARD</h1>";
ptr +="<p>";
ptr +="<table>";
ptr +="<tr>";
ptr +="<th style='text-align:left;color:#fcdf03'>OPERATION</th>";
ptr +="<th style='text-align:left;color:#fcdf03'>CONTROL</th>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>MAIN CONTROL</td>";
ptr +="<td><a href=/><button style=width:100%;background-color:#3449eb;color:white>GO TO</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOn/'><button style=width:100%;background-color:#3449eb;color:white>ON</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOff/'><button style=width:100%;background-color:#3449eb;color:white>OFF</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DS18B20 TEMP SENSOR ON D3</td>";
ptr +="<td><a href=\'/TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>TEMP &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BH1750 LIGHT SENSOR ON D2 (SDA) D1 (SCL)</td>";
ptr +="<td><a href=\'/LUX_READ/'><button style=width:100%;background-color:#3449eb;color:white>LUX &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DHT22 HUMIDITY/TEMP SENSOR ON D4</td>";
ptr +="<td><a href=\'/HUMIDITY_TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>Humid&nbsp/&nbspTemp&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BME280 PRESSURE/TEMP/HUMID SENSOR ON D2 (SDA)  D1  (SCL)</td>";
ptr +="<td><a href=\'/BME280_BAROMETER_ETC/'><button style=width:100%;background-color:#3449eb;color:white>Pres&nbsp/&nbspTemp&nbsp/&nbspHumid&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>SD CARD MANAGEMENT</td>";
ptr +="<td><a href=\'/SD_CARD/'><button style=width:100%;background-color:#3449eb;color:white>PROCEED</button></a></td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="</p>";
ptr +="<p>";
ptr +="<h1>PIN D0 CIRCUIT ON</h1>";
ptr +="</p>";
ptr +="<br>";
ptr +="<img src='data:image/jpeg;base64,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'/>";
ptr +="</CENTER>";  
ptr +="</BODY>";
ptr +="</HTML>";
return ptr;
}

String PinLowHTML(){ //TURN PIN D0 OFF
  String ptr = "<!DOCTYPE html> <html>\n";
ptr +="<HEAD>";
ptr +="<TITLE>WIRELESS CONTROL STATION</TITLE>";
ptr +="</HEAD>";
ptr +="<BODY style='background-color:black;color:white;'>";
ptr +="<CENTER>";
ptr +="<br>";
ptr +=Test;
ptr +="<!-- Line Below Prints Mac No. & Board Label No. -->";
ptr +="<h5>MAC NO.&nbsp&nbsp&nbsp<font style='color:cyan'>";
ptr +=Mac;
ptr +="</font>&nbsp&nbsp&nbspOf&nbsp&nbsp&nbspNodeMCU&nbsp&nbspBOARD";
ptr +="#<font style='font-size:20px;color:cyan'>";
ptr += board;
ptr +="</font></h5><br>";
ptr +="<h1>WIRELESS CONTROL STATION</h1>";
ptr +="<h1>SWITCHBOARD</h1>";
ptr +="<p>";
ptr +="<table>";
ptr +="<tr>";
ptr +="<th style='text-align:left;color:#fcdf03'>OPERATION</th>";
ptr +="<th style='text-align:left;color:#fcdf03'>CONTROL</th>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>MAIN CONTROL</td>";
ptr +="<td><a href=/><button style=width:100%;background-color:#3449eb;color:white>GO TO</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOn/'><button style=width:100%;background-color:#3449eb;color:white>ON</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOff/'><button style=width:100%;background-color:#3449eb;color:white>OFF</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DS18B20 TEMP SENSOR ON D3</td>";
ptr +="<td><a href=\'/TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>TEMP &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BH1750 LIGHT SENSOR ON D2 (SDA) D1 (SCL)</td>";
ptr +="<td><a href=\'/LUX_READ/'><button style=width:100%;background-color:#3449eb;color:white>LUX &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DHT22 HUMIDITY/TEMP SENSOR ON D4</td>";
ptr +="<td><a href=\'/HUMIDITY_TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>Humid&nbsp/&nbspTemp&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BME280 PRESSURE/TEMP/HUMID SENSOR ON D2 (SDA)  D1  (SCL)</td>";
ptr +="<td><a href=\'/BME280_BAROMETER_ETC/'><button style=width:100%;background-color:#3449eb;color:white>Pres&nbsp/&nbspTemp&nbsp/&nbspHumid&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>SD CARD MANAGEMENT</td>";
ptr +="<td><a href=\'/SD_CARD/'><button style=width:100%;background-color:#3449eb;color:white>PROCEED</button></a></td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="</p>";
ptr +="<p>";
ptr +="<h1>PIN D0 CIRCUIT OFF</h1>";
ptr +="</p>";
ptr +="<br>";
ptr +="<img src='data:image/jpeg;base64,/9j/4AAQSkZJRgABAQEBLAEsAAD/2wBDAAMCAgMCAgMDAwMEAwMEBQgFBQQEBQoHBwYIDAoMDAsKCwsNDhIQDQ4RDgsLEBYQERMUFRUVDA8XGBYUGBIUFRT/wAALCAA3ACEBAREA/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/9oACAEBAAA/APzz+BX7P3jT9ovxkPDngvTfttyiebc3ErhILWLOC8jnhR+pr9FvBn/BIHwXonhi1u/G3i/VNRv5j876RGsduoxzt3Dcfx60/wAQ/wDBK/4STafcQ6Z4i8QadetGfLuLgrIiH+FmUDkE9QOlfnd8ef2dvGH7PPih9I8TWDC3kZhZ6nD81tdqOjI3TOO3UV5dX7lf8Ev/AILaT4M/Zn0LVEihn1LxSW1PULiLBZo1YiGFz2A64FfSvjnxI11PLpsGPssLBZZBgs0nXavow9K87v5iehPBJHl9Qx6lP9v+9Xz3+2N8PrX4o/s++I7G4+e50eJtT08xYOJEHKqT/AR96vxor9xv+CbvxJj1b9jjw3DpcsMd1o1xLp14XG5ojv3Lkd94PHpXtmozFnfcGDMxDLnkMedpP98/3q5nUJc5A+fdlflO3JHUL6MO7d68j+PnijTvDHwZ8b6lqk4ism02WBSPl813XChB256r3r8T6+qf+Cfv7T9/8BvixbaHd3I/4Q7xPNHZ6lDIMiNicJKvowJAz6V+vmpyrHLJEOUHyKM8FTyFz/dI5zXN3ga5l8tQrtJhPn4Dein0Udj3r8y/+CgX7SNx4t8T3nw00ZtmgaTcK19OQN95dKOp9FXOBivjKtvwbfx6Z4v0O8ndY4be+gld3GVVVdSSR7AV+6aeO9I1izs7+DXNLmtri3jdJPtajKlR2zx9KanirRbV/tFz4g0eKGMFnaS+TgAemecdq/EX4xXkWofFXxbdQXMd3BNqc7pPEdyOC5IIPpXG0V9kfBSztf8AhUmivI43y+YXWQ8khiBg/SpvHGl2h8La2bayS4uFspWVCpcgBTls9setfGNFFe1/Bn46az4T046BDqa2QJzaSS2i3CZJ5jcHkA9iOlaPxP8A2i/EmpaNeaD/AGpFNLcAxXUltZrAI0PWNW6nPcmvA6K//9k='/>";
ptr +="</CENTER>";  
ptr +="</BODY>";
ptr +="</HTML>";
return ptr;
}

String DS18B20_Temp_Sensor_HTML(){ //READ DS18B20 TEMP SENSOR
  String ptr = "<!DOCTYPE html> <html>\n";
ptr +="<HEAD>";
ptr +="<!-- THE LINE BELOW DISPLAYS THE CONVERTED temp_raw VARIABLE (temp_final) IN THE BROWSER TAB! -->";
ptr +="<script>document.title = 'TEMP= ' + " + temp_final + "</script>";
ptr +="<!-- THE LINE BELOW REFRESHES THE PAGE EVERY 'content=' SECONDS -->";
ptr +="<meta http-equiv='refresh' content='900'>";
ptr +="</HEAD>";
ptr +="<BODY style='background-color:black;color:white;'>";
ptr +="<center>";
ptr +="<br>";
ptr +=Test;
ptr +="<!-- Line Below Prints Mac No. & Board Label No. -->";
ptr +="<h5>MAC NO.&nbsp&nbsp&nbsp<font style='color:cyan'>";
ptr +=Mac;
ptr +="</font>&nbsp&nbsp&nbspOf&nbsp&nbsp&nbspNodeMCU&nbsp&nbspBOARD";
ptr +="#<font style='font-size:20px;color:cyan'>";
ptr += board;
ptr +="</font></h5><br>";
ptr +="<h1>WIRELESS CONTROL STATION</h1>";
ptr +="<h1>SWITCHBOARD</h1>";
ptr +="<table>";
ptr +="<tr>";
ptr +="<th style='text-align:left;color:#fcdf03'>OPERATION</th>";
ptr +="<th style='text-align:left;color:#fcdf03'>CONTROL</th>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>MAIN CONTROL</td>";
ptr +="<td><a href=/><button style=width:100%;background-color:#3449eb;color:white>GO TO</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOn/'><button style=width:100%;background-color:#3449eb;color:white>ON</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOff/'><button style=width:100%;background-color:#3449eb;color:white>OFF</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DS18B20 TEMP SENSOR ON D3</td>";
ptr +="<td><a href=\'/TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>TEMP &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BH1750 LIGHT SENSOR ON D2 (SDA) D1 (SCL)</td>";
ptr +="<td><a href=\'/LUX_READ/'><button style=width:100%;background-color:#3449eb;color:white>LUX &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DHT22 HUMIDITY/TEMP SENSOR ON D4</td>";
ptr +="<td><a href=\'/HUMIDITY_TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>Humid&nbsp/&nbspTemp&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BME280 PRESSURE/TEMP/HUMID SENSOR ON D2 (SDA)  D1  (SCL)</td>";
ptr +="<td><a href=\'/BME280_BAROMETER_ETC/'><button style=width:100%;background-color:#3449eb;color:white>Pres&nbsp/&nbspTemp&nbsp/&nbspHumid&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>SD CARD MANAGEMENT</td>";
ptr +="<td><a href=\'/SD_CARD/'><button style=width:100%;background-color:#3449eb;color:white>PROCEED</button></a></td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="<br>";
ptr +="<table>";
ptr +="<tr>";
ptr +="<td style='font-size:140%;text-align:right'><b>&nbsp&nbsp&nbsp&nbspDS18B20 TEMP SENSOR READ ON PIN D3</b></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>&nbsp</td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td style='font-size:140%;text-align:center'>TEMP IS:<b><font color=#3449eb>" + temp_final + "</font>&nbspF</b></td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="<br>";
ptr +="<!-- Note The Exact Construct of 'temp_final' Above With Spaces, Quotes, Plus Signs. This Is The Format In Order To Get A Variable To Print In A Webpage. -->";
ptr +="<!-- Also Note The Required Backslash At The End Of Each Line. -->";
ptr +="<br>";
ptr +="<br>";
ptr +="<img src='data:image/jpeg;base64,/9j/4AAQSkZJRgABAQEBLAEsAAD/2wBDAAYEBQYFBAYGBQYHBwYIChAKCgkJChQODwwQFxQYGBcUFhYaHSUfGhsjHBYWICwgIyYnKSopGR8tMC0oMCUoKSj/2wBDAQcHBwoIChMKChMoGhYaKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCgoKCj/wAARCAA3ACEDASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD5Urr/AAv4Mm1aBLi4d44XGVCDnHZiewrkK+tPgLo1nqupwre26SRRwvPsI+UlW2qcVUVdnDjsROjGKpr3pNJfM8Qvfh15VuWjuH34zu+8qgevyrXBX9nNYXT29yhSRDz7+4r9A/ip4d01/Cc91FaQw3FttdHjUL/F92viv4qWiW+q27qSWdWBPsMED/x6m1Fx5ok4arVjVdCs03ucRRRRUHoBX2H+zpNHb6nJJMypEtnMzM38P71a+Po0aSRUQFnYhQB3Jr6l+Fysmj60rqVYaZcZBGD/AKxa0p9Tyc0nyeyl/e/RntHjDX9N17wBrh0q6W5EaqjFcrtO5f71fFvxb/5CVp9H/wDZa988K/8AIA8Uf9ei/wDoxa8D+Lf/ACErT6P/AOy1UocsWcuBxMsViYVJfyv80cFRRRWJ9Abvgy2+067EARuQblBGQWJCjP4tX09YLHoPg8zhN91qyS20YLf6qFGwzf7zNXzL4FuI7bX42l6suE92BDD+VfTWmS2viLw7aaa93bW1/ZlmtJJn2pMj/eXd/C26taXU+dznm9rH0fL/AIv+GIfBt7BFez6fexbrLUlW1kZPvRfN8rf99V5h8XvD728V7G6BprRzl+nKkA/pn8q9c0zQV0a8h1DxDc2sVvbN5qQR3Kyyzuv3FXbXkvxd103VtezGZFnupHEkPXLFuv1C/wA61q8tjgyyNWNWEX8V3/4D1/E8XooorlPsRVJVgVJBHII7V2uh+PLiyijjuYy+zq64O/0DKf8AGiigzq0oVY8s1dGnN8SFaCQJbyeYR8u35R+rGuD1bUp9TujNcN7Kg6KPQUUVTk5bkUMNSoK1KNilRRRUm5//2Q=='/>";
ptr +="</center>";
ptr +="</BODY>";
ptr +="</HTML>";
return ptr;
}

String BH1750_Light_Sensor_HTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
ptr +="<head>";
ptr +="<!-- THE LINE BELOW DISPLAYS THE CONVERTED bright VARIABLE (brightt) IN THE BROWSER TAB! -->";
ptr +="<script>document.title = 'LUX= ' + " + brightt + "</script>";
ptr +="<!-- THE LINE BELOW REFRESHES THE PAGE EVERY 'content=' SECONDS -->";
ptr +="<meta http-equiv='refresh' content='900'>";
ptr +="</HEAD>";
ptr +="<BODY style='background-color:black;color:white;'>";
ptr +="<center>";
ptr +="<br>";
ptr +=Test;
ptr +="<!-- Line Below Prints Mac No. & Board Label No. -->";
ptr +="<h5>MAC NO.&nbsp&nbsp&nbsp<font style='color:cyan'>";
ptr +=Mac;
ptr +="</font>&nbsp&nbsp&nbspOf&nbsp&nbsp&nbspNodeMCU&nbsp&nbspBOARD";
ptr +="#<font style='font-size:20px;color:cyan'>";
ptr += board;
ptr +="</font></h5><br>";
ptr +="<h1>WIRELESS CONTROL STATION</h1>";
ptr +="<h1>SWITCHBOARD</h1>";
ptr +="<p>";
ptr +="<table>";
ptr +="<tr>";
ptr +="<th style='text-align:left;color:#fcdf03'>OPERATION</th>";
ptr +="<th style='text-align:left;color:#fcdf03'>CONTROL</th>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>MAIN CONTROL</td>";
ptr +="<td><a href=/><button style=width:100%;background-color:#3449eb;color:white>GO TO</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOn/'><button style=width:100%;background-color:#3449eb;color:white>ON</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOff/'><button style=width:100%;background-color:#3449eb;color:white>OFF</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DS18B20 TEMP SENSOR ON D3</td>";
ptr +="<td><a href=\'/TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>TEMP &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BH1750 LIGHT SENSOR ON D2 (SDA) D1 (SCL)</td>";
ptr +="<td><a href=\'/LUX_READ/'><button style=width:100%;background-color:#3449eb;color:white>LUX &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DHT22 HUMIDITY/TEMP SENSOR ON D4</td>";
ptr +="<td><a href=\'/HUMIDITY_TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>Humid&nbsp/&nbspTemp&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BME280 PRESSURE/TEMP/HUMID SENSOR ON D2 (SDA)  D1  (SCL)</td>";
ptr +="<td><a href=\'/BME280_BAROMETER_ETC/'><button style=width:100%;background-color:#3449eb;color:white>Pres&nbsp/&nbspTemp&nbsp/&nbspHumid&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>SD CARD MANAGEMENT</td>";
ptr +="<td><a href=\'/SD_CARD/'><button style=width:100%;background-color:#3449eb;color:white>PROCEED</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>&nbsp</td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="<table>";
ptr +="<tr>";
ptr +="<td style='font-size:140%;text-align:right'><b>&nbsp&nbsp&nbsp&nbspBH1750 LIGHT SENSOR READ ON PIN D1 (SCL) D2 (SDA)</b></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>&nbsp</td>";
ptr +="</tr>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td style='font-size:140%;text-align:center'>LUX IS:<b><font color=#3449eb>" + brightt + "</font></b></td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="<br>";
ptr +="<!-- Note The Exact Construct of 'lux' Above With Spaces, Quotes, Plus Signs. This Is The Format In Order To Get A Variable To Print In A Webpage. -->";
ptr +="<!-- Also Note The Required Backslash At The End Of Each Line. -->";
ptr +="<br>";
ptr +="<br>";
ptr +="<img src='data:image/jpeg;base64,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'/>";
ptr +="</center>";
ptr +="</BODY>";
ptr +="</HTML>";
return ptr;
}

String DHT22_Humidity_Temp_Sensor_HTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
ptr +="<HEAD>";
ptr +="<!-- THE LINE BELOW DISPLAYS THE CONVERTED humidity VARIABLE (humidityy) IN THE BROWSER TAB! -->";
ptr +="<script>document.title = 'HUMIDITY= ' + " + humidityy + "</script>";
ptr +="<!-- THE LINE BELOW REFRESHES THE PAGE EVERY 'content=' SECONDS -->";
ptr +="<meta http-equiv='refresh' content='900'>";
ptr +="</HEAD>";
ptr +="<BODY style='background-color:black;color:white;'>";
ptr +="<center>";
ptr +="<br>";
ptr +=Test;
ptr +="<!-- Line Below Prints Mac No. & Board Label No. -->";
ptr +="<h5>MAC NO.&nbsp&nbsp&nbsp<font style='color:cyan'>";
ptr +=Mac;
ptr +="</font>&nbsp&nbsp&nbspOf&nbsp&nbsp&nbspNodeMCU&nbsp&nbspBOARD";
ptr +="#<font style='font-size:20px;color:cyan'>";
ptr += board;
ptr +="</font></h5><br>";
ptr +="<h1>WIRELESS CONTROL STATION</h1>";
ptr +="<h1>SWITCHBOARD</h1>";
ptr +="<p>";
ptr +="<table>";
ptr +="<tr>";
ptr +="<th style='text-align:left;color:#fcdf03'>OPERATION</th>";
ptr +="<th style='text-align:left;color:#fcdf03'>CONTROL</th>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>MAIN CONTROL</td>";
ptr +="<td><a href=/><button style=width:100%;background-color:#3449eb;color:white>GO TO</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOn/'><button style=width:100%;background-color:#3449eb;color:white>ON</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOff/'><button style=width:100%;background-color:#3449eb;color:white>OFF</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DS18B20 TEMP SENSOR ON D3</td>";
ptr +="<td><a href=\'/TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>TEMP &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BH1750 LIGHT SENSOR ON D2 (SDA) D1 (SCL)</td>";
ptr +="<td><a href=\'/LUX_READ/'><button style=width:100%;background-color:#3449eb;color:white>LUX &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ DHT22 HUMIDITY/TEMP SENSOR ON D4</td>";
ptr +="<td><a href=\'/HUMIDITY_TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>Humid&nbsp/&nbspTemp&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>READ BME280 PRESSURE/TEMP/HUMID SENSOR ON D2 (SDA)  D1  (SCL)</td>";
ptr +="<td><a href=\'/BME280_BAROMETER_ETC/'><button style=width:100%;background-color:#3449eb;color:white>Pres&nbsp/&nbspTemp&nbsp/&nbspHumid&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>SD CARD MANAGEMENT</td>";
ptr +="<td><a href=\'/SD_CARD/'><button style=width:100%;background-color:#3449eb;color:white>PROCEED</button></a></td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="<br>";
ptr +="<table>";
ptr +="<tr>";
ptr +="<td style='font-size:140%;text-align:right'><b>&nbsp&nbsp&nbsp&nbspDHT22 HUMIDITY&nbsp/&nbspTEMP&nbsp&nbspSENSOR&nbsp&nbspREAD&nbsp&nbspON&nbsp&nbspPIN&nbsp&nbspD4</b></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>&nbsp</td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td style='font-size:140%;text-align:center'>HUMIDITY IS:</td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td style='font-size:140%;text-align:center'><b><font color=#3449eb>" + humidityy + "</font>&nbsp%</b></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td style='font-size:140%;text-align:center'>TEMPERATURE IS:</td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td style='font-size:140%;text-align:center'><b><font color=#3449eb>" + temperaturee + "</font>&nbspF</b></td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="<br>";
ptr +="<!-- Note The Exact Construct of 'humidity' Above With Spaces, Quotes, Plus Signs. This Is The Format In Order To Get A Variable To Print In A Webpage. -->";
ptr +="<!-- Also Note The Required Backslash At The End Of Each Line. -->";
ptr +="<br>";
ptr +="<br>";
ptr +="<!-- <img src='data:image/jpeg;base64,/9j/4AAQSkZJRgABAQEBLAEsAAD/2wBDAAMCAgMCAgMDAwMEAwMEBQgFBQQEBQoHBwYIDAoMDAsKCwsNDhIQDQ4RDgsLEBYQERMUFRUVDA8XGBYUGBIUFRT/2wBDAQMEBAUEBQkFBQkUDQsNFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBT/wAARCAA3ADcDASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD8qqKK/UNfDPwi+K/wP+G2t6h8MtP8VeP/ABZYJBZador/ANnXV5dQDyrmaaaIqscKNGWeR1OM45NfMZ3nkckdGVSjKcajavFr3bJyu+ZpWspNttJJXeh14fD+35kpJW79T8x7CxuNUvYLOzhe5u7h1iihjUs7uxAVQB1JJxiv0K+J37CenaT+ynYabpSWNz8U/DyPq+oC2mjee9LKDdWoCnc3lKq7BjrG+Pv16B8I/wDgnD8O/Cdul14yD+Mdac73ginlt9OtiedkYUrJIBnG92GcZ2ivZpf2TPg9JaGA/DLw+EIA3RwMjj3DhwwPvnNfk+fceYXFYmh/Z1Woo0pczagrTtpb3pxfLZtNW1vdPRM93DZVVjCXtYq7Xfb8GfiJRX1t+2n8MPhV8BPjN4YsPDeh3l5F5C6hrmgTai/2fy2b93FHLjzY2ZQzHLNgMmO4r1z4ja38EdN/Yn1zxh8MfCGk6Zeaw8Xh5vtMQuNRsbiT55onlkLNny43KspAYHIxyB+ly4mjKjg69HDVJRxLSTaSUbv7Wra01Vk011PG+qWlUjKaTh+PofnbRRRX2hwFqysbjUruO1tLeS5uZWCRwwoXdyegCjkn6V+qf/BPP4TR+CfghZ+LL0NLrniUOYZJG3m209ZW2Qx8/IryB5GAxk7c9K87/Yy+IPwMs/BN/pfhizXwt8V5tJuYBeeIplknu5zAwAtLrCpGpcjEYEbf7/WvqT9nCSCX9nn4YPbkGL/hHbJfl6bljAf/AMfDZr+d/EDiHE4nB1MAqEqUVUim5aOatJ6WuuW8VZpu9tbWaPq8pwsI1Y1HJN2b9NvxPWdNtd2K3IdMMrxoBy5Cj88Vn6VjatdTpxH2u29N6fzFfn+WYSnOg2z28RVlF3R+B/7V3jV/iB+0d8Q9ZZy8b6xPbw57RQt5UYH/AACNa80s21C9C6XaG4uFuJVZbOHc3mS4IUhB1bDEDjPNanxFDL8QfEwckuNUuQxPXPmtmrfwkS6k+Kng1bIsl4dZs/JZTtIfz02kHtz3r+uoKOFwcVBaQirfJafkfnzvOpr1ZybK0blSpDqcFSOn4UV97/t8eN/2efEGt36aXYvrHxEFyTc6v4VkSC2AD/MtxIVZJnxkZRSQQNznGKK87Jczq5tgoYueGnScvsy39V1t2bSb7GtWgqU3DnTsfAoOOR1r9S/+CdvxRudZ+Eun+CNbgns9QskuLzQZrhCqalp/msJRCx4doZg4YDkKw7A1+WlfpLrPxx+AXgv4GeBPBV54rv8AUPEnhexguNP1XwXbl7vTNQI3yyRTOUjIZ2YPGWKsOuCAR8jx5hZZjgqOAhQlUlOd7xTfJZP3n5XaTTa91u2qR25bU9jUdXmSsuvXyPvDTbvGK2YdTMJVweUIYfzr88vgt/wUy0/VntNH8f8Ah6/fVXlW3h1fw/bq/wBrJOEL2m7KOeMiNyCTwor6QP7VvgXfJAkfiyS9TdusV8J6j54IBLAqYgARg5ya/CauS55lEnhq+Gk+ziuZNd01+tn3SPrI4rDYmPMpffoflj+114Hk+H37SnxC0ooUgbVZb22JGA0M585CPUbZBXmaeGdVPhpvEK2E/wDYqXa2LX4U+ULgoXEW7puKqTj0FfR37W3x9+Gf7RHxZ8L6zZ6Xr+naVZRCx1fUwkP2q7t1fKGKEttV1VnALNzlc4xXr3xQ+LfwC8c/sgat8P8AwFqbeHbjSFh1PT9J1q1aK5uLiNx5jNKNySyyIz9G9AAAAK/oinnOPwmEwEK2Em51OWNTS6gvhcpWvvv5J62asfHSoU5zqOM1ZXt5+h+flFFFfdnmhRRRQBJHI8civGxSRTlWU4IPrmvu/wCIf7f9/wCJ/wBkPTdFh1C6X4iasG0PWLklxi2jRd9wrY2l5kZEIHIzKeMrRRXh5lkuCzarh6mLhzOjLmj69n5Xs7d0jpo1Z0ozUHa6PgyiiivcOYKKKKAP/9k='/>\ -->";
ptr +="</center>";
ptr +="</BODY>";
ptr +="</HTML>";
return ptr;
}

String BME280_Barometer_Sensor_HTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
ptr +="<!-- These were in the parentheses above: float temperature,float humidity,float pressure,float altitude -->";
ptr +="<head>";
ptr +="<!-- THE LINE BELOW DISPLAYS THE CONVERTED bar_inches VARIABLE (bar_inchess) IN THE BROWSER TAB! -->";
ptr +="<script>document.title = " + BME280_Temp_Tab + " + 'deg' + " + BME280_Humid_Tab + " + '%' + " + BME280_Barom_Tab + " + 'in'</script>";
ptr +="<!-- THE LINE BELOW REFRESHES THE PAGE EVERY 'content=' SECONDS -->";
ptr +="<meta http-equiv='refresh' content='900'>";
ptr +="</head>";
ptr +="<BODY style='font-size:150%;background-color:black;color:white'>";
ptr +="<center>";
ptr +="<font style='font-size:75%'>" + Test + "</font><br>";
ptr +="<!-- Line Below Prints Mac No. & Board Label No. -->";
ptr +="<font style='font-size:75%;color:white'>MAC NO.&nbsp&nbsp&nbsp</font><font style='color:cyan;font-size:75%'>";
ptr +=Mac;
ptr +="</font><font style='color:white;font-size:75%'>&nbsp&nbsp&nbspOf&nbsp&nbsp&nbspNodeMCU&nbsp&nbspBOARD";
ptr +="#</font><font style='font-size:20px;color:cyan'>";
ptr += board;
ptr +="</font><br><br>";
ptr +="<font style='font-size:29px;color:white'>ESP8266 Weather Station</font>";
ptr +="<table style='font-size:75%'>";
ptr +="<tr style='width:25%'>";
ptr +="<th style='text-align:left;color:#fcdf03'>OPERATION</th>";
ptr +="<th style='text-align:left;color:#fcdf03'>CONTROL</th>";
ptr +="</tr>";
ptr +="<tr style='width:25%'>";
ptr +="<td>MAIN CONTROL</td>";
ptr +="<td><a href=/><button style=width:100%;background-color:#3449eb;color:white>GO TO</button></a></td>";
ptr +="</tr>";
ptr +="<tr style='width:25%'>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOn/'><button style=width:100%;background-color:#3449eb;color:white>ON</button></a></td>";
ptr +="</tr>";
ptr +="<tr style='width:25%'>";
ptr +="<td>TURN PIN D0 CIRCUIT</td>";
ptr +="<td><a href=\'/LEDOff/'><button style=width:100%;background-color:#3449eb;color:white>OFF</button></a></td>";
ptr +="</tr>";
ptr +="<tr style='width:25%'>";
ptr +="<td>READ DS18B20 TEMP SENSOR ON D3</td>";
ptr +="<td><a href=\'/TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>TEMP &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr style='width:25%'>";
ptr +="<td>READ BH1750 LIGHT SENSOR ON D2 (SDA) D1 (SCL)</td>";
ptr +="<td><a href=\'/LUX_READ/'><button style=width:100%;background-color:#3449eb;color:white>LUX &nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr style='width:25%'>";
ptr +="<td>READ DHT22 HUMIDITY/TEMP SENSOR ON D4</td>";
ptr +="<td><a href=\'/HUMIDITY_TEMP_READ/'><button style=width:100%;background-color:#3449eb;color:white>Humid&nbsp/&nbspTemp&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr style='width:25%'>";
ptr +="<td>READ BME280 PRESSURE/TEMP/HUMID SENSOR ON D2 (SDA)  D1  (SCL)</td>";
ptr +="<td><a href=\'/BME280_BAROMETER_ETC/'><button style=width:100%;background-color:#3449eb;color:white>Pres&nbsp/&nbspTemp&nbsp/&nbspHumid&nbsp&nbspREAD</button></a></td>";
ptr +="</tr>";
ptr +="<tr>";
ptr +="<td>SD CARD MANAGEMENT</td>";
ptr +="<td><a href=\'/SD_CARD/'><button style=width:100%;background-color:#3449eb;color:white>PROCEED</button></a></td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="<br>";
ptr +="<table>";
ptr +="<tr style='width:50%'>";
ptr +="<td>";
ptr +="Temperature ";
ptr +="</td>";
ptr +="<td style='color:#3449eb'>";
ptr +=BME280_temperature;
ptr +="</td>";
ptr +="<td>";
ptr +="<font color=white> &deg;F</font>";
ptr +="</td>";
ptr +="</tr>";
ptr +="<tr style='width:50%'>";
ptr +="<td>";
ptr +="Humidity ";
ptr +="</td>";
ptr +="<td style='color:#3449eb'>";
ptr +=BME280_humidity;
ptr +="</td>";
ptr +="<td>";
ptr +="<font color=white> %</font>";
ptr +="</td>";
ptr +="</tr>";
ptr +="<tr style='width:50%'>";
ptr +="<td>";
ptr +="*Barometer ";
ptr +="</td>";
ptr +="<td style='color:#3449eb'>";
ptr +=bar_inchess;
ptr +="</td>";
ptr +="<td>";
ptr +="<font color=white> Inches</font>";
ptr +="</font>";
ptr +="</td>";
ptr +="</tr>";
ptr +="<tr style='width:50%'>";
ptr +="<td>";
ptr +="Air Pressure ";
ptr +="</td>";
ptr +="<td style='color:#3449eb'>";
ptr +=pressure;
ptr +="</td>";
ptr +="<td>";
ptr +=" <font color=white>Millibars (hPa)</font>";
ptr +="</td>";
ptr +="</tr>";
ptr +="<tr style='width:50%'>";
ptr +="<td>";
ptr +="Altitude ";
ptr +="</td>";
ptr +="<td style='color:#3449eb'>";
ptr +=altitude;
ptr +="</td>";
ptr +="<td>";
ptr +=" <font color=white>Feet Above Sea Level</font>";
ptr +="</td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="<br>";
ptr +="<font color=white>";
ptr +="*" + weather;
ptr +="</font>";
ptr +="</center>";
ptr +="</body>";
ptr +="</html>";
return ptr;
}

String RestartBoardHTML(){ //RESTART ESP8266 NodeMCU
  String ptr = "<!DOCTYPE html> <html>\n";
ptr +="<HEAD>";
ptr +="<TITLE>WIRELESS CONTROL STATION</TITLE>";
ptr +="</HEAD>";
ptr +="<BODY style='background-color:black;color:white;'>";
ptr +="<CENTER>";
ptr +="<br>";
ptr +=Test;
ptr +="<!-- Line Below Prints Mac No. & Board Label No. -->";
ptr +="<h5>MAC NO.&nbsp&nbsp&nbsp<font style='color:cyan'>";
ptr +=Mac;
ptr +="</font>&nbsp&nbsp&nbspOf&nbsp&nbsp&nbspNodeMCU&nbsp&nbspBOARD";
ptr +="#<font style='font-size:20px;color:cyan'>";
ptr += board;
ptr +="</font></h5><br>";
ptr +="<h1>WIRELESS CONTROL STATION</h1>";
ptr +="<h1>SWITCHBOARD</h1>";
ptr +="<h1>ESP8266 NodeMCU RESTARTED.</h1>";
ptr +="<h1>PROGRAM RE-LOAD REQUIRED.</h1>";
ptr +="<h1>CONTROL STATION NO LONGER OPERABLE.</h1>";
ptr +="</p>";
ptr +="<br>";
ptr +="<!-- <img src='data:image/jpeg;base64,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'/> -->";
ptr +="</CENTER>";  
ptr +="</BODY>";
ptr +="</HTML>";
return ptr;
}

String SD_CARD_HTML(){ //SD CARD
  String ptr = "<!DOCTYPE html> <html>\n";
ptr +="<HEAD>";
ptr +="<TITLE>WIRELESS CONTROL STATION</TITLE>";
ptr +="</HEAD>";
ptr +="<BODY style='background-color:black;color:white;'>";
ptr +="<CENTER>";
ptr +="<br>";
ptr +=Test;
ptr +="<!-- Line Below Prints Mac No. & Board Label No. -->";
ptr +="<h5>MAC NO.&nbsp&nbsp&nbsp<font style='color:cyan'>";
ptr +=Mac;
ptr +="</font>&nbsp&nbsp&nbspOf&nbsp&nbsp&nbspNodeMCU&nbsp&nbspBOARD";
ptr +="#<font style='font-size:20px;color:cyan'>";
ptr += board;
ptr +="</font></h5><br>";
ptr +="<h1>WIRELESS CONTROL STATION</h1>";
ptr +="<h1>SWITCHBOARD</h1>";
ptr +="<font style='font-size:40px;color:gold'>SD CARD</font><br>";
ptr +="<font style='font-size:15px;color:gold'>CURRENTLY SET ACTIVE FILE NAME: " + filename + "</font>";

ptr +="<!-- ◘◘◘BELOW ARE INPUT FORMS◘◘◘ -->";

ptr +="<form action='/form_sd_card_filename'>"; //SET SD CARD FILE NAME BUTTON
ptr +="<table>";
ptr +="<tr style='outline: thin solid'>";
ptr +="<td>";
ptr +="SET 8 char FILE NAME INCL EXTENSION (Does Not Create The File):<br>To create the file simply APPEND an entry into it.";
ptr +="</td>";
ptr +="<td>";
ptr +="<input type='text' name='input3' value='' style=width:100%;background-color:#3449eb;color:white>";
ptr +="</td>";
ptr +="<td style='text-align:right'>";
ptr +="<input type='submit' value='SUBMIT' style='width:100px;background-color:#3449eb;color:white'>";
ptr +="</td>";
ptr +="</tr>";
ptr +="</form>"; 

ptr +="<form action='/form_sd_card_append'>"; //APPEND SD CARD FILE BUTTON
ptr +="<tr style='outline: thin solid'>";
ptr +="<td>";
ptr +="CREATE/APPEND THE ABOVE SET ACTIVE SD CARD FILE:<br>";
ptr +="Finish all entries with a separate entry of an '*' asterisk.<br>";
ptr +="This provides a means to build a multiple field RECORD if desired.<br>";
ptr +="The first entry comprises the main index for searching for the entire record."; 
ptr +="</td>";
ptr +="<td>";
ptr +="<input type='text' name='input1' value='' style=width:100%;background-color:#3449eb;color:white>";
ptr +="</td>";
ptr +="<td style='text-align:right'>";
ptr +="<input type='submit' value='APPEND' style='width:100px;background-color:#3449eb;color:white'>";
ptr +="</td>";
ptr +="</tr>";
ptr +="</form>"; 
ptr +="<form action='/form_search_sd_card_file_records'>"; //SEARCH SD CARD FILE RECORDS BUTTON
ptr +="<tr style='outline: thin solid'>";
ptr +="<td>";
ptr +="RETRIEVE A RECORD FROM SET ACTIVE SD CARD FILE:<br>(Accepts Partial Beginning Characters Of The First Word. Case Sensitive.)";
ptr +="</td>";
ptr +="<td>";
ptr +="<input type='text' name='input2' value='' style=width:100%;background-color:#3449eb;color:white>";
ptr +="</td>";
ptr +="<td style='text-align:right'>";
ptr +="<input type='submit' value='SEARCH' style='width:100px;background-color:#3449eb;color:white'>";
ptr +="</td>";
ptr +="</tr>";
ptr +="</form>";

ptr +="<form action='/form_sd_card_update'>"; //UPDATE SD CARD FILE RECORDS BUTTON
ptr +="<tr style='outline: thin solid'>";
ptr +="<td>";
ptr +="UPDATE ENTRIES IN THE ABOVE SET ACTIVE SD CARD FILE:<br>";
ptr +="Finds Byte Position Where Target Term Starts In The File.";
ptr +="</td>";
ptr +="<td>";
ptr +="<input type='text' name='input4' value='' style=width:100%;background-color:#3449eb;color:white>";
ptr +="</td>";
ptr +="<td style='text-align:right'>";
ptr +="<input type='submit' value='UPDATE' style='width:100px;background-color:#3449eb;color:white'>";
ptr +="</td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="</form>"; 

ptr +="<form action='/form_list_sd_card_file_records'>"; //LIST SD CARD FILE RECORDS BUTTON
ptr +="<table>";
ptr +="<tr>";
ptr +="<td style='text-align:center'>";
ptr +="<input type='submit' value='LIST RECS' style='width:100px;background-color:#3449eb;color:white'>";
ptr +="</td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="</form>";

ptr +="<form action='/form_sd_card_file_delete'>"; //DELETE A FILE BUTTON
ptr +="<table>";
ptr +="<tr>";
ptr +="<td style='text-align:center'>";
ptr +="<input type='submit' value='DELETE FILE' style='width:100px;background-color:#3449eb;color:white'>";
ptr +="</td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="</form>";

ptr +="<form action='/form_sd_card_print_directory'>"; //DISPLAY LIST OF DIRECTORY FILES BUTTON
ptr +="<table>";
ptr +="<tr>";
ptr +="<td style='text-align:center'>";
ptr +="<input type='submit' value='DIRECTORY' style='width:100px;background-color:#3449eb;color:white'>";
ptr +="</td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="</form>";

ptr +="<form action='/form_sd_card_go_main'>"; //GO BACK TO MAIN BUTTON
ptr +="<table>";
ptr +="<tr>";
ptr +="<td style='text-align:center'>";
ptr +="<input type='submit' value='MAIN PAGE' style='width:100px;background-color:#3449eb;color:white'>";
ptr +="</td>";
ptr +="</tr>";
ptr +="</table>";
ptr +="</form>";
ptr +="<font style='font-size:20px;color:gold'>" + result + "</font>";
ptr +="<!-- ◘◘◘FORMS END◘◘◘ -->";
ptr +="<br>";
ptr +="<!-- <img src='data:image/jpeg;base64,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'/> -->";
ptr +="</CENTER>";  
ptr +="</BODY>";
ptr +="</HTML>";
return ptr;
}
// *************SUBROUTINES END******************

// example of: goto
//skip_1:
//Test = "It Worked!";
//goto done;

void loop(void) {
ArduinoOTA.handle();    // FOR OTA PROGRAMMING & UPLOADING
server.handleClient();  // REQUIRED FOR SKETCHES TO WORK!
}
//√√√√√√√√√√√√√√√√√√√√√√FUNCTIONS√√√√√√√√√√√√√√√√√√√√√√√√√√√√√√√√√
void funcForm1() { //INPUT FORM1 PROCESS FUNCTION
InputNum1 = server.arg("input1"); 
InputNum2 = server.arg("input2"); 
String s = "<center>INPUT NO. 1: " + InputNum1 + "<br>INPUT NO. 2: " + InputNum2 + "<br><br><a href='/'> Go Back </a></center>";
server.send(200, "text/html", s); //Send web page
}

void funcForm2() { //FORM APPEND PROCESS FUNCTION FOR SD CARD
result="";
InputNum1 = server.arg("input1"); 
if (InputNum1=="" or filename=="") { //EMPTY INPUT FIELD SUBMITTED
Serial.println();
Serial.println("NO APPEND TERM ENTERED OR NO FILE NAME SET!");
result="NO APPEND TERM ENTERED OR NO FILE NAME SET!";
server.send(200, "text/html", SD_CARD_HTML()); //GO BACK TO SD CARD HTML  
}
if (InputNum1!="" and filename!="") { //INPUT FIELD NOT EMPTY
myFile = SD.open(filename, FILE_WRITE); //CREATE and/or OPEN A FILE (BOTH)
myFile.println(InputNum1); //PRINT RECORD TO THE FILE INCLUDING AN END OF RECORD MARKER
append_field_count=append_field_count+1; //COUNT APPENDS ENTERED
if (InputNum1=="*") { //RECORD ENTRY LIMIT ACKNOWLEDGED
append_field_count=0; //APPEND ENTRY COUNT RESET
}
myFile.close();
myFile = SD.open(filename); //RE-OPEN THE FILE FOR 'READING' (default format)
pickkk=""; //CLEAR OUT THE RECORD FOR THE NEXT READ
//Serial.write(myFile.read()); //READ AND PRINT FILE CONTENTS BYTE BY BYTE (myFile.read & Serial.write)
//for (int i = 1; i <= 6; i++) {
Serial.println();      //PRINTS A BLANK LINE
Serial.println();      //PRINTS A BLANK LINE
while (myFile.available()) { //AUTOMATICALLY PROCESS TO END OF FILE BYTE BY BYTE
//while (pick!='\r') {
pick=myFile.read();   //READ A BYTE
pickk=pick;           //CONVERT THE BYTE FROM TYPE CHAR TO TYPE STRING
pickkk=pickkk+pickk;  //BUILD THE STRING BYTE INTO A COMPLETE RECORD
if (pick=='\r'){      //FOUND END OF RECORD MARKER
//Serial.write(pick); //PRINT A SERIAL BYTE
Serial.print(pickkk); //PRINT THE RECORD
trans=pickkk;         //SAVE THE ASSEMBLED RECORD FOR PRINTING
pickkk=""; //CLEAR OUT THE RECORD FOR THE NEXT READ
}
}
myFile.close();
if (append_field_count>9) { //LIMITS AMOUNT OF ENTRIES PER RECORD
result="ENTRY LIMIT REACHED FOR THIS RECORD<br>ENTER AN ASTERISK !!";
}else{
result="THIS ENTRY WAS APPENDED TO THE SDCARD FILE: <br><font color=white>'</font>" + trans + "<font color=white>'</font>";
}
server.send(200, "text/html", SD_CARD_HTML()); //Send web page
}
}

void funcForm3() { //DELETE FILE FUNCTION
if (flag1==0){ //Prevents deletion of a file with out a confirmation
result="ARE YOU SURE YOU WANT TO DELETE THE FILE: <font color=white>"+filename+"</font> ?<br>CLICK DELETE AGAIN TO CONFIRM.";  
}
if (flag1==1){ //Prevents deletion of a file with out a confirmation
SD.remove(filename); //DELETES THE FILE
result="FILE IS NOW DELETED FROM THE SDCARD";
flag1=0; //Prevents deletion of a file with out a confirmation reset
goto skipflag1; //Delete the file confirmed
}
flag1=1; //Delete the file confirmed
skipflag1: //Delete the file confirmed
//String s = "<center>FILE DELETED FROM SDCARD FILE<BR><BR><a href='/'> Go Back To Main Page</a><br><br><a href='/SD_CARD/'>Go Back To SD CARD Page</a></center>";
server.send(200, "text/html", SD_CARD_HTML()); //Send web page
}

void funcForm4() { //JUST INITIATES THE PRINTING OF THE SD CARD DIRECTORY
File root = SD.open("/"); //OPEN THE SD CARD
root.rewindDirectory();   //START AT FIRST FILE IN SD CARD DIRECTORY
printFiles(root,0);       //GO TO printFiles() FUNCTION
}

void printFiles(File dir, int numTabs) { //DOES THE ACTUAL LISTING OF FILES IN DIRECTORY
Serial.println();
Serial.println();
Serial.println("THESE ARE THE FILES ON THE SDCARD:");
file_list=""; //INITIALIZE THE VARIABLE
result="";    //INITIALIZE THE VARIABLE
while (true) { //START CONSTRUCTING THE LIST OF SD CARD FILES
File entry =  dir.openNextFile(); //GRAB A FILE ENTRY ON THE SD CARD DIRECTORY
if (! entry) //MAKE SURE THE SD CARD HAS FILES ON IT TO LIST
{
break;
}
for (uint8_t i = 0; i < numTabs; i++)
{
Serial.print('\t');
}
Serial.println(entry.name()); //PRINT A DIRECTORY ENTRY TO THE SERIAL MONITOR
entry.close();
file_list=file_list+entry.name()+"<br>"; //BUILD THE LIST OF FILES ON THE SD CARD
}
result="FILES ON THIS FAT32 SD CARD:<br>"+file_list; //ADD INFO HEADER TO THE BUILT LIST
server.send(200, "text/html", SD_CARD_HTML()); //Send web page
}

void funcForm5() { //LIST RECORDS IN THE FILE
result=""; //CLEAR RESULT FOR HTML
rec_num_when_listing=0; //RESET VARIABLE
myFile = SD.open(filename, FILE_READ); //OPEN THE FILE
pickkk=""; //CLEAR OUT THE RECORD FOR THE NEXT READ
Serial.println();      //PRINTS A BLANK LINE
Serial.println();      //PRINTS A BLANK LINE
Serial.println("ITEMS IN THE FILE WITH ENTRY NUMBER:");
while (myFile.available()) { //AUTOMATICALLY PROCESS TO END OF FILE BYTE BY BYTE
pick=myFile.read();   //READ A BYTE
pickkk.concat(pick);
if (pick=='\r'){      //FOUND END OF RECORD MARKER
rec_num_when_listing=rec_num_when_listing+1;
Serial.print(rec_num_when_listing);
Serial.println(pickkk); //PRINT THE RECORD
pickkk=""; //CLEAR OUT THE RECORD FOR THE NEXT READ
}
}
myFile.close();
result="RECORDS IN THE SDCARD FILE ARE ALL LISTED IN THE SERIAL MONITOR";
server.send(200, "text/html", SD_CARD_HTML()); //Send web page
}

void funcForm6() { //SEARCH RECORDS IN THE FILE
result=""; //CLEAR RESULT FOR HTML
InputNum2 = server.arg("input2"); //RETRIEVE SEARCH TERM FROM INPUT FORM
if (InputNum2=="" or filename=="") { //EMPTY INPUT FIELD SUBMITTED
Serial.println();
Serial.println("NO SEARCH TERM ENTERED OR NO FILE NAME SET!");
result="NO SEARCH TERM ENTERED OR NO FILE NAME SET!";
server.send(200, "text/html", SD_CARD_HTML()); //GO BACK TO SD CARD HTML  
} //EMPTY INPUT FIELD SUBMITTED
if (InputNum2!="" and filename!="") { //INPUT FIELD NOT EMPTY
myFile = SD.open(filename, FILE_READ); //OPEN THE FILE
pickkk=""; //CLEAR OUT THE RECORD FOR THE NEXT READ
record_list=""; //CLEAR OUT THE RECORD LIST FOR NEW PROCESS
field_num=0;    //Initialize
while (myFile.available()) { //AUTOMATICALLY PROCESS TO END OF FILE BYTE BY BYTE
finish: //CONTINUE SEARCHING FOR DUPLICATE ENTRIES
pick=myFile.read();   //READ A BYTE
pickkk.concat(pick);  //BUILDS THE ENTRY FOR INSPECION
if (pick=='\r'){ //1 //FOUND END OF MAIN ENTRY (SEARCH TARGET)
if (pickkk.indexOf(InputNum2)==1) { //2 FOUND TARGET SEARCH TERM
//BELOW LINE PRINTS THE FOUND MAIN (INDEX) ENTRY. MULTIPLE ENTRIES CAN COMPRISE A RECORD IF ENDED  BY A '*' ENTRY.
Serial.print("SEARCH TERM FOUND:");
Serial.println(pickkk); //PRINTS SEARCH TARGET RECORD TO SERIAL MONITOR AND HTML PAGE IF FOUND
field_num_index=field_num_index+1; //COUNTS MAIN TARGET INDEX RECORDS FOUND
field_num_string=String(field_num_index); //CONVERTS INT TO A STRING
pickkk="<font color=cyan>INSTANCE FOUND "+field_num_string+":</font>"+pickkk;
record_list=record_list+pickkk+"<br>"; //BUILDING LIST OF ENTRIES FOUND
pickkk=""; //RESET VARIABLE
//***BELOW SEARCHES FOR ANY ADDITIONAL FIELDS IN SEARCH TARGET RECORD IF THEY EXIST
field_num=0;    //Initialize
while (myFile.available()) { //3 CONTINUE PRINTING ENTRIES UNTIL END OF FIELDS '*' MARKER FOUND
pick=myFile.read();   //READ A BYTE
pickkk.concat(pick);  //BUILDS THE ENTRY FOR INSPECION
if (pick=='\r'){ //4  //FOUND END OF AN ADDTIONAL ENTRY (FIELD) FOR THE FOUND SEARCH TARGET ENTRY
has_fields=has_fields+1; //MULTI FIELDS DETECTED
if (pickkk.indexOf("*")==1) { //5 FOUND END OF ABSOLUTE RECORD
Serial.println();
Serial.println("END OF COMPLETE SEARCH TERM RECORD");
Serial.println();
goto finish; //CONTINUE SEARCHING FOR DUPLICATE ENTRIES
} //5 ***END SEARCH FOR ANY ADDITIONAL FIELDS IN SEARCH TARGET RECORD IF THEY EXIST
Serial.print(pickkk); //PRINTS TO SERIAL MONITOR ADDITIONAL FIELDS IN RECORD IF ANY
field_num=field_num+1;
field_num_string=String(field_num);
pickkk="<font color=white>Field No. "+field_num_string+":</font>"+pickkk;
record_list=record_list+pickkk+"<br>";
pickkk="";           //RESET VARIABLE
} //4
} //3
} //2
pickkk=""; //CLEAR THE RETRIEVED ENTRY FOR NEXT ENTRY READ
} //1
} //WHILE PROCESS TO END OF FILE
field_num_index=0;    //RESET VARIABLE
field_num=0;          //RESET VARIABLE
field_num_string="";  //RESET VARIABLE
myFile.close();
result=record_list;   //STORE FINAL RESULT TO SEND WITH HTML PAGE
if (has_fields>1) { //RECORD CONTAINS MULTIPLE FIELDS
has_fields=0; //RESET MULTI FIELDS DETECTED
}
has_fields=0; //RESET MULTI FIELDS DETECTED
server.send(200, "text/html", SD_CARD_HTML()); //Send web page
} //INPUT FIELD NOT EMPTY 
} //END VOID

void funcForm7() { //SET A FILE NAME (does not create it)
result=""; //CLEAR RESULTS
InputNum3 = server.arg("input3"); //RETREIVE INPUT FIELD
if (InputNum3=="") { //EMPTY INPUT FIELD SUBMITTED
Serial.println();
Serial.println("NO FILE NAME ENTERED!");
result="NO FILE NAME ENTERED!";
server.send(200, "text/html", SD_CARD_HTML()); //GO BACK TO SD CARD HTML
}
if (InputNum3!="") { //INPUT FIELD NOT EMPTY
filename=InputNum3; //SETS FILENAME
Serial.println();
Serial.println();
Serial.println("FILE NAME SET:  "+filename); //DISPLAY FILENAME IN SERIAL MONITOR
Serial.println();
result="FILE NAME NOW SET FOR ALL OPERATIONS<br>(The First APPEND Is What Creates The File): <br><font color=white>'</font>" + filename + "<font color=white>'</font>"; //SENDS MESSAGE TO SD CARD HTML PAGE
server.send(200, "text/html", SD_CARD_HTML()); //Send web page  
}
}

void funcForm8() { //GO BACK TO MAIN PAGE
result="";
server.send(200, "text/html", MainHTML()); //Send web page  
}

void funcForm9() { //UPDATE A RECORD
result=""; //CLEAR RESULT FOR HTML
pickkk=""; //INITIALIZE VARIABLE
InputNum4 = server.arg("input4"); //RETRIEVE SEARCH TERM FROM INPUT FORM
if (InputNum4=="" or filename=="") { //EMPTY INPUT FIELD SUBMITTED
Serial.println();
Serial.println("NO SEARCH TERM ENTERED OR NO FILE NAME SET!");
result="NO SEARCH TERM ENTERED OR NO FILE NAME SET!";
server.send(200, "text/html", SD_CARD_HTML()); //GO BACK TO SD CARD HTML  
} //EMPTY INPUT FIELD SUBMITTED
if (InputNum4!="" and filename!="") { //INPUT FIELD NOT EMPTY
myFile = SD.open(filename, FILE_READ); //OPEN THE FILE
while (myFile.available()) { //1 AUTO CONTINUE TO END OF FILE
pick=myFile.read();   //READ A BYTE
record_pos=record_pos+1; //COUNTING POSITION IN RECORD
file_pos=file_pos+1; //COUNTING BYTES READ (POSITION IN FILE)
pickkk.concat(pick);  //BUILDS THE ENTRY FOR INSPECION
if (pick=='\r') { //2
if (pickkk.indexOf(InputNum4)==1) { //3 CHECK IF TARGET SEARCH RECORD FOUND
Serial.println("");
Serial.print("FOUND TARGET: ");
Serial.println(pickkk); //PRINT TARGET RECORD FOUND
Serial.println("Target Record Length Including The Two Non-Printing Ending Characters.");
Serial.println("Non-Printing Are Carriage Return Then Line Feed.");
Serial.print("So Total Record Length Is: ");
Serial.println(record_pos); //PRINT RECORD'S LAST BYTE POSITION IN FILE
z=pickkk;     //STORE FOUND TARGET FOR HTML RESULT PRINT
y=record_pos; //CONVERT INT TO STRING AND STORE FOR HTML RESULT PRINT
//Serial.print("file pos ");
//Serial.println(file_pos);
record_pos=record_pos-1; //ADJUST FOR OFFSET
target_is_at_file_pos=file_pos-record_pos; //CALCULATE TARGET'S POSITION IN FILE
Serial.print("Target Term Starts At File Byte Position: ");
Serial.println(target_is_at_file_pos); //FIRST BYTE OF TARGET RECORD'S POSITION IN FILE
} //3
pickkk=""; //RESET VARIABLE
record_pos=0; //RECORD'S END BYTE POSITION IN FILE
} //2 
} //1
file_pos=0; //RESET VARIABLE
myFile.close();
x=target_is_at_file_pos; //CONVERT INT TO STRING
result="FOUND TARGET: <font color=white>"+z+"</font><br>THIS IS THE LAST ENTRY IF DUPLICATE TARGETS FOUND<br>Target Term Starts At File Byte Position: " + x + "<br>Total Record Length (Incl Two Non-Printing End Bytes): " + y + "<br>IF DUPLICATE TARGETS FOUND THEY ARE DISPLAYED IN SERIAL MONITOR"; //STORE FINAL RESULT TO SEND WITH HTML PAGE
server.send(200, "text/html", SD_CARD_HTML()); //Send web page
} //INPUT FIELD NOT EMPTY 
} //END VOID
//√√√√√√√√√√√√√√√√√√√√√√FUNCTIONS END√√√√√√√√√√√√√√√√√√√√√√√√√√√√√√
