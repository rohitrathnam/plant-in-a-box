void getData()
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);
  soilMoist = analogRead(A0);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f) || isnan(soilMoist)) {
    Serial.println("Failed to read from DHT sensor!");
    strcpy(celsiusTemp, "Failed");
    strcpy(fahrenheitTemp, "Failed");
    strcpy(humidityTemp, "Failed");
  }
  else {
    // Computes temperature values in Celsius + Fahrenheit and Humidity
    hic = dht.computeHeatIndex(t, h, false);
    dtostrf(hic, 6, 2, celsiusTemp);
    hif = dht.computeHeatIndex(f, h);
    dtostrf(hif, 6, 2, fahrenheitTemp);
    dtostrf(h, 6, 2, humidityTemp);
    //You can delete the following Serial.print's, it's just for debugging purposes
    /*  Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print(" %\t Temperature: ");
        Serial.print(t);
        Serial.print(" *C ");
        Serial.print(f);
        Serial.print(" *F\t Heat index: ");
        Serial.print(hic);
        Serial.print(" *C ");
        Serial.print(hif);
        Serial.print(" *F");
        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print(" %\t Temperature: ");
        Serial.print(t);
        Serial.print(" *C ");
        Serial.print(f);
        Serial.print(" *F\t Heat index: ");
        Serial.print(hic);
        Serial.print(" *C ");
        Serial.print(hif);
        Serial.println(" *F");*/
  }
}

void checkStats() {
  getData();
  if (!manual) {
    if (soilMoist <= 450) pump = "off";
    else if (soilMoist > 450 && soilMoist < 1000) pump = "on";
    else pump = "off";
  }

  if (timer) {
    if ((lhour.toInt() - hour <= 0 && (lhour.toInt() + duration.toInt()) - hour >= 0) || (lhour.toInt() - hour >= 0 && (lhour.toInt() + duration.toInt()) - hour <= 0))
    {
      light = "on";
      Serial.println("time on");
    }
    else {
      light = "off";
      Serial.println("time off");
    }
  }
  if (pump == "on") digitalWrite(pumpPin, LOW);
  else if (pump == "off") digitalWrite(pumpPin, HIGH);
  if (light == "on") digitalWrite(lightPin, LOW);
  else if (light == "off") digitalWrite(lightPin, HIGH);
}
