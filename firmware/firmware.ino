#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>

// WiFi & server setup
const char* ssid = "xxx";
const char* password = "xxxxxxxx";
const char* server = "192.168.xxx.xx";
const int port = 5000;

#define CAMERA_MODEL_AI_THINKER
#define SERVO_PIN 13
#include "camera_pins.h"

bool servoActive = false;
unsigned long servoStartTime = 0;
unsigned long lastCaptureTime = 0;
const unsigned long captureInterval = 2000;

void startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 16000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(SERVO_PIN, OUTPUT);
  digitalWrite(SERVO_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.println("IP address: " + WiFi.localIP().toString());

  startCamera();
  lastCaptureTime = millis();
}

void loop() {
  unsigned long now = millis();

  // Turn off servo after 500 ms
  if (servoActive && now - servoStartTime > 500) {
    digitalWrite(SERVO_PIN, LOW);
    servoActive = false;
    Serial.println("Servo deactivated.");
  }

  // Wait 2 seconds before capturing new image
  if (now - lastCaptureTime < captureInterval) {
    return;
  }

  lastCaptureTime = now;
  Serial.println("Capturing new image at millis: " + String(now));

  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  WiFiClient client;
  if (!client.connect(server, port)) {
    Serial.println("Connection to server failed");
    esp_camera_fb_return(fb);
    return;
  }

  String boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
  String head = "--" + boundary + "\r\n" +
                "Content-Disposition: form-data; name=\"image\"; filename=\"capture.jpg\"\r\n" +
                "Content-Type: image/jpeg\r\n\r\n";
  String tail = "\r\n--" + boundary + "--\r\n";

  int contentLength = head.length() + fb->len + tail.length();

  client.println("POST /upload HTTP/1.1");
  client.println("Host: " + String(server));
  client.println("Content-Type: multipart/form-data; boundary=" + boundary);
  client.println("Content-Length: " + String(contentLength));
  client.println();
  client.print(head);
  client.write(fb->buf, fb->len);
  client.print(tail);

  // Read server response body
  String body = "";
  unsigned long timeout = millis() + 5000;
  while (client.connected() && millis() < timeout) {
    while (client.available()) {
      body += (char)client.read();
    }
  }

  Serial.println("Response body:");
  Serial.println(body);

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, body);

  if (!error) {
    String cls = doc["class"];
    float conf = doc["confidence"];
    Serial.println("Class: " + cls + " Confidence: " + String(conf, 2));
    cls.trim();
    if (cls == "metal") {
      Serial.println("Metal detected. Activating servo...");
      digitalWrite(SERVO_PIN, HIGH);
      servoStartTime = millis();
      servoActive = true;
    }
  } else {
    Serial.println("Failed to parse JSON.");
  }

  esp_camera_fb_return(fb);
  client.stop();
}
