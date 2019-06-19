unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

bool getTime()
{
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);

  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
    delay(1000);
    return (false);
  }
  else {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    highWord = word(packetBuffer[40], packetBuffer[41]);
    lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    secsSince1900 = highWord << 16 | lowWord;
    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // subtract seventy years:
    epoch = secsSince1900 - seventyYears;
    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    realHour = (epoch  % 86400L) / 3600;
    Serial.print(realHour); // print the hour (86400 equals secs per day)
    realMin = (epoch  % 3600) / 60;
    Serial.print(realMin); // print the minute (3600 equals secs per minute)
    realSec = epoch % 60;
    Serial.println(realSec); // print the second
    hour = realHour;
    minutes = realMin;
    seconds = realSec;
    return (true);
  }
}

void updateTime()
{
  timeNow = millis() / 1000;
  seconds = timeNow - timeLast;
  if (seconds == 60) {
    timeLast = timeNow;
    minutes = minutes + 1;
    seconds = 0;
  }
  if (minutes == 60) {
    while (!getTime());
  }
}

