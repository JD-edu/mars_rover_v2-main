import websockets
import asyncio
import cv2
import numpy as np
import time
import sys
from datetime import datetime
from threading import Thread
import pygame
import queue

black = (0,0,0)

connections = set()
connections.update([
    "ws://cobot.center:8286/pang/ws/sub?channel=instant&name=dGVzdA==&track=video",
    "ws://cobot.center:8286/pang/ws/sub?channel=instant&name=dGVzdA==&track=colink",
    "ws://cobot.center:8286/pang/ws/pub?channel=instant&name=dGVzdA==&track=metric"
]
)

class PygameThread(Thread):
    def __init__(self, key_que_):
        Thread.__init__(self)
        print("thread")
        pygame.init()
        self.window  = pygame.display.set_mode((640, 480))
        pygame.display.set_caption("RSSP")
        self.clock = pygame.time.Clock()
        self.key_que = key_que_
        
        

    def run(self):
        running = True
        while running:
            self.clock.tick(30)
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
            key_event = pygame.key.get_pressed()
            if key_event[pygame.K_LEFT]:
                self.key_que.put(0)
                #print(self.key_que.qsize())

            if key_event[pygame.K_RIGHT]:
                self.key_que.put(1)
                #print(self.key_que.qsize())

            if key_event[pygame.K_UP]:
                self.key_que.put(2)
                #print(self.key_que.qsize())

            if key_event[pygame.K_DOWN]:
                self.key_que.put(3)
                #print(self.key_que.qsize())

            pygame.display.update()
        pygame.quit()
            
async def websockets_handler(uri):
    async with websockets.connect(uri, ping_interval=None) as ws:
        print("Connected to", uri)
        track = uri.split("&track=", 1)[1]
        if track == "video":
            prev_frame_time = 0
            curr_frame_time = 0
            fps = 0
        
            while True:
                await asyncio.sleep(0.0016)
                img_binary_data = await ws.recv()
                encoded_img = np.frombuffer(img_binary_data, dtype=np.uint8)
            
                img = cv2.imdecode(encoded_img, cv2.IMREAD_COLOR)
                curr_frame_time = time.time()
                fps = round(1/(curr_frame_time - prev_frame_time), 2)
                cv2.putText(img, str(fps), (20, 70), cv2.FONT_HERSHEY_PLAIN, 4, (0, 255, 0), 2)
                cv2.imshow(uri, img)
                prev_frame_time = curr_frame_time
                if cv2.waitKey(1) & 0xFF == ord('q'):
                    break
            cv2.destroyAllWindows()

               
        elif track == "colink":
            while True:
                await asyncio.sleep(0.016)
                colink_data = await ws.recv()
                print(colink_data)
                
        elif track == "metric":
            count = 0
            while  True: 
                await asyncio.sleep(0.016)
                if key_queue.qsize() > 0:
                    print(key_queue.qsize())
                    metric_data = datetime.now()
                    metric_data = key_queue.get()
                    await ws.send("source: &track=" + track + ", data: " + str(metric_data))
                else:
                    count += 1
                    if count > 100:
                        await ws.send("source: &track=" + track + ", data: " + str(count))
                        count = 0
                    
                


async def connect_websocket():
    tasks = [websockets_handler(uri) for uri in connections]
    await asyncio.gather(*tasks)


def main():
    asyncio.run(connect_websocket())


if __name__ == '__main__':
    key_queue = queue.Queue()
    pt = PygameThread(key_queue)
    pt.daemon = True
    pt.start()
    main()
