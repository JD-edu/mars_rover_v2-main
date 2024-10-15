import websockets
import asyncio
import cv2
import numpy as np
import time
from datetime import datetime

connections = set()
connections.update([
    
    #moth original 
    #"ws://cobot.center:8286/pang/ws/sub?channel=instant&name=dGVzdA==&track=video",
    #"ws://cobot.center:8286/pang/ws/sub?channel=instant&name=dGVzdA==&track=colink",
    #"ws://cobot.center:8286/pang/ws/pub?channel=instant&name=dGVzdA==&track=metric"
    "ws://cobot.center:8286/pang/ws/sub?channel=c3rl3f86n88jq9lrl3hg&track=colink&mode=bundle",
    "ws://cobot.center:8286/pang/ws/sub?channel=c3rl43o6n88jq9lrl3og&track=video"
]
)

cx = 0
cy = 0
async def websockets_handler(uri):
    global cx, cy   
    async with websockets.connect(uri, ping_interval=None) as ws:
        print("Connected to", uri)
        track = uri.split("&track=", 1)[1]
        print(track)
        if track == "video":
            while True:
                #await asyncio.sleep(0.0016)
                img_binary_data = await ws.recv()
                encoded_img = np.frombuffer(img_binary_data, dtype=np.uint8)
            
                img_ = cv2.imdecode(encoded_img, cv2.IMREAD_COLOR)
                img = cv2.rotate(img_, cv2.ROTATE_90_COUNTERCLOCKWISE)
                cv2.imshow(uri, img)
                img_cvt = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
                img_mask1 = cv2.inRange(img_cvt, np.array([0, 100, 100]), np.array([20, 255, 255]))
                img_mask2 = cv2.inRange(img_cvt, np.array([160, 100, 100]), np.array([180, 255, 255]))
                img_mask = img_mask1 + img_mask2
                cont_list, hierachy = cv2.findContours(img_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
                try:
                    c = max(cont_list, key=cv2.contourArea)
                    M = cv2.moments(c)
                    cx = int(M['m10']/M['m00'])
                    cy = int(M['m01']/M['m00'])
                    img_con = cv2.drawContours(img, c, -1, (0,0,255), 1)
                except:
                    pass
                
                
                image = cv2.circle(img, (cx,cy), 10, (0,255,0), -1)
                cv2.imshow('mask', image)
                if cv2.waitKey(1) & 0xFF == ord('q'):
                    break
            cv2.destroyAllWindows()
               
        elif track == "colink&mode=bundle":
            while True:
                await asyncio.sleep(0.2)
                #colink_data = await ws.recv()
                if cx > 160:
                    #await ws.send("source: &track=" + track + ", data: AU")
                    await ws.send('AU')
                    print('left')
                elif cx < 140:
                    #await ws.send("source: &track=" + track + ", data: STOP")
                    await ws.send('STOP')
                    print('right')

                #rint(colink_data)
        elif track == "metric":
            while True:
                await asyncio.sleep(1)
                metric_data = datetime.now()
                await ws.send("source: &track=" + track + ", data: " + str(metric_data))


async def connect_websocket():
    tasks = [websockets_handler(uri) for uri in connections]
    await asyncio.gather(*tasks)


def main():
    asyncio.run(connect_websocket())


if __name__ == '__main__':
    main()
