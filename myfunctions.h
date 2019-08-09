void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}


void writeLed(bool LEDon)
{
  // Note inverted logic for Adafruit HUZZAH board
  if (LEDon)
    digitalWrite(ledPin, 0);
  else
    digitalWrite(ledPin, 1);
}


void writeRelay(bool LEDon)
{
  if (LEDon) {
    digitalWrite(relayPin, HIGH);
    loopStart = millis();
    status = 1;
  }
  else {
    digitalWrite(relayPin, LOW);
    loopStart = 0;
    status = 0;
  }

//  Serial.print("loopStart = ");
//  Serial.println(loopStart);
//  Serial.print("oneday = ");
//  Serial.println(oneDay);

}


String currentStatus () {

  if (status) {

    long val = ((loopStart + oneDay) - millis()) / 1000;

    String days = String(elapsedDays(val));
    String hours = String(numberOfHours(val));
    String minutes = String(numberOfMinutes(val));
    String seconds = String(numberOfSeconds(val));

//    Serial.print ("days = ");
//    Serial.println (days);

    String statusStr = "";

    statusStr = statusStr +
                       "<p> System is On</p>" +
                       "<p>Turns off in " +
                       " " +
                       days +
                       " days " +
                       hours +
                       " hours " +
                       minutes +
                       " mins " +
                       seconds +
                       "secs</p>";

    return statusStr;
  }
  return "<p> System is Off</p>";
}

String buildPage() {

  String page = "";

  page = INDEX_HEAD + INDEX_BODY_1 + currentStatus() + FORM + INDEX_BODY_2;

  return page;
}


void handleSubmit()
{
  String LEDvalue;
  String TimeValue = "24";

  if (server.hasArg("TIME")) {
    TimeValue = server.arg("TIME");
    oneDay = TimeValue.toInt() * oneHour;
  }
  else {
    oneDay = 24 * oneHour;
  }

  if (!server.hasArg("LED")) return returnFail("BAD ARGS");
  LEDvalue = server.arg("LED");
  if (LEDvalue == "1") {
    writeLed(true);
    writeRelay(true);
    server.send(200, "text/html", buildPage() );
  }
  else if (LEDvalue == "0") {
    writeLed(false);
    writeRelay(false);
    server.send(200, "text/html", buildPage() );
  }
  else {
    returnFail("Bad LED value");
  }
}


void handleRoot()
{
  if (server.hasArg("LED")) {
    handleSubmit();
  }
  else {
    server.send(200, "text/html", buildPage() );
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
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


/*
 * Imperative to turn the LED on using a non-browser http client.
 * For example, using wget.
 * $ wget http://esp8266webform/ledon
 */
void handleLedOn()
{
  writeLed(true);
  writeRelay(true);
  returnOK();
}


/*
 * Imperative to turn the LED off using a non-browser http client.
 * For example, using wget.
 * $ wget http://esp8266webform/ledoff
 */
void handleLedOff()
{
  writeLed(false);
  writeRelay(false);
  loopStart = 0;
  returnOK();
}


