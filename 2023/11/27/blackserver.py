# do pip install blacksheep
from blacksheep import Application, ws, WebSocket

async def echo(websocket):
    await websocket.accept()
    while True:
        message = await websocket.receive_text()
        await websocket.send_text(f"Echo: {message}")

app = Application()
connected = set()

@ws("/")
async def ws_handler(websocket: WebSocket):
    print("connection")
    await websocket.accept()
    connected.add(websocket)
    try:
        while True:
            message = await websocket.receive_text()
            for client in connected:
                if client is not websocket:
                    await client.send_text(message)
    finally:
        print("disconnection")
        connected.remove(websocket)

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8080)