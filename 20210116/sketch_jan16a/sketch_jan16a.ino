void setup() {
  Serial.begin(9600);
}

void loop() {
  for(int i=0; i<=255; i++)
  {
    analogWrite(9, i);
    Serial.println(i);
    delay(1);
    }
    
    for(int i=255; i>=0; i--)
    {
      analogWrite(9, i);
      Serial.println(i);
      delay(1);
    }
    delay(500); // 500ms = 0.5sec
}
