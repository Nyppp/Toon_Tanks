# Toon_Tanks
언리얼 엔진으로 만든 3d 슈팅 게임

Udemy 온라인 강의
Unreal Engine C++ Developer: Learn C++ and Make Video Games 를 통해 만든 게임입니다.

## 클래스 다이어그램
### Pawn 클래스 다이어그램
![image](https://user-images.githubusercontent.com/63279872/166604864-9a17fc33-2812-4a9d-bf4a-2b596766a6bf.png)
<br>BasePawn 클래스는 Pawn클래스를 상속받아 만들어졌으며, 탱크의 몸체 구성과 발사 동작만이 구현되어 있음
<br>Tank 클래스는 플레이어가 조종하게 될 Pawn으로, 이동에 대한 Move()함수와 Turn() 함수가 존재함
<br>Tower 클래스는 Enemy 역할로, 이동의 기능은 없으며 포대를 플레이어에 조준하여 발사하는 기능만이 존재함


## 샘플 코드
### 오브젝트 구성(BasePawn.cpp)

  ![image](https://user-images.githubusercontent.com/63279872/166591001-afc4892c-d304-4300-93a5-a8238cbfc0b1.png)

- CreateDefaultSubobject를 통해 클래스 생성자에 컴포넌트를 가져와서, RootComponent에 부착시키거나 붙임
- SetupAttachment를 이용해 컴포넌트 간 부모 - 자식 관계를 만듦


### 플레이어 이동(Tank.cpp)
![image](https://user-images.githubusercontent.com/63279872/166605318-681926e7-809b-4d5a-bbd0-62cfe81deddd.png)
