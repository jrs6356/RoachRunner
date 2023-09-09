int ind = 0;
int row = 1;
int frame = 1;
int sel = 0;
int dir = 0;
int num_blurbs = 0;
bool scroll = false;
String topline = "";
String botline = "";
String ct = "";
String cb = "";
uint8_t k[24] = {
  0x0, 0x1, 0x2, 0x3,
  0x4, 0x5, 0x6, 0x7,
  0x8, 0x9, 0xA, 0xB,

  0xC, 0xD, 0xE, 0xF,
  0x10, 0x11, 0x12, 0x13,
  0x14, 0x15, 0x16, 0x17,
};
int j[24] = {
  0, 1, 2, 3,
  4, 5, 6, 7,
  8, 9, 10, 11,

  12, 13, 14, 15,
  16, 17, 18, 19,
  20, 21, 22, 23,
};

void setup() {
  Serial.begin(115200);
  num_blurbs = 3;
  String blurbs[] = {
    "Run Trial",
    "Calibrate",
    "Settings"
  };
  Serial.println("---------------------------------------------------------");
  //Serial.print("Frame:  ");
  //Serial.println(frame);
  //Serial.print("Row:    ");
  //Serial.println(row);
  //rand_func2(num_blurbs, blurbs);
  Serial.println("x\tLine\tc\tBuffer_i");
  rand_func4();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void rand_func1(){
  ind = 6;
  row = ind % 2;
  frame = (ind - row)/2;
  sel = row + frame;
  Serial.print("Index:  ");
  Serial.println(ind);
  Serial.print("Row:  ");
  Serial.println(row);
  Serial.print("Frame:  ");
  Serial.println(frame);
  Serial.print("Selection:  ");
  Serial.println(sel);
}

int rand_func2(int options, String phrases[]){
  int screens = options-2;
  if(row==0){
    if(dir==1){
      row = 1;
    }
    else if(dir==-1){
      switch(frame){
        case 0:
          frame = 0;
          break;
        case 1:
          frame = 0;
          break;
        case 2:
          frame = 1;
          break;
        case 3:
          frame = 2;
          break;
        case 4:
          frame = 3;
          break;
        default:
          frame = 0;
          break;
      }
    }else{}
  }
  else{
    if(dir==-1){
      row = 0;
    }
    else if(dir==1){
      switch(frame){
        case 0:
          frame = 1;
          break;
        case 1:
          frame = 2;
          break;
        case 2:
          frame = 3;
          break;
        case 3:
          frame = 4;
          break;
        case 4:
          frame = 4;
          break;
        default:
          frame = 0;
          break;
      }
    }
  }
  if(frame>screens){
    frame = screens;
  }else{}
  if(row==1){
    ct = "  ";
    cb = "> ";
  }
  else{
    ct = "> ";
    cb = "  ";
  }
  switch(frame){
    case 0:
      topline = ct + phrases[0];
      botline = cb + phrases[1];
      break;
    case 1:
      topline = ct + phrases[1];
      botline = cb + phrases[2];
      break;
    case 2:
      topline = ct + phrases[2];
      botline = cb + phrases[3];
      break;
    case 3:
      topline = ct + phrases[3];
      botline = cb + phrases[4];
      break;
    case 4:
      topline = ct + phrases[4];
      botline = cb + phrases[5];
      break;
    default:
      topline = ct + phrases[0];
      botline = cb + phrases[1];
      break;
  }
  ind = frame + row;
  Serial.println("----------------");
  Serial.println(topline);
  Serial.println(botline);
  scroll = false;
  return ind;
}

void rand_func3(int options, String phrases[]){
  Serial.print("Number of Options: ");
  Serial.println(options);
  for(int i = 0; i<options; i++){
    Serial.print("Option ");
    Serial.print(i);
    Serial.print(":   ");
    Serial.println(phrases[i]);
  }
  topline = "> " + phrases[0];
  botline = "  " + phrases[1];
  Serial.println("-------");
  Serial.println(topline);
  Serial.println(botline);
}

void rand_func4(){
  for (uint8_t i=0; i < 168; i++) {
    drawchar((i % 21) * 6, i/21, i);
  }
}

void  drawchar(uint8_t x, uint8_t line, char c) {
  Serial.println("-------------");
  //Serial.print(x);Serial.print("\t");Serial.print(line);Serial.print("\t");Serial.println(c);
  for (uint8_t m =0; m<5; m++ ) {
    //st7565_buffer[x + (line*128) ] = pgm_read_byte(font+(c*5)+i);
    ind = x + (line*128);
    Serial.print(x);Serial.print("\t");Serial.print(line);Serial.print("\t");Serial.print(c);Serial.print("\t");Serial.println(ind);
    x++;
  }

  //updateBoundingBox(x, line*8, x+5, line*8 + 8);
}
