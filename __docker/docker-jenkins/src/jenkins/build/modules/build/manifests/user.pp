class build::user {
  bash_exec { "groupadd -o -g 1000 container": }
  bash_exec { "useradd -m -o -s /bin/bash -u 1000 -g 1000 container": }
}
