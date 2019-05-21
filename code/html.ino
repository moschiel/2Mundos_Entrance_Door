extern String logo_PNGbase64, snake_PNGbase64 ;
bool bootOTA = false;
bool webStored, SwebStored, snake, Referer, gotData, admin;

String sname,sscore;

void run_html(WiFiClient client){
    acionado    = false;
    snake       = false;
    webStored   = true;
    SwebStored  = true;
    bootOTA     = false;
    Referer     = false;
    gotData     = false;
    admin       = false;

    String firstLine = header.substring(0, header.indexOf("\n"));
    
    if(firstLine.indexOf("GET /door_open")>= 0){
      acionado = true;
      // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
      // and a content-type so the client knows what's coming, then a blank line:
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type:text/plain");
      //client.println("Connection: close");
      client.println();
      client.println("open");
      // The HTTP response ends with another blank line
      client.println();  
    }
    else if(firstLine.indexOf("GET /snake")>= 0){
      snake=true;
      run_game(client);
    }  
    else if(firstLine.indexOf("GET /sdata")>= 0){
      snake=true;
      gotData=true;
      //String header_test = "GET /sdata:roger,8899,\n";
      //Serial.printf("sizeof('GET /sdata:')-1:%d\n",sizeof("GET /sdata:")-1);
      //Serial.printf("indexOf('n'):%d\n",header.indexOf("\n"));
      String sdata = firstLine.substring(sizeof("GET /sdata:")-1, firstLine.indexOf("\n"));
      //Serial.printf("sdata:%s\n",sdata.c_str());
      //Serial.printf("indexOf(',')-1:%d\n",sdata.indexOf(','));
      sname = sdata.substring(0, sdata.indexOf(','));
      //Serial.printf("sname:%s\n",sname.c_str());
      sdata.replace(sname,"");
      //Serial.printf("sdata:%s\n",sdata.c_str());
      sdata.replace(",","");
      //Serial.printf("sdata:%s\n",sdata.c_str());
      sdata.replace("\n","");
      //Serial.printf("sdata:%s\n",sdata.c_str());
      sscore = sdata.substring(0,4);
      //Serial.printf("sscore:%s\n",sscore.c_str());
      
      if(sname.length() == 5 && sscore.length() == 4 && isNumeric(sscore)){
        if(header.indexOf("Referer")>= 0){
          Referer = true;
          int read_valor = sscore.toInt();
          int stored_valor;
          for (int i = 0; i<10; i++){
            stored_valor = atoi(&score[i][0]);
            if(read_valor > stored_valor){
              char buff_name[6], buff_score[5];
              sname.toCharArray(buff_name,6);
              sscore.toCharArray(buff_score,5);
              insertList(buff_name, buff_score, i);
              break;
            }
          }
        }
      }else{
        Serial.println("score inválido");
      }
      run_game(client);
    }else if (firstLine.indexOf("GET /null")>=0){
      webStored = false;
      run_app(client);
    }else if (firstLine.indexOf("GET /snull")>=0){
      snake = true;
      SwebStored = false;
      run_game(client);
    }else if (firstLine.indexOf("GET /boot")>=0){  
      bootOTA = true;
      run_boot(client);   
    }else if (firstLine.indexOf("GET /admin")>=0){
      webStored = false;  
      admin = true;
      run_app(client);   
    }else{
      run_app(client);
    }

}

