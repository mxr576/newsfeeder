FROM dockerizedrupal/base-debian-jessie:1.1.0

MAINTAINER Dezső BICZÓ <mxr576@gmail.com>

LABEL vendor=dockerizedrupal.com

ENV TERM xterm

ADD ./src /src

RUN /src/entrypoint.sh build

VOLUME ["/jenkins"]

EXPOSE 80
EXPOSE 443

ENTRYPOINT ["/src/entrypoint.sh", "run"]
