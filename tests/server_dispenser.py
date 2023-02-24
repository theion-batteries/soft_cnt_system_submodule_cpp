import socket

def run_server():
    s = socket.socket()
    print("Socket successfully created")
    port = 8882
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
                

                if data == b'on\r\n':
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')

                elif data == b'off\r\n':
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')

                elif data == b'vibrate\r\n':
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')

                elif data == b'dur\r\n':
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')

                elif data == b'dur?\r\n':
                    print(f"received: {data}")
                    print(f"sent: 200")
                    c.send('200'.encode('UTF-8'))

                elif data == b'dur=200\r\n':
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')

                elif data == b'freq=200\r\n':
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')

                elif data == b'freq?\r\n':
                    print(f"received: {data}")
                    print(f"sent: 200")
                    c.send('200'.encode('UTF-8'))

                elif data == b'help\r\n':    
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
