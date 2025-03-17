FROM ubuntu:22.04


WORKDIR /app


RUN apt-get update 
RUN apt-get install -y gcc
RUN apt-get install -y python3-pip
RUN apt-get install -y unzip
RUN pip3 install kaggle


RUN mkdir -p /root/.kaggle
RUN echo '{"username":"reynandriel","key":"2ad7ef98289c27683b38802b7ad03d71"}' > kaggle.json 
RUN mv kaggle.json /root/.kaggle/
RUN chmod 600 /root/.kaggle/kaggle.json

COPY ayamgoreng.c /app/

RUN gcc -o ayamgoreng ayamgoreng.c


CMD ["./ayamgoreng"]
