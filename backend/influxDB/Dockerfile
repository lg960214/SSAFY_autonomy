#FROM python:3.8.10 as python-stage

# Python 애플리케이션 빌드 및 설정
# 이 부분에 Python 애플리케이션의 빌드 및 설정 명령을 추가하세요.
#RUN pip install influxdb-client


# 두 번째 스테이지: InfluxDB 2.7 이미지 기반 이미지
FROM influxdb:2.7

# InfluxDB 설정
# 이 부분에 InfluxDB 설정 및 데이터베이스 생성 명령을 추가하세요.

ENV DOCKER_INFLUXDB_INIT_MODE=setup
ENV DOCKER_INFLUXDB_INIT_USERNAME=PANZER
ENV DOCKER_INFLUXDB_INIT_PASSWORD=Webxa305#
ENV DOCKER_INFLUXDB_INIT_ORG=Webx
ENV DOCKER_INFLUXDB_INIT_BUCKET=MaiL
ENV DOCKER_INFLUXDB_INIT_ADMIN_TOKEN=8A80B1097C150034D
ENV DOCKER_INFLUXDB_INIT_RETENTION=1w
# 최종 이미지: Python 3.8.10 위에 InfluxDB 2.7 이미지 복사
#FROM python-stage as final

# 필요한 파일 또는 설정 복사
# 이 부분에 Python 애플리케이션과 InfluxDB에서 필요한 파일을 복사하는 명령을 추가하세요.

# COPY . /code/influxdb2
# 컨테이너 실행 명령 (예시)
#CMD ["python", "DBstart.py"]
