import numpy as np
import sounddevice as sd

def audio_load():
    # Set the sample rate and duration.
    sample_rate = 44100
    duration = 5

    # Generate the time array.
    time = np.arange(0, duration, 1/sample_rate)

    # Initialize an empty array to store the melody.
    melody = np.zeros_like(time)

    # Define the frequencies for the melody.
    frequencies = [261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25]

    # Generate the melody.
    for i, freq in enumerate(frequencies):
        melody += np.sin(2 * np.pi * freq * time * (i + 1))

    # Normalize the melody to prevent clipping.
    melody /= np.max(np.abs(melody))

    # Play the melody.
    sd.play(melody, sample_rate)
    sd.wait()

audio_load()
