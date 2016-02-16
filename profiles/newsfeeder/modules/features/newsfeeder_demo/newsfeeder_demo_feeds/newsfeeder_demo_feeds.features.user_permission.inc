<?php
/**
 * @file
 * newsfeeder_demo_feeds.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function newsfeeder_demo_feeds_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer feeds'.
  $permissions['administer feeds'] = array(
    'name' => 'administer feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  // Exported permission: 'clear feed_importer feeds'.
  $permissions['clear feed_importer feeds'] = array(
    'name' => 'clear feed_importer feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  // Exported permission: 'import feed_importer feeds'.
  $permissions['import feed_importer feeds'] = array(
    'name' => 'import feed_importer feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock feed_importer feeds'.
  $permissions['unlock feed_importer feeds'] = array(
    'name' => 'unlock feed_importer feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  return $permissions;
}
