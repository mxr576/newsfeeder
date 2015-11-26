class build::mysql::packages {
  package {[
      'mysql-client',
      'php5-mysql',
    ]:
    ensure => present
  }
}
