FROM alpine:latest

RUN apk add --no-cache gcc musl-dev

WORKDIR /app

COPY chat.c /app

RUN gcc -o chat chat.c -pthread

