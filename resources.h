const char* ssid = "The Hulk";
const char* password = "hulksmash123";
String text = "";

//static const uint8_t D0   = 16;
//static const uint8_t D1   = 5;
//static const uint8_t D2   = 4;
//static const uint8_t D3   = 0;
//static const uint8_t D4   = 2;
//static const uint8_t D5   = 14;
//static const uint8_t D6   = 12;
//static const uint8_t D7   = 13;
//static const uint8_t D8   = 15;
//static const uint8_t D9   = 3;
//static const uint8_t D10  = 1;

const int pumpPin = 16;
const int lightPin = 5;

const String HTML_start = "<html>";
const String HTML_end = "</html>";
const String HTML_form = "<h2>Manual Control</h2><form action='' method='post'>Pump <input type='radio' name='pump' value='on'> on<input type='radio' name='pump' value='off'> off<br><br>Light <input type='radio' name='light' value='on'> on<input type='radio' name='light' value='off'> off<br><br><input type='submit' value='Update'></form>";
const String HTML_form1 = "<p>Light Start Time<p><form action='' method='post'>Hour <input type='text' name='lhour'><br> Number of hours <input type='text' name='duration'><br><input type='submit' value='Update'></form>";
const String HTML_redirect = "<meta http-equiv='refresh' content='0; URL=http://plantinabox.local/'><meta name='keywords' content='automatic redirection'>";

unsigned int localPort = 2390;

const unsigned long seventyYears = 2208988800UL;
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

long timeNow, timeLast;
long seconds, minutes, hour, realHour, realMin, realSec;
String lhour, duration;
unsigned long highWord, lowWord, secsSince1900, epoch;
String pump = "off", light = "off";

bool timer = false;
bool manual = false;

#define DHTPin 0
#define DHTTYPE DHT22

char celsiusTemp[7];
char fahrenheitTemp[7];
char humidityTemp[7];
int soilMoist;
float h, t, f, hic, hif;

void handleRoot();
void returnFail(String msg);
void handleManualCtrl();
void handleSubmit();
void handleSubmit1();
void returnOK();
void handleNotFound();
void handleTime();

unsigned long sendNTPpacket(IPAddress& address);
bool getTime();
void updateTime();

void checkStats();
void getData();
