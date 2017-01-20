

enum {
  EVENT_UPDATE = 0b00000001,
};

IntervalTimer main_loop_timer;
IntervalTimer buzzer_timer;
volatile uint16_t events;
byte led_counter;


#define LED_1   29
#define LED_2   30
#define LED_3   31
#define LED_4   32
#define BUZZER  23

void setup() {

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  analogWriteFrequency(BUZZER, 2000000);
  analogWrite(BUZZER, 128);
  main_loop_timer.begin(eventChecker, 100000);
  main_loop_timer.priority(0);
}

void loop() {
  if (events & EVENT_UPDATE){
    digitalWrite(LED_1, led_counter == 0);
    digitalWrite(LED_2, led_counter == 1);
    digitalWrite(LED_3, led_counter == 2);
    digitalWrite(LED_4, led_counter == 3);
    led_counter++;
    if(led_counter > 3){led_counter = 0;}
    events &= ~EVENT_UPDATE;
  }
}

void eventChecker(void) {
  events |= EVENT_UPDATE;
}


