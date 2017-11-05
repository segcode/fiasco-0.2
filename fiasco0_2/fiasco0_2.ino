#include <util/crc16.h>

#define RADIOPIN 13

void setup()  {
  Serial.begin(9600);
  pinMode(RADIOPIN, OUTPUT);
}


void loop() {
  String datastring = "yo\n";
  String checksum = String(gps_CRC16_checksum(datastring));
 
  rtty_txstring(checksum);
  rtty_txstring(",");
  rtty_txstring(datastring);
  rtty_txstring("\n");
}


void rtty_txstring (String s) {
  Serial.println(s);
  for (int i = 0; i < s.length(); i++) {
    char c = s.charAt(i);
    for (int j = 0; j < 8; j++) {
      if (c & (1 << j)) {
        rtty_txbit(1);
      }
      else {
        rtty_txbit(0);
      }
    }
  }
}

void rtty_txbit (int bit) {
  if (bit)
  {
    // high
    Serial.println(1);
    digitalWrite(RADIOPIN, HIGH);
  }
  else
  {
    Serial.println(0);
    // low
    digitalWrite(RADIOPIN, LOW);

  }
  
  delay(100);
}

uint16_t gps_CRC16_checksum (String s) {
  size_t i;
  uint16_t crc;
  uint8_t c;

  crc = 0xFFFF;

  // Calculate checksum ignoring the first two $s
  for (i = 2; i < s.length(); i++)
  {
    c = s.charAt(i);
    crc = _crc_xmodem_update (crc, c);
  }

  return crc;
}
