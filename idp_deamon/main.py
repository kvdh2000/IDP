import argparse
from logging import Logging
from queue import Queue

logging = Logging("main")


def main(args):
    while True:


        


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='idp deamon')
    parser.add_argument('real', type=bool, help='enable if the deamon is running in real mode.')

    main(parser.parse_args())
    test =  Queue.get_nowait()
    Queue.
