# do:
# pip install sanic
import sanic

app = sanic.Sanic(__name__)

connected = set()

@app.websocket('/')
async def sendToOthers(request, ws):
    connected.add(ws)
    try:
        while True:
            message = await ws.recv()
            for client in connected:
                if client is not ws:
                    await client.send(message)
    finally:
        connected.remove(ws)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)