import pyaudio
import wave
import sys
from aubio import tempo, source
import time

FORMAT = pyaudio.paInt16
CHANNELS = 2
RATE = 44100
CHUNK = 1024
RECORD_SECONDS = 5
WAVE_OUTPUT_FILENAME = "temp.wav"


from concurrent.futures import ThreadPoolExecutor
from time import sleep

def return_after_5_secs(message):
    sleep(5)
    return message

pool = ThreadPoolExecutor(3)

future = pool.submit(return_after_5_secs, ("hello"))
print(future.done())
sleep(5)
print(future.done())
print(future.result())

#138
def get_bpm():
    audio = pyaudio.PyAudio()
    # recording
    stream = audio.open(format=FORMAT, channels=CHANNELS,
                        rate=RATE, input=True,
                        frames_per_buffer=CHUNK)
    print("recording...")
    frames = []

    for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
        data = stream.read(CHUNK)
        frames.append(data)
    print("finished recording")

    # stop Recording
    stream.stop_stream()
    stream.close()
    audio.terminate()

    waveFile = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
    waveFile.setnchannels(CHANNELS)
    waveFile.setsampwidth(audio.get_sample_size(FORMAT))
    waveFile.setframerate(RATE)
    waveFile.writeframes(b''.join(frames))
    waveFile.close()

    # analysing
    win_s = 512  # fft size
    hop_s = win_s // 2  # hop size

    samplerate = 0
    if len(sys.argv) > 2: samplerate = int(sys.argv[2])

    s = source(WAVE_OUTPUT_FILENAME, samplerate, hop_s)
    samplerate = s.samplerate
    o = tempo("default", win_s, hop_s, samplerate)

    # tempo detection delay, in samples
    # default to 4 blocks delay to catch up with
    delay = 4. * hop_s

    # list of beats, in samples
    beats = []

    # total number of frames read
    total_frames = 0
    while True:
        samples, read = s()
        is_beat = o(samples)
        if is_beat:
            this_beat = int(total_frames - delay + is_beat[0] * hop_s)
            beats.append(this_beat / float(samplerate))  # timestamps van de beats
        total_frames += read
        if read < hop_s: break

    beats_delta = []
    lastbeat = 0.0

    for beat in beats:
        beats_delta.append(beat - lastbeat)
        lastbeat = beat

    def avrg_bps(deltas):
        beats_delta_sum = 0
        for delta in deltas:
            beats_delta_sum += delta
        return beats_delta_sum / len(deltas)

    print(60 / avrg_bps(beats_delta))

if __name__ == '__main__':
    while True:
        get_bpm()


