<?php
/**
 * @file
 * Plugin definition.
 * See context/context_ui/export_ui/context.inc
 */

$plugin = array(
  'access' => 'administer contexts',
  'schema' => 'context',
  'menu' => array(
    'menu prefix' => 'admin/structure',
    'menu item' => 'context',
    'menu title' => 'Context',
    'menu description' => 'Associate menus, views, blocks, etc. with different contexts to structure your site.',
  ),
  'title singular' => t('context'),
  'title singular proper' => t('Context'),
  'title plural' => t('contexts'),
  'title plural proper' => t('Contexts'),
  'form' => array(
    'settings' => 'context_ui_form',
    'submit' => 'context_ui_form_submit',
  ),
  'handler' => array(
    'class' => 'context_docket_export_ui',
    'parent' => 'context_export_ui',
  ),
);