void run_game(WiFiClient client){
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
    // and a content-type so the client knows what's coming, then a blank line:
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
  
  client.print(
  "<!DOCTYPE html>" \
  "<html>" \
    "<head>" \
      "<title>Snake Game</title>" \
      "<link rel='icon' href='http://2mundos.net/favicon.png'>" \ 
      "<meta charset='UTF-8'>" \
      "<meta name='description' content='Just having fun with a basic of javascript, fell free to improve this'>" \
      "<meta name='author' content='Roger Moschiel'>" \
      "<meta name='viewport' content='width=device-width,initial-scale=1'>" \
    "</head>" \
    "<style media='screen' type='text/css'>" \
      "html{font-family:Helvetica;display:inline-block;margin:0px auto;text-align:center;}" \
      ".button1{padding: 10px 24px;}" \
    "</style>" \
    "<script>");
    if(!SwebStored){      
      client.print( \
      "var FuncLoadGame=function(min_score){" \
          "var stage=document.getElementById('stage');" \
          "var ctx=stage.getContext('2d');" \
          "document.addEventListener('keydown',keyPush);" \
          "document.getElementById('up').onclick=moveup;" \
          "document.getElementById('left').onclick=moveleft;" \
          "document.getElementById('right').onclick=moveright;" \
          "document.getElementById('down').onclick=movedown;" \
          "setInterval(game,250);" \
          "const vel=1;" \
          "var vx,vy=0;" \
          "var px,py=30/2;" \
          "var qp=30;" \ 
          "var tp=300/qp;" \
          "var ax,ay=0;" \
          "var ay=Math.floor(Math.random()*qp);" \
          "var last_dir=0;" \
          "var hold_event=false;" \
          "var trail=[];" \
          "var tail_size=5;" \
          "var tail=tail_size;" \
          "var points=0;" \
          "var reset_game=true;" \
          "function game(){" \
            "hold_event=false;" \
            "if(vx!= 0 || vy != 0 || reset_game){" \  
              "if(reset_game){" \
                "points=0;" \
                "vx=vy=0;" \
                "py=30/2;" \
                "tail=tail_size;" \
                "trail=[];" \
                "last_dir=0;" \
                "for(var i=0;i<tail_size;i++){" \
                  "px=10+i;" \
                  "trail.push({x:px,y:py});" \  
                "}" \
                "var run;" \
                "do{" \
                  "run=false;" \
                  "ax=Math.floor(Math.random()*qp);" \
                  "ay=Math.floor(Math.random()*qp);" \
                  "for(var i=0; i<trail.length; i++){" \
                    "if((trail[i].x==ax)||(trail[i].y==ay)){" \
                      "run=true;" \
                      "break;" \
                    "}" \
                  "}" \
                "}while(run);" \
              "}" \
              "else" \
              "{" \
                "px+=vx;" \
                "py+=vy;" \
                "trail.push({x:px,y:py});" \
              "}" \
              "if(trail.length>tail){" \
                "trail.shift();" \
              "}" \
              "for(var i=0;i<(trail.length-1);i++){" \
                "if((trail[i].x==px)&&(trail[i].y==py)||(px<0)||(px>(qp-1))||(py<0)||(py > (qp-1))){" \
                  "if(points>min_score){" \
                    "popup();" \
                  "}" \
                  "reset_game=true;" \
                  "return;" \
                "}" \
              "}" \
              "if((ax==px)&&(ay==py)){" \
                "tail++;" \
                "points=tail-tail_size;" \
                "sessionStorage.setItem('snake-points',points);"
                "document.getElementById('score').innerHTML='score: '+(points).toString();" \
                "var run;" \
                "do{" \
                  "run=false;" \
                  "ax=Math.floor(Math.random()*qp);" \
                  "ay=Math.floor(Math.random()*qp);" \
                  "for(var i=0;i<trail.length;i++){" \
                    "if((trail[i].x==ax)||(trail[i].y==ay)){" \
                      "run=true;" \
                      "break;" \
                    "}" \
                  "}" \
                "}while(run);" \
              "}" \
              "ctx.fillStyle='#99FF33';" \
              "ctx.fillRect(0,0,stage.width,stage.height);" \ 
              "ctx.fillStyle='red';" \
              "ctx.fillRect(ax*tp+1,ay*tp+1,tp-2,tp-2);" \
              "ctx.fillStyle='black';" \
              "for(var i=0;i<trail.length;i++){" \
                "ctx.fillRect(trail[i].x*tp,trail[i].y*tp,tp-1,tp-1);" \
              "}" \
              "reset_game=false;" \ 
            "}" \
          "}" \
          "function keyPush(event){" \ 
            "if(!hold_event&&(event.keyCode!=last_dir)&&(Math.abs(event.keyCode-last_dir)!=2)){" \
              "last_dir=event.keyCode;" \
              "hold_event=true;" \
              "switch(event.keyCode){" \
                "case 37:" \
                  "vx=-vel;" \
                  "vy=0;" \
                  "break;" \
                "case 38:" \
                  "vx=0;" \
                  "vy=-vel;" \
                  "break;" \
                "case 39:" \
                  "vx=vel;" \
                  "vy=0;" \
                  "break;" \
                "case 40:" \
                  "vx=0;" \
                  "vy=vel;" \
                  "break;" \    
              "}" \
            "}" \
          "}" \
          "function popup2(){" \
            "var txt;" \
            "var person=prompt('Digite seu nome(5 caracteres):', '');" \
            "if (person.length!=5 || person==null || person==''){" \
              "alert('Nome inválido!');" \
              "popup();" \
            "}else{" \
              "var s='0000'+(points).toString();" \
              "s=s.substring(s.length-4);"  
              "window.location.href='/sdata:'+person+','+s+',';" \
            "}" \
          "}" \
          "function popup(){" \
            "if(confirm('Score: '+(points).toString()+' - Registrar Pontuação?')){" \
              "popup2();" \
            "}" \
          "}" \
          "function moveleft(){" \
            "if(last_dir!=39){" \
              "last_dir=37;"  
              "vx=-vel;" \
              "vy=0;" \
            "}" \
          "}" \
          "function moveup(){" \
            "if(last_dir!=40){" \
              "last_dir=38;" 
              "vx=0;" \
              "vy=-vel;" \
            "}" \ 
          "}" \
          "function moveright(){" \
            "if(last_dir!=37){" \ 
              "last_dir=39;"
              "vx=vel;" \
              "vy=0;" \
            "}" \
          "}" \
          "function movedown(){" \
            "if(last_dir!=38){" \ 
              "last_dir=40;"
              "vx=0;" \
              "vy=vel;" \
            "}" \
          "}" \
        "};" \
        "localStorage.setItem('storedLoadGame_1_7',FuncLoadGame.toString());");
    }
    client.print( \
        "var storedLoadGame=localStorage.getItem('storedLoadGame_1_7');" \
        "var webStored, loadGame;" \
        "if(storedLoadGame==null){" \
          "webStored=false;" \ 
        "}else{" \
          "webStored=true;" \
          "loadGame=eval('('+storedLoadGame+')');" \
        "}" \
    "</script>" \
    "<body style='background-color:#00004d;'>" \
      "<p id='score' style='color:white'>score: 0</p>" \
      "<canvas id='stage' width='300' height='300' style='border:2px solid black'></canvas>" \
      "<div style='text-align:center;'>" \
        "<button id='up' class='button1' >UP</button><br><br>" \
        "<button id='left' class='button1' style='margin:25px;'>LEFT</button>" \
        "<button id='right'class='button1' style='margin:25px;'>RIGHT</button><br><br>" \
        "<button id='down' class='button1'>DOWN</button>" \
      "</div>" \
      "<p style='color:white;font-size:20px'>Records:</p>" \
      "<ol style='list-style-position:inside;color:white;font-size:25px'>");

      String list="";
      for (int i = 0; i<10; i++){
        list += "<li>" + String(&names[i][0]) + " " + String(&score[i][0]) + "</li>";
      }
      
      client.print( \
        list +\     
      "</ol>" \
      "<script type='text/javascript'>" \
        "if(webStored==false){" \
          "window.location.href='/snull';" \
        "}else{");
        if(gotData && !Referer){
          client.print( \
          "var stored_points=sessionStorage.getItem('snake-points');" \
          "if((stored_points==" + sscore + ") && confirm('Não foi possível registrar, tente novamente')){" \
              "window.location.href='/sdata:" + sname + "," + sscore + ",';" \
          "}else{" \
              "sessionStorage.setItem('snake-points',0);" \
              "window.onload=loadGame(" + String(String(&score[9][0]).toInt()) + ");" \
          "}");
        }else{
          client.print( \
          "sessionStorage.setItem('snake-points',0);" \
          "window.onload=loadGame(" + String(String(&score[9][0]).toInt()) + ");");      
        }
        client.println( \
        "}" \
      "</script>" \
    "</body>" \
  "</html>");

   // The HTTP response ends with another blank line
  client.println();  
  
}

