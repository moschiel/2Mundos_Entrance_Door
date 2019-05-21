#define WIFI_ACCESS_POINT false 
#define WIFI_SERVER       !WIFI_ACCESS_POINT
#define STATIC_IP         false
#define RESTART_WHEN_OPEN false
#define WATCHDOG          true
#define DOOR_TIMER        false
#define RESET_GAME_LIST   false

#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED
#define DOOR_PIN 4

#include <EEPROM.h>
#include <WiFi.h>

String VERSION = "1.7";
uint8_t boot_addr = 10;
uint8_t name_addr = 20; //20 + 10*6 = 80
uint8_t score_addr = 80; //80 +10*5 = 130
extern char names[10][6]; //5 caracteres mais null ('roger\0')
extern char score[10][5]; //4 caracteres mais null ('9999\0')


#if WIFI_ACCESS_POINT
    #include <WiFiClient.h>
    #include <WiFiAP.h>
    #include "esp_wifi.h"
    // Set these to your desired credentials.  
    const char *ssid = "2Mundos_Door";
    const char *password = "12345678";
#elif WIFI_SERVER
    const char *ssid = "2mundos.net";
    const char *password = "doismundos";
    
    int http_cat[56]={
        100,101,200,201,202,204,206,207,300,301,302,303,304,305,307,400,401,402,403,
        404,405,406,408,409,410,411,412,413,414,415,416,417,418,420,421,422,423,424,
        425,426,429,431,444,450,451,500,502,503,504,506,507,508,509,510,511,599
    };
    
    #if STATIC_IP
        //Static IP address configuration
        IPAddress staticIP(192, 168, 0, 191); //ESP static ip
        IPAddress gateway(192, 168, 0, 1);   //IP Address of your WiFi Router (Gateway)
        IPAddress subnet(255, 255, 255, 0);  //Subnet mask
        IPAddress dns(8, 8, 8, 8);  //DNS
    #endif
#endif

// Variable to store the HTTP request
String header;
bool acionado;
WiFiServer server(80);
hw_timer_t * WDTtimer = NULL; //faz o controle do temporizador (interrupção por tempo)
hw_timer_t * DoorTimer = NULL;
extern void WDT_setup();
extern bool bootOTA;



void setup() 
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(DOOR_PIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(DOOR_PIN, LOW);

    Serial.begin(115200);
    Serial.println();
    Serial.print("Version: ");
    Serial.println(VERSION);

    if (!EEPROM.begin(200)){
      Serial.println("failed to initialise EEPROM"); 
      ESP.restart();
    }
    
    #if RESET_GAME_LIST
      resetList();
      Serial.println("GAME RECORDS RESET...");
      getList();
      String list="";
      for (int i = 0; i<10; i++){
        list += String(&names[i][0]) + " " + String(&score[i][0]) + "\n";
      }
      Serial.println(list);
      while(true);
    #else
      getList();      
    #endif
    
    #if WIFI_ACCESS_POINT
        WiFi_AP();
    #elif WIFI_SERVER
        WiFi_WS();
    #endif

    if(EEPROM.read(boot_addr) == 1){
      Serial.println("Initializing OTA..."); 
      EEPROM.write(boot_addr, 0); //escreve 0 no endereco 10
      EEPROM.commit();
      bootLoaderOTA();
    }

    byte mac[6];
    Serial.print(" - MacAddress:");
    WiFi.macAddress(mac);
    printMac(mac);
    Serial.println();
    server.begin();
    
    Serial.println("Server started");

    #if WATCHDOG
        WDT_setup();
    #endif
    
}

void loop() 
{
    //in the loop, check connection   
    #if WIFI_SERVER
    if((WiFi.status() != WL_CONNECTED))
        WiFi_WS(); 
    #endif

    //in the loop, reset timer (feed watchdog)
    #if WATCHDOG 
        timerWrite(WDTtimer, 0); //reseta o temporizador (alimenta o watchdog) 
    #endif
    
    acionado = false;
    WiFiClient client = server.available();   // listen for incoming clients
    if (client){                             // if you get a client,
        Serial.println("New Client.");           // print a message out the serial port
        String currentLine = "";                // make a String to hold incoming data from the client
        int connected_loop = 0;                 // variable to prevent a client from blocking the system
        while (client.connected()){ // loop while the client's connected    
            if (client.available()){ // if there's bytes to read from the client,
                connected_loop = 0;         
                char c = client.read();             // read a byte, then
                Serial.write(c);                    // print it out the serial monitor
                header += c;
                if (c == '\n'){ // if the byte is a newline character                    
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0){
                        run_html(client);
                        break; // break out of the while loop:
                    }else{ // if you got a newline, then clear currentLine:    
                        currentLine = "";
                    }
                }else if (c != '\r'){ // if you got anything else but a carriage return character,   
                    currentLine += c;      // add it to the end of the currentLine
                }
            }
            if(connected_loop++ > 70) //if client is blocking the system
                break; // break out of the while loop:
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.flush();
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }

    if(acionado){
        #if DOOR_TIMER
            DoorTimer_setup();
            digitalWrite(LED_BUILTIN, HIGH);
            digitalWrite(DOOR_PIN, HIGH);
        #else
            digitalWrite(LED_BUILTIN, HIGH);
            digitalWrite(DOOR_PIN, HIGH);
            delay(500);
            digitalWrite(LED_BUILTIN, LOW);
            digitalWrite(DOOR_PIN, LOW);
        #endif
        
        #if RESTART_WHEN_OPEN
            ESP.restart();
        #endif
    }

    
    if(bootOTA){
        EEPROM.write(boot_addr, 1); //escreve 1 no endereco 10
        EEPROM.commit();
        delay(500);
        ESP.restart(); 
    }
    
}

void WiFi_AP(){
    Serial.println("Configuring access point...");
    // You can remove the password parameter if you want the AP to be open.
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
}

void WiFi_WS(){
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.disconnect();  //Prevent connecting to wifi based on previous configurations
    #if STATIC_IP
        WiFi.config(staticIP, subnet, gateway, dns);
    #endif
    WiFi.begin(ssid, password);
    WiFi.mode(WIFI_STA); //WiFi mode station (connect to wifi router only

    int i = 0;
    
    //while (WiFi.waitForConnectResult() != WL_CONNECTED)
    while (WiFi.status() != WL_CONNECTED) 
    {
        //in the loop, reset timer (feed watchdog)
        //#if WATCHDOG 
        //    timerWrite(WDTtimer, 0); //reseta o temporizador (alimenta o watchdog) 
        //#endif
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);
        digitalWrite(LED_BUILTIN, LOW);
        delay(250);
        Serial.print(".");
        if(i++ > 20)
            ESP.restart();
    }
  
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
