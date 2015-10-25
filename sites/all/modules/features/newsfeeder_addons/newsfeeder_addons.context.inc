<?php
/**
 * @file
 * newsfeeder_addons.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function newsfeeder_addons_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'feeds_source_node';
  $context->description = '';
  $context->tag = 'feeds';
  $context->conditions = array(
    'context_feeds_feeds_source_node' => array(
      'values' => array(
        'TRUE' => 'TRUE',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content',
          'weight' => '-10',
        ),
        'views-2573daed78da9a4144000371c4c9aa09' => array(
          'module' => 'views',
          'delta' => '2573daed78da9a4144000371c4c9aa09',
          'region' => 'content',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('feeds');
  $export['feeds_source_node'] = $context;

  return $export;
}
