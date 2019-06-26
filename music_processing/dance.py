import datetime
import math
import numpy as np

import pyaudio
import wave
import sys
from aubio import tempo, source, pvoc
from concurrent.futures import ThreadPoolExecutor
from time import sleep
from SerialReciever import SerialReciever
from math import floor

debug_mode = True

FORMAT = pyaudio.paInt16
CHANNELS = 2
RATE = 44100
CHUNK = 512

WAVE_OUTPUT_FILENAME = "debug.wav"
pool = ThreadPoolExecutor(1)

win_s = 1024  # fft size
hop_s = win_s // 2  # hop size

freq_per_band = 22000 / hop_s

mid_band_start = math.floor(250/freq_per_band)  # ~250 hertz
treb_band_start = math.floor(2000/freq_per_band)  # 2k hertz

get_bpm = False

serial = SerialReciever("/dev/ttyACM0", 115200)
audio = pyaudio.PyAudio()

stream = audio.open(format=FORMAT, channels=CHANNELS,
                    rate=RATE, input=True,
                    frames_per_buffer=CHUNK)

def analyze_sound(detect_bpm=True):

    RECORD_SECONDS = 0.13
    if detect_bpm:
        RECORD_SECONDS = 5




    # analysing
    o = tempo("default", win_s, hop_s, RATE)
    pv = pvoc(win_s, hop_s)  # phase vocoder voor filters

    # tempo detection delay, in samples
    # default to 4 blocks delay to catch up with
    delay = 4. * hop_s

    # list of beats, in samples
    beats = []
    bands = [0, 0, 0]

    # total number of frames read
    total_frames = 0

    frames = []

    for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
        frames.append(stream.read(CHUNK))

    total_frames = 0
    for frame in frames:
        samples = np.fromstring(frame, dtype=np.float32)
        if detect_bpm:
            # bpm detectie
            is_beat = o(samples)
            if is_beat:
                this_beat = int(total_frames - delay + is_beat[0] * hop_s)
                beats.append(this_beat / float(RATE))  # timestamps van de beats
            total_frames += CHUNK

        # filters
        filtered = pv(samples).norm
        for i in range(len(filtered)):
            if i < mid_band_start:
                bands[0] += filtered[i].item()
            elif i > treb_band_start:
                bands[2] += filtered[i].item()
            else:
                bands[1] += filtered[i].item()



    if detect_bpm:
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

    bands[1] = bands[1] * 80
    bands[2] = bands[2] * 750

    bands_total = 0
    for band in bands:
        bands_total += band
    for i in range(len(bands)):
        bands[i] = bands[i] / bands_total

    bands[1] = bands[1]*10
    ret = {"bands": bands}

    if detect_bpm:
        ret["bpm"] = 60 / avrg_bps(beats_delta)

    #print("     {}".format(math.floor((datetime.datetime.now() - start).microseconds / 1000)))

    return ret

def dance(sound_stats):
    """
    stuurt de rupsbanden, de arm en de ledjes aan
    """
    print(sound_stats)
    # als de serial traag is kan dat zijn omdat de inputbuffer vol zit
    """
    serial.send_command("{0}/{1}/{2}".format(
        math.floor(sound_stats["bands"][0] * 512),
        math.floor(sound_stats["bands"][1] * 255),
        math.floor(sound_stats["bands"][2] * 255)
    ))
    
    """


    #serial.send_command(str(floor(sound_stats/10)))

def analyze():
    global get_bpm
    get_bpm = not get_bpm
    #if get_bpm:
    return analyze_sound()
    #else:
    #    return analyze_sound(True)

def run():
    #start = datetime.datetime.now()
    recording = pool.submit(analyze)
    while not recording.done():
        sleep(0.01)
    dance(recording.result())
    #print("         {}".format((datetime.datetime.now() - start).microseconds / 1000 ))


if __name__ == '__main__':
    if debug_mode:
        run()
    else:
        while True:
            run()


