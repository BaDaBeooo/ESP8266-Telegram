#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "MIRAWAY";  
const char* password = "Vuotso@2025";

#define BOT_TOKEN "7640958864:AAEI8C18eU8m3TSa-qRozdNeuTNjywStkkQ"
#define CHAT_ID "-4665142256"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

const int ledPin = LED_BUILTIN;  // Đèn LED tích hợp trên ESP8266
String lastCommand = "";  // Lưu lệnh cuối cùng

String getKeyboard() {
    String keyboard = "[[\"Default LED\"]]";
    return keyboard;
}

void handleNewMessages(int numNewMessages) {
    for (int i = 0; i < numNewMessages; i++) {
        String chat_id = bot.messages[i].chat_id;
        String text = bot.messages[i].text;
        
        if (text == "/batden") {
            lastCommand = text;  // Lưu lệnh
            String keyboardJson = getKeyboard();
            bot.sendMessageWithReplyKeyboard(chat_id, "Chọn đèn cần bật:", "", keyboardJson, true);
        }
        else if (text == "/tatden") {
            lastCommand = text;  // Lưu lệnh
            String keyboardJson = getKeyboard();
            bot.sendMessageWithReplyKeyboard(chat_id, "Chọn đèn cần tắt:", "", keyboardJson, true);
        }
        else if (text == "Default LED") {
            if (lastCommand == "/batden") {
                digitalWrite(ledPin, LOW);  // Bật đèn (active LOW)
                bot.sendMessage(chat_id, "✅ Đã BẬT đèn Default", "");
            }
            else if (lastCommand == "/tatden") {
                digitalWrite(ledPin, HIGH);  // Tắt đèn
                bot.sendMessage(chat_id, "❌ Đã TẮT đèn Default", "");
            }
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);  // Tắt đèn khi khởi động

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    client.setInsecure();
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.reconnect();
    }
    
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    if (numNewMessages > 0) {
        handleNewMessages(numNewMessages);
    }
    delay(1000);
}
