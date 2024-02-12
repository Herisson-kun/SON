// FFT Test
//
// Compute a 1024 point Fast Fourier Transform (spectrum analysis)
// on audio connected to the Left Line-In pin.  By changing code,
// a synthetic sine wave can be input instead.
//
// The first 40 (of 512) frequency analysis bins are printed to
// the Arduino Serial Monitor.  Viewing the raw data can help you
// understand how the FFT works and what results to expect when
// using the data to control LEDs, motors, or other fun things!
//
// This example code is in the public domain.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//const int myInput = AUDIO_INPUT_LINEIN;
const int myInput = AUDIO_INPUT_MIC;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioInputI2S          audioInput;         // audio shield: mic or line-in
AudioSynthWaveformSine sinewave;
AudioAnalyzeFFT1024    myFFT;
AudioOutputI2S         audioOutput;        // audio shield: headphones & line-out

// Connect either the live input or synthesized sine wave
AudioConnection patchCord1(audioInput, 0, myFFT, 0);
//AudioConnection patchCord1(sinewave, 0, myFFT, 0);

AudioControlSGTL5000 audioShield;

// Définir les fréquences des notes de musique
float frequencies[] = {
  261.63, // Do
  293.66, // Ré
  329.63, // Mi
  349.23, // Fa
  392.00, // Sol
  440.00, // La
  493.88  // Si
};

// Calculer les distances entre les notes consécutives
float intervals[sizeof(frequencies) / sizeof(frequencies[0]) - 1];
void calculateIntervals() {
  for (unsigned int i = 0; i < sizeof(intervals) / sizeof(intervals[0]); i++) {
    intervals[i] = (frequencies[i + 1] - frequencies[i]) / 2.0;
  }
}

void setup() {
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  calculateIntervals();
  AudioMemory(12);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);

  myFFT.windowFunction(AudioWindowHanning1024);
}

void loop() {
  
  if (myFFT.available()) {
    if (myFFT.available()) {
    // Chaque fois que de nouvelles données FFT sont disponibles
    // Parcourir toutes les valeurs de la FFT pour trouver la valeur maximale
    float maxFFT = 0.0; // Initialisation de la valeur maximale de la FFT
    int indexMAX = 0;    // Initialisation de l'indice correspondant à la valeur maximale
    for (int i = 0; i < 40; i++) {
      float n = myFFT.read(i);
      if (n > maxFFT) {
        maxFFT = n; // Mettre à jour la valeur maximale si une nouvelle valeur plus grande est trouvée
        indexMAX = i; // Mettre à jour l'indice correspondant à la valeur maximale
      }
    }

    // Afficher la valeur maximale de la FFT et l'indice correspondant
    Serial.print("Max FFT: ");
    Serial.print(maxFFT);
    Serial.print(" at index: ");
    Serial.println(indexMAX);
  }

    // Initialiser les variables pour la note la plus proche et la différence minimale
    int nearestNoteIndex = -1;
    float minDifference = 9999.0;

    // Parcourir toutes les notes de musique
    for (unsigned int i = 0; i < sizeof(frequencies) / sizeof(frequencies[0]); i++) {
      // Calculer le rayon de l'intervalle autour de la note actuelle
      float leftRadius = i > 0 ? intervals[i - 1] : intervals[0];
      float rightRadius = i < sizeof(intervals) / sizeof(intervals[0]) - 1 ? intervals[i] : intervals[sizeof(intervals) / sizeof(intervals[0]) - 2];

      // Vérifier si la fréquence détectée se situe dans l'intervalle autour de la note actuelle
      if (detectedFrequency >= frequencies[i] - leftRadius && detectedFrequency <= frequencies[i] + rightRadius) {
        // Calculer la différence entre la fréquence détectée et la fréquence de la note actuelle
        float difference = abs(detectedFrequency - frequencies[i]);
        // Si la différence est plus petite que la différence minimale actuelle, mettre à jour la note la plus proche
        if (difference < minDifference) {
          minDifference = difference;
          nearestNoteIndex = i;
        }
      }
    }

    // Afficher la note la plus proche détectée
    if (nearestNoteIndex != -1) {
      Serial.print("Nearest note: ");
      Serial.println(nearestNoteIndex); // Vous pouvez afficher la fréquence ou le nom de la note ici si nécessaire
    } else {
      Serial.println("No note detected within the specified interval.");
    }
  }
  delay(100);
}

