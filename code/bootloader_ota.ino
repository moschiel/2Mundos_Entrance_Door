
//https://github.com/esp8266/Arduino/blob/master/doc/ota_updates/readme.rst
//#include <WiFi.h> //lib para configuração do Wifi
#include <ArduinoOTA.h> //lib do ArduinoOTA 
#include <ESPmDNS.h> //lib necessária para comunicação network
#include <WiFiUdp.h> //lib necessária para comunicação network

void errorOTA(ota_error_t error);

int port = 3232;
const char* Hostname = "2Mundos-ESP";
//const char* Host_password = "doismundos";


 
void bootLoaderOTA()
{
  // A porta fica default como 3232
  ArduinoOTA.setPort(port);
 
  // Define o hostname (opcional)
  ArduinoOTA.setHostname(Hostname);
  // Define a senha (opcional)
  //ArduinoOTA.setPassword(Host_password);

 
  // É possível definir uma criptografia hash md5 para a senha usando a função "setPasswordHash"
  // Exemplo de MD5 para senha "admin" = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  //define o que será executado quando o ArduinoOTA iniciar
  ArduinoOTA.onStart( startOTA ); //startOTA é uma função criada para simplificar o código 
 
  //define o que será executado quando o ArduinoOTA terminar
  ArduinoOTA.onEnd( endOTA ); //endOTA é uma função criada para simplificar o código 
 
  //define o que será executado quando o ArduinoOTA estiver gravando
  ArduinoOTA.onProgress( progressOTA ); //progressOTA é uma função criada para simplificar o código 
 
  //define o que será executado quando o ArduinoOTA encontrar um erro
  ArduinoOTA.onError( errorOTA );//errorOTA é uma função criada para simplificar o código 
   
  //inicializa ArduinoOTA
  ArduinoOTA.begin();
 
  //exibe pronto e o ip utilizado pelo ESP
  Serial.println("OTA MODE Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  while(true)  {
    //Handle é descritor que referencia variáveis no bloco de memória
    //Ele é usado como um "guia" para que o ESP possa se comunicar com o computador pela rede
    ArduinoOTA.handle();
  }

}

//funções de exibição dos estágios de upload (start, progress, end e error) do ArduinoOTA
void startOTA()
{
   String type;
    
   //caso a atualização esteja sendo gravada na memória flash externa, então informa "flash"
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "flash";
    else  //caso a atualização seja feita pela memória interna (file system), então informa "filesystem"
      type = "filesystem"; // U_SPIFFS
 
    //exibe mensagem junto ao tipo de gravação
    Serial.println("Start updating " + type);
}
 
//exibe mensagem
void endOTA()
{
  Serial.println("\nUpdate finished\n");
  Serial.println("\nRestarting ESP...\n");
  delay(10);
  //ESP.restart();
}
 
//exibe progresso em porcentagem
void progressOTA(unsigned int progress, unsigned int total)
{
   Serial.printf("Progress: %u%%\r\n", (progress / (total / 100))); 
}

//caso aconteça algum erro, exibe especificamente o tipo do erro
void errorOTA(ota_error_t error)
{  
      Serial.printf("Error[%u]: ", error);
       
      if (error == OTA_AUTH_ERROR) 
        Serial.println("Auth Failed\n");
      else
      if (error == OTA_BEGIN_ERROR)
        Serial.println("Begin Failed\n");
      else
      if (error == OTA_CONNECT_ERROR)
        Serial.println("Connect Failed\n");
      else
      if (error == OTA_RECEIVE_ERROR) 
        Serial.println("Receive Failed\n");
      else
      if (error == OTA_END_ERROR)
        Serial.println("End Failed\n");
}
