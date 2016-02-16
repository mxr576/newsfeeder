<?php
/**
 * @file
 * Provide views data and handlers.
 */

/**
 * Implements hook_views_data().
 */
function newsfeeder_addons_views_data() {
  $data = [];

  $data['newsfeeder_addons'] = [
    'table' => [
      'group' => t('Newsfeeder addons'),
      'join' => [
        // Exist in all views.
        '#global' => [],
      ],
    ],
  ];

  $data['newsfeeder_addons']['published_feed_item_count'] = [
    'title' => t('Number of published feed items'),
    'help' => t('Display the number of published feed items.'),
    'field' => [
      'handler' => 'views_handler_field_published_feed_items_count',
      'click sortable' => FALSE,
    ],
  ];

  $data['newsfeeder_addons']['unpublished_feed_item_count'] = [
    'title' => t('Number of unpublished feed items'),
    'help' => t('Display the number of unpublished feed items.'),
    'field' => [
      'handler' => 'views_handler_field_unpublished_feed_items_count',
      'click sortable' => FALSE,
    ],
  ];

  return $data;
}
