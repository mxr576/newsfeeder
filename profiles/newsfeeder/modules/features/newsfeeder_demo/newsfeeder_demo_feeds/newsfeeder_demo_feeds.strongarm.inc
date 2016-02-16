<?php
/**
 * @file
 * newsfeeder_demo_feeds.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function newsfeeder_demo_feeds_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'stanbol_enhancer_default_stanbol_endpoint';
  $strongarm->value = 'http://stanbol:8080';
  $export['stanbol_enhancer_default_stanbol_endpoint'] = $strongarm;

  return $export;
}
