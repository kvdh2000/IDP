from enum import Enum


class Color_Mode(Enum):
    SINGLE = 0  # alles 1 kleur
    COMPLEMENTATRY = 1  # twee kleuren tengenover elkaar
    ANALAGOUS = 2  # drie  kleuren naast elkaar
    TRIAD = 3  #  drie kleuren in een driehoek
    SPLIT_COMPLEMENTARY = 4  #  1 aan ene kan 2 aan andere kant


class Fade_Mode(Enum):
    ABRUPT = 0
    FADE = 1

class Drive_Mode(Enum):
    STRAIGHT = 0
    CIRCLE = 1


def color_command(color_mode, fade_mode, lower_bound,
                  upper_bound, cycles_per_sec):
    """
    :param color_mode:
    :param fade_mode:
    :param lower_bound:
    :param upper_bound:
    :param cycles_per_sec:
    :return:
    """
    send_command("color cmd")

def set_colors(hsvs):
    print(hsvs)


def marm_command(amount, cycles_per_second):
    send_command("cycle cmd")


def drive_command(drive_mode, amount, cycles_per_second):
    send_command("drive cmd")


def send_command(command):
    # verstuurd commando over de serial.
    print(command)


if __name__ == '__main__':
    pass
