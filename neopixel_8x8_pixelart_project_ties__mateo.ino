#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN    2
#define PIXEL_COUNT 64

Adafruit_NeoPixel pixels(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

static int button1Pin = 13;
static int button2Pin = 12;
static int button3Pin = 11;

int button1State = 0;
int button2State = 0;
int button3State = 0;

// Define colors
static uint32_t white = pixels.Color(255, 255, 255);
static uint32_t black = pixels.Color(0, 0, 0);
static uint32_t green = pixels.Color(0, 255, 0);
static uint32_t red = pixels.Color(255, 0, 0);
static uint32_t blue = pixels.Color(0, 0, 255);

static uint32_t colors[] = {black, green, red, blue};
int currentColor = 0;
bool selected = false;
int drawing[] = {
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
};

int cursor = 0;

void setup() {
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);

  Serial.begin(9600);

  pixels.begin();
  pixels.clear();
  pixels.setBrightness(5);
  pixels.show();
}

void goLeft() {
  currentColor = 0;
  selected = false;

  if (cursor == 0) {
    cursor = 63;
  } else {
    cursor--;
  }
}

void goRight() {
  currentColor = 0;
  selected = false;

  if (cursor == 63) {
    cursor = 0;
  } else {
    cursor++;
  }
}

void changePixel() {
  selected = true;

  currentColor++;

  if (currentColor > 3) {
    currentColor = 0;
  }

  if (currentColor < 0) {
    currentColor = 3;
  }

  drawing[cursor] = currentColor;
  Serial.print("Set ");
  Serial.print(cursor);
  Serial.print(" to ");
  Serial.println(currentColor);
}

void renderDrawing() {
  pixels.clear();

  for (int i = 0; i < 64; i++) {
    if (i == cursor && !selected) {
      pixels.setPixelColor(i, white);
    } else {
      pixels.setPixelColor(i, colors[drawing[i]]);
    }
  }
}

void loop() {
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);
  
  if (button1State == LOW) {
    Serial.println("BUTTON1");
    goLeft();
  }
  
  if (button3State == LOW) {
    Serial.println("BUTTON3");
    goRight();
  }

  if (button2State == LOW) {
    Serial.println("BUTTON2");
    changePixel();
  }

  renderDrawing();

  delay(300);
  pixels.show();

  
  if (Serial.available()) { // check if data is available to read
    String input = Serial.readString(); // read the incoming data as string
    if (input.indexOf("where cursor") >= 0) { // check if the word "hello" is found in the input string
      Serial.println(cursor); // print a message to the serial monitor
    }

    if (input.indexOf("show drawing") >= 0) {
      for (int i = 0; i < 64; i++) {
        if (i > 0 && i % 7 == 0) {
          Serial.println(drawing[i]);
        } else {
          Serial.print(drawing[i]);
          Serial.print("|");          
        }
      }
    }

  }

}
