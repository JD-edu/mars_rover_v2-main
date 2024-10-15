# Moth_Python_Example

Moth Server를 이용하여 Video, Colink, Metric 데이터를 Python Server와 Client를 이용해 송수신하는 예제입니다. 

이 예제에서 Video 데이터는 동작 환경의 기본 카메라 데이터, Colink와 Metric은 1초 간격의 현재 시각입니다.

두 스크립트를 독립된 환경에서 실행하여 데이터 송수신을 확인할 수 있습니다.

## Dependency

```python
websockets
asyncio
python-opencv
numpy
```

## ToDo

### Moth URI 변경

Moth URI는 각 스크립트의 다음 코드에서 수정이 가능합니다. 자세한 사항은 Moth SDK 개발자 가이드에서 확인할 수 있습니다.

```python
connections.update([
    "ws://cobot.center:8286/pang/ws/pub?channel=instant&name=dGVzdA==&track=video",
    "ws://cobot.center:8286/pang/ws/pub?channel=instant&name=dGVzdA==&track=colink",
    "ws://cobot.center:8286/pang/ws/sub?channel=instant&name=dGVzdA==&track=metric"
]
)
```

## Support

Python 3.7 and up 

