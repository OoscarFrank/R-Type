#!/bin/bash

imageTag=rtype:server
containerName=rtype-server

if [[ $1 == "-h" ]]; then
    echo -e "-b\tbuild the image"
    echo -e "-rm\tremove the running container"
    echo -e "-r\trun the container"
    exit 0
fi

if [[ $1 == "-rm" ]]; then
    docker stop $containerName
    docker rm $containerName
fi

if [[ $1 == "-b" ]]; then
    docker build -t $imageTag .
fi

if [[ $1 == "-r" ]]; then
    docker run --name="$containerName" -p 4242:4242/udp -d  $imageTag
fi

if [[ $1 == "" ]]; then
    echo "No options specified. Use -h for help"
fi
