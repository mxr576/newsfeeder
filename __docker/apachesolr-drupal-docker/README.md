# Apache Solr 4.x & 5.x (not only) for Drupal
---

Apache Solr 4.x - 5.x Docker image (not only) for Drupal.

### How to use these images:

```sh
git clone https://github.com/mxr576/apachesolr-drupal-docker.git
cd apachesolr-drupal-docker
```
Choose your version and open its folder, for example, if you choosed the 4.x version:

```sh
docker build -t mxr576/apachesolr-5.x-drupal-docker .
docker run -id -p 8983:8983 -t mxr576/apachesolr-4.x-drupal-docker
```

Pre-built images also available on DockerHub:
* https://hub.docker.com/r/mxr576/apachesolr-4.x-drupal-docker/
* https://hub.docker.com/r/mxr576/apachesolr-5.x-drupal-docker/

### Extras

You could specify the heap size of the JVM with the **SOLR_MEM_SIZE** variable (by default is set to 512 MB):

```sh
docker run -id -p 8983:8983 -e SOLR_MEM_SIZE=1g -t mxr576/apachesolr-4.x-drupal-docker
```

If you would like store the data on the host:

**Solr 4.x:**

```sh
docker run -id -p 8983:8983 -e SOLR_MEM_SIZE=1g -v path_on_the_host:/opt/solr/example/solr/collection1/data -t mxr576/apachesolr-4.x-drupal-docker
```
**Solr 5.x**
```sh
docker run -id -p 8983:8983 -e SOLR_MEM_SIZE=1g -v path_on_the_host:/opt/solr/server/solr/drupal/data -t mxr576/apachesolr-5.x-drupal-docker
```

If you would like to use the Solr's built in partial search function on the text fields, then you can easily enable it on any container:

```sh
docker run -id -p 8983:8983-e PARTIAL_SEARCH_ENABLED=true -t mxr576/apachesolr-5.x-drupal-docker
```

**HINT**: When you use the Solr 5.x version, then the "Solr path" has to be set to `/solr/drupal` instead of `/solr` on Search API server settings page.

### Credits
Thanks for @Coornail the base image, that I've modified and extended.
