FROM rikorose/gcc-cmake

WORKDIR /app

COPY . .

RUN apt update
RUN apt install -y rpm iproute2
RUN cd server/ && ./build.sh

EXPOSE 4242

WORKDIR /app/server/build/Release

CMD ["./r-type_server"]
