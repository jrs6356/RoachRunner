import asyncio
import struct
import bleak

# Define the UUIDs for the service and characteristics
service_uuid = "19B10000-E8F2-537E-4F6C-D104768A1214"
string_characteristic_uuid = "19B10001-E8F2-537E-4F6C-D104768A1214"
float_characteristic_uuid = "19B10002-E8F2-537E-4F6C-D104768A1214"

# Replace with the actual MAC address of the peripheral
device_address = "a8:61:0a:28:62:73"

async def handle_string_data(sender: int, data: bytearray):
    print("Received string:", data.decode("utf-8"))

async def handle_float_data(sender: int, data: bytearray):
    decoded_float = struct.unpack("<f", data)[0]
    print("Received float:", decoded_float)

async def main():
    async with bleak.BleakClient(device_address, services=[service_uuid], timeout=10) as client:
        characteristics = client.service.characteristics
        print(client.service.description)
        for characteristic in characteristics:
            print(characteristic)

        #for characteristic in characteristics:
            #if characteristic.uuid == string_characteristic_uuid:
                #await client.start_notify(characteristic, handle_string_data)
            #elif characteristic.uuid == float_characteristic_uuid:
                #await client.start_notify(characteristic, handle_float_data)

        #while True:
            #await asyncio.sleep(1)  # Keep the connection alive

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
