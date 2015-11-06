<?php
/**
 * @file
 * newsfeeder_demo_search.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function newsfeeder_demo_search_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nf_current_search_daily_stat_searcher';
  $strongarm->value = 'search_api@feed_items';
  $export['nf_current_search_daily_stat_searcher'] = $strongarm;

  return $export;
}
