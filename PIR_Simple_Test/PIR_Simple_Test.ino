int sensor = 0;
int val;
boolean hasMotion;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(sensor);
  if(val > 500){
    hasMotion = true;
  }else{
    hasMotion = false;
  }
  Serial.println(hasMotion);
  
}
