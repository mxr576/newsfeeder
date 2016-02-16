<?php
/**
 * @file
 * newsfeeder_demo_cts.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function newsfeeder_demo_cts_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create feed_item content'.
  $permissions['create feed_item content'] = array(
    'name' => 'create feed_item content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create feed_source content'.
  $permissions['create feed_source content'] = array(
    'name' => 'create feed_source content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any feed_item content'.
  $permissions['delete any feed_item content'] = array(
    'name' => 'delete any feed_item content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any feed_source content'.
  $permissions['delete any feed_source content'] = array(
    'name' => 'delete any feed_source content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own feed_item content'.
  $permissions['delete own feed_item content'] = array(
    'name' => 'delete own feed_item content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own feed_source content'.
  $permissions['delete own feed_source content'] = array(
    'name' => 'delete own feed_source content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any feed_item content'.
  $permissions['edit any feed_item content'] = array(
    'name' => 'edit any feed_item content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any feed_source content'.
  $permissions['edit any feed_source content'] = array(
    'name' => 'edit any feed_source content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own feed_item content'.
  $permissions['edit own feed_item content'] = array(
    'name' => 'edit own feed_item content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own feed_source content'.
  $permissions['edit own feed_source content'] = array(
    'name' => 'edit own feed_source content',
    'roles' => array(),
    'module' => 'node',
  );

  return $permissions;
}
