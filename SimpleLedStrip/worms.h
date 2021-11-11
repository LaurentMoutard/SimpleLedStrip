

const int nbOfWorms = 10;
const int nbPropertiesInWorms = 9;
typedef struct 
    {
      int R;
      int G;
      int B;
      int taille;
      int lenteur;
      int sens;
      int positionWorm;
      int cyclEnCours;
      int pauseOuWorm;
    } Worm;

Worm  allWorms[nbOfWorms];

unsigned short bigImage[PixelCount][nbOfWorms][3]; // Un tableau contenant l'état RGB des Wormations max en RGB

String typeWorm = "color";





void emptyStrip(){
   strip.ClearTo(RgbColor(0, 0, 0));

  for (uint16_t pixel = 0; pixel< strip.PixelCount(); pixel++)
  {
    for (uint16_t worm = 0; worm < nbOfWorms; worm++)
    {
      for (uint16_t rgb = 0; rgb < 3; rgb++)
      {
        bigImage[pixel][worm][rgb] = 0;
      }
    }
  }

    for (short numWorm = 0 ; numWorm <  nbOfWorms ; numWorm++){
      allWorms[numWorm].R = 0;
      allWorms[numWorm].G = 0;
      allWorms[numWorm].B = 0;
      allWorms[numWorm].taille = 0;
      allWorms[numWorm].lenteur = 0;
      allWorms[numWorm].sens = 0;
      allWorms[numWorm].positionWorm = 0;
      allWorms[numWorm].cyclEnCours = 0;
      allWorms[numWorm].pauseOuWorm = 0;
    }

}


// crée une Worm ou crée une pause
void initWorm(short numWorm) {
 
  int pauseOuWorm = random(0, 2);
  if (pauseOuWorm == 0) { //c'est une pause
    allWorms[numWorm].R = random(300, 1001); // durée de la pause en cycles
    allWorms[numWorm].G = 0;
    allWorms[numWorm].B = 0;
    allWorms[numWorm].taille = 0;
    allWorms[numWorm].lenteur = 0;
    allWorms[numWorm].sens = 0;
    allWorms[numWorm].positionWorm = 0;
    allWorms[numWorm].cyclEnCours = 0;
    allWorms[numWorm].pauseOuWorm = 0;// c'est une pause - 1 c'est un worm

//    Serial.print("Nouvelle Worm  N° : ");
//    Serial.print(numWorm);
//    Serial.print(" - Pause  : ");
//    Serial.println(allWorms[numWorm][0]);
  } else {
    short R, G, B ;
    if(typeWorm == "color"){
         R = random(100);
         G = random(100);
         B = random(100);
      
    }else{
         R = random(100);
         G = R;
         B = R;
    }

    short taille = random(5,150); //la taille d'un vers peut aller jusque 150 pixel
    short lenteur = random(1, 20); //la lenteur d'un vers peut aller jusque 20 cycles
    int sens = random(1, 3); //le sens peut être 1 ou 2
    if (sens == 1) {
      sens = -1;
    } else {
      sens = 1 ;
    }
    int positionWorm = 0;

    if (sens == 1) {
      positionWorm = 0;
    } else
    {
      positionWorm = strip.PixelCount() - 1;
    }
    allWorms[numWorm].R = R;
    allWorms[numWorm].G = G;
    allWorms[numWorm].B = B;
    allWorms[numWorm].taille = taille;
    allWorms[numWorm].lenteur = lenteur;
    allWorms[numWorm].sens = sens;
    allWorms[numWorm].positionWorm = positionWorm;
    allWorms[numWorm].cyclEnCours = 0;
    allWorms[numWorm].pauseOuWorm = pauseOuWorm; // 1 : c'est une Worm (un Worm) - 0 c'est une pause
  }

}

void initAllWorms() {
  emptyStrip();
  for (short numWorm = 0 ; numWorm <  nbOfWorms ; numWorm++)
  {
    initWorm(numWorm);
  }

//  au début les Worms démarrent n'importe où
  for (short numWorm = 0 ; numWorm <  nbOfWorms ; numWorm++)
  {
    // On place les worms un peu n'importe où dans le strip
    allWorms[numWorm].positionWorm = random(0,300);
  }
}

