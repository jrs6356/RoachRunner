import asyncio
import bleak
import struct

# Define the UUIDs for the service and characteristic
service_uuid = "19B10000-E8F2-537E-4F6C-D104768A1214"
string_characteristic_uuid = "19B10001-E8F2-537E-4F6C-D104768A1214"
float_characteristic_uuid = "19B10002-E8F2-537E-4F6C-D104768A1214"
#characteristic_callbacks = {
#    "19B10001-E8F2-537E-4F6C-D104768A1214": lambda data: handle_data1(data),
#    "19B10002-E8F2-537E-4F6C-D104768A1214": lambda data: handle_data2(data)
#}

# Define the MAC address of the Arduino GIGA R1
device_address = "a8:61:0a:28:62:73"  # Replace with the actual MAC address

async def handle_data_string(sender: int, data: bytearray):
    datR = data.decode("utf-8")
    print("Received data: ", datR)

async def handle_data_float(sender: int, data: bytearray):
    datR = struct.unpack('<f', data)[0]
    print("Received data: ", datR)

async def main():
    async with bleak.BleakClient(device_address, services=[service_uuid]) as client:
        await client.start_notify(string_characteristic_uuid, handle_data_string)
        await client.start_notify(float_characteristic_uuid, handle_data_float)
        #await asyncio.sleep(10)  # Adjust the duration as needed
        #await client.stop_notify(string_characteristic_uuid)
        while True:
            await asyncio.sleep(1)  # Keep the connection alive

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
