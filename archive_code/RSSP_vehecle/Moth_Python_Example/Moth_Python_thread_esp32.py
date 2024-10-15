import websockets
import asyncio
import cv2
import numpy as np
import pygame
import queue
from threading import Thread

class AsyncThread(Thread):
    def __init__(self):
        Thread.__init__(self)
        
    def run(self):
        asyncio.run(connect_websocket())

async def connect_websocket():
    async with websockets.connect("ws://cobot.center:8286/pang/ws/sub?channel=c3rl43o6n88jq9lrl3og&track=video", ping_interval=None) as ws:
        count = 0
        while True:
            img_binary_data = await ws.recv()
            encoded_img = np.frombuffer(img_binary_data, dtype = np.uint8)
            img = cv2.imdecode(encoded_img, cv2.IMREAD_COLOR)
            cv2.imshow("esp32cam", img)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                return
        
if __name__ == '__main__':
    at = AsyncThread()
    at.daemon = True
    at.start()
    while True:
        pass
