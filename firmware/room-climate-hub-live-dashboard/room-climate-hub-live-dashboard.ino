#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "control.h"
#include "config.h"
constexpr uint8_t PIR_PIN = 27, RELAY_PIN = 26, SDA_PIN = 21, SCL_PIN = 22;
constexpr uint8_t INA_ADDR = 0x40;
WebServer server(80);
Control control;
uint32_t lastSample = 0;
bool initialized = false;
bool writeRegister(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(INA_ADDR); Wire.write(reg);
  Wire.write(uint8_t(value >> 8)); Wire.write(uint8_t(value));
  return Wire.endTransmission() == 0;
}
bool readRegister(uint8_t reg, uint16_t &value) {
  Wire.beginTransmission(INA_ADDR); Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(INA_ADDR, uint8_t(2)) != 2) return false;
  value = uint16_t(Wire.read()) << 8; value |= uint8_t(Wire.read()); return true;
}
String telemetry() {
  String result = "{\"project_id\":1,\"uptime_ms\":" + String(millis());
  result += ",\"motion\":"; result += control.motion ? "true" : "false";
  result += ",\"current_mA\":"; result += control.valid ? String(control.current_mA, 1) : "null";
  result += ",\"sensor_ok\":"; result += control.valid ? "true" : "false";
  result += ",\"relay\":"; result += control.relay ? "true" : "false";
  return result + "}";
}
const char PAGE[] PROGMEM = R"html(<!doctype html><html><meta charset="utf-8"><title>Room Climate Hub</title><h1>Live motion and lamp current</h1><p>Low-voltage educational prototype. No climate sensor is fitted.</p><pre id="data">Loading</pre><button onclick="set(false)">Lamp OFF</button><button onclick="set(true)">Lamp ON</button><script>async function refresh(){try{const r=await fetch('/api/status');document.getElementById('data').textContent=JSON.stringify(await r.json(),null,2)}catch(e){document.getElementById('data').textContent='Device unreachable'}}async function set(on){await fetch('/api/relay?on='+Number(on),{method:'POST'});refresh()}refresh();setInterval(refresh,1000)</script></html>)html";
void setup() {
  pinMode(RELAY_PIN, OUTPUT); digitalWrite(RELAY_PIN, LOW); pinMode(PIR_PIN, INPUT);
  Serial.begin(115200); Wire.begin(SDA_PIN, SCL_PIN);
  // 32V bus range, +/-320mV shunt, continuous 12-bit shunt and bus conversions.
  // 0.1 ohm shunt, 100uA/bit current LSB: 0.04096/(0.0001*0.1) = 4096.
  initialized = writeRegister(0x00, 0x399F) && writeRegister(0x05, 4096);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP(AP_SSID, AP_PASSWORD)) Serial.println("AP startup failed; relay remains off");
  server.on("/", HTTP_GET, [](){ server.send(200,"text/html",PAGE); });
  server.on("/api/status", HTTP_GET, [](){ server.send(200,"application/json",telemetry()); });
  server.on("/api/relay", HTTP_POST, [](){
    if (!server.hasArg("on") || (server.arg("on") != "0" && server.arg("on") != "1")) {
      server.send(400,"text/plain","on must be 0 or 1"); return;
    }
    if (server.arg("on") == "1" && !control.valid) { server.send(409,"text/plain","sensor not ready"); return; }
    control.command(server.arg("on") == "1"); digitalWrite(RELAY_PIN, control.relay ? HIGH : LOW);
    server.send(200,"application/json",telemetry());
  });
  server.onNotFound([](){ server.send(404,"text/plain","Not found"); });
  server.begin();
}
void loop() {
  server.handleClient();
  const uint32_t now = millis(); if (uint32_t(now-lastSample) < 1000) return; lastSample=now;
  uint16_t raw=0, bus=0;
  if (!initialized) initialized = writeRegister(0,0x399F) && writeRegister(5,4096);
  bool ok = initialized && readRegister(2,bus) && (bus & 2) && !(bus & 1) && readRegister(4,raw);
  if (!ok) initialized=false;
  control.sample(digitalRead(PIR_PIN)==HIGH, float(int16_t(raw))*0.1f, ok);
  digitalWrite(RELAY_PIN, control.relay ? HIGH : LOW); Serial.println(telemetry());
}
