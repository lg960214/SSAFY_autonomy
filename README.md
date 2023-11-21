# MaiL : My AI Light

# 1. TEAM

### 팀 소개
![Untitled (8)](https://github.com/lg960214/SSAFY_autonomy/assets/46098797/8de63d45-70cc-4700-a902-b2e75d60000d)

# 2. Introduction

### 개요

| 팀명 | WebX |
| --- | --- |
| 프로젝트명 | MaiL (My AI Light) |
| 프로젝트 진행기간 | 2023.10.09 - 2023.11.20 |
| 개발 인원 | 6명 |

### 프로젝트 소개

Matter protocol을 활용한 On-device AI 조명 개발

### 기술 스택

<div align=center><h1>🛠️ STACKS</h1></div>

<div>
<h3>Language</h3> 
<img src="https://img.shields.io/badge/c-A8B9CC?style=for-the-badge&logo=c&logoColor=white">
<img src="https://img.shields.io/badge/c++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white">
<img src="https://img.shields.io/badge/python-3776AB?style=for-the-badge&logo=python&logoColor=white">
<br>
</div>

<div>
<h3>Backend</h3>
<img src="https://img.shields.io/badge/FastAPI-009688?style=for-the-badge&logo=FastAPI&logoColor=white">
<img src="https://img.shields.io/badge/influxDB-22ADF6?style=for-the-badge&logo=influxDB&logoColor=white">
<img src="https://img.shields.io/badge/Nginx-009639?style=for-the-badge&logo=nginx&logoColor=white">
<img src="https://img.shields.io/badge/Minio-C72E49?style=for-the-badge&logo=Minio&logoColor=white">
<br>
</div>

<div>
<h3>Config</h3>
<img src="https://img.shields.io/badge/PyPI-3775A9?style=for-the-badge&logo=PyPI&logoColor=white">
<img src="https://img.shields.io/badge/Poetry-60A5FA?style=for-the-badge&logo=Poetry&logoColor=white">
</div>

<div>
<h3>Development Environment</h3>
<img src="https://img.shields.io/badge/Amazon EC2-FF9900?style=for-the-badge&logo=Amazon EC2&logoColor=black">
<img src="https://img.shields.io/badge/ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=black">
<img src="https://img.shields.io/badge/docker-2496ED?style=for-the-badge&logo=docker&logoColor=white">
<img src="https://img.shields.io/badge/kubernetes-326CE5?style=for-the-badge&logo=kubernetes&logoColor=white">
<img src="https://img.shields.io/badge/kubeflow-326CE5?style=for-the-badge&logo=kubeflow&logoColor=white">
<br>
</div>

<div>
<h3>Tool</h3>
<img src="https://img.shields.io/badge/gitlab-FC6D26?style=for-the-badge&logo=gitlab&logoColor=white">
<img src="https://img.shields.io/badge/git-F05032?style=for-the-badge&logo=git&logoColor=white">
<img src="https://img.shields.io/badge/Visual Studio Code-007ACC?style=for-the-badge&logo=Visual Studio Code&logoColor=white">
<img src="https://img.shields.io/badge/Vim-019733?style=for-the-badge&logo=Vim&logoColor=white">
<img src="https://img.shields.io/badge/Jupyter_Notebook-F37626?style=for-the-badge&logo=Jupyter&logoColor=white">
<img src="https://img.shields.io/badge/Canva-00C4CC?style=for-the-badge&logo=Canva&logoColor=white">
<br>
</div>

<div>
<h3>Embedded System</h3>
<img src="https://img.shields.io/badge/Espressif-E7352C?style=for-the-badge&logo=espressif&logoColor=white">
<img src="https://img.shields.io/badge/SmartThings-15BFFF?style=for-the-badge&logo=SmartThings&logoColor=white">
<img src="https://img.shields.io/badge/Matter-4B5562?style=for-the-badge&logo=Matter&logoColor=white">
<img src="https://img.shields.io/badge/FreeRTOS-84c225?style=for-the-badge&logo=FreeRTOS&logoColor=white">
<br>
</div>

<div>
<h3>Artificial Intelligence</h3>
<img src="https://img.shields.io/badge/Tensorflow-FF6F00?style=for-the-badge&logo=Tensorflow&logoColor=white">
<img src="https://img.shields.io/badge/Numpy-013243?style=for-the-badge&logo=Numpy&logoColor=white">
<img src="https://img.shields.io/badge/Pandas-150458?style=for-the-badge&logo=Pandas&logoColor=white">
<img src="https://img.shields.io/badge/scikitlearn-F7931E?style=for-the-badge&logo=scikitlearn&logoColor=white">
<br>
</div>

<div>
<h3>Communication</h3>

<img src="https://img.shields.io/badge/Jira-0052CC?style=for-the-badge&logo=Jira Software&logoColor=white">
<img src="https://img.shields.io/badge/Notion-000000?style=for-the-badge&logo=Notion&logoColor=white">
<img src="https://img.shields.io/badge/Mattermost-0058CC?style=for-the-badge&logo=Mattermost&logoColor=white">
<br>
</div>


### 특장점 및 차별점

- on device AI
    - esp32와 같이 작은 SoC에 AI를 적용함으로써 기존 IoT기기에서는 AI 적용 시 서버에 많은 부담을 준다는 점을 해결
    - 실시간으로 device에서 추론하여 인터넷 연결 **의존도** 및 **지연 시간 감소**
    - AI를 위해 DRAM 영역을 줄이고 Flash memory 활용 등을 통하여 IRAM 확보 ( IRAM 사용량 96.4% → IRAM 사용량 62.5% )
- device
    - FreeRTOS를 ESP-IDF를 사용하여 직접 최적화
        - custom partition config
        - GPIO pin control
    - 메모리 자원을 적게 사용하는 새로운 프로토콜로 AI 모델 업데이트
- MLOps
    - Kubeflow를 활용하여 AI experiment를 위한 인프라 구축
    - Continuous Training 구현 및 Trigger에 의한 Pipeline 배포 자동화

### 주요 기능 & 핵심 기술

- AI 모드를 사용한 전등 밝기 조절
- 수동 모드를 사용한 전등 밝기 조절
- MLOps를 활용한 새로운 AI 학습 모델 업데이트

# 3. PROJECT

### 사용기술

| 임베디드 | 서버 | 인프라 | 인공지능 | 협업 |
| --- | --- | --- | --- | --- |
| ESP-IDF | FastAPI (API) | Docker | Tensorflow | GitLab |
| Matter | InfluxDB (Sensor data DB) | Kubernetes | numpy | JIRA |
| FreeRTOS | nginx (reverse proxy) | Kubeflow | pandas | Notion |
| Tensorflow lite<br>for Microcontroller |  | Minio | scikitlearn | Mattermost |

### API 명세서

앞에 `http://[공인IP]:[포트]` 는 공통, `test용 productionName : M16M, Nginx 포트 : 8002`



| API 이름 | 기능                                                                                                       | 경로 | 메서드 | 파라미터 | Request  | Response | 비고 |
| --- |----------------------------------------------------------------------------------------------------------| --- | --- | --- | --- | --- | --- |
| postSensorData | 센서데이터 송신.<br>상세<br><br>서버측에서는 이 데이터를 influxDB에 저장                                                        | `/upload/sensor/{productionName}`| POST | productionName | {"MM": int,<br>"DD": int,"HH": int,<br>"Min": int,<br>"Sec": int,<br>"Day": int,<br>"Illuminance": int,<br>"Manual": bool,<br>"Brightness": int,<br>"Movement": bool,<br>"On": bool} | 정상<br>return “OK”<br><br>오류 시<br>return “Not Good” | 자료형과 컬럼 명을 정확히 지켜야함 |
| getTfliteFile | 학습파일 다운로드.<br><br>상세<br>서버측에서는 minio 객체 스토리지에 결과물이 나왔나 확인 후, 존재하면 60byte씩 읽어서 전송.<br>클라이언트 측에서 여러번 요청해야함 | `/download/tflitefile/{productionName}/{order}` | GET | productionNameorder |                                                                                                          | 정상<br><br>파일 전송 시<br>return “0”+ data<br>전송 완료 시<br>return “1”<br><br>오류 시<br><br>minio에 파일이 없을시<br>return “2” | finish byte 앞에 설정<br>0 : 데이터 보내는 중<br>1 : 다 보냄<br>2 : 보낼 데이터 자체가 없음<br><br>데이터는 실행파일이므로 base64로 인코딩후 utf-8로 디코딩하여 전송 |
| getTimeFile | 시간 정보 수신 | /MaiL/gettime | GET |  |  | 정상<br><br>return int(time.time())<br><br>오류 시<br><br>return 1 | Unix timestamp값으로 리턴 |

### 아키텍처

**임베디드 & 서비스**
![Untitled (9)](https://github.com/lg960214/SSAFY_autonomy/assets/46098797/7387ed99-5115-4524-9101-82917e07c7fd)  
  
  
**서버**
![Untitled (10)](https://github.com/lg960214/SSAFY_autonomy/assets/46098797/289cd7a7-3314-421b-8e20-af33e23d1ff2)  
  
  
**MLOps**
![Untitled (11)](https://github.com/lg960214/SSAFY_autonomy/assets/46098797/eb063656-df0f-4e29-b426-941d1e29b41d)  
  
  
## 그라운드 룰

### 1. Perforce Commit Convention

|  | Category | Information | Details |
| --- | --- | --- | --- |
| ✨ | feat | 기능 구현 | 새로운 기능 추가, 수정 |
| 🐛 | bugfix | 버그 수정 | 기능 구현 후 실행 시 발견된 에러 핸들링 |
| 💚 | build | 빌드 & 배포 | 빌드 및 배포파일 생성시 |
| 🔀 | save | 임시 저장 | 로컬의 변경이 있어야 할 경우 사용 |
| 📝 | test | 테스트 코드 작성 | 테스트를 진행 및 완성에 작성 |
|  |  |  |  |

### 2. 기타 협업 관련

### 💻 회의 진행

1. 매일 오전 9시, 오후 5시 30분 2회에 걸쳐 **데일리 스크럼(Daily Scrum)**을 진행해, 개인별 당일 목표를 설정하고 진행 상황을 공유합니다.
2. 매주 금요일 오후 5시 30분에 **스프린트 세션(Sprint Session)**을 진행해 일주일간 프로젝트의 진행 상황 및 추후 진행 목표를 설정합니다.
3. 데일리 스크럼과 스프린트 세션은 팀장이 회의를 주재하고, 다른 팀원들이 돌아가며 회의록을 작성합니다.
4. 회의에 적극적으로 참여하고, 팀장의 지목에 따라 본인의 의견을 반드시 제시합니다.

### 💻 코드 리뷰

1. **코드 리뷰(Code Review)**는 점심시간을 활용해 필요한 부분만 진행합니다.
2. 서로 다른 코드 스타일을 합의한 **코딩 컨벤션(Coding Convention)**에 따라 일원화합니다.
    1. 기본 룰을 따르되 코드 리뷰 중 본인만의 괜찮은 컨벤션이 생길 경우 협의 하에 팀 공식 컨벤션으로 정하고 따릅니다.
3. 코드 리뷰는 우선순위에 따라 빠르게 진행하며, 사소한 의견을 반영할 지에 대한 부분은 코드 작성자가 선택할 수 있도록 합니다.

### 💻 코드 작성

1. 에러(Error)가 발생 시 1시간 정도는 혼자서 고민해보고, 해결이 되지 않을 경우 팀원들과 바로 공유합니다.
2. 에러를 해결하기 위해 고민한 내용 및 해결 과정은 노션에 정리하여 공유합니다.
3. 코드에 **주석(Comment)을 작성**하는 습관을 생활화하여, 다른 팀원들이 내가 작성한 코드를 이해하기 쉽도록 합니다.
4. 모르는 것이 생기면 GitLab Issue Template을 활용하여 질문과 답변을 공유합니다.

### 💻 깃 관리

1. 풀리퀘스트(Pull Request)가 있을 경우, 이를 확인했다는 의미에서 최소한 1개 이상의 의견을 남겨야 합니다.
2. 풀리퀘스트 시 의견 갈등이 생겼다면, 충분한 토론과 의견 수렴 과정을 거쳐 **다수의 의견**을 따라야 합니다.
3. 커밋(Commit)하기 전에 고칠 부분을 한 번 더 점검합니다.
4. 1가지 기능 또는 1가지 함수를 새로 만들 때마다 커밋하는 습관을 생활화합니다.
5. *커밋 메시지(Commit Message)**는 합의한 **커밋 컨벤션(Commit Convention)**에 따라 최대한 상세하게 작성합니다.
6. 깃 브랜치(Branch) 규칙에 따라 브랜치를 관리하고, 모든 작업은 올바른 브랜치에서 작업해야 합니다.



🥈 생활 수칙

### 💻 개인 일정 관리 및 연락

1. 개인 일정이 생긴 경우 **반드시 미리 다른 팀원들에게 공유**합니다.
2. **프로젝트 중간에 취업 등으로 수료하게 된 경우, 도의적 차원에서 특화 프로젝트를 합니다.**
3. 카카오톡(KakaoTalk), 디스코드(Discord), 매터모스트(Mattermost) 등을 통한 연락을 확인했을 때는, **확인했다는 의미의 답변 또는 이모지(Emoji)로 표시**합니다.
4. 매일 9시부터 6시까지는 **프로젝트에 집중**합니다. 주말과 평일 6시 이후는 자율적으로 충분히 쉬도록 합니다. 이는 프로젝트 마감 전 상황에 따라 변동될 수 있습니다.
5. 임의 지각의 경우, 모든 팀원들에게 **커피**를 돌리도록 합니다. 9시 5분전까지는 메가커피, 10분 전까지는 바나프레소, 이후는 스타벅스.


### 시연 영상

![ezgif com-resize](https://github.com/lg960214/SSAFY_autonomy/assets/46098797/9eb60a04-3fde-46aa-9a79-74765e0e99a1)



**[ 시연 기능 - model update ]**

특정 시간 ( 자정 12시 ) 에 추가된 학습 데이터로 생성된 새로운 model를 업데이트

**[ 시연 내용 ]**

어두워지면 밝아지는 모델 → 밝아지면 어두워지는 모델

**[ 시연 세부 내용 ]**

AI 모델을 서버에서 다운로드 받는 시점은 부팅 / 특정 시간 ( 자정 12시) 입니다.

시연 시 esp32를 재부팅하여 model를 업데이트 합니다.
