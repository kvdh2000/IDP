from bluetooth import *
import subprocess

def blueTooth():
    addr = '98:D3:33:81:1E:37'
    passkey = '1234'
    # kill any "bluetooth-agent" process that is already running
    subprocess.call("kill -9 `pidof bluetooth-agent`",shell=True)

    # Start a new "bluetooth-agent" process where XXXX is the passkey
    status = subprocess.call("bluetooth-agent " + passkey + " &",shell=True)
    try:
        client_socket = BluetoothSocket(RFCOMM)
        client_socket.connect((addr, 3))
        client_socket.send("1")
    except BluetoothError as err:
        print("Shits broken yo")
        pass
    test = "Something"
    print(test)
    client_socket.close()
    return test 