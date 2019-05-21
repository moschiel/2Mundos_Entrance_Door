extern String logo_PNGbase64;

void run_html(WiFiClient client )
{
      // turns the GPIOs on and off
    if (header.indexOf("GET /door_open") >= 0){ 
        Serial.println("door_open");
        acionado = true;
    }
    
    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
    // and a content-type so the client knows what's coming, then a blank line:
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    // Display the HTML web page
    client.print( \
    "<!DOCTYPE html>" \
        "<html>" \
            "<head>" \
                "<title>2Mundos - IoT Door</title>" \
                "<link rel='icon' href='data:,'>" \  
                "<meta charset='UTF-8'>" \
                "<meta name='description' content='Interface Web para Controle da Porta da 2Mundos'>" \
                "<meta name='author' content='Roger Moschiel'>" \
                "<meta name='viewport' content='width=device-width,initial-scale=1'>");
    
            #if WIFI_ACCESS_POINT
                client.print("<meta http-equiv='refresh' content='5;url=//192.168.4.1'>");
            #endif
            
                client.print( \
                "<style media='screen' type='text/css'>" \
                    "html{font-family:Helvetica;display:inline-block;margin:0px auto;text-align:center;}" \
                    ".button {width:250px;background-color:#4CAF50;border:2px solid #4CAF50;color:white;padding:6px 40px;text-decoration:none;font-size:30px;margin:2px;cursor:pointer;}" \
                    ".button:active{background-color:white;color:#4CAF50;}" \
                    ".button:hover{background-color:white;color:#4CAF50;}" \
                "</style>" \
                "<script>" \
                    "var canvas_size=200;" \
                    "var w=canvas_size/2;" \
                    "var r=75;" \
                    "function DrawCircleButton(){" \ 
                        "var c = document.getElementById('myCanvas');" \
                        "var ctx = c.getContext('2d');" \
                        "ctx.beginPath();" \
                        "ctx.arc(w,w,r,0,2*Math.PI);" \ 
                        "ctx.lineWidth=3;" \
                        "ctx.strokeStyle='white';" \
                        "ctx.stroke();" \
                        "ctx.fillStyle='#0080ff';" \
                        "ctx.fill();" \
                        "ctx=c.getContext('2d');" \
                        "ctx.font ='25pt Arial';" \
                        "ctx.fillStyle = 'white';" \
                        "ctx.textAlign = 'center';");
                    if(acionado){
                        client.print( \
                        "ctx.fillText('PRONTO', 100, 110);" \
                    "}"
                    "function DrawArc(){" \
                        "var c = document.getElementById('myCanvas');" \
                        "var ctx = c.getContext('2d');" \
                        "ctx.beginPath();" \
                        "ctx.arc(w,w,r+6,0,2*Math.PI);" \
                        "ctx.lineWidth = 10;" \
                        "ctx.strokeStyle='#4CAF50';" \
                        "ctx.stroke();" \
                    "}");
                    }else{
                        client.print( \
                        "ctx.fillText('ABRIR', 100, 110);" \
                    "}" \
                    "var k = 0.0;" \
                    "function DrawArc(){" \
                        "var c = document.getElementById('myCanvas');" \
                        "var ctx = c.getContext('2d');" \
                        "ctx.beginPath();" \
                        "ctx.arc(w,w,r+6,0,2*Math.PI*k);" \
                        "ctx.lineWidth = 10;" \
                        "ctx.strokeStyle='#4CAF50';" \
                        "ctx.stroke();" \
                        "k=k+0.1;" \
                        "if(k<1.0){" \
                            "setTimeout(function(){DrawArc();},25);" \
                        "}else{" \
                            "window.location.href='/door_open';" \
                        "}" \
                    "}");
                    }
                client.print( \    
                "</script>" \
            "</head>" \
            "<body style='background-color:#00004d;'>" \
                "<p>" \
                    "<img src='" + logo_PNGbase64 + "' style='width:300px;'>" \
                "</p>" \
                "<hr>" \
                "<p>" \
                    "<canvas id='myCanvas' width='200' height='200' style='border:1px solid #00004d;'></canvas>" \ 
                "</p>" \
                "<script>" \
                    "DrawCircleButton();");
                    if(acionado == false){
                        client.print( \
                        "var a = document.getElementById('myCanvas');" \
                        "a.setAttribute( 'onClick', 'DrawArc()' );");
                    }else{
                        client.print("DrawArc();");
                    #if WIFI_SERVER
                        static int cat_index = 0;
                        if(cat_index++ >= 56)
                            cat_index = 0;
                        client.print("setTimeout(()=> window.location.href = 'https://http.cat/" + String(http_cat[cat_index]) + "',2000);");
                    #elif WIFI_ACCES_POINT
                        client.print("window.location.href='/door_open';");
                    #endif
                    }
                client.println( \     
                "</script>" \ 
            "</body>" \
        "</html>");
    
    // The HTTP response ends with another blank line
    client.println();
}
