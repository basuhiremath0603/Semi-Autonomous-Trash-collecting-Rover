#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>////////////////////////////////

// ---------------- WIFI ----------------
const char* ssid = "your wifi ssid";
const char* password = "your wifi password";

// ---------------- LCD ----------------
// ⚠️ If LCD not visible, change 0x27 to 0x3F
#define SDA_PIN 21
#define SCL_PIN 22
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- MOTOR PINS ----------------
#define L_IN1 27
#define L_IN2 14
#define R_IN1 12
#define R_IN2 13

// ---------------- ULTRASONIC ----------------
#define TRIG_PIN 33
#define ECHO_PIN 25

// ---------------- IR SENSOR ----------------
#define IR_PIN 34   // ACTIVE LOW

// ---------------- SERVO ----------------
#define SERVO_PIN 26
Servo collectorServo;

// ---------------- GLOBALS ----------------
WebServer server(80);
bool autoMode = false;
#define OBSTACLE_DIST 10   // cm

// ---------------- DISTANCE FUNCTION ----------------
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 100;
  return duration * 0.034 / 2;
}

// ---------------- MOTOR CONTROL ----------------
void stopMotors() {
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, LOW);
}

void forward() {
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN1, HIGH);
  digitalWrite(R_IN2, LOW);
}

void backward() {
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);
  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, HIGH);
}

void turnLeft() {
  stopMotors();
  delay(150);
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);
  digitalWrite(R_IN1, HIGH);
  digitalWrite(R_IN2, LOW);
  delay(400);
  stopMotors();
}

void turnRight() {
  stopMotors();
  delay(150);
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, HIGH);
  delay(400);
  stopMotors();
}

// ---------------- TRASH COLLECTION ----------------
void collectTrash() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Collecting...");

  stopMotors();

  collectorServo.write(30);
  delay(700);

  collectorServo.write(110);
  delay(700);

  collectorServo.write(0);
  delay(700);

  lcd.clear();
  lcd.print("Trash Collected");
  delay(1000);
}

// ---------------- AUTONOMOUS MODE ----------------
void autonomousMove() {

  lcd.setCursor(0, 0);
  lcd.print("Searching Trash ");
  lcd.setCursor(0, 1);
  lcd.print("Auto Mode ON   ");

  forward();

  long distance = readDistanceCM();
  int trash = digitalRead(IR_PIN);

  if (distance < OBSTACLE_DIST) {
    lcd.clear();
    lcd.print("Obstacle Ahead");
    stopMotors();
    delay(300);
    lcd.setCursor(0, 1);
    lcd.print("Avoiding...");
    turnLeft();
  }
  else if (trash == LOW) {
    lcd.clear();
    lcd.print("Trash Detected");
    stopMotors();
    delay(300);
    collectTrash();
  }
}

// ---------------- WEB PAGE ----------------
String webpage() {
  return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Trash Robot</title>
<style>
body { background:#111; color:white; text-align:center; font-family:Arial; }
button {
  width:140px; height:55px; font-size:18px;
  margin:8px; border-radius:10px; border:none;
}
.f { background:#2ecc71; }
.b { background:#3498db; }
.l { background:#f1c40f; }
.r { background:#f39c12; }
.s { background:#e74c3c; }
.a { background:#9b59b6; }
</style>
</head>
<body>
<h2>ESP32 Trash Robot</h2>

<button class="a" onclick="fetch('/auto_on')">AUTO ON</button>
<button class="a" onclick="fetch('/auto_off')">AUTO OFF</button><br>

<button class="f" onclick="fetch('/f')">FORWARD</button><br>
<button class="l" onclick="fetch('/l')">LEFT</button>
<button class="s" onclick="fetch('/s')">STOP</button>
<button class="r" onclick="fetch('/r')">RIGHT</button><br>
<button class="b" onclick="fetch('/b')">BACK</button>

</body>
</html>
)rawliteral";
}

// ---------------- ROUTES ----------------
void setupRoutes() {

  server.on("/", []() {
    server.send(200, "text/html", webpage());
  });

  server.on("/auto_on", []() {
    autoMode = true;
    lcd.clear();
    lcd.print("AUTO MODE ON");
    server.send(200, "text/plain", "AUTO ON");
  });

  server.on("/auto_off", []() {
    autoMode = false;
    stopMotors();
    lcd.clear();
    lcd.print("MANUAL MODE");
    server.send(200, "text/plain", "AUTO OFF");
  });

  server.on("/f", []() {
    autoMode = false;
    forward();
    server.send(200, "text/plain", "F");
  });

  server.on("/b", []() {
    autoMode = false;
    backward();
    server.send(200, "text/plain", "B");
  });

  server.on("/l", []() {
    autoMode = false;
    turnLeft();
    server.send(200, "text/plain", "L");
  });

  server.on("/r", []() {
    autoMode = false;
    turnRight();
    server.send(200, "text/plain", "R");
  });

  server.on("/s", []() {
    stopMotors();
    server.send(200, "text/plain", "S");
  });
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  // I2C + LCD (FIXED ORDER)
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  delay(100);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ESP32 WROOM");
  lcd.setCursor(0, 1);
  lcd.print("Initializing");
  delay(1500);

  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);
  pinMode(R_IN1, OUTPUT);
  pinMode(R_IN2, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_PIN, INPUT);

  collectorServo.attach(SERVO_PIN);
  collectorServo.write(0);
  stopMotors();

  WiFi.begin(ssid, password);
  lcd.clear();
  lcd.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  lcd.clear();
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  setupRoutes();
  server.begin();
}

// ---------------- LOOP ----------------
void loop() {
  server.handleClient();

  if (autoMode) {
    autonomousMove();
  }
}