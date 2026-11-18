// === Pin Ultrasonik Kanan ===
#define trigKanan A0
#define echoKanan A1

// === Pin Ultrasonik Kiri ===
#define trigKiri A3
#define echoKiri A2   // dipindah biar tidak bentrok dengan motor pemotong

// === Pin Motor Driver L298N (roda penggerak) ===
#define IN1 2   // Motor kanan maju
#define IN2 3   // Motor kanan mundur
#define IN3 4   // Motor kiri maju
#define IN4 5   // Motor kiri mundur
#define ENA A4   // Kecepatan motor kanan (PWM)
#define ENB A5   // Kecepatan motor kiri (PWM)

// === Pin Motor Driver L298N (pemotong rumput) ===
#define IN_PEMOTONG1 6
#define IN_PEMOTONG2 7
// ENA motor pemotong dijumper ke 5V di modul driver

// === Pin Sensor Warna TCS3200 ===
#define S0 8
#define S1 9
#define S2 10
#define S3 11
#define sensorOut 12

// Variabel global
int speedMotor = 150;   // Kecepatan motor roda (0-255)
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
const int tinggiPemotong = 1; // ketinggian pemotong rumput (cm)

// === Fungsi Baca Jarak Ultrasonik ===
long bacaJarak(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long durasi = pulseIn(echoPin, HIGH, 40000); // timeout 40 ms
  long jarak = durasi * 0.034 / 2;

  if (jarak < 2 || jarak > 300) return -1; // -1 = tidak terdeteksi
  return jarak;
}

// === Fungsi Gerak Motor Roda ===
void maju() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedMotor);
  analogWrite(ENB, speedMotor);
}

void belokKiri() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedMotor);
  analogWrite(ENB, speedMotor);
}

void belokKanan() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedMotor);
  analogWrite(ENB, speedMotor);
}

void berhenti() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// === Fungsi Motor Pemotong ===
void pemotongOn() {
  digitalWrite(IN_PEMOTONG1, HIGH);
  digitalWrite(IN_PEMOTONG2, LOW);
}

void pemotongOff() {
  digitalWrite(IN_PEMOTONG1, LOW);
  digitalWrite(IN_PEMOTONG2, LOW);
}

// === Fungsi Baca Warna dari TCS3200 ===
String bacaWarna() {
  // Baca Red
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  redFrequency = pulseIn(sensorOut, LOW);
  delay(10);

  // Baca Green
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  greenFrequency = pulseIn(sensorOut, LOW);
  delay(10);

  // Baca Blue
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blueFrequency = pulseIn(sensorOut, LOW);
  delay(10);

  // === Klasifikasi Warna (threshold perlu kalibrasi manual) ===
  if (greenFrequency < redFrequency && greenFrequency < blueFrequency) {
    return "Hijau";
  } 
  else if (redFrequency < greenFrequency && redFrequency < blueFrequency && greenFrequency < blueFrequency) {
    return "KuningKecoklatan";  
  } 
  else if (redFrequency < greenFrequency && blueFrequency < greenFrequency) {
    return "Coklat";
  } 
  else {
    return "TidakDikenal";
  }
}

// === SETUP ===
void setup() {
  // Motor roda
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Motor pemotong
  pinMode(IN_PEMOTONG1, OUTPUT);
  pinMode(IN_PEMOTONG2, OUTPUT);

  // Sensor Ultrasonik
  pinMode(trigKanan, OUTPUT);
  pinMode(echoKanan, INPUT);
  pinMode(trigKiri, OUTPUT);
  pinMode(echoKiri, INPUT);

  // Sensor Warna
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Set scaling frekuensi TCS3200 = 100%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);

  Serial.begin(9600);
}

// === LOOP ===
void loop() {
  long jarakKanan = bacaJarak(trigKanan, echoKanan);
  long jarakKiri = bacaJarak(trigKiri, echoKiri);

  String warna = bacaWarna();

  // === Kontrol Dinamo Pemotong Berdasarkan Warna ===
  if (warna == "Hijau" || warna == "KuningKecoklatan" || warna == "Coklat") {
    pemotongOn(); 
  } else {
    pemotongOff();
  }

  // === Cetak semua data ringkas di Serial Monitor ===
  Serial.print("JarakKanan=");
  if (jarakKanan == -1) Serial.print("ND"); else { Serial.print(jarakKanan); Serial.print("cm"); }
  Serial.print(" | JarakKiri=");
  if (jarakKiri == -1) Serial.print("ND"); else { Serial.print(jarakKiri); Serial.print("cm"); }
  Serial.print(" | Warna="); Serial.print(warna);
  Serial.print(" | Pemotong=");
  if (warna == "Hijau" || warna == "KuningKecoklatan" || warna == "Coklat") Serial.print("ON"); else Serial.print("OFF");
  Serial.print(" | TinggiPemotong=");
  Serial.print(tinggiPemotong);
  Serial.println("cm");

  // === Logika Hindar Rintangan untuk roda (1-5 cm saja) ===
  if ((jarakKanan >= 1 && jarakKanan <= 5) && (jarakKiri >= 1 && jarakKiri <= 5)) {
    berhenti();
    delay(300);
    belokKiri();
    delay(400);
  }
  else if (jarakKanan >= 1 && jarakKanan <= 5) {
    belokKiri();
    delay(400);
  }
  else if (jarakKiri >= 1 && jarakKiri <= 5) {
    belokKanan();
    delay(400);
  }
  else {
    maju(); // kalau >5 cm, anggap jalan kosong
  }
}
