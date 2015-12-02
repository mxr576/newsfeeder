<?php
/**
 * @file
 * newsfeeder_demo_cts.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function newsfeeder_demo_cts_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|feed_item|teaser';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'feed_item';
  $ds_fieldsetting->view_mode = 'teaser';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'post_date' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'ds_post_date_short',
    ),
  );
  $export['node|feed_item|teaser'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function newsfeeder_demo_cts_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|feed_item|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'feed_item';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_feed_item_content',
        1 => 'field_feed_item_url',
        2 => 'field_feed_item_categories',
        3 => 'field_feed_item_authors',
        4 => 'field_feed_item_keywords',
        5 => 'field_feed_item_stanbol_orgs',
        6 => 'field_feed_item_stanbol_people',
        7 => 'field_feed_item_stanbol_places',
      ),
    ),
    'fields' => array(
      'field_feed_item_content' => 'ds_content',
      'field_feed_item_url' => 'ds_content',
      'field_feed_item_categories' => 'ds_content',
      'field_feed_item_authors' => 'ds_content',
      'field_feed_item_keywords' => 'ds_content',
      'field_feed_item_stanbol_orgs' => 'ds_content',
      'field_feed_item_stanbol_people' => 'ds_content',
      'field_feed_item_stanbol_places' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|feed_item|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|feed_item|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'feed_item';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'title',
        1 => 'post_date',
        2 => 'field_feed_item_content',
        3 => 'field_feed_item_url',
        4 => 'group_additional_info',
        5 => 'field_feed_item_stanbol_people',
        6 => 'field_feed_item_stanbol_orgs',
        7 => 'field_feed_item_keywords',
      ),
    ),
    'fields' => array(
      'title' => 'ds_content',
      'post_date' => 'ds_content',
      'field_feed_item_content' => 'ds_content',
      'field_feed_item_url' => 'ds_content',
      'group_additional_info' => 'ds_content',
      'field_feed_item_stanbol_people' => 'ds_content',
      'field_feed_item_stanbol_orgs' => 'ds_content',
      'field_feed_item_keywords' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|feed_item|teaser'] = $ds_layout;

  return $export;
}
