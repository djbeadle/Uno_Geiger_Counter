void rtc_serial() {
  DateTime now = RTC.now();
  Serial.print("RTC Says: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}

void rtc_LCD() {
  DateTime now = RTC.now();
  lcd.print("RTC: ");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  lcd.println();
}

