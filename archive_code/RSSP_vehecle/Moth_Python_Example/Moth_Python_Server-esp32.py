import websockets
import asyncio
import cv2
import time
from datetime import datetime

URI = "ws://cobot.center:8286/pang/ws/pub?channel=instant&name=dGVzdA==&track=video"



async def websockets_handler(uri):
    async with websockets.connect(uri, ping_interval=None) as ws:
        print("Connected to", uri)
        track = uri.split("&track=", 1)[1]
        frame_data = cv2.VideoCapture(0)
        while True:
            await asyncio.sleep(0.016)
            _, image = frame_data.read()
            binary_image = cv2.imencode(".JPEG", image)[1].tobytes()
            await ws.send(binary_image)
      
def main():
    asyncio.run(websockets_handler(URI))

if __name__ == '__main__':
    main()
