<?php
/**
 * @file
 * newsfeeder_demo_search.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function newsfeeder_demo_search_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer search_api'.
  $permissions['administer search_api'] = array(
    'name' => 'administer search_api',
    'roles' => array(),
    'module' => 'search_api',
  );

  return $permissions;
}
