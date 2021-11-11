// ESP32 Dev Module
#include <NeoPixelBus.h>

const uint16_t PixelCount = 300; // make sure to set this to the number of pixels in your strip
const uint16_t PixelPin = 12;  // make sure to set this to the correct pin, ignored for Esp8266

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

#include "worms.h"

int progEnCours = 1;
int newProg = 1;

int switchPIN = 15;
int switchStatus;
int switchStatusLast = LOW;


void SetRandomSeed()
{
  uint32_t seed;

  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  delay(1);

  for (int shifts = 3; shifts < 31; shifts += 3)
  {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }

  // Serial.println(seed);
  randomSeed(seed);
}


void setup()
{
  Serial.begin(115200);
  strip.Begin();
  strip.Show();

  pinMode(switchPIN, INPUT);

  SetRandomSeed();
}


void loop()
{
  switchStatus = digitalRead(switchPIN);   
//  Serial.print("switchStatus : ");
//  Serial.println(switchStatus);
  if (switchStatus == HIGH)  // if status of button has changed
    {
      newProg += 1;
      delay(500);
    }

 
  // Si on a appuyé sur le bouton
  if(progEnCours != newProg){
    switch (newProg) {
        case 1:
          break;
        case 2:
          break;
        case 3:
          break;
        case 4:
          break;
        case 5: 
          typeWorm = "color";
          initAllWorms();
          break;
        case 6: 
          typeWorm = "white";
          initAllWorms();
          break;
        case 7: 
          initBlinkColor();
          Serial.printf("InitProcess ! %i \n", newProg);
          break;
        case 8:
          initBlinkWhite();
          Serial.printf("InitProcess ! %i \n", newProg);
          break;
    }
    
    progEnCours = newProg;

    if (progEnCours >  8){
      newProg = 1;
      progEnCours = 1;
    }
    Serial.printf("Programme en cours : %i \n", progEnCours);
  }


  switch (progEnCours) {
      case 1:
        fixLight(50);
        break;
      case 2:
        fixLight(100);
        break;
      case 3:
        fixLight(150);
        break;
      case 4:
        fixLight(200);
        break;
      case 5: 
        processWorms();
        break;
      case 6: 
        processWorms();
        break;
      case 7: 
        processBlinkColor();
        //Serial.printf("InitProcess ! %i \n", newProg);
        break;
      case 8:
        processBlinkWhite();
        //Serial.printf("InitProcess ! %i \n", newProg);
        break;
  }

}

void initBlinkColor(){
  strip.ClearTo(RgbColor(0, 0, 0));
  short R, G, B ;
  for (uint16_t index = 0; index < strip.PixelCount(); index++)
  {
      R = random(100);
      G = random(100);
      B = random(100);
      strip.SetPixelColor(index, RgbColor(R, G, B));
  }
  strip.Show();
}

void processBlinkColor(){

  short R, G, B ;
  RgbColor rgb;
  SetRandomSeed();
  for (uint16_t index = 0; index < strip.PixelCount(); index++)
  {
    rgb = strip.GetPixelColor(index);
//    Serial.print(rgb.R);
//    Serial.print("->");
    R = doEvol(rgb.R);
//    Serial.println(R);
    G = doEvol(rgb.G);
    B = doEvol(rgb.B);
//    lumTotale = lumTotale + R + G + B;
    strip.SetPixelColor(index, RgbColor(R, G, B));
  }
  delay(random(100, 1000));
  strip.Show();
//  Serial.printf("lumTotale ! %i \n", lumTotale);

}

int getOneOrMinusOne(){
    int value = random(1, 3); //le sens peut être 1 ou 2
    if (value == 1) {
      value = -1;
    } else {
      value = 1 ;
    }
    return value;
}

int doEvol(int compColor){
  int oneOrMo, facteur, pixelChange;
  int maxEvol = 20;
  oneOrMo = getOneOrMinusOne();
//  Serial.print(oneOrMo);
//  Serial.print("*");
  facteur = random(1,maxEvol);
//  Serial.print(facteur);
//  Serial.print(" = ");
  pixelChange =  oneOrMo * facteur ;
//  Serial.print(pixelChange);
//  Serial.print("->");

  if (compColor + pixelChange < 0){
    compColor = 0;
  }else if (compColor + pixelChange > 125){
    compColor = 125;
  }else{
    compColor = compColor + pixelChange;
  }
  return compColor;
}


void initBlinkWhite(){
  strip.ClearTo(RgbColor(0, 0, 0));
  short R, G, B ;
  for (uint16_t index = 0; index < strip.PixelCount(); index++)
  {
      R = random(30);
//  Serial.print("R ");
//  Serial.print(R);
      G = R;
//  Serial.print(" G ");
//  Serial.print(G);
      B = R;
//  Serial.print(" B ");
//  Serial.println(B);
      strip.SetPixelColor(index, RgbColor(R, G, B));
  }
  strip.Show();
}

void processBlinkWhite(){

  short R, G, B ;
  RgbColor rgb;
  int evol;
  for (uint16_t index = 0; index < strip.PixelCount(); index++)
  {
    rgb = strip.GetPixelColor(index);
    evol = doEvol(rgb.R)/2;
    R = R + evol;
    G = R;
    B = R;
    strip.SetPixelColor(index, RgbColor(R, G, B));
  }
  delay(random(100, 1000));
  strip.Show();
}

void fixLight(int intensite){
  strip.ClearTo(RgbColor(intensite, intensite, intensite));
  strip.Show();
}
