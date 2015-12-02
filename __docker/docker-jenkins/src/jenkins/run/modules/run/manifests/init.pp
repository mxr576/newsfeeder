class run {
  include run::jenkins
  include run::nginx
  include run::timezone
  include run::user

  if $mysql == "On" {
    include run::mysql
  }

  if $openvpn == "On" {
    include run::openvpn
  }
}
