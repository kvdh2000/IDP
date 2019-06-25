#! /usr/bin/env python

import sys, os.path
from functools import reduce

import leather
from aubio import pvoc, source, float_type
from numpy import zeros, log10, vstack
import matplotlib.pyplot as plt


def get_spectrogram(filename, samplerate=0):
    win_s = 1024  # fft window size
    mid_band_start = 13
    treb_band_start = 100
    hop_s = win_s // 2  # hop size
    fft_s = win_s // 2 + 1  # spectrum bins

    a = source(filename, samplerate, hop_s)  # source file
    if samplerate == 0: samplerate = a.samplerate
    pv = pvoc(win_s, hop_s)  # phase vocoder
    # specgram[n] is tijd specgram[][n] is freq spectrum
    specgram = zeros([0, fft_s], dtype=float_type)  # numpy array to store spectrogram

    _bands = []

    # analysis
    while True:
        samples, read = a()  # read file
        filtered = pv(samples).norm
        #specgram = vstack((specgram, filtered))  # store new norm vector
        bands = [0,0,0]
        for i in range(len(filtered)):
            if i < mid_band_start:
                bands[0] += filtered[i].item()
            elif i > treb_band_start:
                bands[2] += filtered[i].item()
            else:
                bands[1] += filtered[i].item()
        _bands.append(bands)
        if read < a.hop_size: break

    print("done calc now plotting")

    chart = leather.Chart('Line')
    #chart.add_line(cumulative_spec, x=(lambda row, index: index), y=(lambda row, index: row.item()))
    chart.add_line(_bands, x=(lambda row, index: index), y=(lambda row, index: row[0]), name="bass")
    chart.add_line(_bands, x=(lambda row, index: index), y=(lambda row, index: row[1]), name="mid")
    chart.add_line(_bands, x=(lambda row, index: index), y=(lambda row, index: row[2]), name="treble")
    chart.to_svg('charts/custom_data2.svg')




if __name__ == '__main__':
    get_spectrogram("debug.wav")