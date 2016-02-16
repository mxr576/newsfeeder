<?php
/**
 * @file
 * newsfeeder_demo_search.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function newsfeeder_demo_search_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'feeds_search';
  $context->description = '';
  $context->tag = 'newsfeeder_demo';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'newsfeeder_demo_feeds_search' => 'newsfeeder_demo_feeds_search',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'newsfeeder_daily_search_results-nf_daily_search_results_on_chart' => array(
          'module' => 'newsfeeder_daily_search_results',
          'delta' => 'nf_daily_search_results_on_chart',
          'region' => 'featured',
          'weight' => '-10',
        ),
        'views-a6edfdf004d3cfbd263146e4433f9c77' => array(
          'module' => 'views',
          'delta' => 'a6edfdf004d3cfbd263146e4433f9c77',
          'region' => 'content',
          'weight' => '-10',
        ),
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content',
          'weight' => '-9',
        ),
        'facetapi-RlQjEMk10r0QfZQPri1g9jxV5GZxPAqh' => array(
          'module' => 'facetapi',
          'delta' => 'RlQjEMk10r0QfZQPri1g9jxV5GZxPAqh',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'facetapi-dYhj4Yy9S40ZixN8FFuJhixc2aE7Zlk2' => array(
          'module' => 'facetapi',
          'delta' => 'dYhj4Yy9S40ZixN8FFuJhixc2aE7Zlk2',
          'region' => 'sidebar_second',
          'weight' => '-9',
        ),
        'facetapi-j0TyTsMkw8DI2AcEIAy4frvzOWold2fu' => array(
          'module' => 'facetapi',
          'delta' => 'j0TyTsMkw8DI2AcEIAy4frvzOWold2fu',
          'region' => 'sidebar_second',
          'weight' => '-8',
        ),
      ),
    ),
    'region' => array(
      'responsive_bartik' => array(
        'disable' => array(
          'sidebar_first' => 'sidebar_first',
          'header' => 0,
          'help' => 0,
          'page_top' => 0,
          'page_bottom' => 0,
          'highlighted' => 0,
          'featured' => 0,
          'content' => 0,
          'sidebar_second' => 0,
          'triptych_first' => 0,
          'triptych_middle' => 0,
          'triptych_last' => 0,
          'footer_firstcolumn' => 0,
          'footer_secondcolumn' => 0,
          'footer_thirdcolumn' => 0,
          'footer_fourthcolumn' => 0,
          'footer' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('newsfeeder_demo');
  $export['feeds_search'] = $context;

  return $export;
}
