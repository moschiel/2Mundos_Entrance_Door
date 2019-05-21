void run_html(WiFiClient client )
{
    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
    // and a content-type so the client knows what's coming, then a blank line:
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    

    // Display the HTML web page
    String html_code = \
    "<!DOCTYPE html><html>" \
        "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">" \
        "<link rel=\"icon\" href=\"data:,\">";
        client.println(html_code);

        #if WIFI_ACCESS_POINT
            client.println("<meta http-equiv=\"refresh\" content=\"5;url=//192.168.4.1\">");
        #endif
    
        // CSS to style the buttons
        html_code = \
        "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}" \
            ".button { background-color: #0080ff; border: none; color: white; padding: 16px 40px;" \
            "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}" \
            ".button:active { background-color: white; color:#0080ff}" \
            ".button2 {background-color: #555555;}" \
        "</style></head>";
        client.println(html_code);
      
      // Web Page Heading
      client.println("<body style=\"background-color:#00004d;\"><h1 style=\"color:white;font-size:40px\">2Mundos</h1>");
      client.println("<hr>"); // Divisoria

      // turns the GPIOs on and off
    if (header.indexOf("GET /door_open") >= 0) 
    {
        Serial.println("door_open");
        acionado = true;
    }

    //Display current stateon       
    if (acionado == false) 
    {
        client.println("<p style=\"font-size:25px;color:#0080ff\">Aguardando Comando</p>");
        client.println("<p><a href=\"/door_open\"><button class=\"button\">ABRIR</button></a></p>");
    } 
    else 
    {     
        client.println("<p style=\"font-size:50px;color:#0080ff\">Comando Recebido</p>");
        #if WIFI_SERVER
            static int cat_index = 0;
                if(cat_index++ >= 56)
                    cat_index = 0;
                client.println("<script>setTimeout(()=> window.location.href = \"http://http.cat/" + String(http_cat[cat_index]) +  "\",2000)</script>");
        #endif
    } 
      
    client.println("</body></html>");
    
    // The HTTP response ends with another blank line
    client.println();
}
