import socket

def run_server():
    s = socket.socket()
    print("Socket successfully created")
    port = 8883
    s.bind(('', port))
    print("socket binded to %s" % (port))
    s.listen(5)
    print("socket is listening")

    while True:
        try:
            c, addr = s.accept()
            print('Got connection from', addr)

            while True:
                data = c.recv(5012)
                if not data:
                    # Connection lost, break out of inner loop and reconnect
                    print("Connection lost, reconnecting...")
                    c.close()
                    break
                print("dataaaaa",data)
                

                if data == b'start\r\n':
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')

                elif data == b'stop\r\n':
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')

        except KeyboardInterrupt:
            print("Program stopped by user.")
            break
        except Exception as e:
            print("Error occurred:", e)

    s.close()

if __name__ == "__main__":
    run_server()
