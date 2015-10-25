<?php
/**
 * @file
 * newsfeeder_demo_taxonomies.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function newsfeeder_demo_taxonomies_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer taxonomy'.
  $permissions['administer taxonomy'] = array(
    'name' => 'administer taxonomy',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in feed_item_authors'.
  $permissions['delete terms in feed_item_authors'] = array(
    'name' => 'delete terms in feed_item_authors',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in feed_item_categories'.
  $permissions['delete terms in feed_item_categories'] = array(
    'name' => 'delete terms in feed_item_categories',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in feed_item_keywords'.
  $permissions['delete terms in feed_item_keywords'] = array(
    'name' => 'delete terms in feed_item_keywords',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in feed_item_stanbol_organizations'.
  $permissions['delete terms in feed_item_stanbol_organizations'] = array(
    'name' => 'delete terms in feed_item_stanbol_organizations',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in feed_item_stanbol_people'.
  $permissions['delete terms in feed_item_stanbol_people'] = array(
    'name' => 'delete terms in feed_item_stanbol_people',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in feed_item_authors'.
  $permissions['edit terms in feed_item_authors'] = array(
    'name' => 'edit terms in feed_item_authors',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in feed_item_categories'.
  $permissions['edit terms in feed_item_categories'] = array(
    'name' => 'edit terms in feed_item_categories',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in feed_item_keywords'.
  $permissions['edit terms in feed_item_keywords'] = array(
    'name' => 'edit terms in feed_item_keywords',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in feed_item_stanbol_organizations'.
  $permissions['edit terms in feed_item_stanbol_organizations'] = array(
    'name' => 'edit terms in feed_item_stanbol_organizations',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in feed_item_stanbol_people'.
  $permissions['edit terms in feed_item_stanbol_people'] = array(
    'name' => 'edit terms in feed_item_stanbol_people',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  return $permissions;
}
