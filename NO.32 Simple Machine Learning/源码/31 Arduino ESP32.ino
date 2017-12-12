#include <math.h>

#define B1  12
#define B2  13
#define LED 14

int X[1][2]    =   {{1,0}};
/*these matrices was calculated by python */
float W1[2][3] =   {{0.74000854,  4.47769531, -0.98692059}, 
                    {0.83034991,  4.48772758, -0.55733578}};
float W2[3][1] =   {{-6.17234487}, 
                    {4.8835918}, 
                    {1.28875386}};
float Wo1[1][3];
float sum = 0;
float Y = 0;

/*sigmoid function*/
float sigmoid (float x)
{
    return 1/(1 + exp(-x));
}


void setup()
{
  Serial.begin(115200);
  pinMode(B1, INPUT_PULLUP); 
  pinMode(B2, INPUT_PULLUP); 
  pinMode(LED, OUTPUT); 
  digitalWrite(LED, LOW);
}

void loop()
{
  X[0][0] = digitalRead(B1);
  X[0][1] = digitalRead(B2);
  printf("B1 = %d, B2 = %d\n", X[0][0], X[0][1]);
  
  /* calculate forward part based on weights */
  //hidden layer
  for(int i=0; i<1; i++)
  {
      for(int j=0;j <3; j++)
      {
          for(int k=0; k<2; k++)
          {
              sum += X[i][k]*W1[k][j];
          }
          Wo1[i][j] = sigmoid(sum);
          sum = 0;  
      }
  }
  //output layer
  for(int i=0; i<1; i++)
  {
      for(int j=0;j <1; j++)
      {
          for(int k=0; k<3; k++)
          {
              Y += Wo1[i][k]*W2[k][j];
          } 
      }
  }
  printf("Y = %f\n", (Y));
  Y = round(Y);
  digitalWrite(LED, int(Y));
  Y = 0;
  delay(1000);
}