#!/usr/bin/env bash

VERSION="1.1.7"

shopt -s nullglob

WORKING_DIR="$(pwd)"

hash docker 2> /dev/null

if [ "${?}" -ne 0 ]; then
  echo "jenkinsdata: docker command not found."

  exit 1
fi

help() {
  cat << EOF
Version: ${VERSION}

Usage: jenkinsdata <backup|restore>
EOF

  exit 1
}

if [ "${1}" == "-h" ] || [ "${1}" == "--help" ]; then
  help
fi

unknown_command() {
  echo "jenkinsdata: Unknown command. See 'jenkinsdata --help'"

  exit 1
}

if [ -z "${1}" ]; then
  unknown_command
fi

jenkinsdata_containers() {
  echo "$(docker ps -a | grep jenkins-data | awk '{ print $1 }')"
}

if [ "${1}" = "backup" ]; then
  CONTAINERS="$(jenkinsdata_containers)"

  if [ -n "${CONTAINERS}" ]; then
    for CONTAINER in ${CONTAINERS}; do
      CONTAINER_NAME="$(docker inspect --format='{{.Name}}' ${CONTAINER} | cut -c 2-)"

      docker run \
        --rm \
        --volumes-from "${CONTAINER}" \
        -v "${WORKING_DIR}:/backup" \
        --entrypoint /bin/bash \
        mxr576/docker-jenkins -c "tar czvf /backup/${CONTAINER_NAME}.tar.gz /jenkins"
    done
  fi
elif [ "${1}" = "restore" ]; then
  for FILE in *.tar.gz; do
    CONTAINER="${FILE%%.*}"

    docker run \
      --name "${CONTAINER}" \
      -h "${CONTAINER}" \
      -v /jenkins \
      --entrypoint /bin/echo \
      mxr576/docker-jenkins \
      "Data only container for ${CONTAINER//-data[0-9]*/}."

    docker run \
      --rm \
      --volumes-from "${CONTAINER}" \
      -v "${WORKING_DIR}:/backup" \
      --entrypoint="/bin/bash" \
      mxr576/docker-jenkins -c "tar xzvf /backup/${CONTAINER}.tar.gz"
  done
else
  unknown_command
fi
