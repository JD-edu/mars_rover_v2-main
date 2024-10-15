import websockets
import asyncio
import cv2
import time
from datetime import datetime

connections = set()
connections.update([
    #"ws://cobot.center:8286/pang/ws/pub?channel=instant&name=dGVzdA==&track=video",
    #"ws://cobot.center:8286/pang/ws/pub?channel=instant&name=dGVzdA==&track=colink",
    #"ws://cobot.center:8286/pang/ws/sub?channel=instant&name=dGVzdA==&track=metric"
    #"ws://cobot.center:8286/pang/ws/pub?channel=c3rl3f86n88jq9lrl3hg&track=metric",
    "ws://cobot.center:8286/pang/ws/pub?channel=c3rl43o6n88jq9lrl3og&track=video"
]
)


async def websockets_handler(uri):
    async with websockets.connect(uri, ping_interval=None) as ws:
        print("Connected to", uri)
        track = uri.split("&track=", 1)[1]
        if track == "video":
            frame_data = cv2.VideoCapture(0)
            #frame_data.set(3, 320)
            #frame_data.set(4, 240)
            while True:
                await asyncio.sleep(0.016)
                try:
                    _, image = frame_data.read()
                    #cv2.imshow('win',image)
                    binary_image = cv2.imencode(".JPEG", image)[1].tobytes()
                    await ws.send(binary_image)
                except:
                    pass
        elif track == "colink":
            while True:
                await asyncio.sleep(1)
                colink_data = datetime.now()
                await ws.send("source: &track=" + track + ", data: " + str(colink_data))
        elif track == "metric":
            while True:
                await asyncio.sleep(0.016)
                metric_data = await ws.recv()
                print(metric_data)


async def connect_websocket():
    tasks = [websockets_handler(uri) for uri in connections]
    await asyncio.gather(*tasks)


def main():
    asyncio.run(connect_websocket())


if __name__ == '__main__':
    main()
