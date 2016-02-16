<?php
/**
 * @file
 * newsfeeder_demo_taxonomies.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function newsfeeder_demo_taxonomies_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_feed_item_authors_pattern';
  $strongarm->value = 'feed-item/authors/[term:name]';
  $export['pathauto_taxonomy_term_feed_item_authors_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_feed_item_categories_pattern';
  $strongarm->value = 'feed-item/categories/[term:name]';
  $export['pathauto_taxonomy_term_feed_item_categories_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_feed_item_keywords_pattern';
  $strongarm->value = 'feed-item/keywords/[term:name]';
  $export['pathauto_taxonomy_term_feed_item_keywords_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_feed_item_stanbol_organizations_pattern';
  $strongarm->value = 'feed-item/stanbol-organizations/[term:name]';
  $export['pathauto_taxonomy_term_feed_item_stanbol_organizations_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_feed_item_stanbol_people_pattern';
  $strongarm->value = 'feed-item/stanbol-people/[term:name]';
  $export['pathauto_taxonomy_term_feed_item_stanbol_people_pattern'] = $strongarm;

  return $export;
}
