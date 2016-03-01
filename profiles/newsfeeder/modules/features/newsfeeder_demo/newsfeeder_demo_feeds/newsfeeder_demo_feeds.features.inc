<?php
/**
 * @file
 * newsfeeder_demo_feeds.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function newsfeeder_demo_feeds_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_default_feeds_importer_enhancer().
 */
function newsfeeder_demo_feeds_default_feeds_importer_enhancer() {
  $items = array();
  $items['feed_importer'] = entity_import('feeds_importer_enhancer', '{
    "id" : "feed_importer",
    "config" : {
      "feeder_enhancer_api_full_content_enhancer" : {
        "status" : 1,
        "weight" : "-50",
        "proxy_settings" : {
          "proxy_address" : "http:\\/\\/wce:8001\\/?url=",
          "timeout" : "60",
          "number_of_tries" : "10",
          "feed_item_url_field" : "field_feed_item_url"
        },
        "properties" : { "full_content" : "field_feed_item_content" }
      },
      "feeder_enhancer_api_stanbol_enhancer" : {
        "status" : 1,
        "weight" : "-49",
        "stanbol_enhancer" : {
          "enabled" : true,
          "field_to_analyze" : "field_feed_item_content",
          "minimum_confidence_level" : "0.5",
          "process_immediately" : true,
          "mapping" : {
            "language" : "language",
            "organizations_term" : "field_feed_item_stanbol_orgs",
            "people_term" : "field_feed_item_stanbol_people",
            "places_geofield" : "field_feed_item_stanbol_places"
          }
        }
      },
      "newsfeeder_demo_find_keywords_find_keywords_feed_enhancer" : {
        "status" : 1,
        "weight" : "-48",
        "proxy_settings" : {
          "proxy_address" : "http:\\/\\/fkit:9999",
          "timeout" : "30",
          "vocabularies" : "3",
          "text_field" : "field_feed_item_content",
          "distance" : "3"
        },
        "properties" : { "keywords" : "field_feed_item_keywords" }
      },
      "feed_item_enhancer_plugin_publish_if_content_language" : { "status" : 1, "weight" : "-47", "publish_if" : { "language" : "en" } }
    }
  }');
  return $items;
}
