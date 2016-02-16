<?php
/**
 * @file
 * newsfeeder_demo_taxonomies.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function newsfeeder_demo_taxonomies_taxonomy_default_vocabularies() {
  return array(
    'feed_item_authors' => array(
      'name' => 'Feed item: Authors',
      'machine_name' => 'feed_item_authors',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'feed_item_categories' => array(
      'name' => 'Feed Item: Categories',
      'machine_name' => 'feed_item_categories',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'feed_item_keywords' => array(
      'name' => 'Feed item: Keywords',
      'machine_name' => 'feed_item_keywords',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'feed_item_stanbol_organizations' => array(
      'name' => 'Feed Item: Stanbol: Organizations',
      'machine_name' => 'feed_item_stanbol_organizations',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'feed_item_stanbol_people' => array(
      'name' => 'Feed Item: Stanbol: People',
      'machine_name' => 'feed_item_stanbol_people',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
