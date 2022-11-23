void setup() 
{
    // put your setup code here, to run once:
  Serial.begin(115200);  

  pinMode(2, OUTPUT);


  




}

int pixelToLight=0;

void loop() {
  // put your main code here, to run repeatedly:


  int delay=26;


pixelToLight++;

  digitalWrite(2,1);


  usleep(500);

  digitalWrite(2,0);
  usleep(delay*2);

  digitalWrite(2,1);
  usleep(delay);

  digitalWrite(2,0);
  usleep(delay);

  digitalWrite(2,1);
  usleep(delay);

  digitalWrite(2,0);
  usleep(delay*2);

  digitalWrite(2,1);
  usleep(delay);


  digitalWrite(2,0);
  usleep(delay);

  uint8_t checkSum = 74;



  for (int i=0;i<72;i++)
  {

  uint8_t dataByte=0b00000000;

if (pixelToLight/8==i)
{
dataByte=0b00000001 << (pixelToLight%8);
}
     
  

  checkSum += dataByte ;

  digitalWrite(2,1);
  usleep(32);

  digitalWrite(2,0);
  usleep(delay);

  // 8 bits follow
  for (int x=0;x<8;x++)
  {
    int bit = (dataByte >> x) << 7 & 0b10000000 ;
 


    digitalWrite(2,bit);
    usleep(delay);
  }
  }

  digitalWrite(2,1);
  usleep(32);

  digitalWrite(2,0);
  usleep(delay);

  // checksum begins here 

  for (int x=0;x<8;x++)
  {
    int bit = (checkSum >> x) << 7 & 0b10000000 ;

    digitalWrite(2,bit);
    usleep(delay);
  }

// checksum Over
  digitalWrite(2,1);
  usleep(300000);

}
