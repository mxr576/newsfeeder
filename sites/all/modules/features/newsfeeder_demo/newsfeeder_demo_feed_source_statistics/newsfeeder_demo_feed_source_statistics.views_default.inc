<?php
/**
 * @file
 * newsfeeder_demo_feed_source_statistics.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function newsfeeder_demo_feed_source_statistics_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'newsfeeder_demo_feeds_source_statistics';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'NewsFeeder Demo: Feeds source statistics';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Feeds source statistics';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'views_content_cache';
  $handler->display->display_options['cache']['results_min_lifespan'] = '-1';
  $handler->display->display_options['cache']['results_max_lifespan'] = '21600';
  $handler->display->display_options['cache']['output_min_lifespan'] = '-1';
  $handler->display->display_options['cache']['output_max_lifespan'] = '21600';
  $handler->display->display_options['cache']['keys'] = array(
    'node' => array(
      'feed_item' => 'feed_item',
      'feed_source' => 0,
      'test' => 0,
    ),
    'node_only' => array(
      'node_changed' => 0,
    ),
  );
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'published_feed_item_count' => 'published_feed_item_count',
    'unpublished_feed_item_count' => 'unpublished_feed_item_count',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
    'published_feed_item_count' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'unpublished_feed_item_count' => array(
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Feeds item: Owner feed */
  $handler->display->display_options['relationships']['feed_nid']['id'] = 'feed_nid';
  $handler->display->display_options['relationships']['feed_nid']['table'] = 'feeds_item';
  $handler->display->display_options['relationships']['feed_nid']['field'] = 'feed_nid';
  $handler->display->display_options['relationships']['feed_nid']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'feed_nid';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Newsfeeder addons: Number of published feed items */
  $handler->display->display_options['fields']['published_feed_item_count']['id'] = 'published_feed_item_count';
  $handler->display->display_options['fields']['published_feed_item_count']['table'] = 'newsfeeder_addons';
  $handler->display->display_options['fields']['published_feed_item_count']['field'] = 'published_feed_item_count';
  $handler->display->display_options['fields']['published_feed_item_count']['label'] = 'Published feed items';
  $handler->display->display_options['fields']['published_feed_item_count']['separator'] = '';
  /* Field: Newsfeeder addons: Number of unpublished feed items */
  $handler->display->display_options['fields']['unpublished_feed_item_count']['id'] = 'unpublished_feed_item_count';
  $handler->display->display_options['fields']['unpublished_feed_item_count']['table'] = 'newsfeeder_addons';
  $handler->display->display_options['fields']['unpublished_feed_item_count']['field'] = 'unpublished_feed_item_count';
  $handler->display->display_options['fields']['unpublished_feed_item_count']['label'] = 'Unpublished feed items';
  $handler->display->display_options['fields']['unpublished_feed_item_count']['separator'] = '';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'feed_nid';
  $handler->display->display_options['filters']['type']['value'] = array(
    'feed_source' => 'feed_source',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'newsfeeder-demo/feeds-source-statistics';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Feeds source statistics';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $translatables['newsfeeder_demo_feeds_source_statistics'] = array(
    t('Master'),
    t('Feeds source statistics'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Owner feed'),
    t('Title'),
    t('Published feed items'),
    t('.'),
    t('Unpublished feed items'),
    t('Page'),
  );
  $export['newsfeeder_demo_feeds_source_statistics'] = $view;

  return $export;
}
