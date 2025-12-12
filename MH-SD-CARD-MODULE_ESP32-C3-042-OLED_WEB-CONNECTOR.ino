#include <WiFi.h>
#include <SPI.h>
#include <SD.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

static const int PIN_SD_CS   = 10;
static const int PIN_SD_SCK  = 4;
static const int PIN_SD_MOSI = 3;
static const int PIN_SD_MISO = 1;

AsyncWebServer server(80);
File uploadFile;

String htmlIndex() {
  return R"HTML(
<!doctype html><html><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>ESP32-C3 SD</title></head><body>
<h3>SD File Manager</h3>

<form method="POST" action="/upload" enctype="multipart/form-data">
  <input type="file" name="data">
  <button type="submit">Upload</button>
</form>

<p><button onclick="loadList()">Refresh</button></p>
<ul id="list"></ul>

<script>
async function loadList(){
  const r = await fetch('/list');
  const files = await r.json();
  const ul = document.getElementById('list');
  ul.innerHTML = '';
  for (const f of files){
    const li = document.createElement('li');
    const name = f.name;
    li.innerHTML =
      `${name} (${f.size} B)
       <a href="/download?name=${encodeURIComponent(name)}">download</a>
       <a href="#" onclick="del('${name}')">delete</a>`;
    ul.appendChild(li);
  }
}
async function del(name){
  if(!confirm('Delete '+name+'?')) return;
  await fetch('/delete?name='+encodeURIComponent(name));
  loadList();
}
loadList();
</script>
</body></html>
)HTML";
}

void setup() {
  Serial.begin(115200);

  // WLAN als Access Point
  WiFi.softAP("ESP32C3-SD", "12345678");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  // SPI + SD init
  SPI.begin(PIN_SD_SCK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);
  if (!SD.begin(PIN_SD_CS)) {
    Serial.println("SD mount failed!");
    while (true) delay(1000);
  }
  Serial.println("SD mounted.");

  // Startseite
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
    req->send(200, "text/html; charset=utf-8", htmlIndex());
  });

  // Liste als JSON
  server.on("/list", HTTP_GET, [](AsyncWebServerRequest *req){
    String out = "[";
    File root = SD.open("/");
    File f = root.openNextFile();
    bool first = true;
    while (f) {
      if (!first) out += ",";
      first = false;
      String name = String("/") + f.name();
      out += "{\"name\":\"" + name + "\",\"size\":" + String((uint32_t)f.size()) + "}";
      f = root.openNextFile();
    }
    out += "]";
    req->send(200, "application/json", out);
  });

  // Download
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *req){
    if (!req->hasParam("name")) return req->send(400, "text/plain", "missing name");
    String path = req->getParam("name")->value();
    if (!SD.exists(path)) return req->send(404, "text/plain", "not found");
    req->send(SD, path, "application/octet-stream", true);
  });

  // Delete
  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *req){
    if (!req->hasParam("name")) return req->send(400, "text/plain", "missing name");
    String path = req->getParam("name")->value();
    if (!SD.exists(path)) return req->send(404, "text/plain", "not found");
    SD.remove(path);
    req->send(200, "text/plain", "ok");
  });

  // Upload (multipart)
  server.on(
    "/upload", HTTP_POST,
    [](AsyncWebServerRequest *req){ req->send(200, "text/plain", "upload ok"); },
    [](AsyncWebServerRequest *req, String filename, size_t index, uint8_t *data, size_t len, bool final){
      if (index == 0) {
        String path = "/" + filename;
        if (SD.exists(path)) SD.remove(path);
        uploadFile = SD.open(path, FILE_WRITE);
      }
      if (uploadFile) uploadFile.write(data, len);
      if (final && uploadFile) uploadFile.close();
    }
  );

  server.begin();
}

void loop() {}
