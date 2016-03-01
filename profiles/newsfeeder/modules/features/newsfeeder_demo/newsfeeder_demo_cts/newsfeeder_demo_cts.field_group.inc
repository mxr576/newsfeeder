<?php
/**
 * @file
 * newsfeeder_demo_cts.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function newsfeeder_demo_cts_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_additional_info|node|feed_item|teaser';
  $field_group->group_name = 'group_additional_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'feed_item';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Additional info',
    'weight' => '4',
    'children' => array(
      0 => 'field_feed_item_stanbol_orgs',
      1 => 'field_feed_item_stanbol_people',
      2 => 'field_feed_item_keywords',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Additional info',
      'instance_settings' => array(
        'classes' => 'group-additional-info field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_additional_info|node|feed_item|teaser'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Additional info');

  return $field_groups;
}
