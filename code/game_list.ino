char names[10][6]; //5 caracteres mais null ('roger\0')
char score[10][5]; //4 caracteres mais null ('9999\0')

void getList(){
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 6; j++){
      names[i][j] = EEPROM.read(name_addr+(i*6)+j);
      if(names[i][j] == '\0'){
        j=6; //break out the inner for loop 
      }else if (j==5){ //se chegou no ultimo e ainda nao tem '/0', insere (isto nao devria ocorrer)
        names[i][j] = '\0';
      }
    }

    for(int j = 0; j < 5; j++){
      score[i][j] = EEPROM.read(score_addr+(i*5)+j);
      if(score[i][j] == '\0'){
        j=5; //break out the inner for loop 
      }else if (j==4){ //se chegou no ultimo e ainda nao tem '/0', insere (isto nao devria ocorrer)
        score[i][j] = '\0';
      }
    }
  }
}


void resetList(){
  for(int i = 0; i < 10; i++){
    
    for(int j = 0; j < 6; j++){
      if(j==5)
        EEPROM.write(name_addr+(i*6)+j,'\0');
      else 
        EEPROM.write(name_addr+(i*6)+j,'A');
    }
    
    for(int j = 0; j < 5; j++){
      if(j==4)
        EEPROM.write(score_addr+(i*5)+j,'\0');
      else 
        EEPROM.write(score_addr+(i*5)+j,'0');
    }
  }
  EEPROM.commit();
}


void setList(){
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 6; j++){
      if(j==5)
        EEPROM.write(name_addr+(i*6)+j,'\0');
      else 
        EEPROM.write(name_addr+(i*6)+j,names[i][j]); 
    }
    
    for(int j = 0; j < 5; j++){
      if(j==4)
        EEPROM.write(score_addr+(i*5)+j,'\0');
      else 
        EEPROM.write(score_addr+(i*5)+j,score[i][j]); 
    }
  }
  EEPROM.commit();
}

void insertList(char *sname, char *sscore, int i){
  ShiftList(i);
  for(int j = 0; j < 6; j++){
    if(j==5)
      names[i][j]= '\0';
    else
      names[i][j]= sname[j];
  }

  for(int j = 0; j < 5; j++){
    if(j==4)
      score[i][j]= '\0';
    else 
      score[i][j]= sscore[j]; 
  }
  setList();
}

void ShiftList(int index){
  for(int i=9; i>index; i--){
    for(int j = 0; j < 6; j++){
        names[i][j]= names[i-1][j]; 
    }
    for(int j = 0; j < 5; j++){
        score[i][j]= score[i-1][j]; 
    }
  }
}
