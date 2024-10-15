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
    "ws://cobot.center:8286/pang/ws/sub?channel=c3rl3f86n88jq9lrl3hg&track=colink",
    "ws://cobot.center:8286/pang/ws/sub?channel=c3rl43o6n88jq9lrl3og&track=video"
]
)


async def websockets_handler(uri):
    async with websockets.connect(uri, ping_interval=None) as ws:
        print("Connected to", uri)
        track = uri.split("&track=", 1)[1]
        print(track)
        if track == "video":
            prev_frame_time = 0
            curr_frame_time = 0
            fps = 0
        
            while True:
                #await asyncio.sleep(0.0016)
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
