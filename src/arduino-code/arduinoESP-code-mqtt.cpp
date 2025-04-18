// #include <WiFi.h>
// #include <PubSubClient.h>
// #include "esp_camera.h"
// #include "base64.h"  // Add this library for base64 encoding

// // Wi-Fi credentials
// const char* ssid = "Redmi 12";
// const char* password = "123456789";

// // MQTT Broker settings
// const char* mqtt_server = "192.168.90.88";  // Your PC's IP
// const char* mqtt_topic_img = "esp32_cam_image";  // Topic for image frames

// WiFiClient espClient;
// PubSubClient client(espClient);

// // Camera pins (for AI Thinker model)
// #define PWDN_GPIO_NUM     32
// #define RESET_GPIO_NUM    -1
// #define XCLK_GPIO_NUM      0
// #define SIOD_GPIO_NUM     26
// #define SIOC_GPIO_NUM     27
// #define Y9_GPIO_NUM       35
// #define Y8_GPIO_NUM       34
// #define Y7_GPIO_NUM       39
// #define Y6_GPIO_NUM       36
// #define Y5_GPIO_NUM       21
// #define Y4_GPIO_NUM       19
// #define Y3_GPIO_NUM       18
// #define Y2_GPIO_NUM        5
// #define VSYNC_GPIO_NUM    25
// #define HREF_GPIO_NUM     23
// #define PCLK_GPIO_NUM     22

// void setup_camera() {
//   camera_config_t config;
//   config.ledc_channel = LEDC_CHANNEL_0;
//   config.ledc_timer = LEDC_TIMER_0;
//   config.pin_d0 = Y2_GPIO_NUM;
//   config.pin_d1 = Y3_GPIO_NUM;
//   config.pin_d2 = Y4_GPIO_NUM;
//   config.pin_d3 = Y5_GPIO_NUM;
//   config.pin_d4 = Y6_GPIO_NUM;
//   config.pin_d5 = Y7_GPIO_NUM;
//   config.pin_d6 = Y8_GPIO_NUM;
//   config.pin_d7 = Y9_GPIO_NUM;
//   config.pin_xclk = XCLK_GPIO_NUM;
//   config.pin_pclk = PCLK_GPIO_NUM;
//   config.pin_vsync = VSYNC_GPIO_NUM;
//   config.pin_href = HREF_GPIO_NUM;
//   config.pin_sscb_sda = SIOD_GPIO_NUM;
//   config.pin_sscb_scl = SIOC_GPIO_NUM;
//   config.pin_pwdn = PWDN_GPIO_NUM;
//   config.pin_reset = RESET_GPIO_NUM;
//   config.xclk_freq_hz = 20000000;
//   config.pixel_format = PIXFORMAT_JPEG;
//   config.frame_size = FRAMESIZE_QVGA;  // Lower resolution for smaller image
//   config.jpeg_quality = 80;            // Lower quality = smaller size
//   config.fb_count = 1;

//   esp_err_t err = esp_camera_init(&config);
//   if (err != ESP_OK) {
//     Serial.printf("Camera init failed: 0x%x", err);
//     ESP.restart();
//   }
// }

// void reconnect() {
//   while (!client.connected()) {
//     Serial.print("Connecting to MQTT...");
//     if (client.connect("ESP32CAMClient")) {
//       Serial.println("connected.");
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" retrying in 5s...");
//       delay(5000);
//     }
//   }
// }

// void setup() {
//   Serial.begin(115200);
//   delay(1000);
//   Serial.println("Connecting to WiFi...");

//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\nWiFi connected!");

//   client.setServer(mqtt_server, 1883);
//   setup_camera();
// }

// void loop() {
//   if (!client.connected()) {
//     reconnect();
//   }
//   client.loop();

//   // Send a simple test message ("Hello World") to verify MQTT topic
//   bool sent = client.publish("esp32_cam_image", "Hello World");
//   if (sent) {
//     Serial.println("Test message sent");
//   } else {
//     Serial.println("Failed to send test message");
//   }

//   // Capture image from the camera
//   camera_fb_t* fb = esp_camera_fb_get();
//   if (!fb) {
//     Serial.println("Camera capture failed");
//     return;
//   }

//   Serial.printf("Captured image size: %d bytes\n", fb->len);

//   // ===== OPTION 2: Base64 encoded image (more compatible) =====
//   String encoded = base64::encode(fb->buf, fb->len);
//   bool sent_image = client.publish(mqtt_topic_img, encoded.c_str());

//   if (sent_image) {
//     Serial.printf("Published %d bytes\n", fb->len);
//   } else {
//     Serial.println("Failed to publish image");
//   }

//   esp_camera_fb_return(fb);
//   delay(3000);  // Adjust as needed
// }

#include <WiFi.h>
#include <PubSubClient.h>
#include "esp_camera.h"
#include "base64.h"  // Base64 encoding library

// ==== WiFi credentials ====
const char* ssid = "Redmi 12";
const char* password = "123456789";

// ==== MQTT settings ====
const char* mqtt_server = "192.168.90.88";  // Change to your MQTT broker IP
const char* mqtt_topic_img = "esp32_cam_image";  // Topic for image frames

WiFiClient espClient;
PubSubClient client(espClient);

// ==== ESP32-CAM pin definitions (AI Thinker model) ====
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ==== Setup the camera ====
void setup_camera() {
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
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Recommended settings for streaming
  config.frame_size = FRAMESIZE_QVGA;     // 320x240 for smaller image
  config.jpeg_quality = 15;               // 0-63 lower = better quality
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x", err);
    ESP.restart();
  }
}

// ==== MQTT reconnect function ====
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32CAMClient")) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5s...");
      delay(5000);
    }
  }
}

// ==== Setup function ====
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  client.setServer(mqtt_server, 1883);
  setup_camera();
}

// ==== Main loop ====
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Capture frame
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Debug info
  Serial.printf("Captured image size: %d bytes\n", fb->len);

  // Base64 encode the image buffer
  String encoded = base64::encode(fb->buf, fb->len);

  // Publish to MQTT topic
  bool sent = client.publish(mqtt_topic_img, encoded.c_str());

  if (sent) {
    Serial.println("Image published");
  } else {
    Serial.println("Failed to publish image");
  }

  // Return the frame buffer
  esp_camera_fb_return(fb);

  // Optional: short delay to avoid flooding (tweak for performance)
  delay(100);  // You can try reducing this even more (e.g., 50 ms)
}
