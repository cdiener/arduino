#define SENSOR A0
#define PUMP 15
#define RED 10
#define YELLOW 9
#define GREEN 8
#define dry 280

int dryness = 1024;
int old = 0;
bool emergency_off, pump_on;

void setup() {
  pinMode(SENSOR, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  digitalWrite(PUMP, HIGH);
  pump_on = false;
  emergency_off = false;
}

void loop() {
  old = dryness;
  dryness = analogRead(SENSOR);

  // emergency stop if tank empty or tube diconnected
  if (pump_on && (old - dryness) < 10) {
    emergency_off = true;
  }
  
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
  if (dryness <= dry) {
    digitalWrite(GREEN, HIGH);
  } else if (dryness <= 320) {
    digitalWrite(YELLOW, HIGH);
  } else {
    digitalWrite(RED, HIGH);
  }

  if ((!emergency_off) && (dryness >= dry)) {
    digitalWrite(PUMP, LOW);
    pump_on = true;
  } else {
    digitalWrite(PUMP, HIGH);
    pump_on = false;
  }
  delay(5000);
}
