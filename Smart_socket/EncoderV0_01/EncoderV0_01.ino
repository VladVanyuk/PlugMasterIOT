#include <Encoder.h>


const int encoderPinA = 13;
const int encoderPinB = 12;
// Пін для кнопки
const int buttonPin = 14;
// Пін світлодіода (використовуйте пін, що підтримує PWM)
const int ledPin = 2; 

// Ініціалізація об'єкту енкодера
Encoder myEnc(encoderPinA, encoderPinB);
// Остання зчитана позиція енкодера
long lastEncoderPosition  = 0;

// Змінна для зберігання стану кнопки
bool lastButtonState = HIGH; 
// Зберігає останній час, коли кнопка була натиснута
unsigned long lastDebounceTime = 0;  
// Затримка для "дребезгу" кнопки
unsigned long debounceDelay = 50;    
// Чи вмиканий світлодіод
bool ledState = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // Опитування енкодера
  long encoderPosition = myEnc.read() / 4; // Ділімо на 4 для кожного кроку енкодера
  if (encoderPosition != lastEncoderPosition) {
    lastEncoderPosition = encoderPosition;
    int brightness = map(encoderPosition, 0, 1023, 0, 255); // Відображення значення енкодера на значення PWM
    analogWrite(ledPin, brightness);
    Serial.print("Яскравість: ");
    Serial.println(brightness);
  }
  
  // Опитування кнопки
  bool reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Якщо стан кнопки стабілізувався
    if (reading != lastButtonState) {
      lastButtonState = reading;
      // Змінюємо стан світлодіода тільки якщо кнопка була відпущена
      if (lastButtonState == LOW) {
        ledState = !ledState;
        // Якщо світлодіод має бути вимкнений, встановлюємо яскравість у 0
        analogWrite(ledPin, ledState ? map(encoderPosition, 0, 1023, 0, 255) : 0);
      }
    }
  }
  
  // Затримка для зменшення дребезгу кнопки
  delay(50);
}
