# SONIFICATION OF AIR POLLUTION DATA USING BELA

Final Assignment for ECS7012P: Music and Audio Programming


https://bela.io/

Aural comparison of hourly air pollution levels (NO2 values) on Mile End Road, London, from a week in April of 2019 and 2020 (https://www.londonair.org.uk/). 
Sonification is conducted by applying a spectral delay effect to "Air on the G String", Wilhelmj's arrangement of a Bach composition (http://www.orangefreesounds.com/air-o...).
Spectral delay is achieved by a total of 10 bandpass filters with different cutoff frequencies delayed in time by different amounts and with different gains.
Levels of NO2 are mapped to feedback and gain of every delay line, as well as to movement of cutoff frequencies.
Higher values of NO2 correspond to:
  - Higher delay feedback for each line/filter band;
  - Higher gain of delayed content for each line/filter band;
  - Lower cutoff frequencies for each line/filter band;
In summary, low pollution/NO2 levels approximate the output to a "clean" rendition of the piece, whilst high pollution/NO2 levels "pollute" it with delay. Holding a button allows to switch from the 2020 scenario (no button pressed) to the 2019 scenario (button pressed and held).

Demonstration video at: https://youtu.be/uO5if7Xgi9M

School of Electronic Engineering and Computer Science 

Queen Mary University of London  

Spring 2020
