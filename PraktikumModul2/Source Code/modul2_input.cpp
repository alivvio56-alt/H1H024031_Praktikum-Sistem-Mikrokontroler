// 7-Segment Common Anode ditambah push button

// Pin mapping segment: a b c d e f g dp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};
const int btnUp = 2;  // Interrupt pin buat pushbutton
volatile int currentDigit = 0;  // Current digit to display (volatile for ISR)
bool buttonPressed = false;     // Flag for button press

byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, //0
  {0,1,1,0,0,0,0,0}, //1
  {1,1,0,1,1,0,1,0}, //2
  {1,1,1,1,0,0,1,0}, //3 
  {0,1,1,0,0,1,1,0}, //4
  {1,0,1,1,0,1,1,0}, //5
  {1,0,1,1,1,1,1,0}, //6
  {1,1,1,0,0,0,0,0}, //7
  {1,1,1,1,1,1,1,0}, //8
  {1,1,1,1,0,1,1,0}, //9
  {1,1,1,0,1,1,1,0}, //A
  {0,0,1,1,1,1,1,0}, //b
  {1,0,0,1,1,1,0,0}, //C
  {0,1,1,1,1,0,1,0}, //d
  {1,0,0,1,1,1,1,0}, //E
  {1,0,0,0,1,1,1,0}  //F
};

// Fungsi tampil digit common anode dibalik
void displayDigit(int num)
{
  for(int i=0; i<8; i++)
  {
    digitalWrite(segmentPins[i], !digitPattern[num][i]); // <-- dibalik
  }
}

// Interrupt Service Routine for button
void btnISR()
{
  buttonPressed = true;
}

void setup()
{
  for(int i=0; i<8; i++)
  {
    pinMode(segmentPins[i], OUTPUT);
  }
  
  pinMode(btnUp, INPUT_PULLUP);  // Button with internal pullup
  attachInterrupt(digitalPinToInterrupt(btnUp), btnISR, FALLING);  // Interrupt on falling edge
}

void loop()
{
  // Check button pencet
  if(buttonPressed)
  {
    currentDigit = (currentDigit + 1) % 16;  // Next digit (0-15, loops back)
    buttonPressed = false;  // Reset flag
  }
  
  displayDigit(currentDigit);
  delay(100);  // Small delay for stability
}