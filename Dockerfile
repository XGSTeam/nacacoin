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
RUN git clone https://github.com/XGSTeam/nacacoin/nacacoin.git && \
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
