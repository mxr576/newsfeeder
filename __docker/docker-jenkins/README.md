# docker-jenkins

A Docker image for [Jenkins](http://jenkins-ci.org/) with [Nginx](http://nginx.org/) in front of it providing support for HTTPS. Also supports connecting to OpenVPN server.

## tl;dr;

This is a modified version of the DockerizedDrupal's Jenkins image. This image has pre-installed mysql-client and php5-mysql packages, because this way the Jenkins will be able to run drush commands. (Drush should be mounted somewhere.)

## Run the container

    CONTAINER="jenkins-data" && sudo docker run \
      --name "${CONTAINER}" \
      -h "${CONTAINER}" \
      -v /jenkins \
      --entrypoint /bin/echo \
      mxr576/docker-jenkins "Data-only container for Jenkins."

    CONTAINER="jenkins" && sudo docker run \
      --name "${CONTAINER}" \
      -h "${CONTAINER}" \
      -p 80:80 \
      -p 443:443 \
      --volumes-from jenkins-data \
      --cap-add NET_ADMIN \
      -e SERVER_NAME="localhost" \
      -e TIMEZONE="Etc/UTC" \
      -e TIMEOUT="300" \
      -e PROTOCOLS="https,http" \
      -e OPENVPN="Off" \
      -e OPENVPN_DEVICE="TAP" \
      -e OPENVPN_HOST="" \
      -e OPENVPN_PORT="1194" \
      -e OPENVPN_USERNAME="" \
      -e OPENVPN_PASSWORD="" \
      -e OPENVPN_CA_CERTIFICATE="" \
      -d \
      mxr576/docker-jenkins

## Build the image

    TMP="$(mktemp -d)" \
      && git clone https://github.com/mxr576/docker-jenkins.git "${TMP}" \
      && cd "${TMP}" \
      && git checkout 1.1.7 \
      && sudo docker build -t mxr576/docker-jenkins . \
      && cd -

## Changing the container behaviour on runtime through environment variables

    // TODO

## Back up Jenkins data

    sudo tools/jenkinsdata backup

## Restore Jenkins data from a backup

    sudo tools/jenkinsdata restore

## License

**MIT**
