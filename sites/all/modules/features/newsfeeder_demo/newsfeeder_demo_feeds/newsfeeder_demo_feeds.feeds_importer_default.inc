<?php
/**
 * @file
 * newsfeeder_demo_feeds.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function newsfeeder_demo_feeds_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'feed_importer';
  $feeds_importer->config = array(
    'name' => 'Feed importer',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => 1,
        'use_pubsubhubbub' => 0,
        'designated_hub' => '',
        'request_timeout' => '',
        'auto_scheme' => 'http',
        'accept_invalid_cert' => 0,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSyndicationParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'description',
            'target' => 'field_feed_item_content',
            'format' => 'filtered_html',
          ),
          2 => array(
            'source' => 'timestamp',
            'target' => 'created',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'url',
            'target' => 'field_feed_item_url:url',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'author_name',
            'target' => 'field_feed_item_authors',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'tags',
            'target' => 'field_feed_item_categories',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'url',
            'target' => 'url',
            'unique' => 1,
          ),
          7 => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
        'update_existing' => '0',
        'update_non_existent' => 'skip',
        'input_format' => 'filtered_html',
        'skip_hash_check' => 0,
        'bundle' => 'feed_item',
        'delete_with_source' => 1,
      ),
    ),
    'content_type' => 'feed_source',
    'update' => 0,
    'import_period' => '43200',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 1,
  );
  $export['feed_importer'] = $feeds_importer;

  return $export;
}