//remplit le tableau bigImage[numWorm] sur un cycle
void calcWorm(short numWorm) {
//  Serial.print(" - CalcWorm   : ");
//  Serial.println(numWorm);

// Si c'est une pause on l'incrémente
  if (allWorms[numWorm].pauseOuWorm == 0) {
    allWorms[numWorm].cyclEnCours ++;
    // si on a fait toute la durée de la pause on recrée un worm
    if (allWorms[numWorm].cyclEnCours>= allWorms[numWorm].R ) {
//      Serial.print(" - Fin de la pause  : ");
//      Serial.println(numWorm);
      initWorm(numWorm);
    }
    return;
  }
  // C'est un Worm et pas une pause
  short R = allWorms[numWorm].R;
  short G = allWorms[numWorm].G;
  short B = allWorms[numWorm].B;
  short taille = allWorms[numWorm].taille;
  short lenteur = allWorms[numWorm].lenteur;
  short sens = allWorms[numWorm].sens;
  short positionWorm = allWorms[numWorm].positionWorm;
//  int darkenValue = 255 / taille;
  int darkenValue = ((max(max(R,G),B))/ taille) + 1;
  short cycleEnCours = allWorms[numWorm].cyclEnCours;

  // on incrémente le cycle
  allWorms[numWorm].cyclEnCours++;

  //Serial.print(" - positionWorm  : ");
  //Serial.println(positionWorm);

  // si c'est un temps mort
  if (cycleEnCours % lenteur != 0) {
//    Serial.println(" Attente ");
    return;
  }

  if (sens == 1) {
    // Le worm est terminé
    if (positionWorm >= strip.PixelCount() + taille ) {
//      Serial.print(" - Fin du Worm  : ");
//      Serial.println(numWorm);
      initWorm(numWorm);
      return;
    }

//    Serial.print(" - positionWorm  : ");
//    Serial.println(positionWorm);
    for (short index = 0; index < strip.PixelCount() + taille; index++)// TODO regarder si le -1 est indispensable
    {
      if (index == positionWorm)
      {// Premier Pixel
        if(index < strip.PixelCount()){
          
          bigImage[index][numWorm][0] = R;
          bigImage[index][numWorm][1] = G;
          bigImage[index][numWorm][2] = B;
        }
        // Pixels de queue
        for (uint16_t pixelQueue = 1; pixelQueue <= taille; pixelQueue++)
        {
          int positionPixel = index - pixelQueue;
          int assombrissement = pixelQueue * darkenValue;
          if (positionPixel >= 0 && positionPixel < strip.PixelCount()) {
            //rouge
            if (R > assombrissement )
            {
              bigImage[positionPixel][numWorm][0] = R  - assombrissement ;
            } else {
              bigImage[positionPixel][numWorm][0] = 0 ;
            }
            //vert
            if (G > assombrissement )
            {
              bigImage[positionPixel][numWorm][1] = G  - assombrissement ;
            } else {
              bigImage[positionPixel][numWorm][1] = 0 ;
            }
            // bleu
            if (B > assombrissement )
            {
              bigImage[positionPixel][numWorm][2] = B  - assombrissement ;
            } else {
              bigImage[positionPixel][numWorm][2] = 0 ;
            }

//               Serial.print(" Pixel  : ");
//               Serial.print(positionPixel);
//               Serial.print(" - R  : ");
//               Serial.print(bigImage[positionPixel][numWorm][0]);
//               Serial.print(" - G  : ");
//               Serial.print(bigImage[positionPixel][numWorm][1]);
//               Serial.print(" - B  : ");
//               Serial.println(bigImage[positionPixel][numWorm][2]);
          }
        }
//         Serial.print(" Pixel  : ");
//         Serial.println(index);
      }
      else { // Tous les autres pixels
        if(index >= 0 && index < strip.PixelCount()){
          bigImage[index][numWorm][0] = 0;
          bigImage[index][numWorm][1] = 0;
          bigImage[index][numWorm][2] = 0;
        }
      }
    }
    positionWorm++;
    allWorms[numWorm].positionWorm = positionWorm;
  } 
  else 
  { // Le sens = -1
 
    // Le worm est terminé
    if (positionWorm < -taille ) {
//      Serial.print(" - Fin du Worm sens - 1 : ");
//      Serial.println(numWorm);
      initWorm( numWorm);
      return;
    }

 
//    Serial.print(" - positionWorm  : ");
//    Serial.println(positionWorm);

    for (short index = strip.PixelCount() - 1 ; index >= -taille ; index--)
    {
      if (index == positionWorm)
      {
        // premier Pixel
        if(index >= 0){
          
          bigImage[index][numWorm][0] = R;
          bigImage[index][numWorm][1] = G;
          bigImage[index][numWorm][2] = B;
        }
        for (uint16_t pixelQueue = 1; pixelQueue <= taille; pixelQueue++)
        {
          // Pixels de la queue
          int positionPixel = index + pixelQueue;
          int assombrissement = pixelQueue * darkenValue;

          if (positionPixel >= 0 && positionPixel < strip.PixelCount()) {
            //rouge
            if (R > assombrissement )
            {
              bigImage[positionPixel][numWorm][0] = R  - assombrissement ;
            } else {
              bigImage[positionPixel][numWorm][0] = 0 ;
            }
            //vert
            if (G > assombrissement )
            {
              bigImage[positionPixel][numWorm][1] = G  - assombrissement ;
            } else {
              bigImage[positionPixel][numWorm][1] = 0 ;
            }
            // bleu
            if (B > assombrissement )
            {
              bigImage[positionPixel][numWorm][2] = B  - assombrissement ;
            } else {
              bigImage[positionPixel][numWorm][2] = 0 ;
            }

//             Serial.print(" Pixel - : ");
//             Serial.print(positionPixel);
//             Serial.print(" - R  : ");
//             Serial.print(bigImage[positionPixel][numWorm][0]);
//             Serial.print(" - G  : ");
//             Serial.print(bigImage[positionPixel][numWorm][1]);
//             Serial.print(" - B  : ");
//             Serial.println(bigImage[positionPixel][numWorm][2]);
          }
        }
//         Serial.print(" Pixel  : ");
//         Serial.println(index);
      }
      else {
        // Tous les autres Pixels
        if(index >= 0 && index < strip.PixelCount()){
          bigImage[index][numWorm][0] = 0;
          bigImage[index][numWorm][1] = 0;
          bigImage[index][numWorm][2] = 0;
        }
      }
    }
    positionWorm--;
    allWorms[numWorm].positionWorm = positionWorm;

  }
  //    Serial.print(" - Position  : ");
  //    Serial.println(allWorms[numWorm][6]);
}

