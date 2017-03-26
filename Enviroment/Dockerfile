FROM ubuntu:xenial
RUN locale-gen en_US.UTF-8
RUN locale-gen zh_TW.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install sudo
RUN adduser --disabled-password --gecos "" ubuntu
RUN echo "ubuntu:ubuntu" | chpasswd
RUN usermod -aG sudo ubuntu
RUN apt-get install -y openssh-server vim-nox git wget curl gcc g++
RUN mkdir /var/run/sshd

EXPOSE 22
CMD    ["/usr/sbin/sshd", "-D"]

