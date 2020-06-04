# Sonification of Air Pollution Data Using Bela

Final Assignment for ECS7012P: Music and Audio Programming


https://bela.io/


Recent reports point towards a positive impact in air pollution levels due to the lockdown policies related with COVID-19 (https://www.eea.europa.eu/themes/air/air-quality-and-covid19). Thus, one of the motivations for this assignment was to inspect if information about such impacts could be conveyed by leveraging the audio modality. However, it is important to notice that causal relations between lockdown and air pollution quality are not possible to assess within the scope of this assignment, as it represents solely an auditory depiction of data, which values, differences and variations might have occurred due other external factors. 

Specifically, this project entails an aural comparison of hourly air pollution levels (NO2 values) on Mile End Road, London, from a week in April of 2019 and 2020 (https://www.londonair.org.uk/). 
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
