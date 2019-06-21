from pprint import pprint

from main import color_command, drive_command, marm_command, Color_Mode, Fade_Mode, Drive_Mode
from time import time, sleep


def color_cmd_dict(color_mode, fade_mode, lower_bound,
                   upper_bound, cycles_per_sec):
    return {
        "type": "color",
        "c_mode": color_mode,
        "f_mode": fade_mode,
        "l_bound": lower_bound,
        "u_bound": upper_bound,
        "c_per_sec": cycles_per_sec
    }


def drive_dict(drive_mode, amount, cycles_per_second):
    return {
        "type": "drive",
        "drive_mode": drive_mode,
        "amount": amount,
        "cycles_per_second": cycles_per_second
    }


def marm_dict(amount, cycles_per_second):
    return {
        "type": "marm",
        "amount": amount,
        "cycles_per_second": cycles_per_second
    }


dance = [
    [1, color_cmd_dict(
        Color_Mode.COMPLEMENTATRY, Fade_Mode.ABRUPT, 0.5, 0.8, 0.5
    )],
    [5, color_cmd_dict(
        Color_Mode.COMPLEMENTATRY, Fade_Mode.FADE, 0.5, 0.8, 0.5
    )],
    [5, drive_dict(Drive_Mode.CIRCLE, 0.5, 0.3)],
    [6, marm_dict(0.4, 0.1)],
]

def do_dance():
    start_time = time()
    cmd_idx = 0
    while cmd_idx < len(dance):
        while time() - start_time > dance[cmd_idx][0]:
            cmd = dance[cmd_idx][1]
            if cmd["type"] == "color":
                print("second: {} command:".format(time() - start_time))
                pprint(cmd)
                color_command(cmd["c_mode"], cmd["f_mode"] ,cmd["l_bound"],
                              cmd["u_bound"], cmd["c_per_sec"])
            elif cmd["type"] == "drive":
                drive_command(cmd["drive_mode"], cmd["amount"],
                              cmd["cycles_per_second"])
            elif cmd["type"] == "marm":
                marm_command(cmd["amount"], cmd["cycles_per_second"])
            cmd_idx += 1
            if not (cmd_idx < len(dance)):
                break
        sleep(0.5)

if __name__ == '__main__':
    do_dance()