#include "AmbiLight.hpp"

#define serialRate 500000

AmbiLight *ambi;

void setup() {
  Serial.begin(serialRate);
  // Send "Magic Word" string to host
  Serial.print("Ada\n");
  ambi = new AmbiLight{66};
}

void loop() { ambi->ambiLoop(); }