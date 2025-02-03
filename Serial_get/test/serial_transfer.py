import serial

port = 'COM3'
baudrate = 9600

try:
    # シリアルポートを開く
    ser = serial.Serial(port, baudrate, timeout=1)
    print(f'Listening on {port} at {baudrate} baud...')

    while True:
        # データを読み取る
        data = ser.readline().decode('utf-8').strip()
        if data:
            print(f'Received: {data}')
except serial.SerialException as e:
    print(f'Serial error: {e}')
except KeyboardInterrupt:
    print('Stopped by user')
finally:
    ser.close()