void afficheWorm(short numWorm) {
  for (uint16_t index = 0; index < strip.PixelCount(); index++)
  {
    strip.SetPixelColor(index, RgbColor(bigImage[index][numWorm][0], bigImage[index][numWorm][1], bigImage[index][numWorm][2]));
  }
  strip.Show();
}


void afficheAllWorms() {
//  int sizeAllWorm = sizeof(allWorms) / sizeof(allWorms[0]) ;
//  Serial.print("Affiche All Worms - Taille du tableau de Worms : ");
//  Serial.println(sizeAllWorm);
  int RpixelEncours = 0;
  int GpixelEncours = 0;
  int BpixelEncours = 0;
  for (uint16_t index = 0; index < strip.PixelCount(); index++)
  {
    RpixelEncours = 0;
    GpixelEncours = 0;
    BpixelEncours = 0;

    for (short numWorm = 0 ; numWorm <  nbOfWorms ; numWorm++)
    {
      RpixelEncours += bigImage[index][numWorm][0];
      if (RpixelEncours > 255 ) {
        RpixelEncours = 255;
      }
      GpixelEncours += bigImage[index][numWorm][1];
      if (GpixelEncours > 255 ) {
        GpixelEncours = 255;
      }
      BpixelEncours += bigImage[index][numWorm][2];
      if (BpixelEncours > 255 ) {
        BpixelEncours = 255;
      }
    }
//    Serial.print("RpixelEncours : ");
//    Serial.println(RpixelEncours);
//    Serial.print("GpixelEncours : ");
//    Serial.println(GpixelEncours);
//    Serial.print("BpixelEncours : ");
//    Serial.println(BpixelEncours);
    strip.SetPixelColor(index, RgbColor(RpixelEncours, GpixelEncours, BpixelEncours));
  }
  strip.Show();
//  delay(100);
}

void processWorms(){
  //La taille de la première dimension d'un tableau multidemensionnel
//  int sizeAllWorm = sizeof(allWorms) / sizeof(allWorms[0]) ;
  for (short numWorm = 0 ; numWorm <  nbOfWorms ; numWorm++)
  {
    calcWorm(numWorm);
  }
  afficheAllWorms();
//    calcWorm(0);
//    afficheWorm(0);
  
}
