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
  $strongarm->name = 'nf_daily_search_result_current_kw_param';
  $strongarm->value = 'keywords';
  $export['nf_daily_search_result_current_kw_param'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nf_daily_search_result_current_searcher';
  $strongarm->value = 'search_api@feed_items';
  $export['nf_daily_search_result_current_searcher'] = $strongarm;

  return $export;
}
