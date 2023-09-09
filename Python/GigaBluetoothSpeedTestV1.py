import asyncio
import struct
import bleak
import time

# Replace with the actual MAC address of the peripheral
device_address = "a8:61:0a:28:62:73"
characteristic_uuid = "19B10001-E8F2-537E-4F6C-D104768A1214"

data_size = 512  # Size of the data to receive (must match Arduino script)

received_data = bytearray()

async def handle_data(sender: int, data: bytearray):
    global received_data
    received_data += data

async def main():
    async with bleak.BleakClient(device_address) as client:
        await client.start_notify(characteristic_uuid, handle_data)

        start_time = time.time()  # Start time when data reception begins

        # Wait for the entire data to be received
        
        while len(received_data) < data_size:
            await asyncio.sleep(0.1)

        end_time = time.time()    # End time when data reception is completed

        # Calculate throughput in bytes per second
        elapsed_time = end_time - start_time
        throughput = data_size / elapsed_time

        print(f"Received data size: {len(received_data)} bytes")
        print(f"Time taken: {elapsed_time:.2f} seconds")
        print(f"Throughput: {throughput:.2f} bytes/s")

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
