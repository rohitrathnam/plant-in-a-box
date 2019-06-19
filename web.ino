void handleRoot()
{
  text = "Temperature " + String(celsiusTemp) + "*C<p>Humidity " + String(humidityTemp) + "%<p>Soil Moisture " + String(soilMoist) + "<p>Time " + String(hour) + ":" + String(minutes) + ":" + String(seconds) + "<p> Pump: " + pump + "<p>Light: " + light;
  server.send(200, "text/html", HTML_start + text + HTML_end);
}

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void handleManualCtrl()
{
  if (server.hasArg("pump") || server.hasArg("light")) {
    handleSubmit();
  }
  server.send(200, "text/html", HTML_start + HTML_form + HTML_end);
}

void handleTime()
{
  server.send(200, "text/html", HTML_start + HTML_form1 + HTML_end);
  if (server.hasArg("lhour") || server.hasArg("duration")) {
    handleSubmit1();
  }
}

void handleSubmit()
{
  if (!server.hasArg("pump") || !server.hasArg("light")) {
    return returnFail("BAD ARGS");
  }
  else {
    pump = server.arg("pump");
    manual = true;
    light = server.arg("light");
  }

  Serial.println(pump + light);

  if (pump == "on") {
    server.send(200, "text/html", HTML_start + HTML_redirect + HTML_end);
  }
  else if (pump == "off") {
    server.send(200, "text/html", HTML_start + HTML_redirect + HTML_end);
  }
  if (light == "on") {
    server.send(200, "text/html", HTML_start + HTML_redirect + HTML_end);
  }
  else if (light == "off") {
    server.send(200, "text/html", HTML_start + HTML_redirect + HTML_end);
  }
  else {
    returnFail("Bad value");
  }
}

void handleSubmit1()
{
  if (!server.hasArg("lhour") || !server.hasArg("duration")) {
    return returnFail("BAD ARGS");
  }
  else {
    lhour = server.arg("lhour");
    duration = server.arg("duration");
    timer = true;
    server.send(200, "text/html", HTML_start + HTML_redirect + HTML_end);
  }
}

void returnOK()
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
