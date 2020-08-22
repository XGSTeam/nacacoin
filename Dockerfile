FROM ubuntu:bionic

LABEL      name="NACA coin build" \
           maintainer="XGSTeam <info@nacacoin.org>"

RUN apt-get update && \
		apt-get -y install build-essential && \
		apt-get -y install libdb++-dev && \
		apt-get -y install libboost-all-dev && \
		apt-get -y install libevent-dev && \
		apt-get -y install libqt5gui5 libqt5core5a libqt5dbus5 \
    	qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler && \
    apt-get -y install libqrencode3 libqrencode-dev && \
    apt-get -y  install git vim autoconf f2c && \
    apt-get -y install bsdmainutils libssl-dev libtool pkg-config

ARG	GIT_USER_NAME
ARG GIT_PASSWD
ENV GIT_USER_NAME=${GIT_USER_NAME}
ENV GIT_PASSWD=${GIT_PASSWD}
RUN git clone https://${GIT_USER_NAME}:${GIT_PASSWD}@bitbucket.org/xgsteam/nacacoin.git && \
		cd nacacoin && \
		sh autogen.sh && \
		./configure --with-incompatible-bdb --with-gui=yes

RUN	cd nacacoin/src && \
		make && \
		#make check && \
		make install && \
		cd ../.. && \
		rm -rf nacacoin

EXPOSE 19777 19776 9777 9776 19775
ENV QT_QPA_PLATFORM=offscreen


CMD nacacoin-qt -testnet


# Command to build the image:
# docker build -t nacacoin:latest --build-arg GIT_USER_NAME=${GIT_USER_NAME} --build-arg GIT_PASSWD=${GIT_PASSWD} .

# Save the docker image:
# docker save nacacoin:latest | gzip > nacacoin_latest.tar.gz

# Load the docker image on the host machine:
# ubuntu@ec2-18-216-130-161.us-east-2.compute.amazonaws.com

# IP=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
# xhost + $IP
#docker run -it  -e DISPLAY=$IP:0 -v /tmp/.X11-unix:/tmp/.X11-unix nacacoinqt
