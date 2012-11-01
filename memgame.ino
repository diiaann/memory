

/* Diandian Xiao, dxiao */
 
/* This game is a memory game */
/******************************/

/* Gameplay starts with the grid flashing 1 block.
This indicates level one. After the block has flashed,
the user can move around the selector dock with two dials.
If the user inputs correctly the flashed block, the user progreses
to the next level and two blocks flash. */
 
const int cols[8] = 
	{ 4, A5, A4, A1, A3, A2, 3, 2};
const int rows[8] = 
	{ 10, 12, 9, 11, 6, 8, 0, 7};

const int leftPent = A6;
const int rightPent = A7;
const int button = 5;

int level = 1;
int time = 0;
int buttonPresses = 0;
int numSelect = 0;
int interval = 0;

boolean displayLevel = true;
boolean displayInput = false;
boolean checkLevel = false;
boolean startLevel = true;
boolean displayResult = false;
boolean result = true;

int leftPentValue = 0;
int rightPentValue = 0;
int inputArray[16][2]= {
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1},
  {-1,-1}
};

int keyArray[8][8] = { 
  {0, 0, 0, 0, 0, 0, 0, 0},  
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0} 
};

int check[8][8] = { 
  {0, 0, 0, 0, 0, 0, 0, 0},  
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 1}, 
  {0, 0, 0, 0, 0, 0, 1, 0}, 
  {0, 0, 0, 0, 0, 1, 0, 0}, 
  {1, 0, 0, 0, 1, 0, 0, 0}, 
  {0, 1, 0, 1, 0, 0, 0, 0}, 
  {0, 0, 1, 0, 0, 0, 0, 0} 
};

int cross[8][8] = { 
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0}, 
  {0, 0, 0, 1, 1, 0, 0, 0}, 
  {0, 0, 0, 1, 1, 0, 0, 0}, 
  {0, 0, 1, 0, 0, 1, 0, 0}, 
  {0, 1, 0, 0, 0, 0, 0, 0}, 
  {1, 0, 0, 0, 0, 0, 0, 1} 
};



void setup() {
  //Loops through and setups the pins for the LED
  
  for(int i=0; i<8; i++){
    pinMode(cols[i], OUTPUT);
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i],HIGH);
  }  

  pinMode(leftPent,INPUT);
  pinMode(rightPent,INPUT);
  pinMode(button,INPUT);
  randomSeed(analogRead(A7));

}

void loop() {

  if (startLevel){
    clearArray(keyArray);
    createBoard(level);
    startLevel = false;
  }

  if (displayLevel){
    displayArray(keyArray);
    time++;
  }

  if(time>350){
    delay(1000);
    time = 0;
    displayLevel=!displayLevel;
    displayInput=!displayInput;
    setLow();
  }
  
  if (displayInput){
    readInput();
    int buttonState = digitalRead(button);
    //Checks if button is HIGH
    if (buttonState == HIGH){
      //Adds input to the Input Array
      addInput(leftPentValue/128, rightPentValue/128);
      if(numSelect==level){
        displayInput = false;  
        result = checkInput(level);
        displayResult = true;
      }
    }
  }
  if (displayResult){
    interval++; //interval increases so the x or check will display
    if(result){
      displayArray(check);
    }
    else{
      displayArray(cross);
      level=1;
    }
    
    
    if (interval>100){
      if (result){
        level++;
      }
      else{
        level = 1;
      }
      reset();
      displayResult=false;
      startLevel = true;
      displayLevel = true;
      interval = 0;
    }  
  }
}  

/* ************ Helper Methods *********************** */

/* Clears the input array and resets the numSelect variable */
void reset(){
  for(int i=0; i< 16; i++){
    inputArray[i][0] = -1;
    inputArray[i][1] = -1;
  }
  numSelect = 0;
}

/* Stores the input into array and checks for duplicates */
void addInput(int row, int col){
  Serial.println("row/col");
  Serial.println(row);
  Serial.println(col);
  Serial.println("----");
  if (numSelect ==0){
    inputArray[0][0]=row;
    inputArray[0][1]=col;
    numSelect++;
      Serial.println(numSelect);
  }
  else{
   
    if (inputArray[numSelect-1][0]==row && inputArray[numSelect-1][1]==col){
      //do nothing
    }
    else{
      inputArray[numSelect][0]=row;
      inputArray[numSelect][1]=col;
      numSelect++;
      Serial.println(numSelect);
    }
  }
  printInputArray();
}

/* Checks the input for the level and determines if it matches in the key array*/

boolean checkInput(int level){
  for(int i=0; i<level;i++){
    int row = inputArray[i][0];
    int col = inputArray[i][1];
    //if it comes across a wrong answer it returns false
    if(keyArray[col][row]!=1){
      return false;
    }
  }
  //makes it through and finds everything correct
  return true;
}

/* This is the part of the level where the user inputs their selection */
void readInput(){
  int col;
  int row;
  leftPentValue = analogRead(leftPent);
  rightPentValue = analogRead(rightPent);
  col = leftPentValue/128; //Since our grid is 8x8 we need to map the pent inputs
  row = rightPentValue/128; // using 1024/8 which is 128
  digitalWrite(rows[row],LOW);
  digitalWrite(cols[col],HIGH);
  delay(1);
  digitalWrite(cols[col],LOW);
  digitalWrite(rows[row],HIGH);
}

/* Randomly generates a board based on the level (number of blocks) */
void createBoard(int level){
  int i = 0;
  while(i<level){
    long randCol = random(8);
    long randRow = random(8);
    if(keyArray[randRow][randCol]==0){ //checks for duplicates
      keyArray[randRow][randCol]=1;
      i++;
    }
  }
}

void setLow(){
  for (int i=0; i <8; i++){
      digitalWrite(rows[i],HIGH);
      digitalWrite(cols[i],LOW);
  }
}

/* Clears the array for the next use */
void clearArray(int array[8][8]){
  for(int i=0; i<8; i++){
    for(int j=0;j<8; j++){
      keyArray[i][j]=0;
    }
  }
}

/* Used for debugging */
void printArray(int array[8][8]){
  Serial.println("{");
  for(int i=0; i<8; i++){
    Serial.print("(");
    for(int j=0;j<8;j++){
      Serial.print(array[i][j]);
    }
    Serial.println(")");
  }
  Serial.println("}"); 
}

/* Used for debugging */

void printInputArray(){
  for(int i = 0; i <16; i++){
    Serial.print("(");
    Serial.print(inputArray[i][0]);
    Serial.print(", ");
    Serial.print(inputArray[i][1]);
    Serial.println(")");
  }
}

/* Display what is in a 8x8 matrix with 1's being lit and 0's not lit */
void displayArray(int array[8][8]){
  for(int i=0; i<8; i++){
    digitalWrite(rows[i],LOW);
    for(int j=0;j<8; j++){
      if(array[i][j]==1){
        digitalWrite(cols[j],HIGH);
      }
      else{
        digitalWrite(cols[j],LOW);
      }
    }
    delay(1);
    digitalWrite(rows[i],HIGH);
  }
}