void run_boot(WiFiClient client){

  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
    // and a content-type so the client knows what's coming, then a blank line:
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    
    // Display the HTML web page
    client.println( \
    "<!DOCTYPE html>" \
        "<html>" \
            "<head>" \
                "<title>2Mundos - IoT Door</title>" \
                "<link rel='icon' href='http://2mundos.net/favicon.png'>" \ 
                "<meta charset='UTF-8'>" \
                "<meta name='description' content='we are in boot mode, upload new firmware through Arduino IDE'>" \
                "<meta name='author' content='Roger Moschiel'>" \
                "<meta name='viewport' content='width=device-width,initial-scale=1'>" \
                "<style media='screen' type='text/css'>" \
                    "html{font-family:Helvetica;display:inline-block;margin:0px auto;text-align:center;}" \
                "</style>" \
            "</head>" \
            "<body style='background-color:#00004d;'>" \
                "<p style='color:white;'>BOOTLOADER MODE</p>" \ 
            "</body>" \
        "</html>");
    
    // The HTTP response ends with another blank line
    client.println();
}

void run_app(WiFiClient client){

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
                "<meta name='description' content='Just practicing a little of web, fell free to improve this'>" \
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
                        "ctx.clearRect(0,0,c.width, c.height);" \
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
                                "loadHttpRequest('/door_open');" \
                            "}" \
                        "}else{" \
                            "k=0;" \
                        "}" \     
                    "};" \
                    "var FuncHttpRequest = function(url){" \
                        "var xhttp;" \
                        "xhttp=new XMLHttpRequest();" \
                        "xhttp.onreadystatechange = function() {" \
                          "if (this.readyState == 4 && this.status == 200) {" \
                              "console.log('XMLHttpResponde:');" \
                              "console.log(String(this.responseText).trim());" \
                              "if(String(this.responseText).trim() == 'open'){" \  
                                  "DrawRoundBtn('PRONTO');" \
                                  "DrawArc(true);");
                                  #if WIFI_SERVER
                                      if (admin==false){
                                          static int cat_index = 0;
                                          if(cat_index++ >= 56)
                                              cat_index = 0;
                                          client.print("setTimeout(()=> window.location.href = 'https://http.cat/" + String(http_cat[cat_index]) + "',2000);");
                                      }else{
                                          client.print("setTimeout(function(){DrawRoundBtn('ABRIR');},2000);");  
                                      }
                                  #else
                                      client.print("setTimeout(function(){DrawRoundBtn('ABRIR');},2000);");
                                  #endif
                              client.print( \    
                              "}" \
                          "}" \
                        "};" \
                        "xhttp.open('GET', url, true);" \
                        "xhttp.send();" \
                    "};" \
                    "localStorage.setItem('storedDrawRoundBtn_1_9',FuncDrawRoundBtn.toString());" \ 
                    "localStorage.setItem('storedDrawArc_1_9',FuncDrawArc.toString());" \
                    "localStorage.setItem('storedHttpReq_1_9',FuncHttpRequest.toString());" \
                    "localStorage.setItem('Logo2mBase64_1_9','" + logo_PNGbase64 + "');" \
                    "localStorage.setItem('snakeBase64_1_9','" + snake_PNGbase64 + "');" \    
                    );
                }
                
                client.println( \
                    "var storedDrawRoundBtn=localStorage.getItem('storedDrawRoundBtn_1_9');" \
                    "var storedDrawArc=localStorage.getItem('storedDrawArc_1_9');" \
                    "var storedHttpReq=localStorage.getItem('storedHttpReq_1_9');" \
                    "var Logo2mBase64=localStorage.getItem('Logo2mBase64_1_9');" \
                    "var snakeBase64=localStorage.getItem('snakeBase64_1_9');" \
                    "var DrawRoundBtn,DrawArc;" \
                    "var webStored;" \
                    "if((storedDrawRoundBtn==null)||(storedDrawArc==null) || (storedHttpReq==null)||(Logo2mBase64==null)||(snakeBase64==null)){"
                        "webStored=false;" \
                    "}else{" \
                        "webStored=true;" \
                        "DrawRoundBtn=eval('('+storedDrawRoundBtn+')');" \
                        "DrawArc=eval('('+storedDrawArc+')');" \
                        "loadHttpRequest=eval('('+storedHttpReq+')');" \
                    "}" \
                    "var canvas_size=200;" \
                    "var w=canvas_size/2;" \
                    "var r=75;" \
                    "var k=0;" \     
                "</script>" \
            "</head>" \
            "<body style='background-color:#00004d;'>" \
                "<p>" \
                    "<img id='logo2m' src='' style='width:300px;'>" \
                "</p>" \
                "<hr>" \
                "<p>" \
                    "<canvas id='myCanvas' width='200' height='200' style='cursor:pointer;border:1px solid #00004d;'></canvas>" \
                    "<p style='color:white;position:fixed;bottom: 0px;'>Version: " + VERSION + "</p>" \ 
                    "<a href='/snake'><img id='snake' src='' style='position:absolute;bottom:8px;right: 16px;'></a>" \
                "</p>" \
                "<script>" \
                    "if(webStored==false){" \
                        "window.location.href='/null';" \
                    "}else{"\  
                        "document.getElementById('logo2m').src=Logo2mBase64;" \
                        "document.getElementById('snake').src=snakeBase64;" \
                        "DrawRoundBtn('ABRIR');" \
                        "var a = document.getElementById('myCanvas');" \
                        "a.setAttribute( 'onClick', 'DrawArc(false)' );" \
                    "}" \     
                "</script>" \ 
            "</body>" \
        "</html>");
    
    // The HTTP response ends with another blank line
    client.println();
}
