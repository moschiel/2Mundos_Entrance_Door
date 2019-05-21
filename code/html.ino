extern String logo_PNGbase64;
bool bootOTA = false;
bool webStored;

void run_html(WiFiClient client )
{
    
    bootOTA = (header.indexOf("GET /boot")>=0);
    
    webStored = !(header.indexOf("GET /null")>=0);
    
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
                "<link rel='icon' href='http://2mundos.net/favicon.png'>" \ 
                "<meta charset='UTF-8'>" \
                "<meta name='description' content='Interface web para controle da porta da 2Mundos'>" \
                "<meta name='author' content='Roger Moschiel'>" \
                "<meta name='viewport' content='width=device-width,initial-scale=1'>");
            #if WIFI_ACCESS_POINT
                if(acionado)
                    client.print("<meta http-equiv='refresh' content='5;url=//192.168.4.1'>");
            #endif
            
                client.print( \
                "<style media='screen' type='text/css'>" \
                    "html{font-family:Helvetica;display:inline-block;margin:0px auto;text-align:center;}" \
                "</style>" \
                "<script>");
                if(!webStored){      
                    client.print( \
                    "var FuncDrawRoundBtn=function(text){" \ 
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
                        "ctx.textAlign = 'center';" \
                        "ctx.fillText(text, 100, 110);" \
                    "};" \
                    "var FuncDrawArc=function(fill){" \
                        "var c = document.getElementById('myCanvas');" \
                        "var ctx = c.getContext('2d');" \
                        "ctx.beginPath();" \
                        "if(fill==true){k=1;}" \
                        "ctx.arc(w,w,r+6,0,2*Math.PI*k);" \
                        "ctx.lineWidth = 10;" \
                        "ctx.strokeStyle='#4CAF50';" \
                        "ctx.stroke();" \
                        "if(fill==false){"
                            "k=k+0.1;" \
                            "if(k<1.0){" \
                                "setTimeout(function(){DrawArc(false);},25);" \
                            "}else{" \
                                "window.location.href='/door_open';" \
                            "}" \
                        "}"    
                    "};" \
                    "localStorage.setItem('storedDrawRoundBtn_1_5',FuncDrawRoundBtn.toString());" \ 
                    "localStorage.setItem('storedDrawArc_1_5',FuncDrawArc.toString());" \
                    "localStorage.setItem('Logo2mBase64_1_5','" + logo_PNGbase64 + "');" \    
                    );
                }
                if(!bootOTA){
                    client.print( \
                    "var storedDrawRoundBtn=localStorage.getItem('storedDrawRoundBtn_1_5');" \
                    "var storedDrawArc=localStorage.getItem('storedDrawArc_1_5');" \
                    "var Logo2mBase64=localStorage.getItem('Logo2mBase64_1_5');" \
                    "var DrawRoundBtn,DrawArc;" \
                    "var webStored;" \
                    "if((storedDrawRoundBtn==null)||(storedDrawArc==null)||(Logo2mBase64==null)){"
                        "webStored=false;" \
                    "}else{" \
                        "webStored=true;" \
                        "DrawRoundBtn=eval('('+storedDrawRoundBtn+')');" \
                        "DrawArc=eval('('+storedDrawArc+')');" \
                    "}" \
                    "var canvas_size=200;" \
                    "var w=canvas_size/2;" \
                    "var r=75;" \
                    "var k=0;");
                }
                client.print( \     
                "</script>" \
            "</head>" \
            "<body style='background-color:#00004d;'>");
            if(bootOTA){
                client.print("<p style='color:white;'>BOOTLOADER MODE</p>");
            }else{
                client.print( \
                "<p>" \
                    "<img id='logo2m' src='' style='width:300px;'>" \
                "</p>" \
                "<hr>" \
                "<p>" \
                    "<canvas id='myCanvas' width='200' height='200' style='cursor:pointer;border:1px solid #00004d;'></canvas>" \
                    "<p style='color:white;position:fixed;bottom: 0px;'>Version: " + VERSION + "</p>" \ 
                "</p>" \
                "<script>" \
                    "if(webStored==false){" \
                        "window.location.href='/null';" \
                    "}else{"\  
                        "document.getElementById('logo2m').src=Logo2mBase64;");
                    if(acionado == false){
                        client.print( \
                        "DrawRoundBtn('ABRIR');" \
                        "var a = document.getElementById('myCanvas');" \
                        "a.setAttribute( 'onClick', 'DrawArc(false)' );");
                    }else{ // se acionado
                        client.print( \
                        "DrawRoundBtn('PRONTO');" \
                        "DrawArc(true);");
                        #if WIFI_SERVER
                            static int cat_index = 0;
                            if(cat_index++ >= 56)
                                cat_index = 0;
                        client.print("setTimeout(()=> window.location.href = 'https://http.cat/" + String(http_cat[cat_index]) + "',2000);");
                        #endif
                    }
                client.println( \
                    "}" \     
                "</script>");
            }
            client.println( \  
            "</body>" \
        "</html>");
    
    // The HTTP response ends with another blank line
    client.println();
}
