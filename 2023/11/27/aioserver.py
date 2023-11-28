import aiohttp
import aiohttp.web
import asyncio
connected = set()

async def websocket_handler(request):
    ws = aiohttp.web.WebSocketResponse()
    await ws.prepare(request)
    print("got connection")
    connected.add(ws)

    async for msg in ws:
        if msg.type == aiohttp.WSMsgType.TEXT:
            for client in connected:
                if client is not ws:
                    await client.send_str(msg.data)
        elif msg.type == aiohttp.WSMsgType.ERROR:
            print(f"WebSocket error: {ws.exception()}")
    print("connection closing")
    connected.remove(ws)
    return ws



def main():
    app = aiohttp.web.Application()
    app.router.add_route('GET', '/', websocket_handler)
    aiohttp.web.run_app(app, host='0.0.0.0', port=8080)


if __name__ == '__main__':
    main()