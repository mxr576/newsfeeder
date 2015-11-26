class run {
  include run::jenkins
  include run::nginx
  include run::timezone

  if $mysql == "On" {
    include run::user
    include run::mysql
  }

  if $openvpn == "On" {
    include run::openvpn
  }
}
