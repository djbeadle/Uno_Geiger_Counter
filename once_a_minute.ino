void once_a_minute() {
  // fetch the time
  DateTime now = RTC.now();
  // log time
  // log milliseconds since starting
  uint32_t m = millis();
  logfile.print(m); // milliseconds since start
  logfile.print(", ");
#if ECHO_TO_SERIAL
  Serial.print("Millis: ");
  Serial.print(m); // milliseconds since start
  Serial.print(" ");
#endif
  logfile.print('"');
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
  logfile.print('"');
  logfile.print(", ");
#if ECHO_TO_SERIAL
  Serial.print("Time: ");
  Serial.print('"');
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.print('"');
  Serial.print(" ");
#endif //ECHO_TO_SERIAL
  logfile.print(CPM);
  logfile.print(", ");
  logfile.print(TOTAL_COUNT);
  logfile.print(", ");
#if ECHO_TO_SERIAL
  Serial.print("CPM: ");
  Serial.print(CPM);
  Serial.print(" ");
  Serial.print(" TOTAL_COUNT: ");
  Serial.print(TOTAL_COUNT);
  Serial.print(" ");
#endif //ECHO_TO_SERIAL
  CURRENT_AVE = (float)TOTAL_COUNT / (float)MINUTE_COUNTER;
  logfile.print(CURRENT_AVE);
  logfile.print(",");
#if ECHO_TO_SERIAL
  Serial.print(" CURRENT_AVE: ");
  Serial.print(CURRENT_AVE);
  Serial.print(" ");
#endif 
  if(FIRST_RUN == true){
    logfile.print("YES");
    logfile.print(",");
  }
  if(FIRST_RUN == false){
    logfile.print("NO");
    logfile.print(",");
  }
#if ECHO_TO_SERIAL
  if(FIRST_RUN == true){
    Serial.print(" First Run: ");
    Serial.print("YES");
    Serial.print(" ");
  }
  if(FIRST_RUN == false){
    Serial.print(" First Run: ");
    Serial.print("NO");
    Serial.print(" ");
    Serial.println(" ");
  }
#endif
  Serial.println(" ");
  logfile.println("");
  x = false;
  CPM = 0;
}
