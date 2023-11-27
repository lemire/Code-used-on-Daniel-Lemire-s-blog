
import asyncio
import websockets
import time

async def client1():
    async with websockets.connect('ws://localhost:8080') as websocket:
        message = 'client 1!'
        round_trips = 0
        start = time.time_ns()
        await websocket.send(message)
        while True:
            response = await websocket.recv()
            round_trips += 1
            if(round_trips % 65536 == 0):
                end = time.time_ns()
                duration = (end - start)/1000000000
                print("rate: ",round_trips/duration," round trips per second")
            await websocket.send(message)

async def client2():
    async with websockets.connect('ws://localhost:8080') as websocket:
        message = 'client 2!'
        while True:
            response = await websocket.recv()
            await websocket.send(message)

async def main():
    task1 = asyncio.create_task(client1())
    task2 = asyncio.create_task(client2())
    await asyncio.gather(task1, task2)

asyncio.run(main())