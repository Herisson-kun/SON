#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <string>
#include <iostream>

AudioInputI2S            i2s1;           //xy=352,135
AudioAnalyzeNoteFrequency notefreq1;      //xy=600,228
AudioOutputI2S           i2s2;           //xy=626,144
AudioConnection          patchCord1(i2s1, 0, notefreq1, 0);
AudioConnection          patchCord2(i2s1, 0, i2s2, 0);
AudioConnection          patchCord3(i2s1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=525,305

//---------------------------------------------------------------------------------------

const int redPinLow =  14;
const int greenPin =  16;
const int redPinHigh =  17;

// Définir les fréquences des notes de musique
float frequencies[] = {
  82.41, // E2
  110.00, // A2
  146.43, // D3
  196.00, // G3
  246.94, // B3
  329.63, // E4
};

String notes[] = {
  "E2",
  "A2",
  "D3",
  "G3",
  "B3",
  "E4",
};

// Calculer les distances entre les notes consécutives
float intervalsRight[6];
float intervalsLeft[6];

void calculateIntervals() {
  for (unsigned int i = 0; i < 5; i++) {
    intervalsRight[i] = (frequencies[i + 1] - frequencies[i]) / 2.0;
    if (i == 0) {
      intervalsLeft[i] = intervalsRight[i];
    }
    else {
      intervalsLeft[i] = intervalsRight[i-1];
    }
  }
  intervalsLeft[5] = intervalsRight[4];
  intervalsRight[5] = intervalsRight[4];
}

void setup() {

  AudioMemory(30);
  Serial.begin(9600);
  calculateIntervals();

  pinMode(redPinLow, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPinHigh, OUTPUT);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(30);
  delay(1000);

  notefreq1.begin(.15);//pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
    // read back fundamental frequency
    if (notefreq1.available()) {
       float note = notefreq1.read();
       float prob = notefreq1.probability();
      Serial.printf("Fréquence: %3.2f | Probability: %.2f\n", note, prob);

      // Parcourir toutes les notes de musique
      for (unsigned int i = 0; i < 6; i++) {

        // Vérifier si la fréquence détectée se situe dans l'intervalle autour de la note actuelle
        if (note >= frequencies[i] - intervalsLeft[i] && note <= frequencies[i] + intervalsRight[i]) {
          // Calculer la différence entre la fréquence détectée et la fréquence de la note actuelle
          Serial.print(frequencies[i]);
          Serial.printf(" %3.2f\n", note);
          float difference = note - frequencies[i];
          if (difference < -3) {
            Serial.println("Note d'accord: " + notesAccord[i] + " | Trop bas");
            digitalWrite(greenPin, LOW);
            digitalWrite(redPinHigh, LOW);
            digitalWrite(redPinLow, HIGH);

          }
          else if (difference > 3){
            Serial.println("Note d'accord: " + notesAccord[i] + " | Trop haut");
            digitalWrite(greenPin, LOW);
            digitalWrite(redPinHigh, HIGH);
            digitalWrite(redPinLow, LOW);
          }
          else{
            Serial.println("Note d'accord: " + notesAccord[i] + " | Parfait !");
            digitalWrite(greenPin, HIGH);
            digitalWrite(redPinHigh, LOW);
            digitalWrite(redPinLow, LOW);
          }
        }
      }
  }
}