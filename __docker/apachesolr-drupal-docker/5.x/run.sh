#!/bin/bash

# On the first run, we need to copy the selected configurition files.
if [ ! -f /opt/solr/server/solr/drupal/conf/.installed ]; then
	# Check, if partial search function should be enabled.
	if [ "${PARTIAL_SEARCH_ENABLED}" == "false" ]; then
		cp /srv/solr/schemas/default/* /opt/solr/server/solr/drupal/conf
	else
		cp /srv/solr/schemas/partial_search/* /opt/solr/server/solr/drupal/conf
	fi
  touch /opt/solr/server/solr/drupal/conf/.installed
fi

echo "(Re)creating Solr core, please wait..."
/opt/solr/bin/solr start
sleep 10
/opt/solr/bin/solr create -c drupal -d /opt/solr/server/solr/drupal
killall java
echo "Solr core successfully (re)created, restarting Solr with supervisord."

# Start supervisored, which will start Solr.
/usr/bin/supervisord -c /etc/supervisor/conf.d/supervisord.conf
