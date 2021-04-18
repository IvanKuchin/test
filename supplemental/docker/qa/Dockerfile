FROM ubuntu:xenial as libwebsocket

RUN apt update && apt install --no-install-recommends --no-install-suggests -y \
	    build-essential \
	    ca-certificates \
	    cmake \
	    git \
	    libssl-dev \
	    pkg-config \

	&& git clone -b v2.2.0 --depth 1 https://github.com/warmcat/libwebsockets.git \
	&& cd /libwebsockets \
	&& mkdir build \
	&& cd build \
	&& cmake -DLWS_WITHOUT_DAEMONIZE:BOOL=FALSE -DLWS_WITH_STATIC=1 -DLWS_WITH_SHARED=1 -DCMAKE_INSTALL_PREFIX:PATH=/usr/local/share/libwebsockets/ .. \
	&& make install \

	&& apt remove --purge -y \
	    build-essential \
	    cmake \
	    git \
	&& apt autoremove -y \
	&& apt clean


############################################################################################


FROM ubuntu:xenial

ARG DB_NAME
ARG DB_LOGIN
ARG DB_PASSWORD
ARG DB_HOST

LABEL maintainer="ivan.kuchin@gmail.com"
LABEL org.opencontainers.image.source="https://github.com/IvanKuchin/__BUILD_REPO__"

# following two commands suppose to be reused from cache
RUN apt update &&  apt install --no-install-recommends --no-install-suggests -y git ca-certificates

##########################
# copy from libwebsocket #
##########################
COPY --from=libwebsocket /usr/local/share/libwebsockets/ /usr/local/share/libwebsockets/

RUN git clone --depth 1 https://github.com/IvanKuchin/__BUILD_REPO__.git /backend
WORKDIR /backend
RUN git clone --depth 1 https://github.com/IvanKuchin/test_pi.git ./src/pi

RUN apt-get install --no-install-recommends --no-install-suggests -y software-properties-common
RUN add-apt-repository -y ppa:jonathonf/ffmpeg-4
RUN apt update
RUN apt-get install --no-install-recommends --no-install-suggests -y \ 
        build-essential \
        cmake \
        ffmpeg \
        graphicsmagick-libmagick-dev-compat \
        libavcodec-dev \
        libavformat-dev \
        libavutil-dev \
        libcurl4-openssl-dev \
        libgraphicsmagick++-dev \
        libhpdf-dev \
        libmaxminddb-dev \
        libmysqlclient-dev \
        libssl-dev \
        libwebp-dev \
        libarchive-dev \
        rapidjson-dev \
        pkg-config

#####################
# locale generation #
#####################
RUN apt-get install --no-install-recommends --no-install-suggests -y locales
RUN locale-gen ru_RU.utf8
RUN locale-gen en_US.utf8
RUN update-locale
RUN export LC_ALL=en_US.utf8

#############################
# libwkhtmltox installation #
#############################
RUN apt-get install --no-install-recommends --no-install-suggests -y wget
RUN wget https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.trusty_amd64.deb
RUN dpkg -i  wkhtmltox_0.12.5-1.trusty_amd64.deb || true     # trick to force dpkg return successfull return code
RUN apt install -f --no-install-recommends --no-install-suggests -y

##################
# save commit id #
##################
RUN git rev-parse HEAD > ./supplemental/git/git_commit_id

##################### 
# build from source #
##################### 
RUN mkdir build
RUN cd build && cmake .. && make -j2 && make install

###########################
# test connectivity to DB #
###########################
# RUN apt install --no-install-recommends --no-install-suggests -y mysql-client
# RUN mysqladmin -u $DB_LOGIN -p$DB_PASSWORD -h $DB_HOST ping

################
# run QA tests #
################
# RUN cd build && ctest --output-on-failure

###################
# DB installation #
###################
# RUN ["/bin/bash", "-c", "debconf-set-selections <<< 'mysql-server mysql-server/root_password password $DB_PASSWORD'"]
# RUN ["/bin/bash", "-c", "debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password $DB_PASSWORD'"]
# RUN debconf-show mysql-server
# RUN apt-get install --no-install-recommends --no-install-suggests -y mysql-server

###########
# DB init #
###########
# RUN mysql -h localhost -P 3306 -u $DB_LOGIN -p$DB_PASSWORD
# RUN ["/bin/bash", "-c", "mysql -u $DB_LOGIN -p$DB_PASSWORD <<< 'CREATE DATABASE $DB_NAME;'"]
# RUN echo CREATE USER $DB_LOGIN; | mysql -u $DB_LOGIN -p$DB_PASSWORD
# RUN echo GRANT ALL ON $DB_NAME.* to '$DB_LOGIN'@'localhost' identified by '$DB_PASSWORD'; | mysql  -u $DB_LOGIN -p$DB_PASSWORD
# COPY ./supplemental/docker/qa/sql /tmp/
# RUN mysql -u $DB_LOGIN -p$DB_PASSWORD $DB_NAME < /tmp/sql

############################################################
## forward request and error logs to docker log collector ##
############################################################
#RUN ln -sf /dev/stdout /var/log/nginx/access.log \
# && ln -sf /dev/stderr /var/log/nginx/error.log

##########################
# DB tables provisioning #
##########################
# RUN apt install -f --no-install-recommends --no-install-suggests -y mysql-client
# RUN chmod +x ./supplemental/docker/qa/run.sh

STOPSIGNAL SIGTERM

CMD ["/bin/bash", "-c", "cd ./build && ctest --output-on-failure"]
