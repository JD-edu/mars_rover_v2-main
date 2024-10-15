import websockets
import asyncio
import cv2
import numpy as np
import pygame
import queue
from threading import Thread

black = (0,0,0)

class PygameThread(Thread):
    def __init__(self, key_que_):
        Thread.__init__(self)
        print("thread")
        pygame.init()
        self.window  = pygame.display.set_mode((640, 480))
        pygame.display.set_caption("RSSP")
        self.clock = pygame.time.Clock()
        self.key_que = key_que_
        self.gamepads = []
        for a in range(0, pygame.joystick.get_count()):
            print( pygame.joystick.get_count())
            self.gamepads.append(pygame.joystick.Joystick(a))
            self.gamepads[-1].init()
            print(self.gamepads[-1].get_name())

    def run(self):
        running = True
        while running:
            self.clock.tick(10)
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
            if event.type == pygame.JOYBUTTONDOWN:
                    print(event.button)

            if event.type == pygame.JOYAXISMOTION:
                if key_queue.qsize() > 2:
                    with key_queue.mutex:
                        key_queue.queue.clear()
                else:
                    x1 = int(self.gamepads[-1].get_axis(0)*100)
                    x2 = int(self.gamepads[-1].get_axis(1)*100)
                    x3 = int(self.gamepads[-1].get_axis(2)*100)
                    x4 = int(self.gamepads[-1].get_axis(3)*100)
                    x_all = 'a'+str(x1)+'b'+str(x2)+'c'+str(x3)+'d'+str(x4)+'e' 
                    self.key_que.put(x_all)

            key_event = pygame.key.get_pressed()
            if key_queue.qsize() > 3:
                with key_queue.mutex:
                    key_queue.queue.clear()
            else:
                if key_event[pygame.K_LEFT]:
                    self.key_que.put('f0')
                    #print(self.key_que.qsize())

                if key_event[pygame.K_RIGHT]:
                    self.key_que.put('f1')
                    #print(self.key_que.qsize())

                if key_event[pygame.K_UP]:
                    self.key_que.put('f2')
                    #print(self.key_que.qsize())

                if key_event[pygame.K_DOWN]:
                    self.key_que.put('f3')
                    #print(self.key_que.qsize())

            pygame.display.update()
        pygame.quit()

async def connect_websocket():
    async with websockets.connect("ws://cobot.center:8286/pang/ws/sub?channel=instant&name=dGVzdA==&track=<label>", ping_interval=None) as ws:
        count = 0
        while True:
            img_binary_data = await ws.recv()
            encoded_img = np.frombuffer(img_binary_data, dtype = np.uint8)
            img = cv2.imdecode(encoded_img, cv2.IMREAD_COLOR)
            cv2.imshow("esp32cam", img)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                return
            if key_queue.qsize() > 0:
                print(key_queue.qsize())
                metric_data = key_queue.get()
                await ws.send(str(metric_data))
            else:
                count += 1
                if count > 50:
                    await ws.send(str(count))
                    count = 0
    

def main():
    asyncio.run(connect_websocket())

if __name__ == '__main__':
    key_queue = queue.Queue()
    pt = PygameThread(key_queue)
    pt.daemon = True
    pt.start()
    main()

